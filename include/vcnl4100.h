#ifndef CC2538_VCNL4100_H_
#define CC2538_VCNL4100_H_

#include "i2c.h"

#define VCNL4100_I2CADDRESS									(0x60)

/*	Commands	*/
#define VCNL4100_CMD_ALSCONF								0x0	/*	ALS configuration register 	*/
#define VCNL4100_CMD_ALSTHDH								0x1	/*	ALS high interrupt threshold, LSB & MSB	*/
#define VCNL4100_CMD_ALSTHDL								0x2	/*	ALS	low interrupt threshold, LSB & MSB	*/
#define VCNL4100_CMD_PS_CONF1_CONF2							0x3	/*	PS configuration registers 1 & 2	*/
#define VCNL4100_CMD_PS_CONF3_PSSPO							0x4	/*	PS configuration register 3 & sunlight protection value	*/
#define VCNL4100_CMD_PS_THDL_THDH							0x6	/*	PS low & high interrupt thresholds	*/
#define VCNL4100_CMD_PSDATA									0x8	/*	PS data	*/
#define VCNL4100_CMD_ALSDATA								0x9	/*	ALS data	*/
#define VCNL4100_CMD_INTFLAG								0xB	/*	Interrupt flags	*/

/*	VCNL4100_CMD_ALSCONF	*/
#define VCNL4100_CMD_ALSCONF_IT_80MS						(0x0)
#define VCNL4100_CMD_ALSCONF_IT_160MS						(0x1 << 6)
#define VCNL4100_CMD_ALSCONF_IT_320MS						(0x2 << 6)
#define VCNL4100_CMD_ALSCONF_IT_640MS						(0x3 << 6)
#define VCNL4100_CMD_ALSCONF_PERS_1							(0x0)
#define VCNL4100_CMD_ALSCONF_PERS_2							(0x1 << 2)
#define VCNL4100_CMD_ALSCONF_PERS_4							(0x2 << 2)
#define VCNL4100_CMD_ALSCONF_PERS_8							(0x3 << 2)
#define VCNL4100_CMD_ALSCONF_INTEN_INTERRUPTENABLE			(0x1 << 1)
#define VCNL4100_CMD_ALSCONF_SD_ALSON						(0x0)
#define VCNL4100_CMD_ALSCONF_SD_ALSOFF						(0x1)

/*	VCNL4100_CMD_PS_CONF1	*/
#define VCNL4100_CMD_PS_CONF1_DUTY_1_5120					(0x0)
#define VCNL4100_CMD_PS_CONF1_DUTY_1_640					(0x1 << 6)
#define VCNL4100_CMD_PS_CONF1_DUTY_1_80						(0x2 << 6)
#define VCNL4100_CMD_PS_CONF1_DUTY_1_20						(0x3 << 6)
#define VCNL4100_CMD_PS_CONF1_IT_1							(0x0)
#define VCNL4100_CMD_PS_CONF1_IT_1_3						(0x1 << 4)
#define VCNL4100_CMD_PS_CONF1_IT_1_6						(0x2 << 4)
#define VCNL4100_CMD_PS_CONF1_IT_2							(0x3 << 4)
#define VCNL4100_CMD_PS_CONF1_PERS_1						(0x0)
#define VCNL4100_CMD_PS_CONF1_PERS_2						(0x1 << 2)
#define VCNL4100_CMD_PS_CONF1_PERS_4						(0x2 << 2)
#define VCNL4100_CMD_PS_CONF1_PERS_8						(0x3 << 2)
#define VCNL4100_CMD_PS_CONF1_SD_PSON						(0x0)
#define VCNL4100_CMD_PS_CONF1_SD_PSOFF						(0x1)

/*	VCNL4100_CMD_PS_CONF2	*/
#define VCNL4100_CMD_PS_CONF2_ITB_1_2						(0x0)
#define VCNL4100_CMD_PS_CONF2_ITB_1							(0x1 << 6)
#define VCNL4100_CMD_PS_CONF2_ITB_2							(0x2 << 6)
#define VCNL4100_CMD_PS_CONF2_ITB_4							(0x3 << 6)
#define VCNL4100_CMD_PS_CONF2_GAIN_DIVBY4					(0x0)
#define VCNL4100_CMD_PS_CONF2_GAIN_DIVBY2					(0x1 << 4)
#define VCNL4100_CMD_PS_CONF2_GAIN_1						(0x2 << 4)
#define VCNL4100_CMD_PS_CONF2_GAIN_2						(0x3 << 4)
#define VCNL4100_CMD_PS_CONF2_SPINTEN_SUNLIGHTINTDIS		(0x0)
#define VCNL4100_CMD_PS_CONF2_SPINTEN_SUNLIGHTINTEN			(0x1 << 2)
#define VCNL4100_CMD_PS_CONF2_INTEN_INTERRUPTDISABLE		(0x0)
#define VCNL4100_CMD_PS_CONF2_INTEN_INTERRUPTENABLE			(0x1)

/*	VCNL4100_CMD_PS_CONF3	*/
#define VCNL4100_CMD_PS_CONF3_AV_DIVBY2						(0x0)
#define VCNL4100_CMD_PS_CONF3_AV_DIVBY4						(0x1 << 6)
#define VCNL4100_CMD_PS_CONF3_AV_DIVBY8						(0x2 << 6)
#define VCNL4100_CMD_PS_CONF3_AV_DIVBY16					(0x3 << 6)
#define VCNL4100_CMD_PS_CONF3_AVEN_AVGDISABLE				(0x0)
#define VCNL4100_CMD_PS_CONF3_AVEN_AVGENABLE				(0x1 << 5)
#define VCNL4100_CMD_PS_CONF3_AF_NORMAL						(0x0)
#define VCNL4100_CMD_PS_CONF3_AF_ACTIVEFORCEENABLE			(0x1 << 3)
#define VCNL4100_CMD_PS_CONF3_TRIG_NOAFMODETRIGGER			(0x0)
#define VCNL4100_CMD_PS_CONF3_TRIG_TRIGGER1TIMECYCLE		(0x1 << 2)
#define VCNL4100_CMD_PS_CONF3_MPULSE_MPULSEDISABLE			(0x0)
#define VCNL4100_CMD_PS_CONF3_MPULSE_MPULSEENABLE			(0x1 << 1)

/*	VCNL4100_CMD_PS_PSSPO	*/
#define VCNL4100_CMD_PS_SPO_0xFFWHILEPSINSUNLIGHT			(0xA0)
#define VCNL4100_CMD_PS_SPO_0x00WHILEPSINSUNLIGHT			(0x20)

/*		*/
#define VCNL4100_CMD_INTFLAG_SPFLEAVE_LEAVINGSUNLIGHTPROT	(0x1 << 7)
#define VCNL4100_CMD_INTFLAG_SPFENTER_ENTERINGSUNLIGHTPROT	(0x1 << 6)
#define VCNL4100_CMD_INTFLAG_ALSIFL_LOWTHRESHOLDEVENT		(0x1 << 5)
#define VCNL4100_CMD_INTFLAG_ALSIFH_HIGHTHRESHOLDEVENT		(0x1 << 4)
#define VCNL4100_CMD_INTFLAG_PSIFCLOSE_HIGHTHRESHOLDEVENT	(0x1 << 1)
#define VCNL4100_CMD_INTFLAG_PSIFAWAY_LOWTHRESHOLDEVENT		(0x1)


void vcnl4100_write(uint8_t cmd, uint8_t datal, uint8_t datah);
uint16_t vcnl4100_read(uint8_t cmd);
void vcnl4100PSConfig(uint8_t config1, uint8_t config2);//, 
					  //uint8_t config3, uint8_t spo);
void vcnl4100ALSConfig(uint8_t alsconf);
void vcnl4100PSInit(void);
void vcnl4100ALSInit(void);
uint8_t vcnl4100PSRead(void);
uint16_t vcnl4100ALSRead(void);

/*		*/
#endif