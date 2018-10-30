#include <stdint.h>
#include <stdio.h>


#include "../include/gptimer.h"
#include "../include/sys_ctrl.h"
#include "../include/gpio.h"
#include "../include/i2c.h"
#include <openthread/cli.h>

#if OPENTHREAD_CONFIG_ENABLE_DEBUG_UART
	#include <openthread/platform/debug_uart.h>
#endif


void cc2538GPTimerStartClock(volatile gptimer_t *GPTIMER)
{
	switch((uint32_t)GPTIMER)
	{
		case GPTIMER0_BASE:
			SYS_CTRL->rcgcgpt |= SYS_CTRL_RCGCGPT_GPT0_ENABLEGPTIMER0CLOCK;
			SYS_CTRL->scgcgpt |= SYS_CTRL_SCGCGPT_GPT0_ENABLEGPTIMER0CLOCK;
			SYS_CTRL->dcgcgpt |= SYS_CTRL_DCGCGPT_GPT0_ENABLEGPTIMER0CLOCK;
			break;
		case GPTIMER1_BASE:
			SYS_CTRL->rcgcgpt |= SYS_CTRL_RCGCGPT_GPT1_ENABLEGPTIMER1CLOCK;
			SYS_CTRL->scgcgpt |= SYS_CTRL_SCGCGPT_GPT1_ENABLEGPTIMER1CLOCK;
			SYS_CTRL->dcgcgpt |= SYS_CTRL_DCGCGPT_GPT1_ENABLEGPTIMER1CLOCK;
			break;
		case GPTIMER2_BASE:
			SYS_CTRL->rcgcgpt |= SYS_CTRL_RCGCGPT_GPT2_ENABLEGPTIMER2CLOCK;
			SYS_CTRL->scgcgpt |= SYS_CTRL_SCGCGPT_GPT2_ENABLEGPTIMER2CLOCK;
			SYS_CTRL->dcgcgpt |= SYS_CTRL_DCGCGPT_GPT2_ENABLEGPTIMER2CLOCK;
			break;
		case GPTIMER3_BASE:
			SYS_CTRL->rcgcgpt |= SYS_CTRL_RCGCGPT_GPT3_ENABLEGPTIMER3CLOCK;
			SYS_CTRL->scgcgpt |= SYS_CTRL_SCGCGPT_GPT3_ENABLEGPTIMER3CLOCK;
			SYS_CTRL->dcgcgpt |= SYS_CTRL_DCGCGPT_GPT3_ENABLEGPTIMER3CLOCK;
			break;
	}
}

void cc2538GPTimerStopClock(volatile gptimer_t *GPTIMER)
{
	switch((uint32_t)GPTIMER)
	{
		case GPTIMER0_BASE:
			SYS_CTRL->rcgcgpt &= ~SYS_CTRL_RCGCGPT_GPT0_ENABLEGPTIMER0CLOCK;
			SYS_CTRL->scgcgpt &= ~SYS_CTRL_SCGCGPT_GPT0_ENABLEGPTIMER0CLOCK;
			SYS_CTRL->dcgcgpt &= ~SYS_CTRL_DCGCGPT_GPT0_ENABLEGPTIMER0CLOCK;
			break;
		case GPTIMER1_BASE:
			SYS_CTRL->rcgcgpt &= ~SYS_CTRL_RCGCGPT_GPT1_ENABLEGPTIMER1CLOCK;
			SYS_CTRL->scgcgpt &= ~SYS_CTRL_SCGCGPT_GPT1_ENABLEGPTIMER1CLOCK;
			SYS_CTRL->dcgcgpt &= ~SYS_CTRL_DCGCGPT_GPT1_ENABLEGPTIMER1CLOCK;
			break;
		case GPTIMER2_BASE:
			SYS_CTRL->rcgcgpt &= ~SYS_CTRL_RCGCGPT_GPT2_ENABLEGPTIMER2CLOCK;
			SYS_CTRL->scgcgpt &= ~SYS_CTRL_SCGCGPT_GPT2_ENABLEGPTIMER2CLOCK;
			SYS_CTRL->dcgcgpt &= ~SYS_CTRL_DCGCGPT_GPT2_ENABLEGPTIMER2CLOCK;
			break;
		case GPTIMER3_BASE:
			SYS_CTRL->rcgcgpt &= ~SYS_CTRL_RCGCGPT_GPT3_ENABLEGPTIMER3CLOCK;
			SYS_CTRL->scgcgpt &= ~SYS_CTRL_SCGCGPT_GPT3_ENABLEGPTIMER3CLOCK;
			SYS_CTRL->dcgcgpt &= ~SYS_CTRL_DCGCGPT_GPT3_ENABLEGPTIMER3CLOCK;
			break;
	}
}

void cc2538GPTimerStartNVIC(volatile gptimer_t *GPTIMER, gptimer_side_t SIDE)
{
	switch(SIDE)
	{
		case A:
			switch((uint32_t)GPTIMER)
			{	
				case GPTIMER0_BASE:
					NVIC_ENABLE(INT_GPTIMER0A);
					break;
				case GPTIMER1_BASE:
					NVIC_ENABLE(INT_GPTIMER1A);
					break;
				case GPTIMER2_BASE:
					NVIC_ENABLE(INT_GPTIMER2A);
					break;
				case GPTIMER3_BASE:
					NVIC_ENABLE(INT_GPTIMER3A);
					break;
			}
			break;
		case B:
			switch((uint32_t)GPTIMER)
			{	
				case GPTIMER0_BASE:
					NVIC_ENABLE(INT_GPTIMER0B);
					break;
				case GPTIMER1_BASE:
					NVIC_ENABLE(INT_GPTIMER1B);
					break;
				case GPTIMER2_BASE:
					NVIC_ENABLE(INT_GPTIMER2B);
					break;
				case GPTIMER3_BASE:
					NVIC_ENABLE(INT_GPTIMER3B);
					break;
			}
			break;
	}
}


void cc2538GPTimerStopNVIC(volatile gptimer_t *GPTIMER, gptimer_side_t SIDE)
{
	switch(SIDE)
	{
		case A:
			switch((uint32_t)GPTIMER)
			{	
				case GPTIMER0_BASE:
					NVIC_DISABLE(INT_GPTIMER0A);
					break;
				case GPTIMER1_BASE:
					NVIC_DISABLE(INT_GPTIMER1A);
					break;
				case GPTIMER2_BASE:
					NVIC_DISABLE(INT_GPTIMER2A);
					break;
				case GPTIMER3_BASE:
					NVIC_DISABLE(INT_GPTIMER3A);
					break;
			}
			break;
		case B:
			switch((uint32_t)GPTIMER)
			{	
				case GPTIMER0_BASE:
					NVIC_DISABLE(INT_GPTIMER0B);
					break;
				case GPTIMER1_BASE:
					NVIC_DISABLE(INT_GPTIMER1B);
					break;
				case GPTIMER2_BASE:
					NVIC_DISABLE(INT_GPTIMER2B);
					break;
				case GPTIMER3_BASE:
					NVIC_DISABLE(INT_GPTIMER3B);
					break;
			}
			break;
	}
}


void cc2538GPTimer32BitInit(volatile gptimer_t *GPTIMER, 
					   		uint32_t PERIOD)
{
	if (GPTIMER->ctl & (GPTIMER_CTL_TBEN_TIMERENABLE | GPTIMER_CTL_TAEN_TIMERENABLE))
	{	
		#if OPENTHREAD_CONFIG_ENABLE_DEBUG_UART
		otPlatDebugUart_printf("gptimer.c:cc2538GPTimer32BitInit:ERROR:GPTimer already in use, not configuring.\n\r");
		#endif
		return;
	}

	uint64_t sys_clock_freq = SYS_CTRL_SYSCLOCKFREQ;
	uint64_t count = (sys_clock_freq * PERIOD) / 1000;
	
	cc2538GPTimerStartClock(GPTIMER);
    GPTIMER->ctl &= ~(GPTIMER_CTL_TBEN_TIMERENABLE | GPTIMER_CTL_TAEN_TIMERENABLE);
	GPTIMER->cfg = 0x0;
	GPTIMER->cfg |= GPTIMER_CFG_GPTMCFG_32BIT;
	GPTIMER->tamr |= GPTIMER_TxMR_TxCDIR_COUNTDOWN |
					 GPTIMER_TxMR_TxMR_PERIODIC;
	GPTIMER->tailr = (uint32_t)(count & 0xFFFFFFFF);
	GPTIMER->imr |= GPTIMER_IMR_TATOIM_TIMEOUTINTMASKENABLE;
	cc2538GPTimerStartNVIC(GPTIMER, A);
}


void cc2538GPTimer32BitStart(volatile gptimer_t *GPTIMER)
{
	if (GPTIMER->ctl & (GPTIMER_CTL_TBEN_TIMERENABLE | GPTIMER_CTL_TAEN_TIMERENABLE))
	{
		#if OPENTHREAD_CONFIG_ENABLE_DEBUG_UART
		otPlatDebugUart_printf("gptimer.c:cc2538GPTimer32BitStart:WARNING:GPTimer already running, will not start again.\n\r");
		#endif
		return;
	}
	GPTIMER->ctl |= GPTIMER_CTL_TAEN_TIMERENABLE; 
}


void cc2538GPTimer32BitStop(volatile gptimer_t *GPTIMER)
{
	cc2538GPTimerStopNVIC(GPTIMER, A);
	GPTIMER->icr |= GPTIMER_ICR_TATOCINT_TIMEOUTINT;
	GPTIMER->ctl &= ~(GPTIMER_CTL_TAEN_TIMERENABLE);
}


void cc2538GPTimer16BitInit(volatile gptimer_t *GPTIMER, 
					   		uint32_t PERIOD,
					   		gptimer_side_t SIDE)
{
	/*	If the timer side choosen is running, do nothing */
	if (GPTIMER->ctl & (SIDE == A ? GPTIMER_CTL_TAEN_TIMERENABLE : GPTIMER_CTL_TBEN_TIMERENABLE))
	{	
		#if OPENTHREAD_CONFIG_ENABLE_DEBUG_UART
		otPlatDebugUart_printf("gptimer.c:cc2538GPTimer16BitInit:ERROR:GPTimer already in use, not configuring.\n\r");
		#endif
		return;
	}
	
	/* if the configuration is not set in 16Bit, check for the side A (In case previous "if" was to B)	*/
	if (((GPTIMER->cfg & 0x7) != GPTIMER_CFG_GPTMCFG_16BIT) &&
		GPTIMER->ctl & GPTIMER_CTL_TAEN_TIMERENABLE)
	{
		#if OPENTHREAD_CONFIG_ENABLE_DEBUG_UART
		otPlatDebugUart_printf("gptimer.c:cc2538GPTimer16BitInit:ERROR:GPTimer already in use (as 32bit), not configuring.\n\r");
		#endif
		return;
	}
	
	uint64_t sys_clock_freq = SYS_CTRL_SYSCLOCKFREQ;
	uint64_t count = (sys_clock_freq * PERIOD) / 1000;
	
	cc2538GPTimerStartClock(GPTIMER);
	GPTIMER->ctl &= ~(SIDE == A ? GPTIMER_CTL_TAEN_TIMERENABLE : GPTIMER_CTL_TBEN_TIMERENABLE);
	
	if ((GPTIMER->cfg & 0x7) != GPTIMER_CFG_GPTMCFG_16BIT)
		GPTIMER->cfg = GPTIMER_CFG_GPTMCFG_16BIT;

	if (SIDE == A)
	{
		GPTIMER->tamr |= GPTIMER_TxMR_TxMR_PERIODIC | GPTIMER_TxMR_TxCDIR_COUNTUP;
		GPTIMER->tailr = (uint32_t)((count) & 0xFFFF);
		GPTIMER->tapr = (uint32_t)((count >> 16) & 0xFF);
		GPTIMER->imr |= GPTIMER_IMR_TATOIM_TIMEOUTINTMASKENABLE;
		cc2538GPTimerStartNVIC(GPTIMER, SIDE);
	}
	else
	{
		GPTIMER->tbmr |= GPTIMER_TxMR_TxMR_PERIODIC | GPTIMER_TxMR_TxCDIR_COUNTUP;
		GPTIMER->tbilr = (uint32_t)((count) & 0xFFFF);
		GPTIMER->tbpr = (uint32_t)((count >> 16) & 0xFF);
		GPTIMER->imr |= GPTIMER_IMR_TBTOIM_TIMEOUTINTMASKENABLE;
		cc2538GPTimerStartNVIC(GPTIMER, SIDE);
	}
}


void cc2538GPTimer16BitStart(volatile gptimer_t *GPTIMER,
					   		 gptimer_side_t SIDE)
{
	if (GPTIMER->ctl & (SIDE == A ? GPTIMER_CTL_TAEN_TIMERENABLE : GPTIMER_CTL_TBEN_TIMERENABLE))
	{
		#if OPENTHREAD_CONFIG_ENABLE_DEBUG_UART
		otPlatDebugUart_printf("gptimer.c:cc2538GPTimer32BitStart:WARNING:GPTimer already running, will not start again.\n\r");
		#endif

		return;
	}
	GPTIMER->ctl |= (SIDE == A ? GPTIMER_CTL_TAEN_TIMERENABLE : GPTIMER_CTL_TBEN_TIMERENABLE); 
}


void cc2538GPTimer16BitStop(volatile gptimer_t *GPTIMER,
					   		gptimer_side_t SIDE)
{
	cc2538GPTimerStopNVIC(GPTIMER, SIDE);
	GPTIMER->icr |= (SIDE == A ? GPTIMER_ICR_TATOCINT_TIMEOUTINT : GPTIMER_ICR_TBTOCINT_TIMEOUTINT);
	GPTIMER->ctl &= ~(SIDE == A ? GPTIMER_CTL_TAEN_TIMERENABLE : GPTIMER_CTL_TBEN_TIMERENABLE);
}


void cc2538GPTimerPWMInit(volatile gptimer_t *GPTIMER,
						  gptimer_side_t SIDE,
						  uint32_t PERIOD, /* usec */
						  uint8_t INIT_DUTY /* 0% - 100% */)
{
	/*	If the timer side choosen is running, do nothing */
	if (GPTIMER->ctl & (SIDE == A ? GPTIMER_CTL_TAEN_TIMERENABLE : GPTIMER_CTL_TBEN_TIMERENABLE))
	{	
		#if OPENTHREAD_CONFIG_ENABLE_DEBUG_UART
			otPlatDebugUart_printf("gptimer.c:cc2538GPTimerPWMInit:ERROR:GPTimer already in use, not configuring.\n\r");
		#endif
		return;
	}
	
	/* if the configuration is not set in 16Bit, check for the side A (In case previous "if" was to B)	*/
	if (((GPTIMER->cfg & 0x7) != GPTIMER_CFG_GPTMCFG_16BIT) &&
		GPTIMER->ctl & GPTIMER_CTL_TAEN_TIMERENABLE)
	{
		#if OPENTHREAD_CONFIG_ENABLE_DEBUG_UART
			otPlatDebugUart_printf("gptimer.c:cc2538GPTimerPWMInit:ERROR:GPTimer already in use (as 32bit), not configuring.\n\r");
		#endif
		return;
	}

	IOCMUX->ioc_pa4_sel = IOC_MUX_OUT_SEL_GPT0CP1;
    IOCMUX->ioc_pa4_over = IOC_OVERRIDE_OE;
    GPIO_A->afsel |= GPIO_PIN_4;

	uint64_t sys_clock_freq = SYS_CTRL_SYSCLOCKFREQ;
	uint64_t count = (sys_clock_freq * PERIOD) / 1000000;
	uint32_t duty = (count * (INIT_DUTY > 100 ? 100 : INIT_DUTY))/100;
	
	cc2538GPTimerStartClock(GPTIMER);
	GPTIMER->ctl &= ~(SIDE == A ? GPTIMER_CTL_TAEN_TIMERENABLE : GPTIMER_CTL_TBEN_TIMERENABLE);
	
	if ((GPTIMER->cfg & 0x7) != GPTIMER_CFG_GPTMCFG_16BIT)
		GPTIMER->cfg = GPTIMER_CFG_GPTMCFG_16BIT;

	volatile uint32_t *txmr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tamr) : &(GPTIMER->tbmr));
	volatile uint32_t *txilr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tailr) : &(GPTIMER->tbilr));
	volatile uint32_t *txpr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tapr) : &(GPTIMER->tbpr));
	volatile uint32_t *txmatchr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tamatchr) : &(GPTIMER->tbmatchr));
	volatile uint32_t *txpmr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tapmr) : &(GPTIMER->tbpmr));
	
	*txmr |= GPTIMER_TxMR_TxMR_PERIODIC | 
			 GPTIMER_TxMR_TxCDIR_COUNTDOWN |
			 GPTIMER_TxMR_TxCMR_EDGECOUNT |
			 GPTIMER_TxMR_TxAMS_PWMMODE |
			 GPTIMER_TxMR_TxILD_UPDTMTARONTIMEOUT;

	*txilr = (uint32_t)((count) & 0xFFFF);
	*txpr = (uint32_t)((count >> 16) & 0xFF);
	*txmatchr = (uint32_t)((duty) & 0xFFFF);
	*txpmr = (uint32_t)((duty >> 16) & 0xFF);
	
	GPTIMER->ctl |= (SIDE == A ? GPTIMER_CTL_TAEN_TIMERENABLE : GPTIMER_CTL_TBEN_TIMERENABLE);	
}


void cc2538GPTimerPWMStop(volatile gptimer_t *GPTIMER, gptimer_side_t SIDE)
{
	volatile uint32_t *txmr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tamr) : &(GPTIMER->tbmr));

	if (!(*txmr & GPTIMER_TxMR_TxAMS_PWMMODE))
	{	
		#if OPENTHREAD_CONFIG_ENABLE_DEBUG_UART
			otPlatDebugUart_printf("gptimer.c:cc2538GPTimerPWMStop:ERROR:GPTimer not configured in PWM. Will not stop timer.\n\r");
		#endif
		return;
	}
	
	GPTIMER->ctl &= ~(SIDE == A ? GPTIMER_CTL_TAEN_TIMERENABLE : GPTIMER_CTL_TBEN_TIMERENABLE);
}


void cc2538GPTimerPWMSetDuty(volatile gptimer_t *GPTIMER, gptimer_side_t SIDE, uint8_t DUTY /* 0% - 100% */)
{
	volatile uint32_t *txmr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tamr) : &(GPTIMER->tbmr));

	if (!(*txmr & GPTIMER_TxMR_TxAMS_PWMMODE))
	{	
		#if OPENTHREAD_CONFIG_ENABLE_DEBUG_UART
			otPlatDebugUart_printf("gptimer.c:cc2538GPTimerPWMSetDuty:ERROR:GPTimer not configured in PWM. Will not modify duty.\n\r");
		#endif
		return;
	}

	volatile uint32_t *txilr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tailr) : &(GPTIMER->tbilr));
	volatile uint32_t *txpr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tapr) : &(GPTIMER->tbpr));
	volatile uint32_t *txmatchr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tamatchr) : &(GPTIMER->tbmatchr));
	volatile uint32_t *txpmr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tapmr) : &(GPTIMER->tbpmr));
	
	uint64_t count = (((*txpr) & 0xFF) << 16) | ((*txilr) & 0xFFFF);
	uint32_t duty = (count * (DUTY > 100 ? 100 : DUTY))/100;
	
	*txmatchr = (uint32_t)((duty) & 0xFFFF);
	*txpmr = (uint32_t)((duty >> 16) & 0xFF);
}


void cc2538GPTimerPWMSetPeriod(volatile gptimer_t *GPTIMER, gptimer_side_t SIDE, uint32_t PERIOD /* ms */)
{
	volatile uint32_t *txmr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tamr) : &(GPTIMER->tbmr));

	if (!(*txmr & GPTIMER_TxMR_TxAMS_PWMMODE))
	{	
		#if OPENTHREAD_CONFIG_ENABLE_DEBUG_UART
			otPlatDebugUart_printf("gptimer.c:cc2538GPTimerPWMSetPeriod:ERROR:GPTimer not configured in PWM. Will not change period.\n\r");
		#endif
		return;
	}
	
	volatile uint32_t *txilr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tailr) : &(GPTIMER->tbilr));
	volatile uint32_t *txpr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tapr) : &(GPTIMER->tbpr));
	volatile uint32_t *txmatchr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tamatchr) : &(GPTIMER->tbmatchr));
	volatile uint32_t *txpmr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tapmr) : &(GPTIMER->tbpmr));
	
	uint64_t count = (((*txpr) & 0xFF) << 16) | ((*txilr) & 0xFFFF);
	uint32_t duty = (((((*txpmr) & 0xFF) << 16) | ((*txmatchr) & 0xFFFF)) * 100) / count;
	uint64_t sys_clock_freq = SYS_CTRL_SYSCLOCKFREQ;
	count = (sys_clock_freq * PERIOD) / 1000000;
	duty = (count * duty)/100;
	
	*txilr = (uint32_t)((count) & 0xFFFF);
	*txpr = (uint32_t)((count >> 16) & 0xFF);
	*txmatchr = (uint32_t)((duty) & 0xFFFF);
	*txpmr = (uint32_t)((duty >> 16) & 0xFF);
}


void cc2538GPTimerPWMSetDutyPeriod(volatile gptimer_t *GPTIMER, gptimer_side_t SIDE, uint32_t PERIOD /* ms */, uint8_t DUTY /* 0% - 100% */)
{
	volatile uint32_t *txmr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tamr) : &(GPTIMER->tbmr));

	if (!(*txmr & GPTIMER_TxMR_TxAMS_PWMMODE))
	{	
		#if OPENTHREAD_CONFIG_ENABLE_DEBUG_UART
			otPlatDebugUart_printf("gptimer.c:cc2538GPTimerPWMSetDutyPeriod:ERROR:GPTimer not configured in PWM. Will not modify.\n\r");
		#endif
		return;
	}

	volatile uint32_t *txilr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tailr) : &(GPTIMER->tbilr));
	volatile uint32_t *txpr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tapr) : &(GPTIMER->tbpr));
	volatile uint32_t *txmatchr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tamatchr) : &(GPTIMER->tbmatchr));
	volatile uint32_t *txpmr = (volatile uint32_t *)(SIDE == A ? &(GPTIMER->tapmr) : &(GPTIMER->tbpmr));
	
	uint64_t sys_clock_freq = SYS_CTRL_SYSCLOCKFREQ;
	uint64_t count = (sys_clock_freq * PERIOD) / 1000000;
	uint32_t duty = (count * (DUTY > 100 ? 100 : DUTY))/100;
	
	*txilr = (uint32_t)((count) & 0xFFFF);
	*txpr = (uint32_t)((count >> 16) & 0xFF);
	*txmatchr = (uint32_t)((duty) & 0xFFFF);
	*txpmr = (uint32_t)((duty >> 16) & 0xFF);
}

