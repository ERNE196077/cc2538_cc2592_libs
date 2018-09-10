#include <stdint.h>
#include <stdio.h>

#include "../include/cc2538-reg.h"
#include "../include/ssim.h"
#include "../include/gpio.h"
#include "../include/sys_ctrl.h"

#define SSI0_PIN_OUT_CLK	IOCMUX->ioc_pb2_sel = IOC_MUX_OUT_SEL_SSI0_CLK_OUT; IOCMUX->ioc_pb2_over = IOC_OVERRIDE_OE  
#define SSI0_PIN_OUT_TXD	IOCMUX->ioc_pb3_sel = IOC_MUX_OUT_SEL_SSI0_TXD; IOCMUX->ioc_pb3_over = IOC_OVERRIDE_OE 
#define SSI0_PIN_OUT_FSS	IOCMUX->ioc_pb4_sel = IOC_MUX_OUT_SEL_SSI0_FSS_OUT; IOCMUX->ioc_pb4_over = IOC_OVERRIDE_OE  
#define SSI0_PIN_IN_RXD		IOCMUX->ioc_ssirxd_ssi0 = IOC_PB5; IOCMUX->ioc_pb5_over = IOC_OVERRIDE_DIS; 
#define SSI0_PINS_AFSEL 	GPIO_B->afsel |= (GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5)

#define SSI1_PIN_OUT_CLK	IOCMUX->ioc_pd0_sel = IOC_MUX_OUT_SEL_SSI0_CLK_OUT; IOCMUX->ioc_pd0_over = IOC_OVERRIDE_OE  
#define SSI1_PIN_OUT_TXD	IOCMUX->ioc_pd1_sel = IOC_MUX_OUT_SEL_SSI0_TXD; IOCMUX->ioc_pd1_over = IOC_OVERRIDE_OE 
#define SSI1_PIN_OUT_FSS	IOCMUX->ioc_pd2_sel = IOC_MUX_OUT_SEL_SSI0_FSS_OUT; IOCMUX->ioc_pd2_over = IOC_OVERRIDE_OE  
#define SSI1_PIN_IN_RXD		IOCMUX->ioc_ssirxd_ssi1 = IOC_PD3; IOCMUX->ioc_pd3_over = IOC_OVERRIDE_DIS; 
#define SSI1_PINS_AFSEL 	GPIO_D->afsel |= (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)


/*	Default frame is Motorola SPI, need to implement the other two	*/	

void cc2538SSIMInit(volatile ssi_t *ssi, uint32_t frequency, uint32_t cr0, 
					uint32_t cr1, uint32_t im, uint32_t dmactl)
{
	uint32_t maxbrate;
    uint32_t prediv;
    uint32_t clkrate;
	
	if(ssi == SSI0)
	{
		SYS_CTRL->rcgcssi |= SYS_CTRL_RCGCSSI_SSI0_ENABLESSI0CLOCK;
		SYS_CTRL->scgcssi |= SYS_CTRL_SCGCSSI_SSI0_ENABLESSI0CLOCK;
		SYS_CTRL->dcgcssi |= SYS_CTRL_DCGCSSI_SSI0_ENABLESSI0CLOCK;

		SSI0_PIN_OUT_CLK;
		SSI0_PIN_OUT_TXD;
		SSI0_PIN_OUT_FSS;
		SSI0_PIN_IN_RXD;
		SSI0_PINS_AFSEL;
	}
	
	if(ssi == SSI1)
	{
		SYS_CTRL->rcgcssi |= SYS_CTRL_RCGCSSI_SSI1_ENABLESSI1CLOCK;
		SYS_CTRL->scgcssi |= SYS_CTRL_SCGCSSI_SSI1_ENABLESSI1CLOCK;
		SYS_CTRL->dcgcssi |= SYS_CTRL_DCGCSSI_SSI1_ENABLESSI1CLOCK;

		SSI1_PIN_OUT_CLK;
		SSI1_PIN_OUT_TXD;
		SSI1_PIN_OUT_FSS;
		SSI1_PIN_IN_RXD;
		SSI1_PINS_AFSEL;
	}

	/* Configure SSI bitrate */
    maxbrate = SYS_CTRL_SYSCLOCKFREQ / frequency;
    prediv = 0;
    do
    {
        prediv += 2;
        clkrate = (maxbrate / prediv) - 1;
    }
    while(clkrate > 255);
    
   	/*	Master SSI mode	*/
   	ssi->cr1 &= ~SSI_CR1_SSE_ENABLESSI;
	ssi->cr1 = 0x0;
	ssi->cc = 	SSI_CC_CS_PIOSC_BAUDBYSYSDIV | SSI_CC_CS_DSEN_SYSCLOCKBYSYSDIV;
	ssi->cpsr = prediv;
	ssi->cr0 = 	SSI_CR0_SCR_SET_CLOCKRATE(clkrate) | cr0;
	ssi->im = im;
	ssi->dmactl = dmactl;
	ssi->cr1 = cr1;
	//ssi->cr1 |= SSI_CR1_SSE_ENABLESSI;
}

void cc2538SSIMEnable(volatile ssi_t *ssi)
{
	ssi->cr1 |= SSI_CR1_SSE_ENABLESSI;
}

void cc2538SSIMDisable(volatile ssi_t *ssi)
{
	ssi->cr1 &= ~SSI_CR1_SSE_ENABLESSI;
}

void cc2538SSIMWrite(volatile ssi_t *ssi, uint16_t data)
{
	ssi->dr = data;
}

uint16_t cc2538SSIMRead(volatile ssi_t *ssi)
{
	return ssi->dr;
}

