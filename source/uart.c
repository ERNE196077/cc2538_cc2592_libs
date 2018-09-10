/*
 *  Copyright (c) 2016, The OpenThread Authors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 *   This file implements the OpenThread platform abstraction for UART communication.
 *
 */

#include <openthread-core-config.h>
#include <openthread/config.h>

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include <openthread/types.h>
#include <openthread/platform/debug_uart.h>
#include <openthread/platform/logging.h>
#include <openthread/platform/uart.h>

#include "../platform-cc2538.h"
#include "../include/common.h"
#include "../include/sys_ctrl.h"
#include "../include/gpio.h"
#include "../include/uart.h"
#include "utils/code_utils.h"

enum
{
    kPlatformClock     = 32000000,
    kBaudRate          = 115200,
    kReceiveBufferSize = 128,
};

extern void UART0IntHandler(void);

static void processReceive(void);
static void processTransmit(void);

static const uint8_t *sTransmitBuffer = NULL;
static uint16_t       sTransmitLength = 0;

typedef struct RecvBuffer
{
    // The data buffer
    uint8_t mBuffer[kReceiveBufferSize];
    // The offset of the first item written to the list.
    uint16_t mHead;
    // The offset of the next item to be written to the list.
    uint16_t mTail;
} RecvBuffer;

static RecvBuffer sReceive;

static void enable_uart_clocks(void)
{
    static int uart_clocks_done = 0;

    if (uart_clocks_done)
    {
        return;
    }

    uart_clocks_done = 1;

#if OPENTHREAD_CONFIG_ENABLE_DEBUG_UART
    SYS_CTRL->rcgcuart = (SYS_CTRL_RCGCUART_UART1_ENABLEUART1CLOCK | 
                          SYS_CTRL_RCGCUART_UART0_ENABLEUART0CLOCK);
    SYS_CTRL->scgcuart = (SYS_CTRL_SCGCUART_UART1_ENABLEUART1CLOCK | 
                          SYS_CTRL_SCGCUART_UART0_ENABLEUART0CLOCK);
    SYS_CTRL->dcgcuart = (SYS_CTRL_DCGCUART_UART1_ENABLEUART1CLOCK | 
                          SYS_CTRL_DCGCUART_UART0_ENABLEUART0CLOCK);
#else
    SYS_CTRL->rcgcuart = (SYS_CTRL_RCGCUART_UART0_ENABLEUART0CLOCK);
    SYS_CTRL->scgcuart = (SYS_CTRL_SCGCUART_UART0_ENABLEUART0CLOCK);
    SYS_CTRL->dcgcuart = (SYS_CTRL_DCGCUART_UART0_ENABLEUART0CLOCK);
#endif
}

otError otPlatUartEnable(void)
{
    uint32_t div;

    sReceive.mHead = 0;
    sReceive.mTail = 0;

    // clock
    enable_uart_clocks();

    UART0->cc = 0;

    // tx pin
    IOCMUX->ioc_pa1_sel = IOC_MUX_OUT_SEL_UART0_TXD;
    IOCMUX->ioc_pa1_over = IOC_OVERRIDE_OE;
    GPIO_A->afsel |= GPIO_PIN_1;
    
    // rx pin
    IOCMUX->ioc_uartrxd_uart0 = IOC_PA0;
    IOCMUX->ioc_pa0_over = IOC_OVERRIDE_DIS;
    GPIO_A->afsel |= GPIO_PIN_0;



    UART0->ctl = 0;

    // baud rate
    div = (((kPlatformClock * 8) / kBaudRate) + 1) / 2;
    UART0->ibrd = div / 64;
    UART0->fbrd = div % 64;
    UART0->lcrh = UART_LCRH_WLEN_8BITS | 
                  UART_LCRH_STP2_1STOPBITATFRAMEEND | 
                  UART_LCRH_PEN_PARITYDISABLED;

    // configure interrupts
    UART0->im |= UART_IM_RXIM_RECEIVEINTENABLE | 
                 UART_IM_RTIM_TIMEOUTINTENABLE;

    // enable
    UART0->ctl = UART_CTL_UARTEN_UARTENABLE | 
                 UART_CTL_TXE_UARTTXENABLE | 
                 UART_CTL_RXE_UARTRXENABLE;

    // enable interrupts
    NVIC_ENABLE(INT_UART0);



    return OT_ERROR_NONE;
}

otError otPlatUartDisable(void)
{
    return OT_ERROR_NONE;
}

otError otPlatUartSend(const uint8_t *aBuf, uint16_t aBufLength)
{
    otError error = OT_ERROR_NONE;

    otEXPECT_ACTION(sTransmitBuffer == NULL, error = OT_ERROR_BUSY);

    sTransmitBuffer = aBuf;
    sTransmitLength = aBufLength;

exit:
    return error;
}

void processReceive(void)
{
    // Copy tail to prevent multiple reads
    uint16_t tail = sReceive.mTail;

    // If the data wraps around, process the first part
    if (sReceive.mHead > tail)
    {
        otPlatUartReceived(sReceive.mBuffer + sReceive.mHead, kReceiveBufferSize - sReceive.mHead);

        // Reset the buffer mHead back to zero.
        sReceive.mHead = 0;
    }

    // For any data remaining, process it
    if (sReceive.mHead != tail)
    {
        otPlatUartReceived(sReceive.mBuffer + sReceive.mHead, tail - sReceive.mHead);

        // Set mHead to the local tail we have cached
        sReceive.mHead = tail;
    }
}

void processTransmit(void)
{
    otEXPECT(sTransmitBuffer != NULL);

    for (; sTransmitLength > 0; sTransmitLength--)
    {
        while (UART0->fr & UART_FR_TXFF_FIFOORTXREGFULL)
            ;

        UART0->dr = *sTransmitBuffer++;
    }

    sTransmitBuffer = NULL;
    otPlatUartSendDone();

exit:
    return;
}

void cc2538UartProcess(void)
{
    processReceive();
    processTransmit();
}

void UART0IntHandler(void)
{
    uint32_t mis;
    uint8_t  byte;

    mis = UART0->mis;
    UART0->icr = mis;

    if (mis & (UART_IM_RXIM_RECEIVEINTENABLE | UART_IM_RTIM_TIMEOUTINTENABLE))
    {
        while (!(UART0->fr & UART_FR_RXFE_FIFOORRXREGEMPTY))
        {
            byte = UART0->dr;

            // We can only write if incrementing mTail doesn't equal mHead
            if (sReceive.mHead != (sReceive.mTail + 1) % kReceiveBufferSize)
            {
                sReceive.mBuffer[sReceive.mTail] = byte;
                sReceive.mTail                   = (sReceive.mTail + 1) % kReceiveBufferSize;
            }
        }
    }
}

#if OPENTHREAD_CONFIG_ENABLE_DEBUG_UART

int otPlatDebugUart_kbhit(void)
{
    uint32_t v;

    /* get flags */
    v = UART1->fr;

    /* if FIFO empty we have no data */
    return !(v & UART_FR_RXFE_FIFOORRXREGEMPTY);
}

int otPlatDebugUart_getc(void)
{
    int v = 1;

    /* if nothing in fifo */
    if (!otPlatDebugUart_kbhit())
    {
        return -1;
    }

    /* fetch */
    v = (int)UART1->dr;
    v = (v & 0x0ff);
    return v;
}

void otPlatDebugUart_putchar_raw(int b)
{
    /* wait till not busy */
    while (UART1->fr & UART_FR_TXFF_FIFOORTXREGFULL)
        ;

    /* write byte */
    UART1->dr = ((uint32_t)(b & 0x0ff));
}

void cc2538DebugUartInit(void)
{
    int32_t a, b;

    // clocks
    enable_uart_clocks();

    UART1->cc = 0;

    // UART1 - tx pin
    // Using an RF06 Evaluation board
    // http://www.ti.com/tool/cc2538dk
    // PA3 => is jumper position RF1.14
    // To use these, you will require a "flying-lead" UART adapter
    IOCMUX->ioc_pa7_sel = IOC_MUX_OUT_SEL_UART1_TXD;
    IOCMUX->ioc_pa7_over = IOC_OVERRIDE_OE;
    GPIO_A->afsel |= GPIO_PIN_7;
    
    // UART1 - rx pin we don't really use but we setup anyway
    // PA2 => is jumper position RF1.16
    IOCMUX->ioc_uartrxd_uart1 = IOC_PA6;
    IOCMUX->ioc_pa6_over = IOC_OVERRIDE_DIS;
    GPIO_A->afsel |= GPIO_PIN_6;

    //HWREG(UART1_BASE + UART_O_CC) = 0;

    // baud rate
    b = (((kPlatformClock * 8) / kBaudRate) + 1) / 2;
    a = b / 64;
    b = b % 64;

    UART1->ibrd = a;
    UART1->fbrd = b;
    UART1->lcrh = UART_LCRH_WLEN_8BITS | 
                  UART_LCRH_STP2_1STOPBITATFRAMEEND | 
                  UART_LCRH_PEN_PARITYDISABLED;
    /* NOTE:
     *  uart1 is not using IRQs it is tx only
     *  and we block when writing bytes
     */
    UART1->ctl = UART_CTL_UARTEN_UARTENABLE | 
                 UART_CTL_TXE_UARTTXENABLE | 
                 UART_CTL_RXE_UARTRXENABLE;

}

#endif
