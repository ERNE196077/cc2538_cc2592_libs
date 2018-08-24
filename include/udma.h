#ifndef CC2538_UDMA_H_
#define CC2538_UDMA_H_


typedef struct
{	
	uint32_t stat;										// 0x000
	#define UDMA_STAT_DMACHANS 							(0x1F << 16)
	#define UDMA_STAT_DMACHANS_GET_AVAILCHANNELS		(((UDMA->stat & UDMA_DMACHANS) >> 16) + 1)
	#define UDMA_STAT_STATE_IDLE						(0x0)
	#define UDMA_STAT_STATE_READINGDATA 				(0x1)
	#define UDMA_STAT_STATE_READINGSRCPTR 				(0x2)
	#define UDMA_STAT_STATE_READINGDSTPTR 				(0x3)
	#define UDMA_STAT_STATE_READINGSRCDATA 				(0x4)
	#define UDMA_STAT_STATE_WRITINGDSTDATA 				(0x5)
	#define UDMA_STAT_STATE_WAITINGREQCLEAR 			(0x6)
	#define UDMA_STAT_STATE_WRITINGCHDATA 				(0x7)
	#define UDMA_STAT_STATE_STALLED 					(0x8)
	#define UDMA_STAT_STATE_DONE 						(0x9)
	#define UDMA_STAT_STATE_GET_STATE 					((UDMA->state & (0xF << 4)) >> 4)
	#define UDMA_STAT_MASTEN_ENABLE 					(0x1)
	uint32_t cfg;										// 0x004
	#define UDMA_CFG_MASTEN_ENABLE 						(0x1)
	uint32_t ctlbase;									// 0x008
	#define UDMA_CTLBASE_SET_ADDR(ADDRESS) 				UDMA->ctlbase = (ADDRESS << 10)
	uint32_t altbase;									// 0x00C
	#define UDMA_CHANNEL(CHANNEL)						(0x1 << CHANNEL)
	uint32_t waitstat;									// 0x010
	uint32_t swreq;										// 0x014
	uint32_t useburstset;								// 0x018
	uint32_t useburstclr;								// 0x01C
	uint32_t reqmaskset;								// 0x020
	uint32_t reqmaskclr;								// 0x024
	uint32_t enaset;									// 0x028
	uint32_t enaclr;									// 0x02C
	uint32_t altset;									// 0x030
	uint32_t altclr;									// 0x034
	uint32_t prioset;									// 0x038
	uint32_t prioclr;									// 0x03C
	uint32_t reserved0[3];
	uint32_t errclr;									// 0x04C
	#define UDMA_ERRCLR_BUSERRORPENDING 				(0x1)
	uint32_t reserved1[300];
	uint32_t chasgn;									// 0x5000x5000x5000x0x5000x5000x5000x500500
	uint32_t chis;										// 0x504
	uint32_t reserved2[2];
 	#define UDMA_CHMAP_SET(CHANNEL, ASSIGNMENT)			*((volatile uint32_t *)(UDMA_BASE + 0x510 + (4 * (CHANNEL/8)))) |= (ASSIGNMENT << (4 * (CHANNEL % 8)))
 	#define UDMA_CHMAP_CLR(CHANNEL, ASSIGNMENT)			*((volatile uint32_t *)(UDMA_BASE + 0x510 + (4 * (CHANNEL/8)))) &= ~(0xF << (4 * (CHANNEL % 8)))
	uint32_t chmap0;									// 0x510
	uint32_t chmap1;									// 0x514
	uint32_t chmap2;									// 0x518
	uint32_t chmap3;									// 0x51C
} udma_t;
 

typedef struct
{
	uint32_t source;
	uint32_t destination;
	uint32_t control;
	#define UDMA_CHCTL_DSTINC_8BIT						(0x0)
	#define UDMA_CHCTL_DSTINC_16BIT						(0x1 << 30)
	#define UDMA_CHCTL_DSTINC_32BIT						(0x2 << 30)
	#define UDMA_CHCTL_DSTINC_NOINC						(0x3 << 30)
	#define UDMA_CHCTL_DSTSIZE_8BIT						(0x0)
	#define UDMA_CHCTL_DSTSIZE_16BIT						(0x1 << 28)
	#define UDMA_CHCTL_DSTSIZE_32BIT					(0x2 << 28)	
	#define UDMA_CHCTL_SRCINC_8BIT						(0x0)
	#define UDMA_CHCTL_SRCINC_16BIT						(0x1 << 26)
	#define UDMA_CHCTL_SRCINC_32BIT						(0x2 << 26)
	#define UDMA_CHCTL_SRCINC_NOINC						(0x3 << 26)
	#define UDMA_CHCTL_SRCSIZE_8BIT						(0x0)
	#define UDMA_CHCTL_SRCSIZE_16BIT					(0x1 << 24)
	#define UDMA_CHCTL_SRCSIZE_32BIT					(0x2 << 24)	
	#define UDMA_CHCTL_ARBSIZE_1						(0x0)
	#define UDMA_CHCTL_ARBSIZE_2						(0x1 << 14)
	#define UDMA_CHCTL_ARBSIZE_4						(0x2 << 14)
	#define UDMA_CHCTL_ARBSIZE_8						(0x3 << 14)
	#define UDMA_CHCTL_ARBSIZE_16						(0x4 << 14)
	#define UDMA_CHCTL_ARBSIZE_32						(0x5 << 14)
	#define UDMA_CHCTL_ARBSIZE_64						(0x6 << 14)
	#define UDMA_CHCTL_ARBSIZE_128						(0x7 << 14)
	#define UDMA_CHCTL_ARBSIZE_256						(0x8 << 14)
	#define UDMA_CHCTL_ARBSIZE_512						(0x9 << 14)
	#define UDMA_CHCTL_ARBSIZE_1024						(0xA << 14)
	#define UDMA_CHCTL_XFERSIZE(SIZE)					((SIZE - 1) << 4)
	#define UDMA_CHCTL_NXTUSEBURST						(0x1 << 3)
	#define UDMA_CHCTL_XFERMODE_MASK					(0x7)
	#define UDMA_CHCTL_XFERMODE_STOP					(0x0)
	#define UDMA_CHCTL_XFERMODE_BASIC					(0x1)
	#define UDMA_CHCTL_XFERMODE_AUTO					(0x2)
	#define UDMA_CHCTL_XFERMODE_PINGPONG				(0x3)
	#define UDMA_CHCTL_XFERMODE_MEM_SG					(0x4)
	#define UDMA_CHCTL_XFERMODE_MEM_SGA					(0x5)
	#define UDMA_CHCTL_XFERMODE_PER_SG					(0x6)
	#define UDMA_CHCTL_XFERMODE_PER_SGA					(0x7)
	uint32_t reserved;
} ch_ctrl_t;


#define UDMA 											((volatile udma_t *)(UDMA_BASE))
#define UDMA_CTRL_TABLE_ADDR							0x0027B000
#define UDMA_CH_PRIMARY(CHANNEL)						((volatile ch_ctrl_t *)(((UDMA_CTRL_TABLE_ADDR) + (CHANNEL * sizeof(ch_ctrl_t)))))
#define UDMA_CH_ALTERNATE(CHANNEL)						((volatile ch_ctrl_t *)(((UDMA_CTRL_TABLE_ADDR + 0x200)) + (CHANNEL * sizeof(ch_ctrl_t))))

void cc2538DmaInit(void);
void cc2538UdmaMemMemXfer(uint32_t SOURCE, uint32_t DESTINATION, 
						 uint8_t CHANNEL,
						 uint32_t XFER_SIZE /* Bytes */);

#endif