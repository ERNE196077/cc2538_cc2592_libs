#ifndef CC2538_SYSCTRL_H_
#define CC2538_SYSCTRL_H_


#include "cc2538-reg.h"

/*	SYSTEM CONTROL 	*/

typedef struct
{
	uint32_t clock_ctrl;												// 0x000
	#define SYS_CTRL_CLOCK_CTRL_OSC32K_CALDIS_ENABLECALIBRATION			(0x0)
	#define SYS_CTRL_CLOCK_CTRL_OSC32K_CALDIS_DISABLECALIBRATION		(0x1 << 25)
	#define SYS_CTRL_CLOCK_CTRL_OSC32K_32KHZCRYSTALOSC					(0x0)
	#define SYS_CTRL_CLOCK_CTRL_OSC32K_32KHZRCOSC						(0x1 << 24)
	#define SYS_CTRL_CLOCK_CTRL_AMP_DET_DELAYQUALIFOFXOSC				(0x1 << 21)
	#define SYS_CTRL_CLOCK_CTRL_OSC_PD_POWERUPBOTHOSCILATORS			(0x0)
	#define SYS_CTRL_CLOCK_CTRL_OSC_PD_POWERDOWNOSCNOTSELECTED			(0x1 << 17)
	#define SYS_CTRL_CLOCK_CTRL_OSC_32MHZCRYSTALOSC						(0x0)
	#define SYS_CTRL_CLOCK_CTRL_OSC_16MHZHFRCOSC						(0x1 << 16)
	#define SYS_CTRL_CLOCK_CTRL_IO_DIV_32MHZ							(0x0)
	#define SYS_CTRL_CLOCK_CTRL_IO_DIV_16MHZ							(0x1 << 8)
	#define SYS_CTRL_CLOCK_CTRL_IO_DIV_8MHZ								(0x2 << 8)
	#define SYS_CTRL_CLOCK_CTRL_IO_DIV_4MHZ								(0x3 << 8)
	#define SYS_CTRL_CLOCK_CTRL_IO_DIV_2MHZ								(0x4 << 8)
	#define SYS_CTRL_CLOCK_CTRL_IO_DIV_1MHZ								(0x5 << 8)
	#define SYS_CTRL_CLOCK_CTRL_IO_DIV_05MHZ							(0x6 << 8)
	#define SYS_CTRL_CLOCK_CTRL_IO_DIV_025MHZ							(0x7 << 8)
	#define SYS_CTRL_CLOCK_CTRL_SYS_DIV_32MHZ							(0x0)
	#define SYS_CTRL_CLOCK_CTRL_SYS_DIV_16MHZ							(0x1)
	#define SYS_CTRL_CLOCK_CTRL_SYS_DIV_8MHZ							(0x2)
	#define SYS_CTRL_CLOCK_CTRL_SYS_DIV_4MHZ							(0x3)
	#define SYS_CTRL_CLOCK_CTRL_SYS_DIV_2MHZ							(0x4)
	#define SYS_CTRL_CLOCK_CTRL_SYS_DIV_1MHZ							(0x5)
	#define SYS_CTRL_CLOCK_CTRL_SYS_DIV_05MHZ							(0x6)
	#define SYS_CTRL_CLOCK_CTRL_SYS_DIV_025MHZ							(0x7)
	uint32_t clock_sta;													// 0x004
	#define SYS_CTRL_CLOCK_STA_SYNC_32K_32KHZCLOCKSYNCEDTOSYSCLOCK		(0x1 << 26)
	#define SYS_CTRL_CLOCK_STA_OSC32K_CALDIS_CALIBRATIONDISABLED		(0x1 << 25)
	#define SYS_CTRL_CLOCK_STA_OSC32K_32KHZRCOSCILATOR					(0x1 << 24)
	#define SYS_CTRL_CLOCK_STA_RST_POR 									(0)
	#define SYS_CTRL_CLOCK_STA_RST_EXTERNALRESET
	#define SYS_CTRL_CLOCK_STA_RST_WDT
	#define SYS_CTRL_CLOCK_STA_RST_CLDORSOFTWARERESET
	#define SYS_CTRL_CLOCK_STA_SOURCE_CHANGE_SYSCLOCKSOURCECHANGEREQ	(0x1 << 20)
	#define SYS_CTRL_CLOCK_STA_XOSC_STB_XOSCISPOWERUPANDSTABLE			(0x1 << 19)
	#define SYS_CTRL_CLOCK_STA_HSOSC_STB_HSOSCISPOWERUPANDSTABLE		(0x1 << 18)
	#define SYS_CTRL_CLOCK_STA_OSC_PD_OSCNOTSELECTEDISPOWERDOWN			(0x1 << 17)
	#define SYS_CTRL_CLOCK_STA_OSC_CURRENTCLOCKIS16MHZ					(0x1 << 16)
	#define SYS_CTRL_CLOCK_STA_IO_DIV_MASK								~(0x7 << 8)
	#define SYS_CTRL_CLOCK_STA_IO_DIV_32MHZ								(0x0)
	#define SYS_CTRL_CLOCK_STA_IO_DIV_16MHZ								(0x1 << 8)
	#define SYS_CTRL_CLOCK_STA_IO_DIV_8MHZ								(0x2 << 8)
	#define SYS_CTRL_CLOCK_STA_IO_DIV_4MHZ								(0x3 << 8)
	#define SYS_CTRL_CLOCK_STA_IO_DIV_2MHZ								(0x4 << 8)
	#define SYS_CTRL_CLOCK_STA_IO_DIV_1MHZ								(0x5 << 8)
	#define SYS_CTRL_CLOCK_STA_IO_DIV_05MHZ								(0x6 << 8)
	#define SYS_CTRL_CLOCK_STA_IO_DIV_025MHZ							(0x7 << 8)
	#define SYS_CTRL_CLOCK_STA_SYS_DIV_MASK								(0x7)
	#define SYS_CTRL_CLOCK_STA_SYS_DIV_32MHZ							(0x0)
	#define SYS_CTRL_CLOCK_STA_SYS_DIV_16MHZ							(0x1)
	#define SYS_CTRL_CLOCK_STA_SYS_DIV_8MHZ								(0x2)
	#define SYS_CTRL_CLOCK_STA_SYS_DIV_4MHZ								(0x3)
	#define SYS_CTRL_CLOCK_STA_SYS_DIV_2MHZ								(0x4)
	#define SYS_CTRL_CLOCK_STA_SYS_DIV_1MHZ								(0x5)
	#define SYS_CTRL_CLOCK_STA_SYS_DIV_05MHZ							(0x6)
	#define SYS_CTRL_CLOCK_STA_SYS_DIV_025MHZ							(0x7)
	uint32_t rcgcgpt;													// 0x008
	#define SYS_CTRL_RCGCGPT_GPT3_ENABLEGPTIMER3CLOCK					(0x1 << 3)
	#define SYS_CTRL_RCGCGPT_GPT2_ENABLEGPTIMER2CLOCK					(0x1 << 2)
	#define SYS_CTRL_RCGCGPT_GPT1_ENABLEGPTIMER1CLOCK					(0x1 << 1)
	#define SYS_CTRL_RCGCGPT_GPT0_ENABLEGPTIMER0CLOCK					(0x1)
	uint32_t scgcgpt;													// 0x00C
	#define SYS_CTRL_SCGCGPT_GPT3_ENABLEGPTIMER3CLOCK					(0x1 << 3)
	#define SYS_CTRL_SCGCGPT_GPT2_ENABLEGPTIMER2CLOCK					(0x1 << 2)
	#define SYS_CTRL_SCGCGPT_GPT1_ENABLEGPTIMER1CLOCK					(0x1 << 1)
	#define SYS_CTRL_SCGCGPT_GPT0_ENABLEGPTIMER0CLOCK					(0x1)
	uint32_t dcgcgpt;													// 0x010
	#define SYS_CTRL_DCGCGPT_GPT3_ENABLEGPTIMER3CLOCK					(0x1 << 3)
	#define SYS_CTRL_DCGCGPT_GPT2_ENABLEGPTIMER2CLOCK					(0x1 << 2)
	#define SYS_CTRL_DCGCGPT_GPT1_ENABLEGPTIMER1CLOCK					(0x1 << 1)
	#define SYS_CTRL_DCGCGPT_GPT0_ENABLEGPTIMER0CLOCK					(0x1)
	uint32_t srgpt;														// 0x014
	#define SYS_CTRL_SRGPT_GPT3_GPTIMER3RESET							(0x1 << 3)
	#define SYS_CTRL_SRGPT_GPT2_GPTIMER2RESET							(0x1 << 2)
	#define SYS_CTRL_SRGPT_GPT1_GPTIMER1RESET							(0x1 << 1)
	#define SYS_CTRL_SRGPT_GPT0_GPTIMER0RESET							(0x1)
	uint32_t rcgcssi;													// 0x018
	#define SYS_CTRL_RCGCSSI_SSI1_ENABLESSI1CLOCK						(0x1 << 1)
	#define SYS_CTRL_RCGCSSI_SSI0_ENABLESSI0CLOCK						(0x1)
	uint32_t scgcssi;													// 0x01C
	#define SYS_CTRL_SCGCSSI_SSI1_ENABLESSI1CLOCK						(0x1 << 1)
	#define SYS_CTRL_SCGCSSI_SSI0_ENABLESSI0CLOCK						(0x1)
	uint32_t dcgcssi;													// 0x020
	#define SYS_CTRL_DCGCSSI_SSI1_ENABLESSI1CLOCK						(0x1 << 1)
	#define SYS_CTRL_DCGCSSI_SSI0_ENABLESSI0CLOCK						(0x1)
	uint32_t srssi;														// 0x024
	#define SYS_CTRL_SRSSI_SSI1_SSI1RESET								(0x1 << 1)
	#define SYS_CTRL_SRSSI_SSI0_SSI0RESET								(0x1)
	uint32_t rcgcuart;													// 0x028
	#define SYS_CTRL_RCGCUART_UART1_ENABLEUART1CLOCK					(0x1 << 1)
	#define SYS_CTRL_RCGCUART_UART0_ENABLEUART0CLOCK					(0x1)
	uint32_t scgcuart;													// 0x02C
	#define SYS_CTRL_SCGCUART_UART1_ENABLEUART1CLOCK					(0x1 << 1)
	#define SYS_CTRL_SCGCUART_UART0_ENABLEUART0CLOCK					(0x1)
	uint32_t dcgcuart;													// 0x030
	#define SYS_CTRL_DCGCUART_UART1_ENABLEUART1CLOCK					(0x1 << 1)
	#define SYS_CTRL_DCGCUART_UART0_ENABLEUART0CLOCK					(0x1)
	uint32_t sruart;													// 0x034
	#define SYS_CTRL_SRUART_UART1_UART1RESET							(0x1 << 1)
	#define SYS_CTRL_SRUART_UART0_UART0RESET							(0x1)
	uint32_t rcgci2c;													// 0x038
	#define SYS_CTRL_RCGCI2C_I2C0_ENABLEI2C0CLOCK						(0x1)
	uint32_t scgci2c;													// 0x03C
	#define SYS_CTRL_SCGCI2C_I2C0_ENABLEI2C0CLOCK						(0x1)
	uint32_t dcgci2c;													// 0x040
	#define SYS_CTRL_DCGCI2C_I2C0_ENABLEI2C0CLOCK						(0x1)
	uint32_t sri2c;														// 0x044
	#define SYS_CTRL_SRI2C_I2C0_I2C0RESET								(0x1)
	uint32_t rcgcsec;													// 0x048
	#define SYS_CTRL_RCGCSEC_AES_ENABLEAESCLOCK							(0x1 << 1)
	#define SYS_CTRL_RCGCSEC_PKA_ENABLEPKACLOCK							(0x1)
	uint32_t scgcsec;													// 0x04C
	#define SYS_CTRL_SCGCSEC_AES_ENABLEAESCLOCK							(0x1 << 1)
	#define SYS_CTRL_SCGCSEC_PKA_ENABLEPKACLOCK							(0x1)
	uint32_t dcgcsec;													// 0x050
	#define SYS_CTRL_DCGCSEC_AES_ENABLEAESCLOCK							(0x1 << 1)
	#define SYS_CTRL_DCGCSEC_PKA_ENABLEPKACLOCK							(0x1)
	uint32_t srsec;														// 0x054
	#define SYS_CTRL_SRSEC_AES_AESRESET									(0x1 << 1)
	#define SYS_CTRL_SRSEC_PKA_PKARESET									(0x1)
	uint32_t pmctl;														// 0x058
	#define SYS_CTRL_PMCTL_PM_NOPOWERMODE								(0x0)
	#define SYS_CTRL_PMCTL_PM_POWERMODE1								(0x1)
	#define SYS_CTRL_PMCTL_PM_POWERMODE2								(0x2)
	#define SYS_CTRL_PMCTL_PM_POWERMODE3								(0x3)
	uint32_t srcrc;														// 0x05C
	#define SYS_CTRL_CRC_REN_USB_ENABLECHIPRESETIFCRCFAIL				(0x1 << 8)
	#define SYS_CTRL_CRC_REN_RF_ENABLECHIPRESETIFCRCFAIL				(0x1)
	uint32_t reserved0[5];
	uint32_t pwrdbg;													// 0x074
	#define SYS_CTRL_FORCE_WARM_RESET_RESETCHIP							(0x1 << 3)
	uint32_t reserved1[2];
	uint32_t cld;														// 0x080
	#define SYS_CTRL_CLD_VALID_CLOCKLOSSDETECTANDENAREEQUAL				(0x1 << 8)
	#define SYS_CTRL_CLD_EN_ENABLECLOCKLOSSDETECT						(0x1)
	uint32_t reserved2[4];
	uint32_t iwe;														// 0x094
	#define SYS_CTRL_IWE_SM_TIMER_IWE_ENABLESMTIMERWAKEUP				(0x1 << 5)
	#define SYS_CTRL_IWE_USB_IWE_ENABLEUSBWAKEUP						(0x1 << 4)
	#define SYS_CTRL_IWE_PORT_D_IWE_ENABLEPORTDWAKEUP					(0x1 << 3)
	#define SYS_CTRL_IWE_PORT_C_IWE_ENABLEPORTCWAKEUP					(0x1 << 2)
	#define SYS_CTRL_IWE_PORT_B_IWE_ENABLEPORTBWAKEUP					(0x1 << 1)
	#define SYS_CTRL_IWE_PORT_A_IWE_ENABLEPORTAWAKEUP					(0x1)
	uint32_t i_map;														// 0x098
	#define SYS_CTRL_I_MAP_ALTMAP_REGULARINTERRUPMAP					(0x0)
	#define SYS_CTRL_I_MAP_ALTMAP_ALTERNATEINTERRUPMAP					(0x1)
	uint32_t reserved3[3];
	uint32_t rcgcrfc;													// 0x0A8
	#define SYS_CTRL_RCGCRFC_RFC0_ENABLERFC0CLOCK						(0x1)
	uint32_t scgcrfc;													// 0x0AC
	#define SYS_CTRL_SCGCRFC_RFC0_ENABLERFC0CLOCK						(0x1)
	uint32_t dcgcrfc;													// 0x0B0
	#define SYS_CTRL_DCGCRFC_RFC0_ENABLERFC0CLOCK						(0x1)
	uint32_t emuovr;													// 0x0B4
	#define SYS_CTRL_EMUOVR_ICEPICK_FORCE_CLOCK_CG_FORCECLOCKRCG 		(0x1 << 7)
	#define SYS_CTRL_EMUOVR_ICEPICK_FORCE_POWER_CG_FORCECLOCKRCG 		(0x1 << 6)
	#define SYS_CTRL_EMUOVR_ICEPICK_INHIBIT_SLEEP_CG_FORCECLOCKRCG 		(0x1 << 5)
	#define SYS_CTRL_EMUOVR_ICEMELTER_WKUP_CG_FORCECLOCKRCG 			(0x1 << 4)
	#define SYS_CTRL_EMUOVR_ICEPICK_FORCE_CLOCK_PM_AVOIDPOWERMODES 		(0x1 << 3)
	#define SYS_CTRL_EMUOVR_ICEPICK_FORCE_POWER_PM_AVOIDPOWERMODES 		(0x1 << 2)
	#define SYS_CTRL_EMUOVR_ICEPICK_INHIBIT_SLEEP_PM_AVOIDPOWERMODES 	(0x1 << 1)
	#define SYS_CTRL_EMUOVR_ICEMELTER_WKUP_PM_AVOIDPOWERMODES 			(0x1)
} sys_ctrl_t;


#define SYS_CTRL 					((volatile sys_ctrl_t *)SYS_CTRL_BASE)
#define SYS_CTRL_SYSCLOCKFREQ		((SYS_CTRL->clock_sta & SYS_CTRL_CLOCK_CTRL_OSC_16MHZHFRCOSC ? 16000000 : 32000000) >> (SYS_CTRL->clock_sta & SYS_CTRL_CLOCK_STA_SYS_DIV_MASK))


#endif