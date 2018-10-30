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



uint16_t dummy_buf;
uint16_t dummy_c = 0;

extern volatile udmach_t dmactrl[64];

static rgbdev_t ssi0_rgbdev;
static rgbdev_t ssi1_rgbdev;

extern void SSI0IntHandler(void);
extern void SSI1IntHandler(void);


static void ws2812bconv(rgbdev_t *rgbdev)
{
	uint32_t leddatanum = rgbdev->config.xfertype == XFERTYPE_BUFFER ?
							rgbdev->config.lednum:
							1;
	led_t *leds = (led_t *)(rgbdev->leds);
	ws2812bpixel_t *data = (ws2812bpixel_t *)(rgbdev->control.dataptr);
	leddata_t *wsled = NULL;
	ws2812b_t *wsdata = NULL;	

	uint c;
	for(c = 0 ; c < leddatanum ; c++)
	{
		wsled = (leddata_t *)leds;		
		wsdata = (ws2812b_t *)data;
		
		wsdata->r7 = wsled->r0;
		wsdata->r6 = wsled->r1;
		wsdata->r5 = wsled->r2;	
		wsdata->r4 = wsled->r3;
		wsdata->r3 = wsled->r4;
		wsdata->r2 = wsled->r5;
		wsdata->r1 = wsled->r6;
		wsdata->r0 = wsled->r7;
		wsdata->g7 = wsled->g0;
		wsdata->g6 = wsled->g1;
		wsdata->g5 = wsled->g2;
		wsdata->g4 = wsled->g3;
		wsdata->g3 = wsled->g4;
		wsdata->g2 = wsled->g5;
		wsdata->g1 = wsled->g6;
		wsdata->g0 = wsled->g7;
		wsdata->b7 = wsled->b0;
		wsdata->b6 = wsled->b1;
		wsdata->b5 = wsled->b2;
		wsdata->b4 = wsled->b3;
		wsdata->b3 = wsled->b4;
		wsdata->b2 = wsled->b5;
		wsdata->b1 = wsled->b6;
		wsdata->b0 = wsled->b7;
		
		leds++;
		data++;
	} 
}

static void IntHandler(rgbdev_t *rgbdev)
{
	rgbledctrl_t *control = &(rgbdev->control);
	rgbledcfg_t *config =  &(rgbdev->config);
	volatile udmach_t *dma_txpri = control->dma_txpri;
	volatile udmach_t *dma_txalt = control->dma_txalt;
	volatile udmach_t *dma_rxpri = control->dma_rxpri;
	volatile udmach_t *dma_rxalt = control->dma_rxalt;
	NVIC_DISABLE(control->ssiint);
	control->counter++;
	
	if (control->ssi->mis & SSI_MIS_RXMIS_FIFOHALFEMPTY){
		UDMA->chis |= UDMA_CHANNEL(control->dma_rxch);
		UDMA->enaset |= UDMA_CHANNEL(control->dma_rxch);
	}

	if((dma_rxpri->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
	{	
		dma_rxpri->control = control->dma_rxchcfg;
	}

	if((dma_rxalt->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
	{	
		dma_rxalt->control = control->dma_rxchcfg;
	}
	

	if(config->xfertype == XFERTYPE_SINGLE)
	{
		if((dma_txpri->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
		{
			if(!control->xfercomplete)
			{
				switch(config->lednum - control->xfer_c)
				{
					default:
						dma_txpri->source = (uint32_t)(control->dataptr) + (control->xferbs - 1);
						dma_txpri->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE(control->xferbs);
						control->xfer_c++;
						break;
					case 1:
						control->xfercomplete++;
						if(config->ledtype == LEDTYPE_APA102)
						{
							dma_txpri->source = (uint32_t)&(config->eframe) + 3;
							dma_txpri->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE(4);
						}

						break;
				}
				UDMA->chis |= UDMA_CHANNEL(control->dma_txch);
				UDMA->enaset |= UDMA_CHANNEL(control->dma_txch);
			}
		}

		if((dma_txalt->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
		{
			if(!control->xfercomplete)
			{
				switch(config->lednum - control->xfer_c)
				{
					default:
						dma_txalt->source = (uint32_t)(control->dataptr) + (control->xferbs - 1);
						dma_txalt->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE(control->xferbs);
						control->xfer_c++;
						break;
					case 1:
						control->xfercomplete++;
						if(config->ledtype == LEDTYPE_APA102)
						{
							dma_txalt->source = (uint32_t)&(config->eframe) + 3;
							dma_txalt->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE(4);
						}
						break;
				}
				UDMA->chis |= UDMA_CHANNEL(control->dma_txch);
				UDMA->enaset |= UDMA_CHANNEL(control->dma_txch);
			}
		}
	}
	
	if(config->xfertype == XFERTYPE_BUFFER)
	{
		if((dma_txpri->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
		{
			if(!control->xfercomplete)
			{	
				switch(control->xferqty - control->xfer_c)
				{
					default:
						control->xfer_c++;
						dma_txpri->source = (uint32_t)(control->dataptr 
													+ (control->xferbs * (control->xfer_c + 1)) 
													- 1);
						dma_txpri->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE((control->xferbs));
						break;
					case 2:
						control->xfer_c++; 
						dma_txpri->source = (uint32_t)(control->dataptr 
													+ (control->xferbs * control->xfer_c) 
													+ (control->xfersize % control->xferbs) 
													- 1);
						dma_txpri->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE((control->xfersize % control->xferbs));
						break;
					case 1:
						control->xfercomplete++;
						if(config->ledtype == LEDTYPE_APA102)
						{
							dma_txpri->source = (uint32_t)&(config->eframe) + 3;
							dma_txpri->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE(4);
						}
						break;
				}
				UDMA->chis |= UDMA_CHANNEL(control->dma_txch);
				UDMA->enaset |= UDMA_CHANNEL(control->dma_txch);
			}
		}

		if((dma_txalt->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
		{
			if(!control->xfercomplete)
			{	
				switch(control->xferqty - control->xfer_c)
				{
					default:
						control->xfer_c++;
						dma_txalt->source = (uint32_t)(control->dataptr 
													+ (control->xferbs * (control->xfer_c + 1)) 
													- 1);
						dma_txalt->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE((control->xferbs));
						break;
					case 2:
						control->xfer_c++;
						dma_txalt->source = (uint32_t)(control->dataptr 
													+ (control->xferbs * control->xfer_c) 
													+ (control->xfersize % control->xferbs) 
													- 1);
						dma_txalt->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE((control->xfersize % control->xferbs));
						break;
					case 1:
						control->xfercomplete++;
						if(config->ledtype == LEDTYPE_APA102)
						{
							dma_txalt->source = (uint32_t)&(config->eframe) + 3;
							dma_txalt->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE(4);
						}
						break;
				}
				UDMA->chis |= UDMA_CHANNEL(control->dma_txch);
				UDMA->enaset |= UDMA_CHANNEL(control->dma_txch);
			}	
		}
	}
	NVIC_ENABLE(control->ssiint);
}

void SSI0IntHandler(void)
{ 	
	IntHandler(&ssi0_rgbdev);
}

void SSI1IntHandler(void)
{	
	IntHandler(&ssi1_rgbdev);
}


static void RGBSingle(rgbdev_t *rgbdev)
{	
	rgbledctrl_t *control = &(rgbdev->control);
	rgbledcfg_t *config = &(rgbdev->config);
	volatile udmach_t *dma_txpri = control->dma_txpri;
	volatile udmach_t *dma_txalt = control->dma_txalt;
	

	control->xfercomplete = 0;
	
	switch(config->ledtype)
	{
		case LEDTYPE_WS2812B:
			control->xfer_c = 1;
			/*	UDMA TX configuration	*/
			dma_txpri->source = (uint32_t)(control->dataptr) + (control->xferbs - 1);
			dma_txpri->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE(control->xferbs);

			switch(rgbdev->config.lednum - control->xfer_c)
			{			
				case 0:
					control->xfercomplete++;
					break;
				default:
					dma_txalt->source = (uint32_t)(control->dataptr) + (control->xferbs - 1);
					dma_txalt->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE(control->xferbs);
					break;
			}
			break;

	 	case LEDTYPE_APA102:
	 		control->xfer_c = 0;
	
	 		dma_txpri->source = (uint32_t)&(config->sframe) + 3; //(control->xferbs - 1);
			dma_txpri->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE(4);

			dma_txalt->source = (uint32_t)(control->dataptr) + (control->xferbs - 1);
			dma_txalt->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE(control->xferbs);
			
			break;

	}
}


static void RGBBuffer(rgbdev_t *rgbdev)
{
	rgbledctrl_t *control = &(rgbdev->control);
	rgbledcfg_t *config = &(rgbdev->config);

	volatile udmach_t *dma_txpri = control->dma_txpri;
	volatile udmach_t *dma_txalt = control->dma_txalt;
	
	control->xfercomplete = 0;
	
	switch(config->ledtype)
	{
		case LEDTYPE_WS2812B:
			control->xfer_c = 0;
			switch(control->xferqty)
			{
				case 1: 
					dma_txpri->source = (uint32_t)control->dataptr + ((control->xfersize % control->xferbs) - 1);
					dma_txpri->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE((control->xfersize % control->xferbs));
					control->xfercomplete++;
					break;
				case 2:
					dma_txpri->source = (uint32_t)control->dataptr + (control->xferbs - 1);
					dma_txpri->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE(control->xferbs);
					
					dma_txalt->source = (uint32_t)control->dataptr + control->xferbs + (control->xfersize % control->xferbs) - 1;
					dma_txalt->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE((control->xfersize % control->xferbs));
					break;
				default:
					dma_txpri->source = (uint32_t)control->dataptr + (control->xferbs - 1);
					dma_txpri->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE(control->xferbs);

					dma_txalt->source = (uint32_t)control->dataptr + ((control->xferbs * 2) - 1);
					dma_txalt->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE(control->xferbs);
					break;
			}
			break;	
	
		case LEDTYPE_APA102:
			dma_txpri->source = (uint32_t)&(config->sframe) + 3; //(control->xferbs - 1);
			dma_txpri->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE(4);
	 	
			control->xfer_c = 0;
			switch(control->xferqty)
			{
				case 1: 
					dma_txalt->source = (uint32_t)control->dataptr + ((control->xfersize % control->xferbs) - 1);
					dma_txalt->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE((control->xfersize % control->xferbs));
					//control->xfercomplete++;
					break;
				
				default:
					dma_txalt->source = (uint32_t)control->dataptr + (control->xferbs - 1);
					dma_txalt->control = control->dma_txchcfg | UDMA_CHCTL_XFERSIZE(control->xferbs);
					break;
			}
			break;
	}
}


void rgbledTerminate(rgbdev_t *rgbdev)
{
	rgbledctrl_t *control = &(rgbdev->control);
	rgbledcfg_t *config =  &(rgbdev->config);
	
	/* Disabling SSI */
	NVIC_DISABLE(control->ssiint);
	cc2538SSIMTerminate(control->ssi);
	control->ssi = NULL;
	control->ssiint = 0x0;

	/* Disabling DMA */
	UDMA->enaclr |= UDMA_CHANNEL(control->dma_txch);
	UDMA->enaclr |= UDMA_CHANNEL(control->dma_rxch);
	control->dma_txch = 0x0;
	control->dma_rxch = 0x0;
	control->dma_txchcfg = 0x0;
	control->dma_rxchcfg = 0x0;
	control->dma_txpri = NULL;
	control->dma_txalt = NULL;
	control->dma_rxpri = NULL;
	control->dma_rxalt = NULL;
	
	/* Deallocating memory */
	free(rgbdev->control.dataptr);
	free(rgbdev->leds);

	*((uint32_t *)config) = 0x0;
	*((uint32_t *)control) = 0x0;
}

rgbdev_t *rgbledInit(uint32_t config, uint16_t lednum, uint32_t frequency)
//rgbdev_t *cc2538RGBInit(volatile ssi_t *ssi, uint32_t lednum, uint8_t leddatasize, xfertype_t xfertype, void *data, uint32_t frequency)
{
	rgbdev_t *rgbdev;

	if(config & RGBLEDCFG_XFERSSI_SSI1)
	{
		rgbdev = &ssi1_rgbdev;
		rgbdev->control.ssi = SSI1;
		rgbdev->control.ssiint = INT_SSI1;
		rgbdev->control.dma_txch = SSI1_TX_UDMACH;
		rgbdev->control.dma_rxch = SSI1_RX_UDMACH;
	}
	else
	{
		rgbdev = &ssi0_rgbdev;
		rgbdev->control.ssi = SSI0;
		rgbdev->control.ssiint = INT_SSI0;
		rgbdev->control.dma_txch = SSI0_TX_UDMACH;
		rgbdev->control.dma_rxch = SSI0_RX_UDMACH;
	}
	
	*((uint32_t *)&rgbdev->config) = config;
	rgbdev->config.lednum = lednum;
	rgbdev->config.sframe = 0x0;
	rgbdev->config.eframe = 0xFFFFFFFF;
	rgbdev->config.leddatasize = 	rgbdev->config.ledtype == LEDTYPE_WS2812B ?
									sizeof(ws2812bpixel_t) : 
									sizeof(apa102pixel_t);

	rgbdev->control.xfersize = rgbdev->config.leddatasize * lednum; /* BYTES */
	rgbdev->control.dma_txpri = ((udmach_t *)&dmactrl[rgbdev->control.dma_txch]);
	rgbdev->control.dma_txalt = ((udmach_t *)&dmactrl[32 + rgbdev->control.dma_txch]);
	rgbdev->control.dma_rxpri = ((udmach_t *)&dmactrl[rgbdev->control.dma_rxch]);
	rgbdev->control.dma_rxalt = ((udmach_t *)&dmactrl[32 + rgbdev->control.dma_rxch]);
	rgbdev->control.dma_txchcfg = UDMA_CHCTL_DSTINC_NOINC |
									UDMA_CHCTL_DSTSIZE_8BIT |
									UDMA_CHCTL_SRCINC_8BIT |
									UDMA_CHCTL_SRCSIZE_8BIT |
									UDMA_CHCTL_ARBSIZE_4 | 
									UDMA_CHCTL_XFERMODE_PINGPONG;

	rgbdev->control.dma_rxchcfg = UDMA_CHCTL_DSTINC_NOINC |
									UDMA_CHCTL_DSTSIZE_8BIT |
									UDMA_CHCTL_SRCINC_NOINC |
									UDMA_CHCTL_SRCSIZE_8BIT |
									UDMA_CHCTL_ARBSIZE_4 | 
									UDMA_CHCTL_XFERMODE_PINGPONG |
									UDMA_CHCTL_XFERSIZE(1024);

	if(rgbdev->config.xfertype == XFERTYPE_BUFFER)
	{

		rgbdev->control.xferqty = 	(rgbdev->control.xfersize / 1024) + 1; /* BIGGER CHUNKS */
		rgbdev->control.xferbs = 	rgbdev->control.xferqty > 0 ? 
							(1024 / rgbdev->config.leddatasize) * rgbdev->config.leddatasize : 
							rgbdev->control.xfersize;
		switch(rgbdev->config.ledtype)
		{
			case LEDTYPE_APA102:
				rgbdev->control.dataptr = calloc(rgbdev->config.lednum, sizeof(apa102pixel_t));
				rgbdev->leds = rgbdev->control.dataptr;
				break;

			case LEDTYPE_WS2812B:
				rgbdev->control.dataptr = calloc(rgbdev->config.lednum, sizeof(ws2812bpixel_t));
				rgbdev->leds = calloc(rgbdev->config.lednum, sizeof(led_t));
				break;
		}	
	}
	else
	{
		rgbdev->control.xferqty = lednum; /* One LED per DMA transfer. */
		rgbdev->control.xferbs = rgbdev->config.leddatasize;
		
		switch(rgbdev->config.ledtype)
		{
			case LEDTYPE_APA102:
				rgbdev->control.dataptr = calloc(1, sizeof(apa102pixel_t));
				rgbdev->leds = rgbdev->control.dataptr;
				break;

			case LEDTYPE_WS2812B:
				rgbdev->control.dataptr = calloc(1, sizeof(ws2812bpixel_t));
				rgbdev->leds = calloc(1, sizeof(led_t));
				break;
		}	
	}

	/* Validate memory allocation and SSI availability. */
	if((rgbdev->control.dataptr == NULL) || 
		(rgbdev->leds == NULL) ||
		(rgbdev->control.ssi->cr1 & SSI_CR1_SSE_ENABLESSI))
	{
		rgbledTerminate(rgbdev);
		return NULL;
	}

	if(rgbdev->config.ledtype == LEDTYPE_WS2812B)
	{	
		uint16_t *memset = (uint16_t *)(rgbdev->control.dataptr);
		uint32_t memsize = rgbdev->config.xfertype == XFERTYPE_BUFFER ?
							(rgbdev->config.lednum * sizeof(ws2812bpixel_t)) :
							sizeof(ws2812bpixel_t);

		uint16_t c;
		for(c = 0 ; c < (memsize / 6) ; c++)
		{
			*memset = 0x4992;
			*(memset + 1) = 0x9224;
			*(memset + 2) = 0x2449;
			memset += 3;
		}

		switch(memsize % 6)
		{
			case 1:
				*((uint8_t *)memset) = 0x92;
				break;
			case 2:
				*memset = 0x4992;
				break;
			case 3:
				*memset = 0x4992;
				*((uint8_t *)(memset+1)) = 0x24;
				break;
			case 4:
				*memset = 0x4992;
				*(memset + 1) = 0x9224;
				break;
			case 5:
				*memset = 0x4992;
				*(memset + 1) = 0x9224;
				*((uint8_t *)(memset+2)) = 0x49;
				break;
		}
	}
	


	cc2538SSIMInit(rgbdev->control.ssi, frequency, 
		SSI_CR0_FRF_MOTOROLASPIFORMAT | 
		SSI_CR0_SPH_CLOCKONSECONDEDGE |
		SSI_CR0_SPO_CLOCKHIGHWHILEIDLE |  
		SSI_CR0_DSS_8BITDATA,
		0x0, 
		SSI_IM_RXIM_FIFOHALFEMPTY, 
		SSI_DMATCL_TXDMAE_UDMAENABLE | 
		SSI_DMATCL_RXDMAE_UDMAENABLE);
	NVIC_ENABLE(rgbdev->control.ssiint);
	

	cc2538DmaInit();
	UDMA->prioclr |= UDMA_CHANNEL(rgbdev->control.dma_txch);
	UDMA->prioclr |= UDMA_CHANNEL(rgbdev->control.dma_rxch);
	UDMA->useburstclr |= UDMA_CHANNEL(rgbdev->control.dma_txch);
	UDMA->useburstclr |= UDMA_CHANNEL(rgbdev->control.dma_rxch);
	UDMA->reqmaskclr |= UDMA_CHANNEL(rgbdev->control.dma_txch);
	UDMA->reqmaskclr |= UDMA_CHANNEL(rgbdev->control.dma_rxch);	

	rgbdev->control.dma_rxpri->source = (uint32_t)&(rgbdev->control.ssi->dr);
	rgbdev->control.dma_rxpri->destination = (uint32_t)&dummy_buf;
	
	rgbdev->control.dma_rxalt->source = (uint32_t)&(rgbdev->control.ssi->dr);
	rgbdev->control.dma_rxalt->destination = (uint32_t)&dummy_buf;
	
	rgbdev->control.dma_txpri->destination = (uint32_t)&(rgbdev->control.ssi->dr);
	rgbdev->control.dma_txalt->destination = (uint32_t)&(rgbdev->control.ssi->dr);
							
	return rgbdev;
}

		
void rgbledRender(rgbdev_t *rgbdev)
{	
	/*	Stop UDMA channels	*/
	UDMA->enaclr |= UDMA_CHANNEL(rgbdev->control.dma_txch);
	UDMA->enaclr |= UDMA_CHANNEL(rgbdev->control.dma_rxch);
	UDMA->altclr |= UDMA_CHANNEL(rgbdev->control.dma_txch);
	UDMA->altclr |= UDMA_CHANNEL(rgbdev->control.dma_rxch);
	
	/*	UDMA RX configuration	*/
	rgbdev->control.dma_rxpri->control = rgbdev->control.dma_rxchcfg;
	rgbdev->control.dma_rxalt->control = rgbdev->control.dma_rxchcfg;
	rgbdev->control.counter = 0;
	
	if(rgbdev->config.ledtype == LEDTYPE_WS2812B)
		ws2812bconv(rgbdev);

	if (rgbdev->config.xfertype == XFERTYPE_SINGLE)
	{
		RGBSingle(rgbdev);
	}
	else
	{
		RGBBuffer(rgbdev);
	}
	
	UDMA->enaset |= UDMA_CHANNEL(rgbdev->control.dma_txch);
	UDMA->enaset |= UDMA_CHANNEL(rgbdev->control.dma_rxch);
}




void rgbledRenderColor(uint8_t r, uint8_t g, uint8_t b, rgbdev_t *rgbdev)
{	
	rgbledcfg_t *config = &(rgbdev->config);
	
	switch(config->ledtype)
	{
		case LEDTYPE_WS2812B:
			((led_t *)(rgbdev->leds))[0].r = r;
			((led_t *)(rgbdev->leds))[0].g = g;
			((led_t *)(rgbdev->leds))[0].b = b;
			break;

		case LEDTYPE_APA102:
			((apa102pixel_t *)(rgbdev->leds))[0].r = r;
			((apa102pixel_t *)(rgbdev->leds))[0].g = g;
			((apa102pixel_t *)(rgbdev->leds))[0].b = b;
			break;

	}
	rgbledRender(rgbdev);

}



