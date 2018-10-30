#ifndef CC2538_GPTIMER_H_
#define CC2538_GPTIMER_H_


#include "cc2538-reg.h"
#include "common.h"


typedef struct {
	uint32_t cfg;									// 0x000
	#define GPTIMER_CFG_GPTMCFG_32BIT				0x0
	#define GPTIMER_CFG_GPTMCFG_32BIT_RTC			0x1
	#define GPTIMER_CFG_GPTMCFG_16BIT			 	0x4
	uint32_t tamr;									// 0x004
	uint32_t tbmr;									// 0x008
	#define GPTIMER_TxMR_TxPLO_LEGACYOPERATION		(0x0)
	#define GPTIMER_TxMR_TxPLO_CPPSETONTIMEOUT		(0x1 << 11)
	#define GPTIMER_TxMR_TxMRSU_UPDTMATCHNEXTCYCLE	(0x0)
	#define GPTIMER_TxMR_TxMRSU_UPDTMATCHONTIMEOUT	(0x1 << 10)
	#define GPTIMER_TxMR_TxPWMIE_INTDISABLE			(0x0)
	#define GPTIMER_TxMR_TxPWMIE_INTENABLE			(0x1 << 9)
	#define GPTIMER_TxMR_TxILD_UPDTMTARONCYCLE		(0x0)
	#define GPTIMER_TxMR_TxILD_UPDTMTARONTIMEOUT	(0x1 << 8)
	#define GPTIMER_TxMR_TxSANPS_SNAPSHOTMODEDIS	(0x0)		
	#define GPTIMER_TxMR_TxSANPS_SNAPSHOTMODEEN		(0x1 << 7)
	#define GPTIMER_TxMR_TxWOT_WAITONTRIGGERDIS 	(0x0)
	#define GPTIMER_TxMR_TxWOT_WAITONTRIGGEREN		(0x1 << 6)
	#define GPTIMER_TxMR_TxMIE_MATCHINTDIS			(0x0)
	#define GPTIMER_TxMR_TxMIE_MATCHINTEN			(0x1 << 5)
	#define GPTIMER_TxMR_TxCDIR_COUNTDOWN			(0x0)
	#define GPTIMER_TxMR_TxCDIR_COUNTUP				(0x1 << 4)
	#define GPTIMER_TxMR_TxAMS_CAPTUREMODE			(0x0)
	#define GPTIMER_TxMR_TxAMS_PWMMODE				(0x1 << 3)
	#define GPTIMER_TxMR_TxCMR_EDGECOUNT			(0x0)
	#define GPTIMER_TxMR_TxCMR_EDGETIME 			(0x1 << 2)
	#define GPTIMER_TxMR_TxMR_ONESHOT				(0x1)
	#define GPTIMER_TxMR_TxMR_PERIODIC				(0x2)
	#define GPTIMER_TxMR_TxMR_CAPTURE				(0x3)
	uint32_t ctl;									// 0x00C
	#define GPTIMER_CTL_TBPWML_OUTPUTINVERTED		(0x1 << 14)
	#define GPTIMER_CTL_TBOTE_ADCTRIGGEROUTPUT 		(0x1 << 13)
	#define GPTIMER_CTL_TBEVENT_POSITIVEEDGE		(0x0)
	#define GPTIMER_CTL_TBEVENT_NEGATIVEEDGE		(0x1 << 10)
	#define GPTIMER_CTL_TBEVENT_BOTHEDGES 			(0x3 << 10)
	#define GPTIMER_CTL_TBSTALL_TIMERSTOPONDEBUG	(0x1 << 9)
	#define GPTIMER_CTL_TBEN_TIMERENABLE			(0x1 << 8)
	#define GPTIMER_CTL_TAPWML_OUTPUTINVERTED		(0x1 << 6)
	#define GPTIMER_CTL_TAOTE_ADCTRIGGEROUTPUT 		(0x1 << 5)
	#define GPTIMER_CTL_TAEVENT_POSITIVEEDGE		(0x0)
	#define GPTIMER_CTL_TAEVENT_NEGATIVEEDGE		(0x1 << 2)
	#define GPTIMER_CTL_TAEVENT_BOTHEDGES 			(0x3 << 2)
	#define GPTIMER_CTL_TASTALL_TIMERSTOPONDEBUG	(0x1 << 1)
	#define GPTIMER_CTL_TAEN_TIMERENABLE			(0x1 << 0)
	uint32_t sync;									// 0x010
	uint32_t reserved0;								// 0x014
	#define GPTIMER_SYNC_SYNC3_TIMEOUT_A_GPTM3 		(0x1 << 6)
	#define GPTIMER_SYNC_SYNC3_TIMEOUT_B_GPTM3 		(0x2 << 6)
	#define GPTIMER_SYNC_SYNC3_TIMEOUT_AB_GPTM3 	(0x3 << 6)
	#define GPTIMER_SYNC_SYNC2_TIMEOUT_A_GPTM2 		(0x1 << 4)
	#define GPTIMER_SYNC_SYNC2_TIMEOUT_B_GPTM2 		(0x2 << 4)
	#define GPTIMER_SYNC_SYNC2_TIMEOUT_AB_GPTM2 	(0x3 << 4)
	#define GPTIMER_SYNC_SYNC1_TIMEOUT_A_GPTM1 		(0x1 << 2)
	#define GPTIMER_SYNC_SYNC1_TIMEOUT_B_GPTM1 		(0x2 << 2)
	#define GPTIMER_SYNC_SYNC1_TIMEOUT_AB_GPTM1 	(0x3 << 2)
	#define GPTIMER_SYNC_SYNC0_TIMEOUT_A_GPTM0 		(0x1)
	#define GPTIMER_SYNC_SYNC0_TIMEOUT_B_GPTM0 		(0x2)
	#define GPTIMER_SYNC_SYNC0_TIMEOUT_AB_GPTM0 	(0x3)
	uint32_t imr;									// 0x018
	#define GPTIMER_IMR_TBMIM_MATCHINTENABLE 		(0x1 << 11)
	#define GPTIMER_IMR_CBEIM_CAPTUREEVTINTENABLE	(0x1 << 10)
	#define GPTIMER_IMR_CBMIM_CAPTUREMATCHINTENABLE	(0x1 << 9)
	#define GPTIMER_IMR_TBTOIM_TIMEOUTINTMASKENABLE	(0x1 << 8)
	#define GPTIMER_IMR_TAMIM_MATCHINTENABLE 		(0x1 << 4)
	#define GPTIMER_IMR_CAEIM_CAPTUREEVTINTENABLE	(0x1 << 2)
	#define GPTIMER_IMR_CAMIM_CAPTUREMATCHINTENABLE	(0x1 << 1)
	#define GPTIMER_IMR_TATOIM_TIMEOUTINTMASKENABLE	(0x1 << 0)
	uint32_t ris;									// 0x01C
	#define GPTIMER_RIS_TBMRIS_MATCHINT				(0x1 << 11)
	#define GPTIMER_RIS_CBERIS_CAPTUREEVTINT		(0x1 << 10)
	#define GPTIMER_RIS_CBMRIS_CAPTUREMATCHINT		(0x1 << 9)
	#define GPTIMER_RIS_TBTORIS_TIMEOUTINT			(0x1 << 8)
	#define GPTIMER_RIS_TAMRIS_MATCHINT				(0x1 << 4)
	#define GPTIMER_RIS_CAERIS_CAPTUREEVTINT		(0x1 << 2)
	#define GPTIMER_RIS_CAMRIS_CAPTUREMATCHINT		(0x1 << 1)
	#define GPTIMER_RIS_TATORIS_TIMEOUTINT			(0x1)
	uint32_t mis;									// 0x020
	#define GPTIMER_MIS_TBMMIS_MATCHINT				(0x1 << 11)
	#define GPTIMER_MIS_CBEMIS_CAPTUREEVTINT		(0x1 << 10)
	#define GPTIMER_MIS_CBMMIS_CAPTUREMATCHINT		(0x1 << 9)
	#define GPTIMER_MIS_TBTOMIS_TIMEOUTINT			(0x1 << 8)
	#define GPTIMER_MIS_TAMMIS_MATCHINT				(0x1 << 4)
	#define GPTIMER_MIS_CAEMIS_CAPTUREEVTINT		(0x1 << 2)
	#define GPTIMER_MIS_CAMMIS_CAPTUREMATCHINT		(0x1 << 1)
	#define GPTIMER_MIS_TATOMIS_TIMEOUTINT			(0x1)
	uint32_t icr;									// 0x024
	#define GPTIMER_ICR_WUECINT_WRITEUPDATEERROR	(0x1 << 16)
	#define GPTIMER_ICR_TBMCINT_MATCHINT			(0x1 << 11)
	#define GPTIMER_ICR_CBECINT_CAPTUREEVTINT		(0x1 << 10)
	#define GPTIMER_ICR_CBMCINT_CAPTUREMATCHINT		(0x1 << 9)
	#define GPTIMER_ICR_TBTOCINT_TIMEOUTINT			(0x1 << 8)
	#define GPTIMER_ICR_TAMCINT_MATCHINT			(0x1 << 4)
	#define GPTIMER_ICR_CAECINT_CAPTUREEVTINT		(0x1 << 2)
	#define GPTIMER_ICR_CAMCINT_CAPTUREMATCHINT		(0x1 << 1)
	#define GPTIMER_ICR_TATOCINT_TIMEOUTINT			(0x1)
	uint32_t tailr;									// 0x028
	uint32_t tbilr;									// 0x02C
	uint32_t tamatchr;								// 0x030
	uint32_t tbmatchr;								// 0x034
	uint32_t tapr;									// 0x038
	uint32_t tbpr;									// 0x03C
	uint32_t tapmr;									// 0x040
	uint32_t tbpmr;									// 0x044
	uint32_t tar;									// 0x048
	uint32_t tbr;									// 0x04C 
	uint32_t tav;									// 0x050 
	uint32_t tbv;									// 0x054
	uint32_t reserved1;								// 0x058
	uint32_t taps;									// 0x05C
	uint32_t tbps;									// 0x060
	uint32_t tapv;									// 0x064
	uint32_t tbpv;									// 0x068
	uint32_t reserved[982];
	uint32_t pp;									// 0xFC0
} gptimer_t;

#define GPTIMER0 									((volatile gptimer_t *) GPTIMER0_BASE)
#define GPTIMER1 									((volatile gptimer_t *) GPTIMER1_BASE)
#define GPTIMER2 									((volatile gptimer_t *) GPTIMER2_BASE)
#define GPTIMER3 									((volatile gptimer_t *) GPTIMER3_BASE)

typedef enum
{
	A,
	B
} gptimer_side_t;


void cc2538GPTimerStartNVIC(volatile gptimer_t *GPTIMER, gptimer_side_t SIDE);
void cc2538GPTimerStopNVIC(volatile gptimer_t *GPTIMER, gptimer_side_t SIDE);


void cc2538GPTimer32BitInit(volatile gptimer_t *GPTIMER, uint32_t PERIOD /* ms */);
void cc2538GPTimer32BitStart(volatile gptimer_t *GPTIMER);
void cc2538GPTimer32BitStop(volatile gptimer_t *GPTIMER);

void cc2538GPTimer16BitInit(volatile gptimer_t *GPTIMER, uint32_t PERIOD /* ms */, gptimer_side_t SIDE);
void cc2538GPTimer16BitStart(volatile gptimer_t *GPTIMER, gptimer_side_t SIDE);
void cc2538GPTimer16BitStop(volatile gptimer_t *GPTIMER, gptimer_side_t SIDE);

void cc2538GPTimerPWMInit(volatile gptimer_t *GPTIMER, gptimer_side_t SIDE, uint32_t PERIOD /* ms */, uint8_t INIT_DUTY /* 0% - 100% */);
void cc2538GPTimerPWMStop(volatile gptimer_t *GPTIMER, gptimer_side_t SIDE);
void cc2538GPTimerPWMSetDuty(volatile gptimer_t *GPTIMER, gptimer_side_t SIDE, uint8_t DUTY /* 0% - 100% */);
void cc2538GPTimerPWMSetPeriod(volatile gptimer_t *GPTIMER, gptimer_side_t SIDE, uint32_t PERIOD /* ms */);
void cc2538GPTimerPWMSetDutyPeriod(volatile gptimer_t *GPTIMER, gptimer_side_t SIDE, uint32_t PERIOD /* ms */, uint8_t DUTY /* 0% - 100% */);

#endif