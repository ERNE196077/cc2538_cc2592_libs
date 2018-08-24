#include <stdint.h>
#include <stdio.h>
#include "../include/vcnl4100.h"
#include "../include/i2c.h"
#include <openthread/cli.h>

uint8_t rxdatal, rxdatah;


i2cm_unit_t vcnl4100_frame_write[3] = 
	{{I2CM_WRITE_W_START, 	VCNL4100_I2CADDRESS, 0x0, (uint8_t *)NULL, &vcnl4100_frame_write[1]},
     {I2CM_WRITE, 			VCNL4100_I2CADDRESS, 0x0, (uint8_t *)NULL, &vcnl4100_frame_write[2]},
     {I2CM_WRITE_W_STOP, 	VCNL4100_I2CADDRESS, 0x0, (uint8_t *)NULL, NULL}};
    
i2cm_unit_t vcnl4100_frame_receive[3] =
	{{I2CM_WRITE_W_START, 	VCNL4100_I2CADDRESS, 0x0, (uint8_t *)NULL, 	&vcnl4100_frame_receive[1]},
	 {I2CM_RECEIVE_W_START, VCNL4100_I2CADDRESS, 0x0, &rxdatal, 		&vcnl4100_frame_receive[2]},
	 {I2CM_RECEIVE_W_STOP, 	VCNL4100_I2CADDRESS, 0x0, &rxdatah, 		NULL}};


uint8_t default_ps_conf1 = 	VCNL4100_CMD_PS_CONF1_DUTY_1_5120 |
							VCNL4100_CMD_PS_CONF1_IT_1 |
							VCNL4100_CMD_PS_CONF1_PERS_1 |
							VCNL4100_CMD_PS_CONF1_SD_PSON ;
uint8_t default_ps_conf2 = 	VCNL4100_CMD_PS_CONF2_ITB_1_2 |
							VCNL4100_CMD_PS_CONF2_GAIN_DIVBY4 |
							VCNL4100_CMD_PS_CONF2_SPINTEN_SUNLIGHTINTDIS |
							VCNL4100_CMD_PS_CONF2_INTEN_INTERRUPTDISABLE;
uint8_t default_ps_conf3 =	VCNL4100_CMD_PS_CONF3_AV_DIVBY2 |
							VCNL4100_CMD_PS_CONF3_AVEN_AVGDISABLE |
							VCNL4100_CMD_PS_CONF3_TRIG_NOAFMODETRIGGER |
							VCNL4100_CMD_PS_CONF3_MPULSE_MPULSEDISABLE;
// uint8_t default_ps_spo = 	VCNL4100_CMD_PS_SPO_0x00WHILEPSINSUNLIGHT;
 uint8_t default_als_config = 	VCNL4100_CMD_ALSCONF_IT_80MS |
 								VCNL4100_CMD_ALSCONF_PERS_1 |
 								VCNL4100_CMD_ALSCONF_SD_ALSON;



void vcnl4100_write(uint8_t cmd, uint8_t datal, uint8_t datah)
{
	vcnl4100_frame_write[0].txdata = cmd;
	vcnl4100_frame_write[1].txdata = datal;
	vcnl4100_frame_write[2].txdata = datah;
	
	cc2538I2cmStart(&vcnl4100_frame_write[0]);
}


uint16_t vcnl4100_read(uint8_t cmd)
{
	vcnl4100_frame_receive[0].txdata = cmd;

	cc2538I2cmStart(&vcnl4100_frame_receive[0]);

	return (uint16_t)((rxdatah << 8) | rxdatal);
}


void vcnl4100PSConfig(uint8_t config1, uint8_t config2)//, 
					  //uint8_t config3, uint8_t spo)
{
	vcnl4100_write(VCNL4100_CMD_PS_CONF1_CONF2, config1, config2);
	//vcnl4100_write(VCNL4100_CMD_PS_CONF3_PSSPO, config3, spo);
}


void vcnl4100ALSConfig(uint8_t alsconf)
{
	vcnl4100_write(VCNL4100_CMD_ALSCONF, alsconf, 0x0);
}

void vcnl4100PSInit(void)
{
	cc2538I2cmInit(I2C_FREQ_400KHZ);
    vcnl4100PSConfig(default_ps_conf1, default_ps_conf2);//,
    					//0x0, 0x0);
    
}

void vcnl4100ALSInit(void)
{
	cc2538I2cmInit(I2C_FREQ_400KHZ);
    vcnl4100ALSConfig(default_als_config);
}


uint8_t vcnl4100PSRead(void)
{
	return (uint8_t)(vcnl4100_read(VCNL4100_CMD_PSDATA) & 0xFF);
}


uint16_t vcnl4100ALSRead(void)
{
	return vcnl4100_read(VCNL4100_CMD_ALSDATA);
}