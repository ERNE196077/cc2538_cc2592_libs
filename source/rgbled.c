#include <stdint.h>
#include <stdio.h>

#include "../include/cc2538-reg.h"
#include "../include/ssim.h"
#include "../include/udma.h"
#include "../include/rgbled.h"
#include "../include/gpio.h"
#include "../include/sys_ctrl.h"

#include <openthread/cli.h>

#define SSI0_RX_UDMACH		10
#define SSI0_TX_UDMACH		11
#define SSI1_RX_UDMACH		24
#define SSI1_TX_UDMACH		25

#define SSI0_PIN_OUT_CLK	IOCMUX->ioc_pa2_sel = IOC_MUX_OUT_SEL_SSI0_CLK_OUT; IOCMUX->ioc_pa2_over = IOC_OVERRIDE_OE  
#define SSI0_PIN_OUT_TXD	IOCMUX->ioc_pa4_sel = IOC_MUX_OUT_SEL_SSI0_TXD; IOCMUX->ioc_pa4_over = IOC_OVERRIDE_OE 
#define SSI0_PIN_OUT_FSS	IOCMUX->ioc_pa3_sel = IOC_MUX_OUT_SEL_SSI0_FSS_OUT; IOCMUX->ioc_pa3_over = IOC_OVERRIDE_OE  
#define SSI0_PIN_IN_RXD		IOCMUX->ioc_ssirxd_ssi0 = IOC_PA5; IOCMUX->ioc_pa5_over = IOC_OVERRIDE_DIS; 
#define SSI0_PINS_AFSEL 	GPIO_A->afsel |= (GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5)

#define SSI1_PIN_OUT_CLK	IOCMUX->ioc_pd0_sel = IOC_MUX_OUT_SEL_SSI0_CLK_OUT; IOCMUX->ioc_pd0_over = IOC_OVERRIDE_OE  
#define SSI1_PIN_OUT_TXD	IOCMUX->ioc_pd1_sel = IOC_MUX_OUT_SEL_SSI0_TXD; IOCMUX->ioc_pd1_over = IOC_OVERRIDE_OE 
#define SSI1_PIN_OUT_FSS	IOCMUX->ioc_pd2_sel = IOC_MUX_OUT_SEL_SSI0_FSS_OUT; IOCMUX->ioc_pd2_over = IOC_OVERRIDE_OE  
#define SSI1_PIN_IN_RXD		IOCMUX->ioc_ssirxd_ssi1 = IOC_PD3; IOCMUX->ioc_pd3_over = IOC_OVERRIDE_DIS; 
#define SSI1_PINS_AFSEL 	GPIO_D->afsel |= (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)



uint16_t dummy_buf;
uint16_t dummy_c = 0;

extern volatile udmach_t dmactrl[64];

static rgbdev_t ssi0_rgbdev;
static rgbdev_t ssi1_rgbdev;

extern void SSI0IntHandler(void);
extern void SSI1IntHandler(void);

void SSI0IntHandler(void)
{ 	
	rgbdev_t *rgbdev = &ssi0_rgbdev;
	NVIC_DISABLE(rgbdev->ssiint);
	
	if (SSI0->mis & SSI_MIS_RXMIS_FIFOHALFEMPTY){
		UDMA->chis |= UDMA_CHANNEL(SSI0_RX_UDMACH);
		UDMA->enaset |= UDMA_CHANNEL(SSI0_RX_UDMACH);
	}

	if((rgbdev->dma_rxpri->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
	{	
		rgbdev->dma_rxpri->control = rgbdev->dma_rxchcfg;
	}

	if((rgbdev->dma_rxalt->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
	{	
		rgbdev->dma_rxpri->control = rgbdev->dma_rxchcfg;
	}

	if(rgbdev->xfertype == XFER_SINGLE)
	{
		if(!rgbdev->xfercomplete)
		{
			if((rgbdev->dma_txpri->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
			{
				switch(rgbdev->numled - rgbdev->xfer_c)
				{
					default:
						rgbdev->dma_txpri->source = (uint32_t)(rgbdev->dataptr) + (rgbdev->xferbs - 1);
						rgbdev->dma_txpri->control = rgbdev->dma_txchcfg | UDMA_CHCTL_XFERSIZE(rgbdev->xferbs);
						rgbdev->xfer_c++;
						break;
					case 1:
						rgbdev->xfercomplete++;
						break;
				}
			}
			if((rgbdev->dma_txalt->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
			{
				switch(rgbdev->numled - rgbdev->xfer_c)
				{
					default:
						rgbdev->dma_txalt->source = (uint32_t)(rgbdev->dataptr) + (rgbdev->xferbs - 1);
						rgbdev->dma_txalt->control = rgbdev->dma_txchcfg | UDMA_CHCTL_XFERSIZE(rgbdev->xferbs);
						rgbdev->xfer_c++;
						break;
					case 1:
						rgbdev->xfercomplete++;
						break;
				}
			}
		}
	}
	
	if(rgbdev->xfertype == XFER_BUFFER)
	{
		if(!rgbdev->xfercomplete)
		{	
			if((rgbdev->dma_txpri->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
			{
				switch(rgbdev->xferqty - rgbdev->xfer_c)
				{
					default:
						rgbdev->xfer_c++;
						rgbdev->dma_txpri->source = (uint32_t)(rgbdev->dataptr 
													+ (rgbdev->xferbs * (rgbdev->xfer_c + 1)) 
													- 1);
						rgbdev->dma_txpri->control = rgbdev->dma_txchcfg | UDMA_CHCTL_XFERSIZE((rgbdev->xferbs));
						break;
					case 2:
						rgbdev->xfer_c++; 
						rgbdev->dma_txpri->source = (uint32_t)(rgbdev->dataptr 
													+ (rgbdev->xferbs * rgbdev->xfer_c) 
													+ (rgbdev->xfersize % rgbdev->xferbs) 
													- 1);
						rgbdev->dma_txpri->control = rgbdev->dma_txchcfg | UDMA_CHCTL_XFERSIZE((rgbdev->xfersize % rgbdev->xferbs));
						break;
					case 1:
						rgbdev->xfercomplete++;
						break;
				}
			}

			if((rgbdev->dma_txalt->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
			{
				switch(rgbdev->xferqty - rgbdev->xfer_c)
				{
					default:
						rgbdev->xfer_c++;
						rgbdev->dma_txalt->source = (uint32_t)(rgbdev->dataptr 
													+ (rgbdev->xferbs * (rgbdev->xfer_c + 1)) 
													- 1);
						rgbdev->dma_txalt->control = rgbdev->dma_txchcfg | UDMA_CHCTL_XFERSIZE((rgbdev->xferbs));
						break;
					case 2:
						rgbdev->xfer_c++;
						rgbdev->dma_txalt->source = (uint32_t)(rgbdev->dataptr 
													+ (rgbdev->xferbs * rgbdev->xfer_c) 
													+ (rgbdev->xfersize % rgbdev->xferbs) 
													- 1);
						rgbdev->dma_txalt->control = rgbdev->dma_txchcfg | UDMA_CHCTL_XFERSIZE((rgbdev->xfersize % rgbdev->xferbs));
						break;
					case 1:
						rgbdev->xfercomplete++;
						break;
				}
			}	
		}
	}
	NVIC_ENABLE(rgbdev->ssiint);
}

void SSI1IntHandler(void)
{	
	
}


static void RGBSingle(rgbdev_t *rgbdev)
{
	rgbdev->xfercomplete = 0;
	rgbdev->xfer_c = 1;
	
	/*	UDMA TX configuration	*/
	rgbdev->dma_txpri->source = (uint32_t)(rgbdev->dataptr) + 11; //(rgbdev->xferbs - 1);
	rgbdev->dma_txpri->destination = (uint32_t)&(SSI0->dr);
	rgbdev->dma_txpri->control = rgbdev->dma_txchcfg | UDMA_CHCTL_XFERSIZE(rgbdev->xferbs);

	switch(rgbdev->numled - rgbdev->xfer_c)
	{			
		case 0:
			rgbdev->xfercomplete++;
			break;
		default:
			rgbdev->dma_txalt->source = (uint32_t)(rgbdev->dataptr) + (rgbdev->xferbs - 1);
			rgbdev->dma_txalt->destination = (uint32_t)&(rgbdev->ssi->dr);
			rgbdev->dma_txalt->control = rgbdev->dma_txchcfg | UDMA_CHCTL_XFERSIZE(rgbdev->xferbs);
			break;
	}
}


static void RGBBuffer(rgbdev_t *rgbdev)
{
	rgbdev->xfercomplete = 0;
	rgbdev->xfer_c = 1;
		

	rgbdev->dma_txpri->destination = (uint32_t)&(rgbdev->ssi->dr);
	rgbdev->dma_txalt->destination = (uint32_t)&(rgbdev->ssi->dr);
			

	switch(rgbdev->xferqty)
	{
		case 1: 
			rgbdev->dma_txpri->source = (uint32_t)rgbdev->dataptr + ((rgbdev->xfersize % rgbdev->xferbs) - 1);
			rgbdev->dma_txpri->control = rgbdev->dma_txchcfg | UDMA_CHCTL_XFERSIZE((rgbdev->xfersize % rgbdev->xferbs));
			rgbdev->xfercomplete++;
			break;
		case 2:
			rgbdev->dma_txpri->source = (uint32_t)rgbdev->dataptr + (rgbdev->xferbs - 1);
			rgbdev->dma_txpri->control = rgbdev->dma_txchcfg | UDMA_CHCTL_XFERSIZE(rgbdev->xferbs);
			
			rgbdev->dma_txalt->source = (uint32_t)rgbdev->dataptr + rgbdev->xferbs + (rgbdev->xfersize % rgbdev->xferbs) - 1;
			rgbdev->dma_txalt->control = rgbdev->dma_txchcfg | UDMA_CHCTL_XFERSIZE((rgbdev->xfersize % rgbdev->xferbs));
			break;
		default:
			rgbdev->dma_txpri->source = (uint32_t)rgbdev->dataptr + (rgbdev->xferbs - 1);
			rgbdev->dma_txpri->control = rgbdev->dma_txchcfg | UDMA_CHCTL_XFERSIZE(rgbdev->xferbs);

			rgbdev->dma_txalt->source = (uint32_t)rgbdev->dataptr + ((rgbdev->xferbs * 2) - 1);
			rgbdev->dma_txalt->control = rgbdev->dma_txchcfg | UDMA_CHCTL_XFERSIZE(rgbdev->xferbs);
			break;
	}
}


rgbdev_t *cc2538RGBInit(volatile ssi_t *ssi, uint32_t numled, uint8_t leddatasize, xfertype_t xfertype, void *data, uint32_t frequency)
{
	rgbdev_t *rgbdev;
	if(ssi == SSI0)
	{
		rgbdev = &ssi0_rgbdev;
		rgbdev->ssiint = INT_SSI0;
		rgbdev->dma_txch = SSI0_TX_UDMACH;
		rgbdev->dma_rxch = SSI0_RX_UDMACH;
	}
	else
	{
		rgbdev = &ssi1_rgbdev;
		rgbdev->ssiint = INT_SSI1;
		rgbdev->dma_txch = SSI1_TX_UDMACH;
		rgbdev->dma_rxch = SSI1_RX_UDMACH;
	}
	
	rgbdev->ssi = ssi;
	rgbdev->numled = numled;
	rgbdev->leddatasize = leddatasize;
	rgbdev->dataptr = (uint32_t *)data;
	rgbdev->xfertype = xfertype;
	rgbdev->xfersize = leddatasize * numled; /* BYTES */
	rgbdev->dma_txpri = ((udmach_t *)&dmactrl[rgbdev->dma_txch]);
	rgbdev->dma_txalt = ((udmach_t *)&dmactrl[32 + rgbdev->dma_txch]);
	rgbdev->dma_rxpri = ((udmach_t *)&dmactrl[rgbdev->dma_rxch]);
	rgbdev->dma_rxalt = ((udmach_t *)&dmactrl[32 + rgbdev->dma_rxch]);
	rgbdev->dma_txchcfg = UDMA_CHCTL_DSTINC_NOINC |
						UDMA_CHCTL_DSTSIZE_8BIT |
						UDMA_CHCTL_SRCINC_8BIT |
						UDMA_CHCTL_SRCSIZE_8BIT |
						UDMA_CHCTL_ARBSIZE_4 | 
						UDMA_CHCTL_XFERMODE_PINGPONG;

	rgbdev->dma_rxchcfg = UDMA_CHCTL_DSTINC_NOINC |
						UDMA_CHCTL_DSTSIZE_8BIT |
						UDMA_CHCTL_SRCINC_NOINC |
						UDMA_CHCTL_SRCSIZE_8BIT |
						UDMA_CHCTL_ARBSIZE_4 | 
						UDMA_CHCTL_XFERMODE_PINGPONG |
						UDMA_CHCTL_XFERSIZE(1024);

	if(xfertype == XFER_SINGLE)
	{
		rgbdev->xferqty = numled; /* ONE LED PER DMA TRANSFER */
		rgbdev->xferbs = leddatasize;
	}
	else
	{
		rgbdev->xferqty = 	(rgbdev->xfersize / 1024) + 1; /* BIGGER CHUNKS */
		rgbdev->xferbs = 	rgbdev->xferqty > 0 ? 
							(1024 / leddatasize) * leddatasize : 
							rgbdev->xfersize;
	}
	
	// cc2538SSIMInit(ssi, frequency, 
	// 	SSI_CR0_FRF_MOTOROLASPIFORMAT | 
	// 	SSI_CR0_SPH_CLOCKONSECONDEDGE | 
	// 	SSI_CR0_DSS_8BITDATA,
	// 	0x0, 
	// 	SSI_IM_RXIM_FIFOHALFEMPTY, 
	// 	SSI_DMATCL_TXDMAE_UDMAENABLE | 
	// 	SSI_DMATCL_RXDMAE_UDMAENABLE);
	// NVIC_ENABLE(rgbdev->ssiint);
	// ssi->cr1 |= SSI_CR1_SSE_ENABLESSI;

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
	ssi->cr0 = 	SSI_CR0_SCR_SET_CLOCKRATE(clkrate) | 
				SSI_CR0_FRF_MOTOROLASPIFORMAT | 
			 	SSI_CR0_SPH_CLOCKONSECONDEDGE | 
			 	SSI_CR0_DSS_8BITDATA;
	ssi->im = SSI_IM_RXIM_FIFOHALFEMPTY;
	ssi->dmactl = SSI_DMATCL_TXDMAE_UDMAENABLE | 
				  SSI_DMATCL_RXDMAE_UDMAENABLE;
	NVIC_ENABLE(rgbdev->ssiint);
	ssi->cr1 |= SSI_CR1_SSE_ENABLESSI;

	cc2538DmaInit();
	UDMA->prioclr |= UDMA_CHANNEL(rgbdev->dma_txch);
	UDMA->prioclr |= UDMA_CHANNEL(rgbdev->dma_rxch);
	UDMA->useburstclr |= UDMA_CHANNEL(rgbdev->dma_txch);
	UDMA->useburstclr |= UDMA_CHANNEL(rgbdev->dma_rxch);
	UDMA->reqmaskclr |= UDMA_CHANNEL(rgbdev->dma_txch);
	UDMA->reqmaskclr |= UDMA_CHANNEL(rgbdev->dma_rxch);	

	return rgbdev;
}
		
void cc2538RGBRender(rgbdev_t *rgbdev)
{	
	/*	Stop UDMA channels	*/
	UDMA->enaclr |= UDMA_CHANNEL(rgbdev->dma_txch);
	UDMA->enaclr |= UDMA_CHANNEL(rgbdev->dma_rxch);
	UDMA->altclr |= UDMA_CHANNEL(rgbdev->dma_txch);
	UDMA->altclr |= UDMA_CHANNEL(rgbdev->dma_rxch);
	
	/*	UDMA RX configuration	*/
	rgbdev->dma_rxpri->source = (uint32_t)&(SSI0->dr);
	rgbdev->dma_rxpri->destination = (uint32_t)&dummy_buf;
	rgbdev->dma_rxpri->control = rgbdev->dma_rxchcfg;

	rgbdev->dma_rxalt->source = (uint32_t)&(SSI0->dr);
	rgbdev->dma_rxalt->destination = (uint32_t)&dummy_buf;
	rgbdev->dma_rxalt->control = rgbdev->dma_rxchcfg;


	if (rgbdev->xfertype == XFER_SINGLE)
	{
		RGBSingle(rgbdev);
	}
	else
	{
		RGBBuffer(rgbdev);
	}

	
	//rgbdev->ssi->dr = 0x9;
	UDMA->enaset |= UDMA_CHANNEL(rgbdev->dma_txch);
	UDMA->enaset |= UDMA_CHANNEL(rgbdev->dma_rxch);
	
}


void cc2538WS2812Bgetrgb(uint8_t R, uint8_t G, uint8_t B, void *data)
{
	uint32_t *ptrdata;
	ptrdata = (uint32_t *)data;
	*ptrdata = 	((R & (1 << 7)) ? 0x6 : 0x4) << 21 | 
				((R & (1 << 6)) ? 0x6 : 0x4) << 18 | 
				((R & (1 << 5)) ? 0x6 : 0x4) << 15 | 
				((R & (1 << 4)) ? 0x6 : 0x4) << 12 | 
				((R & (1 << 3)) ? 0x6 : 0x4) << 9 | 
				((R & (1 << 2)) ? 0x6 : 0x4) << 6 | 
				((R & (1 << 1)) ? 0x6 : 0x4) << 3 | 
				((R & 1) ? 0x6 : 0x4);

	ptrdata = (uint32_t *)((uint8_t *)data + 3);
	*ptrdata = 	((G & (1 << 7)) ? 0x6 : 0x4) << 21 | 
				((G & (1 << 6)) ? 0x6 : 0x4) << 18 | 
				((G & (1 << 5)) ? 0x6 : 0x4) << 15 | 
				((G & (1 << 4)) ? 0x6 : 0x4) << 12 | 
				((G & (1 << 3)) ? 0x6 : 0x4) << 9 | 
				((G & (1 << 2)) ? 0x6 : 0x4) << 6 | 
				((G & (1 << 1)) ? 0x6 : 0x4) << 3 | 
				((G & 1) ? 0x6 : 0x4);

	ptrdata = (uint32_t *)((uint8_t *)data + 6);
	*ptrdata = 	((B & (1 << 7)) ? 0x6 : 0x4) << 21 | 
				((B & (1 << 6)) ? 0x6 : 0x4) << 18 | 
				((B & (1 << 5)) ? 0x6 : 0x4) << 15 | 
				((B & (1 << 4)) ? 0x6 : 0x4) << 12 | 
				((B & (1 << 3)) ? 0x6 : 0x4) << 9 | 
				((B & (1 << 2)) ? 0x6 : 0x4) << 6 | 
				((B & (1 << 1)) ? 0x6 : 0x4) << 3 | 
				((B & 1) ? 0x6 : 0x4);
}


void cc2538apa102(uint8_t R, uint8_t G, uint8_t B, void *data)
{	
	*((uint32_t *)data) = 0x0;
	*((uint32_t *)data + 1) = (0xFF << 24) | (G << 16) | (B << 8) | R;
	*((uint32_t *)data + 2) = 0xFFFFFFFF;

}
