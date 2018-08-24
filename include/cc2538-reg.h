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
 *   This file includes CC2538 register definitions.
 *
 */

#ifndef CC2538_REG_H_
#define CC2538_REG_H_


#define NVIC_ENABLE(INTERRUPT)					*((volatile uint32_t *)(NVIC_EN_BASE + (4 * (INTERRUPT/32)))) = (1 << (INTERRUPT % 32))
#define NVIC_DISABLE(INTERRUPT)					*((volatile uint32_t *)(NVIC_DIS_BASE + (4 * (INTERRUPT/32)))) = (1 << (INTERRUPT % 32))

/*	REGISTER BASE 	*/
#define NVIC_EN_BASE 							0xE000E100
#define NVIC_DIS_BASE 							0xE000E180
#define SSI0_BASE								0x40008000
#define SSI1_BASE								0x40009000
#define UART0_BASE                              0x4000C000
#define UART1_BASE                              0x4000D000
#define I2CM_BASE								0x40020000
#define I2CS_BASE								0x40020800
#define GPTIMER0_BASE							0x40030000
#define GPTIMER1_BASE							0x40031000
#define GPTIMER2_BASE							0x40032000
#define GPTIMER3_BASE							0x40033000
#define SYS_CTRL_BASE							0x400D2000
#define IOCMUX_BASE								0x400D4000
#define ANALOG_BASE	                            0x400D6000  // ANA_REGS
#define SOCADC_BASE								0x400D7000
#define GPIO_A_BASE                             0x400D9000 
#define GPIO_B_BASE								0x400DA000
#define GPIO_C_BASE								0x400DB000
#define GPIO_D_BASE								0x400DC000
#define	UDMA_BASE								0x400FF000

/*	INTERRUPTS	*/
#define INT_GPIO_A								0
#define INT_GPIO_B								1
#define INT_GPIO_C								2
#define INT_GPIO_D								3
#define INT_UART0                              	5          // UART0 Rx and Tx
#define INT_UART1								6
#define INT_SSI0								7
#define INT_I2C									8
#define INT_ADC									14
#define INT_WDOG								18
#define INT_GPTIMER0A							19
#define INT_GPTIMER0B							20
#define INT_GPTIMER1A							21
#define INT_GPTIMER1B							22
#define INT_GPTIMER2A							23
#define INT_GPTIMER2B							24
#define INT_ANALOG								25
#define INT_RF_TXRX_ALT							26
#define INT_RF_ERROR_ALT						27
#define INT_SYSCONTROL							28
#define INT_FLASHCONTROL						29
#define INT_AES_ALT								30
#define INT_PKA_ALT								31
#define INT_SMTIMER_ALT							32
#define INT_MACTIMER_ALT						33
#define INT_SSI1								34
#define INT_GPTIMER3A							35
#define INT_GPTIMER3B							36
#define INT_DMA_SW								46
#define INT_DMA_ERROR							47
#define INT_USB									140
#define INT_RFCORE_RXTX							141
#define INT_RFCORE_ERROR						142
#define INT_AES									143
#define INT_PKA									144
#define INT_SMTIMER								145
#define INT_MACTIMER							145

#define NVIC_GPIO_A								16
#define NVIC_GPIO_B								17
#define NVIC_GPIO_C								18
#define NVIC_GPIO_D								19
#define NVIC_UART0                             	21          // UART0 Rx and Tx
#define NVIC_UART1								22
#define NVIC_SSI0								23
#define NVIC_I2C								24
#define NVIC_ADC								30
#define NVIC_WDOG								34
#define NVIC_GPTIMER0A							35
#define NVIC_GPTIMER0B							36
#define NVIC_GPTIMER1A							37
#define NVIC_GPTIMER1B							38
#define NVIC_GPTIMER2A							39
#define NVIC_GPTIMER2B							40
#define NVIC_ANALOG								41
#define NVIC_RF_TXRX							42
#define NVIC_RF_ERROR							43
#define NVIC_SYSCONTROL							44
#define NVIC_FLASHCONTROL						45
#define NVIC_AES								46
#define NVIC_PKA								47
#define NVIC_SMTIMER							48
#define NVIC_MACTIMER							49
#define NVIC_SSI1								50
#define NVIC_GPTIMER3A							51
#define NVIC_GPTIMER3B							52
#define NVIC_DMA_SW								62
#define NVIC_DMA_ERROR							63
#define NVIC_USB								156


#endif