#ifndef CC2538_SSIM_H_
#define CC2538_SSIM_H_


#include "cc2538-reg.h"

typedef struct
{	
	uint32_t cr0;									// 0x000
	#define SSI_CR0_SCR_SET_CLOCKRATE(BITRATE)		((BITRATE & 0xFF) << 8)
	#define SSI_CR0_SPH_CLOCKONSECONDEDGE			(0x1 << 7)
	#define SSI_CR0_SPO_CLOCKHIGHWHILEIDLE			(0x1 << 6)
	#define SSI_CR0_FRF_MOTOROLASPIFORMAT			(0x0)				
	#define SSI_CR0_FRF_TEXASINSTRUMENTSFORMAT		(0x1 << 4)				
	#define SSI_CR0_FRF_MICROWIREFORMAT				(0x2 << 4)								
	#define SSI_CR0_DSS_4BITDATA					(0x3)				
	#define SSI_CR0_DSS_5BITDATA					(0x4)
	#define SSI_CR0_DSS_6BITDATA					(0x5)
	#define SSI_CR0_DSS_7BITDATA					(0x6)
	#define SSI_CR0_DSS_8BITDATA					(0x7)
	#define SSI_CR0_DSS_9BITDATA					(0x8)
	#define SSI_CR0_DSS_10BITDATA					(0x9)
	#define SSI_CR0_DSS_11BITDATA					(0xA)
	#define SSI_CR0_DSS_12BITDATA					(0xB)
	#define SSI_CR0_DSS_13BITDATA					(0xC)
	#define SSI_CR0_DSS_14BITDATA					(0xD)
	#define SSI_CR0_DSS_15BITDATA					(0xE)
	#define SSI_CR0_DSS_16BITDATA					(0xF)
	uint32_t cr1;									// 0x004
	#define SSI_CR1_SOD_SSINOTDRIVETXDINSLAVE		(0x1 << 3)				
	#define SSI_CR1_MS_SLAVEMODE 					(0x1 << 2)				
	#define SSI_CR1_SSE_ENABLESSI 					(0x1 << 1)				
	#define SSI_CR1_SSE_DISABLESSI_MASK				~(0x1 << 1)				
	#define SSI_CR1_LBM_SSILOOPBACKMODE 			(0x1)
	uint32_t dr;									// 0x008
	uint32_t sr;									// 0x00C
	#define SSI_SR_BSY_SSIISBUSYORFIFONOTEMPTY		(0x1 << 4)
	#define SSI_SR_RFF_RXFIFOISFULL 				(0x1 << 3)
	#define SSI_SR_RNE_RXISNOTEMPTY 				(0x1 << 2)
	#define SSI_SR_TNF_TXFIFOISNOTFULL 				(0x1 << 1)
	#define SSI_SR_TFE_TXFIFOISEMPTY				(0x1)
	uint32_t cpsr;									// 0x010
	uint32_t im;									// 0x014
	#define SSI_IM_TXIM_FIFOHALFEMPTY		 		(0x1 << 3)
	#define SSI_IM_RXIM_FIFOHALFEMPTY 				(0x1 << 2)
	#define SSI_IM_RTIM_FIFOTIMEOUT					(0x1 << 1)
	#define SSI_IM_RORIM_FIFOOVERRUN 				(0x1)
	uint32_t ris;									// 0x018
	#define SSI_RIS_TXRIS_FIFOHALFEMPTY				(0x1 << 3)
	#define SSI_RIS_RXRIS_FIFOHALFEMPTY				(0x1 << 2)
	#define SSI_RIS_RTRIS_FIFOTIMEOUT				(0x1 << 1)
	#define SSI_RIS_RORRIS_FIFOOVERRUN				(0x1)
	uint32_t mis;									// 0x01C
	#define SSI_MIS_TXMIS_FIFOHALFEMPTY				(0x1 << 3)
	#define SSI_MIS_RXMIS_FIFOHALFEMPTY				(0x1 << 2)
	#define SSI_MIS_RTMIS_FIFOTIMEOUT				(0x1 << 1)
	#define SSI_MIS_RORMIS_FIFOOVERRUN				(0x1)
	uint32_t icr;									// 0x020
	#define SSI_IRC_RTIC_CLEARRXTIMEOUT 			(0x1 << 1)
	#define SSI_IRC_RORIC_CLEARRXOVERRUN			(0x1)
	uint32_t dmactl;								// 0x024
	#define SSI_DMATCL_TXDMAE_UDMAENABLE			(0x1 << 1)
	#define SSI_DMATCL_RXDMAE_UDMAENABLE			(0x1)
	uint32_t reserved0[1000];
	uint32_t cc;									// 0xFC8
	#define SSI_CC_CS_PIOSC_BAUDBYSYSDIV			(0x0)
	#define SSI_CC_CS_PIOSC_BAUDBYIODIV				(0x1)
	#define SSI_CC_CS_DSEN_SYSCLOCKBYSYSDIV 		(0x0)
	#define SSI_CC_CS_DSEN_SYSCLOCKBYPIOSC			(0x1 << 2)
} ssi_t;


#define SSI0 										((volatile ssi_t *)(SSI0_BASE))
#define SSI1 										((volatile ssi_t *)(SSI1_BASE))

void cc2538SSIMInit(volatile ssi_t *ssi, uint32_t frequency, uint32_t cr0, 
					uint32_t cr1, uint32_t im, uint32_t dmactl);
void cc2538SSIMTerminate(volatile ssi_t *ssi);
void cc2538SSImEnable(volatile ssi_t *SSI);
void cc2538SSImWrite(volatile ssi_t *SSI, uint16_t VALUE);
uint16_t cc2538SSImRead(volatile ssi_t *SSI);

#endif