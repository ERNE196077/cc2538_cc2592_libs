#include <stdint.h>
#include <stdio.h>

#include "../include/cc2538-reg.h"
#include "../include/ssim.h"
#include "../include/udma.h"
#include "../include/ws2812b.h"
#include "../include/gpio.h"
#include "../include/sys_ctrl.h"

#include <openthread/cli.h>

#define SSI0_PIN_OUT_CLK	IOCMUX->ioc_pb2_sel = IOC_MUX_OUT_SEL_SSI0_CLK_OUT; IOCMUX->ioc_pb2_over = IOC_OVERRIDE_OE  
#define SSI0_PIN_OUT_TXD	IOCMUX->ioc_pb3_sel = IOC_MUX_OUT_SEL_SSI0_TXD; IOCMUX->ioc_pb3_over = IOC_OVERRIDE_OE 
#define SSI0_PIN_OUT_FSS	IOCMUX->ioc_pb4_sel = IOC_MUX_OUT_SEL_SSI0_FSS_OUT; IOCMUX->ioc_pb4_over = IOC_OVERRIDE_OE  
#define SSI0_PIN_IN_RXD		IOCMUX->ioc_ssirxd_ssi0 = IOC_PB5; IOCMUX->ioc_pb5_over = IOC_OVERRIDE_DIS; 
#define SSI0_PINS_AFSEL 	GPIO_B->afsel = (GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5)

#define SSI1_PIN_OUT_CLK	IOCMUX->ioc_pd0_sel = IOC_MUX_OUT_SEL_SSI0_CLK_OUT; IOCMUX->ioc_pd0_over = IOC_OVERRIDE_OE  
#define SSI1_PIN_OUT_TXD	IOCMUX->ioc_pd1_sel = IOC_MUX_OUT_SEL_SSI0_TXD; IOCMUX->ioc_pd1_over = IOC_OVERRIDE_OE 
#define SSI1_PIN_OUT_FSS	IOCMUX->ioc_pd2_sel = IOC_MUX_OUT_SEL_SSI0_FSS_OUT; IOCMUX->ioc_pd2_over = IOC_OVERRIDE_OE  
#define SSI1_PIN_IN_RXD		IOCMUX->ioc_ssirxd_ssi1 = IOC_PD3; IOCMUX->ioc_pd3_over = IOC_OVERRIDE_DIS; 
#define SSI1_PINS_AFSEL 	GPIO_D->afsel = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)

#define SSI0_RX_UDMACH		10
#define SSI0_TX_UDMACH		11
#define SSI1_RX_UDMACH		24
#define SSI1_TX_UDMACH		25

uint16_t dummy_buf;
uint16_t dummy_c = 0;
extern volatile ch_ctrl_t dmactrl[64];


typedef struct
{
	uint16_t numled;					/*	NUMBER OF LEDs	*/
	uint32_t *data_ptr;					/*	POINTER TO DATA	*/
	ws2812b_xfertype_t xfer_type;		/*	SINGLE COLOR OR BUFFER 	*/
	uint8_t xfer_complete;				/*	COMPLETION FLAG	*/
	uint16_t xfer_c;					/*	TRANSFER COUNTER	*/
	uint16_t xfer_qty;					/* 	QUANTITY OF TRANSFERS	*/
	uint32_t xfer_size;					/* 	TOTAL TRANSFER SIZE (BYTES)	*/
	uint32_t xfer_addr;					/*	ADDRESS FOR CURRENT DATA 	*/
	uint16_t xfer_bs;					/*	SIZE OF EACH TRANSFER 	*/
	uint8_t dma_txch;
	uint8_t dma_rxch;
	volatile ch_ctrl_t *dma_txpri;
	volatile ch_ctrl_t *dma_txalt;
	volatile ch_ctrl_t *dma_rxpri;
	volatile ch_ctrl_t *dma_rxalt;
}ws2812b_dev_t;



static ws2812b_dev_t ssi0_ws2812b;
static ws2812b_dev_t ssi1_ws2812b;

extern void SSI0IntHandler(void);
extern void SSI1IntHandler(void);

void SSI0IntHandler(void)
{ 	

	if (SSI0->mis & SSI_MIS_RXMIS_FIFOHALFEMPTY){
		UDMA->chis |= UDMA_CHANNEL(SSI0_RX_UDMACH);
		UDMA->enaset |= UDMA_CHANNEL(SSI0_RX_UDMACH);

	}

	dummy_c++;
	if(ssi0_ws2812b.xfer_type == WS2812B_SINGLE)
	{
		if(!ssi0_ws2812b.xfer_complete)
		{
			if((ssi0_ws2812b.dma_txpri->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
			{
				switch(ssi0_ws2812b.numled - ssi0_ws2812b.xfer_c)
				{
					default:
						ssi0_ws2812b.dma_txpri->source = (uint32_t)ssi0_ws2812b.data_ptr;
						ssi0_ws2812b.dma_txpri->destination = (uint32_t)&(SSI0->dr);
						ssi0_ws2812b.dma_txpri->control = UDMA_CHCTL_DSTINC_NOINC |
												 UDMA_CHCTL_DSTSIZE_8BIT |
												 UDMA_CHCTL_SRCINC_8BIT |
												 UDMA_CHCTL_SRCSIZE_8BIT |
												 UDMA_CHCTL_ARBSIZE_4 | 
												 UDMA_CHCTL_XFERMODE_PINGPONG |
												 UDMA_CHCTL_XFERSIZE(9);
						ssi0_ws2812b.xfer_c++;
						break;
					case 1:
						ssi0_ws2812b.xfer_complete++;
						break;
				}
			}

			if((ssi0_ws2812b.dma_txalt->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
			{
				switch(ssi0_ws2812b.numled - ssi0_ws2812b.xfer_c)
				{
					default:
						ssi0_ws2812b.dma_txalt->source = (uint32_t)ssi0_ws2812b.data_ptr;
						ssi0_ws2812b.dma_txalt->destination = (uint32_t)&(SSI0->dr);
						ssi0_ws2812b.dma_txalt->control = UDMA_CHCTL_DSTINC_NOINC |
												 UDMA_CHCTL_DSTSIZE_8BIT |
												 UDMA_CHCTL_SRCINC_8BIT |
												 UDMA_CHCTL_SRCSIZE_8BIT |
												 UDMA_CHCTL_ARBSIZE_4 | 
												 UDMA_CHCTL_XFERMODE_PINGPONG |
												 UDMA_CHCTL_XFERSIZE(9);
						ssi0_ws2812b.xfer_c++;
						break;
					case 1:
						ssi0_ws2812b.xfer_complete++;
						break;
				}
			}

		}
		else
		{
			UDMA->enaclr |= UDMA_CHANNEL(SSI0_TX_UDMACH);
			ssi0_ws2812b.dma_txpri->control &= ~UDMA_CHCTL_XFERMODE_MASK;
			UDMA->chis |= UDMA_CHANNEL(SSI0_TX_UDMACH);
			//NVIC_DISABLE(INT_SSI0);
	

		}

		if((ssi0_ws2812b.dma_rxpri->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
		{
			//UDMA->enaclr |= UDMA_CHANNEL(SSI0_RX_UDMACH);
			//ssi0_ws2812b.dma_rxpri->control &= ~UDMA_CHCTL_XFERMODE_MASK;
			//UDMA->chis |= UDMA_CHANNEL(SSI0_RX_UDMACH);
			
			// ssi0_ws2812b.dma_rxpri->source = (uint32_t)&(SSI0->dr);
			// ssi0_ws2812b.dma_rxpri->destination = (uint32_t)&dummy_buf;
			// ssi0_ws2812b.dma_rxpri->control = UDMA_CHCTL_DSTINC_NOINC |
			// 						 UDMA_CHCTL_DSTSIZE_8BIT |
			// 						 UDMA_CHCTL_SRCINC_NOINC |
			// 						 UDMA_CHCTL_SRCSIZE_8BIT |
			// 						 UDMA_CHCTL_ARBSIZE_4 | 
			// 						 UDMA_CHCTL_XFERMODE_PINGPONG |
			// 						 UDMA_CHCTL_XFERSIZE(9);
		}

		// if((ssi0_ws2812b.dma_rxalt->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
		// {
		// 	ssi0_ws2812b.dma_rxalt->source = (uint32_t)&(SSI0->dr);
		// 	ssi0_ws2812b.dma_rxalt->destination = (uint32_t)&dummy_buf;
		// 	ssi0_ws2812b.dma_rxalt->control = UDMA_CHCTL_DSTINC_NOINC |
		// 							 UDMA_CHCTL_DSTSIZE_8BIT |
		// 							 UDMA_CHCTL_SRCINC_NOINC |
		// 							 UDMA_CHCTL_SRCSIZE_8BIT |
		// 							 UDMA_CHCTL_ARBSIZE_4 | 
		// 							 UDMA_CHCTL_XFERMODE_PINGPONG |
		// 							 UDMA_CHCTL_XFERSIZE(9);
		// }
	}
	else
	{
		if(!ssi0_ws2812b.xfer_complete)
		{	
			if((ssi0_ws2812b.dma_txpri->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
			{
				switch(ssi0_ws2812b.xfer_qty - ssi0_ws2812b.xfer_c)
				{
					default:
						ssi0_ws2812b.xfer_c++;
						ssi0_ws2812b.dma_txpri->source = (uint32_t)(ssi0_ws2812b.data_ptr + (ssi0_ws2812b.xfer_bs * ssi0_ws2812b.xfer_c));
						ssi0_ws2812b.dma_txpri->destination = (uint32_t)&(SSI0->dr);
						ssi0_ws2812b.dma_txpri->control = UDMA_CHCTL_DSTINC_NOINC |
												 UDMA_CHCTL_DSTSIZE_8BIT |
												 UDMA_CHCTL_SRCINC_8BIT |
												 UDMA_CHCTL_SRCSIZE_8BIT |
												 UDMA_CHCTL_ARBSIZE_4 | 
												 UDMA_CHCTL_XFERMODE_PINGPONG |
												 UDMA_CHCTL_XFERSIZE(1017);
						break;
					case 2:
						ssi0_ws2812b.xfer_c++;
						ssi0_ws2812b.dma_txpri->source = (uint32_t)(ssi0_ws2812b.data_ptr + (ssi0_ws2812b.xfer_bs * ssi0_ws2812b.xfer_c));
						ssi0_ws2812b.dma_txpri->destination = (uint32_t)&(SSI0->dr);
						ssi0_ws2812b.dma_txpri->control = UDMA_CHCTL_DSTINC_NOINC |
												 UDMA_CHCTL_DSTSIZE_8BIT |
												 UDMA_CHCTL_SRCINC_8BIT |
												 UDMA_CHCTL_SRCSIZE_8BIT |
												 UDMA_CHCTL_ARBSIZE_4 | 
												 UDMA_CHCTL_XFERMODE_PINGPONG |
												 UDMA_CHCTL_XFERSIZE((ssi0_ws2812b.xfer_size % 1017));
						break;
					case 1:
						ssi0_ws2812b.xfer_complete++;
						break;
				}
			}

			if((ssi0_ws2812b.dma_txalt->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
			{
				switch(ssi0_ws2812b.xfer_qty - ssi0_ws2812b.xfer_c)
				{
					default:
						ssi0_ws2812b.xfer_c++;
						ssi0_ws2812b.dma_txalt->source = (uint32_t)(ssi0_ws2812b.data_ptr + (ssi0_ws2812b.xfer_bs * ssi0_ws2812b.xfer_c));
						ssi0_ws2812b.dma_txalt->destination = (uint32_t)&(SSI0->dr);
						ssi0_ws2812b.dma_txalt->control = UDMA_CHCTL_DSTINC_NOINC |
												 UDMA_CHCTL_DSTSIZE_8BIT |
												 UDMA_CHCTL_SRCINC_8BIT |
												 UDMA_CHCTL_SRCSIZE_8BIT |
												 UDMA_CHCTL_ARBSIZE_4 | 
												 UDMA_CHCTL_XFERMODE_PINGPONG |
												 UDMA_CHCTL_XFERSIZE(1017);
						break;
					case 2:
						ssi0_ws2812b.xfer_c++;
						ssi0_ws2812b.dma_txalt->source = (uint32_t)(ssi0_ws2812b.data_ptr + (ssi0_ws2812b.xfer_bs * ssi0_ws2812b.xfer_c));
						ssi0_ws2812b.dma_txalt->destination = (uint32_t)&(SSI0->dr);
						ssi0_ws2812b.dma_txalt->control = UDMA_CHCTL_DSTINC_NOINC |
												 UDMA_CHCTL_DSTSIZE_8BIT |
												 UDMA_CHCTL_SRCINC_8BIT |
												 UDMA_CHCTL_SRCSIZE_8BIT |
												 UDMA_CHCTL_ARBSIZE_4 | 
												 UDMA_CHCTL_XFERMODE_PINGPONG |
												 UDMA_CHCTL_XFERSIZE((ssi0_ws2812b.xfer_size % 1017));
						break;
					case 1:
						ssi0_ws2812b.xfer_complete++;
						break;
				}
			}

			if((ssi0_ws2812b.dma_rxpri->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
			{
				ssi0_ws2812b.dma_rxpri->source = (uint32_t)&(SSI0->dr);
				ssi0_ws2812b.dma_rxpri->destination = (uint32_t)&dummy_buf;
				ssi0_ws2812b.dma_rxpri->control = UDMA_CHCTL_DSTINC_NOINC |
										 UDMA_CHCTL_DSTSIZE_8BIT |
										 UDMA_CHCTL_SRCINC_NOINC |
										 UDMA_CHCTL_SRCSIZE_8BIT |
										 UDMA_CHCTL_ARBSIZE_4 | 
										 UDMA_CHCTL_XFERMODE_PINGPONG |
										 UDMA_CHCTL_XFERSIZE(1017);
			}

			if((ssi0_ws2812b.dma_rxalt->control & 0x7) == UDMA_CHCTL_XFERMODE_STOP)
			{
				ssi0_ws2812b.dma_rxalt->source = (uint32_t)&(SSI0->dr);
				ssi0_ws2812b.dma_rxalt->destination = (uint32_t)&dummy_buf;
				ssi0_ws2812b.dma_rxalt->control = UDMA_CHCTL_DSTINC_NOINC |
										 UDMA_CHCTL_DSTSIZE_8BIT |
										 UDMA_CHCTL_SRCINC_NOINC |
										 UDMA_CHCTL_SRCSIZE_8BIT |
										 UDMA_CHCTL_ARBSIZE_4 | 
										 UDMA_CHCTL_XFERMODE_PINGPONG |
										 UDMA_CHCTL_XFERSIZE(1017);
			}		
		}
		else
		{
			//SSI0->cr1 &= ~SSI_CR1_SSE_ENABLESSI;
		}
	}


}

void SSI1IntHandler(void)
{

}


static void Ws2812bArmSingle(volatile ssi_t *SSI)
{
	ws2812b_dev_t *ws2812b_dev = SSI == SSI0 ? &ssi0_ws2812b : &ssi1_ws2812b;
	ws2812b_dev->xfer_complete = 0;
	ws2812b_dev->xfer_c = 1;
	
	
	/*	UDMA configuration	*/
	UDMA->enaclr |= UDMA_CHANNEL(ws2812b_dev->dma_txch);
	UDMA->altclr |= UDMA_CHANNEL(ws2812b_dev->dma_txch);
	UDMA->prioclr |= UDMA_CHANNEL(ws2812b_dev->dma_txch);
	UDMA->useburstclr |= UDMA_CHANNEL(ws2812b_dev->dma_txch);
	UDMA->reqmaskclr |= UDMA_CHANNEL(ws2812b_dev->dma_txch);
	
	UDMA->enaclr |= UDMA_CHANNEL(ws2812b_dev->dma_rxch);
	UDMA->altclr |= UDMA_CHANNEL(ws2812b_dev->dma_rxch);
	UDMA->prioclr |= UDMA_CHANNEL(ws2812b_dev->dma_rxch);
	UDMA->useburstclr |= UDMA_CHANNEL(ws2812b_dev->dma_rxch);
	UDMA->reqmaskclr |= UDMA_CHANNEL(ws2812b_dev->dma_rxch);
	
	/*	UDMA TX conSSI->dr = 0x99;figuration	*/
	ws2812b_dev->dma_txpri->source = (uint32_t)ws2812b_dev->data_ptr + 8;
	ws2812b_dev->dma_txpri->destination = (uint32_t)&(SSI->dr);
	ws2812b_dev->dma_txpri->control = UDMA_CHCTL_DSTINC_NOINC |
									 UDMA_CHCTL_DSTSIZE_8BIT |
									 UDMA_CHCTL_SRCINC_8BIT |
									 UDMA_CHCTL_SRCSIZE_8BIT |
									 UDMA_CHCTL_ARBSIZE_4 | 
									 UDMA_CHCTL_XFERMODE_PINGPONG |
									 UDMA_CHCTL_XFERSIZE(9);
	


	/*	UDMA RX configuration	*/
	ws2812b_dev->dma_rxpri->source = (uint32_t)&(SSI->dr);
	ws2812b_dev->dma_rxpri->destination = (uint32_t)&dummy_buf;
	ws2812b_dev->dma_rxpri->control = UDMA_CHCTL_DSTINC_NOINC |
									 UDMA_CHCTL_DSTSIZE_8BIT |
									 UDMA_CHCTL_SRCINC_NOINC |
									 UDMA_CHCTL_SRCSIZE_8BIT |
									 UDMA_CHCTL_ARBSIZE_1 | 
									 UDMA_CHCTL_XFERMODE_PINGPONG |
									 UDMA_CHCTL_XFERSIZE(9);

	switch(ws2812b_dev->numled - ws2812b_dev->xfer_c)
	{			
		case 0:
			ws2812b_dev->xfer_complete++;
			break;
		default:
			ws2812b_dev->dma_txalt->source = (uint32_t)ws2812b_dev->data_ptr + 8;
			ws2812b_dev->dma_txalt->destination = (uint32_t)&(SSI->dr);
			ws2812b_dev->dma_txalt->control = UDMA_CHCTL_DSTINC_NOINC |
									 UDMA_CHCTL_DSTSIZE_8BIT |
									 UDMA_CHCTL_SRCINC_8BIT |
									 UDMA_CHCTL_SRCSIZE_8BIT |
									 UDMA_CHCTL_ARBSIZE_4 | 
									 UDMA_CHCTL_XFERMODE_PINGPONG |
									 UDMA_CHCTL_XFERSIZE(9);
			
			ws2812b_dev->dma_rxalt->source = (uint32_t)&(SSI->dr);
			ws2812b_dev->dma_rxalt->destination = (uint32_t)&dummy_buf;
			ws2812b_dev->dma_rxalt->control = UDMA_CHCTL_DSTINC_NOINC |
									 UDMA_CHCTL_DSTSIZE_8BIT |
									 UDMA_CHCTL_SRCINC_NOINC |
									 UDMA_CHCTL_SRCSIZE_8BIT |
									 UDMA_CHCTL_ARBSIZE_4 | 
									 UDMA_CHCTL_XFERMODE_PINGPONG |
									 UDMA_CHCTL_XFERSIZE(9);
			break;
			

	}

	
	
	//UDMA->enaset |= UDMA_CHANNEL(ws2812b_dev->dma_txch);
	UDMA->enaset |= UDMA_CHANNEL(SSI0_RX_UDMACH);
	

}


static void Ws2812bArmBuffer(volatile ssi_t *SSI)
{
	ws2812b_dev_t *ws2812b_dev = SSI == SSI0 ? &ssi0_ws2812b : &ssi1_ws2812b;
	
	UDMA->enaclr |= UDMA_CHANNEL(ws2812b_dev->dma_txch) | UDMA_CHANNEL(ws2812b_dev->dma_rxch);
	UDMA->altclr |= UDMA_CHANNEL(ws2812b_dev->dma_txch) | UDMA_CHANNEL(ws2812b_dev->dma_rxch);
		
	ws2812b_dev->xfer_complete = 0;
	ws2812b_dev->xfer_c = 1;
		
	ws2812b_dev->dma_txpri->source = (uint32_t)ws2812b_dev->data_ptr;
	ws2812b_dev->dma_txpri->destination = (uint32_t)&(SSI->dr);
	ws2812b_dev->dma_txpri->control = 	UDMA_CHCTL_DSTINC_NOINC |
								UDMA_CHCTL_DSTSIZE_8BIT |
								UDMA_CHCTL_SRCINC_8BIT |
								UDMA_CHCTL_SRCSIZE_8BIT |
								UDMA_CHCTL_ARBSIZE_4 | 
								UDMA_CHCTL_XFERMODE_PINGPONG;
	
	ws2812b_dev->dma_rxpri->source = (uint32_t)&(SSI->dr);
	ws2812b_dev->dma_rxpri->destination = (uint32_t)&dummy_buf;
	ws2812b_dev->dma_rxpri->control = UDMA_CHCTL_DSTINC_NOINC |
							 UDMA_CHCTL_DSTSIZE_8BIT |
							 UDMA_CHCTL_SRCINC_NOINC |
							 UDMA_CHCTL_SRCSIZE_8BIT |
							 UDMA_CHCTL_ARBSIZE_4 | 
							 UDMA_CHCTL_XFERMODE_PINGPONG;
							 
	if(ws2812b_dev->xfer_qty - 1)
	{
		ws2812b_dev->dma_txpri->control |= UDMA_CHCTL_XFERSIZE(ws2812b_dev->xfer_bs);
		ws2812b_dev->dma_rxpri->control |= UDMA_CHCTL_XFERSIZE(ws2812b_dev->xfer_bs);
		
		ws2812b_dev->dma_txalt->source = (uint32_t)(ws2812b_dev->data_ptr + ws2812b_dev->xfer_bs);
		ws2812b_dev->dma_txalt->destination = (uint32_t)&(SSI->dr);
		ws2812b_dev->dma_txalt->control = UDMA_CHCTL_DSTINC_NOINC |
								 UDMA_CHCTL_DSTSIZE_8BIT |
								 UDMA_CHCTL_SRCINC_8BIT |
								 UDMA_CHCTL_SRCSIZE_8BIT |
								 UDMA_CHCTL_ARBSIZE_4 | 
								 UDMA_CHCTL_XFERMODE_PINGPONG;

		ws2812b_dev->dma_rxalt->source = (uint32_t)&(SSI->dr);
		ws2812b_dev->dma_rxalt->destination = (uint32_t)&dummy_buf;
		ws2812b_dev->dma_rxalt->control = UDMA_CHCTL_DSTINC_NOINC |
								 UDMA_CHCTL_DSTSIZE_8BIT |
								 UDMA_CHCTL_SRCINC_NOINC |
								 UDMA_CHCTL_SRCSIZE_8BIT |
								 UDMA_CHCTL_ARBSIZE_4 | 
								 UDMA_CHCTL_XFERMODE_PINGPONG;
		
		if(ws2812b_dev->xfer_qty - 2)
		{
			ws2812b_dev->dma_txalt->control |= UDMA_CHCTL_XFERSIZE(ws2812b_dev->xfer_bs);
			ws2812b_dev->dma_rxalt->control |= UDMA_CHCTL_XFERSIZE(ws2812b_dev->xfer_bs);
		}
		else
		{
			ws2812b_dev->dma_txalt->control |= UDMA_CHCTL_XFERSIZE((ws2812b_dev->xfer_size % ws2812b_dev->xfer_bs));
			ws2812b_dev->dma_rxalt->control |= UDMA_CHCTL_XFERSIZE((ws2812b_dev->xfer_size % ws2812b_dev->xfer_bs));
		}
	}
	else
	{
		ws2812b_dev->dma_txpri->control |= UDMA_CHCTL_XFERSIZE((ws2812b_dev->xfer_size % ws2812b_dev->xfer_bs));
		ws2812b_dev->dma_rxpri->control |= UDMA_CHCTL_XFERSIZE((ws2812b_dev->xfer_size % ws2812b_dev->xfer_bs));
		ws2812b_dev->xfer_complete++;
	}
	UDMA->enaset |= UDMA_CHANNEL(ws2812b_dev->dma_txch) | UDMA_CHANNEL(ws2812b_dev->dma_rxch);
}


/*	Default frame is Motorola SPI, need to implementvolatile ssi_t *SSI, uint32_t LEDNUM, void *DATA, ws2812b_xfertype_t XFERTYPE the other two	*/	
void cc2538WS2812BInit(volatile ssi_t *SSI, uint32_t numled, void *data, ws2812b_xfertype_t xfer_type)
{
	SSI->cr1 &= ~SSI_CR1_SSE_ENABLESSI;
	ws2812b_dev_t *ws2812b_dev = SSI == SSI0 ? &ssi0_ws2812b : &ssi1_ws2812b;
	
	ws2812b_dev->numled = numled;
	ws2812b_dev->data_ptr = (uint32_t *)data;
	ws2812b_dev->xfer_type = xfer_type;
	ws2812b_dev->xfer_size = 9 * numled; /* BYTES */

	/* Need 9 bytes to transfer 1 LED = 72 bits
	* MAX DMA transaction is 1024 bytes
	* In a DMA transaction I can send 113 LEDs MAX = 1017 bytes
	*/
	if(xfer_type == WS2812B_SINGLE)
	{
		ws2812b_dev->xfer_qty = numled; /* ONE LED PER DMA TRANSFER */
		ws2812b_dev->xfer_bs = 9;
	}
	else
	{
		ws2812b_dev->xfer_qty = (ws2812b_dev->xfer_size / 1024) + 1; /* BIGGER CHUNKS */
		ws2812b_dev->xfer_bs = 1017;
	}
	
	if(SSI == SSI0)
	{
		ws2812b_dev->dma_txch = SSI0_TX_UDMACH;
		ws2812b_dev->dma_rxch = SSI0_RX_UDMACH;

		ws2812b_dev->dma_txpri = &(dmactrl[ws2812b_dev->dma_txch]);
		ws2812b_dev->dma_txalt = &(dmactrl[ws2812b_dev->dma_txch + 32]);
		ws2812b_dev->dma_rxpri = &(dmactrl[ws2812b_dev->dma_rxch]);
		ws2812b_dev->dma_rxalt = &(dmactrl[ws2812b_dev->dma_rxch + 32]);

		SYS_CTRL->rcgcssi |= SYS_CTRL_RCGCSSI_SSI0_ENABLESSI0CLOCK;
		SYS_CTRL->scgcssi |= SYS_CTRL_SCGCSSI_SSI0_ENABLESSI0CLOCK;
		SYS_CTRL->dcgcssi |= SYS_CTRL_DCGCSSI_SSI0_ENABLESSI0CLOCK;

		SSI0_PIN_OUT_CLK;
		SSI0_PIN_OUT_TXD;
		SSI0_PIN_OUT_FSS;
		SSI0_PIN_IN_RXD;
		SSI0_PINS_AFSEL;
	}
	else
	{
		ws2812b_dev->dma_txch = SSI1_TX_UDMACH;
		ws2812b_dev->dma_rxch = SSI1_RX_UDMACH;

		ws2812b_dev->dma_txpri = UDMA_CH_PRIMARY(ws2812b_dev->dma_txch);
		ws2812b_dev->dma_txalt = UDMA_CH_ALTERNATE(ws2812b_dev->dma_txch);
		ws2812b_dev->dma_rxpri = UDMA_CH_PRIMARY(ws2812b_dev->dma_rxch);
		ws2812b_dev->dma_rxalt = UDMA_CH_ALTERNATE(ws2812b_dev->dma_rxch);

		SYS_CTRL->rcgcssi |= SYS_CTRL_RCGCSSI_SSI1_ENABLESSI1CLOCK;
		SYS_CTRL->scgcssi |= SYS_CTRL_SCGCSSI_SSI1_ENABLESSI1CLOCK;
		SYS_CTRL->dcgcssi |= SYS_CTRL_DCGCSSI_SSI1_ENABLESSI1CLOCK;

		SSI1_PIN_OUT_CLK;
		SSI1_PIN_OUT_TXD;
		SSI1_PIN_OUT_FSS;
		SSI1_PIN_IN_RXD;
		SSI1_PINS_AFSEL;
	}

	/*	Master SSI mode	*/
	SSI->cr1 = 0x0;
	SSI->cc = 	SSI_CC_CS_PIOSC_BAUDBYSYSDIV |
				SSI_CC_CS_DSEN_SYSCLOCKBYSYSDIV;
	SSI->cpsr = 0x32;
	SSI->cr0 = 	SSI_CR0_SCR_SET_CLOCKRATE(0x0) |
				SSI_CR0_FRF_MOTOROLASPIFORMAT |
				SSI_CR0_SPH_CLOCKONSECONDEDGE |
				SSI_CR0_DSS_8BITDATA;
	SSI->im = 	SSI_IM_RXIM_FIFOHALFEMPTY;
	SSI->dmactl |= //SSI_DMATCL_TXDMAE_UDMAENABLE |
				   SSI_DMATCL_RXDMAE_UDMAENABLE;
	NVIC_ENABLE(INT_SSI0);
	SSI->cr1 |= SSI_CR1_SSE_ENABLESSI;
	
				   
	/*	UDMA configuration	*/
	cc2538DmaInit();
}
		
void cc2538WS2812BRender(volatile ssi_t *SSI)
{	

	if (SSI == SSI0)
	{
			Ws2812bArmSingle(SSI);
	}
	else
	{
		if(ssi1_ws2812b.xfer_type == WS2812B_SINGLE)
			Ws2812bArmSingle(SSI);
		else
			Ws2812bArmBuffer(SSI);
	}
	SSI->dr = 0x99;
	SSI->dr = 0x99;
	SSI->dr = 0x99;
	SSI->dr = 0x99;
	SSI->dr = 0x99;
	SSI->dr = 0x99;
	SSI->dr = 0x99;

	int i;
	
	for(i = 0;i < 6; i++)
	otCliUartOutputFormat("SSI: %x  ::: ims %x,  DMA: %x ::: ctl: %x, src %x , dst %x\n\r", SSI->sr, SSI0->mis, UDMA->stat, ssi0_ws2812b.dma_rxpri->control, ssi0_ws2812b.dma_rxpri->source, ssi0_ws2812b.dma_rxpri->destination);
	otCliUartOutputFormat("Ints: %i, %i\n\r", dummy_c, ssi0_ws2812b.xfer_complete);
	//while (SSI0->sr & (SSI_SR_RNE_RXISNOTEMPTY | SSI_SR_RFF_RXFIFOISFULL))

	//UDMA->chis |= UDMA_CHANNEL(SSI0_RX_UDMACH);
	//UDMA->enaset |= UDMA_CHANNEL(SSI0_RX_UDMACH);
	
	for(i = 0;i < 6; i++)
	otCliUartOutputFormat("SSI: %x  ::: ims %x,  DMA: %x ::: ctl: %x, src %x , dst %x\n\r", SSI->sr, SSI0->mis, UDMA->stat, ssi0_ws2812b.dma_rxpri->control, ssi0_ws2812b.dma_rxpri->source, ssi0_ws2812b.dma_rxpri->destination);
	otCliUartOutputFormat("Ints: %i, %i\n\r", dummy_c, ssi0_ws2812b.xfer_complete);
			
	//otCliUartOutputFormat("SSI: %x  ::: ims %x,  DMA: %x ::: ctl: %x, src %x , dst %x\n\r", SSI->sr, SSI0->mis, UDMA->stat, ssi0_ws2812b.dma_rxpri->control, ssi0_ws2812b.dma_rxpri->source, ssi0_ws2812b.dma_rxpri->destination);
	
}


