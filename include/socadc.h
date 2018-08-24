#ifndef CC2538_SOCADC_H_
#define CC2538_SOCADC_H_


#include "cc2538-reg.h"

typedef struct
{
	uint32_t adccon1;
	#define SOCADC_ADCCON1_EOC 						(0x1 << 7)
	#define SOCADC_ADCCON1_ST						(0x1 << 6)
	#define SOCADC_ADCCON1_STSELECT_NOIMPLEMENTED	(0x0 << 4)
	#define SOCADC_ADCCON1_STSELECT_FULLSPEED		(0x1 << 4)
	#define SOCADC_ADCCON1_STSELECT_TIMER1CH0		(0x2 << 4)
	#define SOCADC_ADCCON1_STSELECT_ADCCON1STBIT	(0x3 << 4)
	#define SOCADC_ADCCON1_RCTRL_NORMAL 			(0x0)
	#define SOCADC_ADCCON1_RCTRL_CLOCKLFSRONCE		(0x1 << 2)
	#define SOCADC_ADCCON1_RCTRL_STOPPED 			(0x3 << 2)
	uint32_t adccon2;
	#define SOCADC_ADCCON2_SREF_INTERNALREF			(0x0)
	#define SOCADC_ADCCON2_SREF_EXTREFAIN7			(0x1 << 6)
	#define SOCADC_ADCCON2_SREF_AVDD5PIN			(0x2 << 6)
	#define SOCADC_ADCCON2_SREF_EXTREFAIN67DIF		(0x3 << 6)
	#define SOCADC_ADCCON2_SDIV_7BITENOB			(0x0)
	#define SOCADC_ADCCON2_SDIV_9BITENOB			(0x1 << 4)
	#define SOCADC_ADCCON2_SDIV_10BITENOB			(0x2 << 4)
	#define SOCADC_ADCCON2_SDIV_12BITENOB			(0x3 << 4)
	#define SOCADC_ADCCON2_SCH_AIN0					(0x0)
	#define SOCADC_ADCCON2_SCH_AIN1					(0x01)
	#define SOCADC_ADCCON2_SCH_AIN2					(0x02)
	#define SOCADC_ADCCON2_SCH_AIN3					(0x03)
	#define SOCADC_ADCCON2_SCH_AIN4					(0x04)
	#define SOCADC_ADCCON2_SCH_AIN5					(0x05)
	#define SOCADC_ADCCON2_SCH_AIN6					(0x06)
	#define SOCADC_ADCCON2_SCH_AIN7					(0x07)
	#define SOCADC_ADCCON2_SCH_AIN0AIN1				(0x08)
	#define SOCADC_ADCCON2_SCH_AIN2AIN3				(0x09)
	#define SOCADC_ADCCON2_SCH_AIN4AIN5				(0x0A)
	#define SOCADC_ADCCON2_SCH_AIN6AIN7				(0x0B)
	#define SOCADC_ADCCON2_SCH_GND					(0x0C)
	#define SOCADC_ADCCON2_SCH_TEMPSENSOR			(0x0E)
	#define SOCADC_ADCCON2_SCH_VDD3					(0x0F)
	uint32_t adccon3;
	#define SOCADC_ADCCON3_EREF_INTERNALREF			(0x0)
	#define SOCADC_ADCCON3_EREF_EXTREFAIN7			(0x1 << 6)
	#define SOCADC_ADCCON3_EREF_AVDD5PIN			(0x2 << 6)
	#define SOCADC_ADCCON3_EREF_EXTREFAIN67DIF		(0x3 << 6)
	#define SOCADC_ADCCON3_EDIV_7BITENOB			(0x0)
	#define SOCADC_ADCCON3_EDIV_9BITENOB			(0x1 << 4)
	#define SOCADC_ADCCON3_EDIV_10BITENOB			(0x2 << 4)
	#define SOCADC_ADCCON3_EDIV_12BITENOB			(0x3 << 4)
	#define SOCADC_ADCCON3_ECH_AIN0					(0x0)
	#define SOCADC_ADCCON3_ECH_AIN1					(0x01)
	#define SOCADC_ADCCON3_ECH_AIN2					(0x02)
	#define SOCADC_ADCCON3_ECH_AIN3					(0x03)
	#define SOCADC_ADCCON3_ECH_AIN4					(0x04)
	#define SOCADC_ADCCON3_ECH_AIN5					(0x05)
	#define SOCADC_ADCCON3_ECH_AIN6					(0x06)
	#define SOCADC_ADCCON3_ECH_AIN7					(0x07)
	#define SOCADC_ADCCON3_ECH_AIN0AIN1				(0x08)
	#define SOCADC_ADCCON3_ECH_AIN2AIN3				(0x09)
	#define SOCADC_ADCCON3_ECH_AIN4AIN5				(0x0A)
	#define SOCADC_ADCCON3_ECH_AIN6AIN7				(0x0B)
	#define SOCADC_ADCCON3_ECH_GND					(0x0C)
	#define SOCADC_ADCCON3_ECH_TEMPSENSOR			(0x0E)
	#define SOCADC_ADCCON3_ECH_VDD3					(0x0F)
	uint32_t adcl;
	#define SOCADC_ADCL_ADC_MASK					(0x3F << 2)
	#define SOCADC_ADCL_GET_ADCRESULT 				((SOCADC->adcl & SOCADC_ADCL_ADC_MASK) >> 2)
	uint32_t adch;
	#define SOCADC_ADCH_ADC_MASK					(0xFF)
	#define SOCADC_ADCH_GET_ADCRESULT 				(SOCADC->adch & SOCADC_ADCH_ADC_MASK)
	uint32_t rndl;
	uint32_t rndh;									// 0x018
	uint32_t reserved0[2];
	uint32_t cmpctl;							
	
} socadc_t;

#define SOCADC 							((volatile socadc_t *)(SOCADC_BASE))

#endif