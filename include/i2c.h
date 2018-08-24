#ifndef CC2538_I2C_H_
#define CC2538_I2C_H_

#include <stdint.h>

typedef enum
{
	I2C_FREQ_100KHZ = 100000,
	I2C_FREQ_400KHZ = 400000
} i2c_freq_t;


typedef enum
{
	I2CM_WRITE,
	I2CM_WRITE_W_START,
	I2CM_WRITE_W_STOP,
	I2CM_WRITE_W_STARTSTOP,
	I2CM_RECEIVE,
	I2CM_RECEIVE_W_START,
	I2CM_RECEIVE_W_STOP,
	I2CM_RECEIVE_W_STARTSTOP
}i2cm_op_t;


typedef struct unit_t
{
	i2cm_op_t op;
	uint8_t address;
	uint8_t txdata;
	uint8_t *rxdata;
	struct unit_t *next;
}i2cm_unit_t;

typedef struct
{
	uint32_t sa;									// 0x000
	#define I2CM_SA_SA_SETSLAVEADDRESS(ADDRESS)		(ADDRESS << 1)
	#define I2CM_SA_RS_TRANSMIT						(0x0)
	#define I2CM_SA_RS_RECEIVE						(0x1)
	uint32_t ctrl_stat;								// 0x004
	#define I2CM_CTRL_ACK_ACKDATAAUTOMATICALLY		(0x1 << 3)
	#define I2CM_CTRL_STOP_GENERATESTOPCONDITION	(0x1 << 2)
	#define I2CM_CTRL_START_GENERATESTARTCONDITION	(0x1 << 1)
	#define I2CM_CTRL_RUN_ENABLEMASTERI2C			(0x1)
	#define I2CM_STAT_BUSBUSY_BUSISBUSY				(0x1 << 6)
	#define I2CM_STAT_IDLE_CONTROLLERISIDLE			(0x1 << 5)
	#define I2CM_STAT_ARBLST_ARBITRATIONLOST		(0x1 << 4)
	#define I2CM_STAT_DATACK_TXDATAACKNOWLEDGE		(0x1 << 3)
	#define I2CM_STAT_ADRACK_TXADDRESSACKNOWLEDGE	(0x1 << 2)
	#define I2CM_STAT_ERROR_ERRORINLASTOPERATION	(0x1 << 1)
	#define I2CM_STAT_BUSY_CONTROLLERISBUSY			(0x1)
	uint32_t dr;									// 0x008
	uint32_t tpr;									// 0x00C
	uint32_t imr;									// 0x010
	#define I2CM_IMR_IM_ENABLEINTERRUPTS			(0x1)
	uint32_t ris;									// 0x014
	#define I2CM_RIS_RIS_INTERRUPTPENDING			(0x1)
	uint32_t mis;									// 0x018
	#define I2CM_MIS_MIS_NOTMASKEDINTPENDING		(0x1)
	uint32_t icr;									// 0x01C
	#define I2CM_ICR_IC_CLEARINTERRUPT 				(0x1)
	uint32_t cr;									// 0x020
	#define I2CM_CR_SFE_SLAVEMODEENABLE				(0x1 << 5)
	#define I2CM_CR_MFE_MASTERMODEENABLE			(0x1 << 4)
	#define I2CM_CR_LPBK_LOOPBACKENABLE				(0x1)
} i2cm_t;


typedef struct
{	
	uint32_t oar;									// 0x000
	#define I2CS_STAT_FBR_FIRSTBYTERECEIVED			(0x1 << 2)
	#define I2CS_STAT_TREQ_TRASMITREQUESTED 		(0x1 << 1)
	#define I2CS_STAT_RREQ_RECEIVEREQUESTED 		(0x1)
	#define I2CS_CTRL_DA_ENABLEI2CSLAVE 			(0x1)
	uint32_t ctrl_stat;								// 0x004
	uint32_t dr;									// 0x008
	#define I2CS_IMR_STOPIM_STOPCONDINT				(0x1 << 2)
	#define I2CS_IMR_STARTIM_STARTCONDINT			(0x1 << 1)
	#define I2CS_IMR_DATAIM_DATAREQUESTINT			(0x1)
	uint32_t imr;									// 0x00C
	#define I2CS_RIS_STOPRIS_STOPCONDINT			(0x1 << 2)
	#define I2CS_RIS_STARRRIS_STARTCONDINT			(0x1 << 1)
	#define I2CS_RIS_DATARIS_DATAREQUESTINT			(0x1)
	uint32_t ris;									// 0x010
	#define I2CS_MIS_STOPMIS_STOPCONDINT			(0x1 << 2)
	#define I2CS_MIS_STARMIS_STARTCONDINT			(0x1 << 1)
	#define I2CS_MIS_DATAMIS_DATAREQUESTINT			(0x1)
	uint32_t mis;									// 0x014
	#define I2CS_ICR_STOPIC_STOPCONDINTCLEAR		(0x1 << 2)
	#define I2CS_ICR_STARTIC_STARTCONDINTCLEAR		(0x1 << 1)
	#define I2CS_ICR_DATAIC_DATAREQUESTINTCLEAR		(0x1)
	uint32_t icr;									// 0x018
} i2cs_t;

#define I2CM 										((volatile i2cm_t *)I2CM_BASE)
#define I2CS 										((volatile i2cs_t *)I2CS_BASE)

void cc2538I2cmInit(i2c_freq_t FREQ);
void cc2538I2cmStart(i2cm_unit_t *UNIT);

#endif