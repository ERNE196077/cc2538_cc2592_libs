#include <stdint.h>
#include <stdio.h>

#include "../include/gpio.h"
#include "../include/sys_ctrl.h"
#include "../include/cc2538-reg.h"
#include "../include/i2c.h"

extern void I2CMIntHandler(void);

i2cm_unit_t *HEAD = NULL;
uint8_t arblst = 0;
uint8_t nack = 0;


static void delay(void)
{
    volatile int i;
    for(i = 0; i < 1050; ++i)
    {
      __asm("nop");
    }
}


static void cc2538I2cmWaitForI2c(void)
{	
	while(!(I2CM->ctrl_stat & I2CM_STAT_IDLE_CONTROLLERISIDLE))
		;
	delay();
}


static void cc2538I2cmProcessUnit(i2cm_unit_t *UNIT)
{
	if (UNIT)
	{
		switch (UNIT->op){
			case I2CM_WRITE:
				I2CM->dr = (uint32_t)(UNIT->txdata);
			 	I2CM->ctrl_stat = I2CM_CTRL_RUN_ENABLEMASTERI2C;	
				break;
			case I2CM_WRITE_W_START:
				I2CM->sa = I2CM_SA_SA_SETSLAVEADDRESS(UNIT->address) |
			   			   I2CM_SA_RS_TRANSMIT;
			   	I2CM->dr = (uint32_t)(UNIT->txdata);
			 	I2CM->ctrl_stat = I2CM_CTRL_START_GENERATESTARTCONDITION | 
			 					  I2CM_CTRL_RUN_ENABLEMASTERI2C;	
				break;
			case I2CM_WRITE_W_STOP:
				I2CM->dr = (uint32_t)(UNIT->txdata);
			 	I2CM->ctrl_stat = I2CM_CTRL_STOP_GENERATESTOPCONDITION | 
			 					  I2CM_CTRL_RUN_ENABLEMASTERI2C;	
				break;
			case I2CM_WRITE_W_STARTSTOP:
				I2CM->sa = I2CM_SA_SA_SETSLAVEADDRESS(UNIT->address) |
			   			   I2CM_SA_RS_TRANSMIT;
			 	I2CM->dr = (uint32_t)(UNIT->txdata);
			 	I2CM->ctrl_stat = I2CM_CTRL_STOP_GENERATESTOPCONDITION | 
			 					  I2CM_CTRL_START_GENERATESTARTCONDITION | 
			 					  I2CM_CTRL_RUN_ENABLEMASTERI2C;	
				break;
			case I2CM_RECEIVE:
				I2CM->ctrl_stat = I2CM_CTRL_ACK_ACKDATAAUTOMATICALLY |
					  			  I2CM_CTRL_RUN_ENABLEMASTERI2C;
				break;
			case I2CM_RECEIVE_W_START:
				I2CM->sa = I2CM_SA_SA_SETSLAVEADDRESS(UNIT->address) |
			   			   I2CM_SA_RS_RECEIVE;
				I2CM->ctrl_stat = I2CM_CTRL_ACK_ACKDATAAUTOMATICALLY |
					  			  I2CM_CTRL_START_GENERATESTARTCONDITION | 
					  			  I2CM_CTRL_RUN_ENABLEMASTERI2C;
				break;
			case I2CM_RECEIVE_W_STOP:
				I2CM->ctrl_stat = I2CM_CTRL_STOP_GENERATESTOPCONDITION | 
					  			  I2CM_CTRL_RUN_ENABLEMASTERI2C;
				break;
			case I2CM_RECEIVE_W_STARTSTOP:
				I2CM->sa = I2CM_SA_SA_SETSLAVEADDRESS(UNIT->address) |
			   			   I2CM_SA_RS_RECEIVE;
				I2CM->ctrl_stat = I2CM_CTRL_STOP_GENERATESTOPCONDITION | 
					  			  I2CM_CTRL_START_GENERATESTARTCONDITION | 
					  			  I2CM_CTRL_RUN_ENABLEMASTERI2C;
				break;
		}
	}
}


void cc2538I2cmInit(i2c_freq_t FREQ)
{
	uint32_t tperiod = (SYS_CTRL_SYSCLOCKFREQ / (2 * 10 * FREQ)) - 1;
	SYS_CTRL->rcgci2c |= SYS_CTRL_RCGCI2C_I2C0_ENABLEI2C0CLOCK;
	SYS_CTRL->scgci2c |= SYS_CTRL_SCGCI2C_I2C0_ENABLEI2C0CLOCK;
	SYS_CTRL->dcgci2c |= SYS_CTRL_DCGCI2C_I2C0_ENABLEI2C0CLOCK;

	// scl pin
    IOCMUX->ioc_pb0_sel = IOC_MUX_OUT_SEL_I2C_SCL;
    IOCMUX->ioc_pb0_over = IOC_OVERRIDE_PUE;
    GPIO_B->afsel |= GPIO_PIN_0;
    IOCMUX->ioc_i2cmsscl = IOC_PB0;
    
	// sda pin
    IOCMUX->ioc_pb1_sel = IOC_MUX_OUT_SEL_I2C_SDA;
    IOCMUX->ioc_pb1_over = IOC_OVERRIDE_PUE;
    GPIO_B->afsel |= GPIO_PIN_1;
    IOCMUX->ioc_i2cmssda = IOC_PB1;	

	I2CM->cr = I2CM_CR_MFE_MASTERMODEENABLE;
	I2CM->tpr = tperiod;
	I2CM->imr = I2CM_IMR_IM_ENABLEINTERRUPTS;
	NVIC_ENABLE(INT_I2C);
}


void cc2538I2cmStart(i2cm_unit_t *UNIT)
{

	if(HEAD)
	{	
		i2cm_unit_t *last = HEAD;
		do
			if (last->next == NULL)
				break;
		while((last = last->next));
		last->next = UNIT;	
	}
	else
	{
		arblst = nack = 0;
		HEAD = UNIT;
		cc2538I2cmProcessUnit(HEAD);
	}
	cc2538I2cmWaitForI2c();
}


void I2CMIntHandler(void)
{
	NVIC_DISABLE(INT_I2C);
	while (I2CM->ctrl_stat & I2CM_STAT_BUSY_CONTROLLERISBUSY)
		;
		
	if(I2CM->ctrl_stat & I2CM_STAT_ARBLST_ARBITRATIONLOST)
		arblst = 1;

	if(I2CM->ctrl_stat & I2CM_STAT_ERROR_ERRORINLASTOPERATION)
	{
		I2CM->ctrl_stat = 	I2CM_CTRL_STOP_GENERATESTOPCONDITION |
							I2CM_CTRL_RUN_ENABLEMASTERI2C;
		nack = 1;
	}

	I2CM->icr = I2CM_ICR_IC_CLEARINTERRUPT;
	NVIC_ENABLE(INT_I2C);
	if (!arblst || !nack)
	{	
		if (HEAD->op >= I2CM_RECEIVE)
		{
			*(HEAD->rxdata) = I2CM->dr;
		}
		HEAD = HEAD->next;
		if(HEAD)
			cc2538I2cmProcessUnit(HEAD);
	}
}


