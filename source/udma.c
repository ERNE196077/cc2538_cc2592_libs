#include <stdint.h>
#include "../include/udma.h"
#include "../include/cc2538-reg.h"


volatile udmach_t dmactrl[64] __attribute__((aligned(1024)));

void cc2538DmaInit(void)
{
	UDMA->cfg = UDMA_CFG_MASTEN_ENABLE;
	UDMA->ctlbase = (uint32_t)(&dmactrl);
}


/*	Memory-to-Memory transfer 	*/
void cc2538UdmaMemMemXfer(uint32_t SOURCE, uint32_t DESTINATION, 
						 uint8_t CHANNEL,
						 uint32_t XFER_SIZE /* Bytes */)
{
	volatile udmach_t *dma_ch_pri = &dmactrl[CHANNEL];

	/*	Set default priority	*/
	UDMA->prioclr |= UDMA_CHANNEL(CHANNEL);
	UDMA->altclr |= UDMA_CHANNEL(CHANNEL);
	UDMA->useburstclr |= UDMA_CHANNEL(CHANNEL);
	UDMA->reqmaskclr |= UDMA_CHANNEL(CHANNEL);

	dma_ch_pri->source = SOURCE;
	dma_ch_pri->destination = DESTINATION;

	switch(XFER_SIZE % 4)
	{
		case 0:
			dma_ch_pri->control = UDMA_CHCTL_DSTINC_32BIT |
								  UDMA_CHCTL_DSTSIZE_32BIT |
								  UDMA_CHCTL_SRCINC_32BIT |
								  UDMA_CHCTL_SRCSIZE_32BIT |
								  UDMA_CHCTL_ARBSIZE_8 | 
								  UDMA_CHCTL_XFERMODE_AUTO |
								  UDMA_CHCTL_XFERSIZE((XFER_SIZE/4));
			break;
		case 2:
			dma_ch_pri->control = UDMA_CHCTL_DSTINC_16BIT |
								  UDMA_CHCTL_DSTSIZE_16BIT |
								  UDMA_CHCTL_SRCINC_16BIT |
								  UDMA_CHCTL_SRCSIZE_16BIT |
								  UDMA_CHCTL_ARBSIZE_8 | 
								  UDMA_CHCTL_XFERMODE_AUTO |
								  UDMA_CHCTL_XFERSIZE((XFER_SIZE/2));
			break;
		default:
			dma_ch_pri->control = UDMA_CHCTL_DSTINC_8BIT |
								  UDMA_CHCTL_DSTSIZE_8BIT |
								  UDMA_CHCTL_SRCINC_8BIT |
								  UDMA_CHCTL_SRCSIZE_8BIT |
								  UDMA_CHCTL_ARBSIZE_8 | 
								  UDMA_CHCTL_XFERMODE_AUTO |
								  UDMA_CHCTL_XFERSIZE(XFER_SIZE);
			break;
	}
	UDMA->enaset |= UDMA_CHANNEL(CHANNEL);
	UDMA->swreq |= UDMA_CHANNEL(CHANNEL);
}

