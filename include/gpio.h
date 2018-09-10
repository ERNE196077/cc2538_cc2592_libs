#ifndef CC2538_GPIO_H_
#define CC2538_GPIO_H_

#include "common.h"
#include "cc2538-reg.h"


typedef struct 
{
	uint32_t data;											// GPIO state read (Input) or write (Output); Offset 0x000
	#define GPIO_PIN_0                          (0x1)  		// GPIO pin 0
	#define GPIO_PIN_1                          (0x1 << 1)  // GPIO pin 1
	#define GPIO_PIN_2                          (0x1 << 2)  // GPIO pin 2
	#define GPIO_PIN_3                          (0x1 << 3)  // GPIO pin 3
	#define GPIO_PIN_4                          (0x1 << 4)  // GPIO pin 4
	#define GPIO_PIN_5                          (0x1 << 5)  // GPIO pin 5
	#define GPIO_PIN_6                          (0x1 << 6)  // GPIO pin 6
	#define GPIO_PIN_7                          (0x1 << 7)  // GPIO pin 7
	uint32_t reserved0[255];
	uint32_t dir;											// Direction; 1 - Output; 0 - Input; Offset 0x400
	uint32_t is;											// Int Sense; 1 - Detect level; 0 - Detect edges; Offset 0x404
	uint32_t ibe;											// Int Both Edges; 1 - Both edges trigger int; 0 - GPIO_IEV manages interrupt events.; Offset 0x408
	uint32_t iev;											// Int Event; 1 - Rising Edge/High Level; 0 - Falling Edge/Low Level; Offset 0x40C
	uint32_t ie;											// Int Enable: 1 - Int Enabled; 0 - Int Disabled ; Offset 0x410
	uint32_t ris;											// State of int before masking: 1 - Conditions meet; 0 - No int conditions happened; Offset 0x414
	uint32_t mis;											// State of int after masking: 1 - Int active; 0 - Int not active; Offset 0x418
	uint32_t ic;											// Int Clear; 1 - Write to clear int edge detection; 0 - No effect; Offset 0x41C
	uint32_t afsel;											// Peripheral or GPIO; 1 - Peripheral mode; 0 - GPIO mode; Offset 0x420
	uint32_t reserved1[63];
	uint32_t gpiolock;										// Write 0x4C4F434B to unlock GPIOCR register, other value will lock it; Offset 0x520
	#define GPIO_GPIOLOCK_UNLOCKGPIOCR			(0x4C4F434B)
	#define GPIO_GPIOLOCK_LOCKGPIOCR 			(0x1)
	uint32_t gpiocr;										// Commit register; 1 - Allow AFSEL writes to be commited; 0 - AFSEL writes doesn't take effect; Offset 0x524
	uint32_t reserved2[118];
	uint32_t pmux;											// External decouple control and clock, see datasheet for instructions; Offset 0x700
	#define GPIO_PMUX_CKOEN						(0x1 << 7)
	#define GPIO_PMUX_CKOPIN					(0x1 << 4)
	#define GPIO_PMUX_DCEN						(0x1 << 3)
	#define GPIO_PMUX_DCPIN						(0x1)
	uint32_t p_edge_ctrl;									// Power up interrupt control; 0 - Rising edge; 1 - Falling edge; bit 0 = PA0; bit 31 = PD7; Offset 0x704
	uint32_t pi_ien;										// Power up interrupt enable; 0 - Enable; 1 - Disable; bit 0 = PA0; bit 31 = PD7; Offset 0x710
	uint32_t irq_detect_ack;								// Power up interrupt ack; Read to determine the pin causing the interrupt.; Offset 0x718
	uint32_t usb_irq_ack;									// Same function as GPIO_IRQ_DETECT_ACK_OFFSET but for USB.; Offset 0x71C
	uint32_t irq_detect_unmask;								// Same function as GPIO_IRQ_DETECT_ACK_OFFSET but for masked interrupts.; Offset 0x720
				 	
} gpio_t;

#define GPIO_SET_INPUT(GPIOBASE, PINS)				GPIOBASE->dir &= ~(PINS)
#define GPIO_SET_OUTPUT(GPIOBASE, PINS)				GPIOBASE->dir |= (PINS)
#define GPIO_SET_ASPERIPHERAL(GPIOBASE, PINS)		GPIOBASE->afsel |= (PINS)
#define GPIO_SET_ASGPIO(GPIOBASE, PINS)				GPIOBASE->afsel &= ~(PINS)

typedef struct 
{
	#define IOC_MUX_OUT_SEL_UART0_TXD				(0x0)
	#define IOC_MUX_OUT_SEL_UART1_RTS				(0x1)
	#define IOC_MUX_OUT_SEL_UART1_TXD				(0x2)
	#define IOC_MUX_OUT_SEL_SSI0_TXD				(0x3)
	#define IOC_MUX_OUT_SEL_SSI0_CLK_OUT			(0x4)
	#define IOC_MUX_OUT_SEL_SSI0_FSS_OUT			(0x5)
	#define IOC_MUX_OUT_SEL_SSI0_TX_SER_OUT			(0x6)
	#define IOC_MUX_OUT_SEL_SSI1_TXD				(0x7)
	#define IOC_MUX_OUT_SEL_SSI1_CLK_OUT			(0x8)
	#define IOC_MUX_OUT_SEL_SSI1_FSS_OUT			(0x9)
	#define IOC_MUX_OUT_SEL_SSI1_TX_SER_OUT			(0xA)
	#define IOC_MUX_OUT_SEL_I2C_SDA					(0xB)
	#define IOC_MUX_OUT_SEL_I2C_SCL					(0xC)
	#define IOC_MUX_OUT_SEL_GPT0CP1					(0xD)
	#define IOC_MUX_OUT_SEL_GPT0CP2					(0xE)
	#define IOC_MUX_OUT_SEL_GPT1CP1					(0xF)
	#define IOC_MUX_OUT_SEL_GPT1CP2					(0x10)
	#define IOC_MUX_OUT_SEL_GPT2CP1					(0x11)
	#define IOC_MUX_OUT_SEL_GPT2CP2					(0x12)
	#define IOC_MUX_OUT_SEL_GPT3CP1					(0x13)
	#define IOC_MUX_OUT_SEL_GPT3CP2					(0x14)	
	uint32_t ioc_pa0_sel;                           // 0x400D4000
	uint32_t ioc_pa1_sel;                           // 0x400D4004
	uint32_t ioc_pa2_sel;                           // 0x400D4008
	uint32_t ioc_pa3_sel;                           // 0x400D400C
	uint32_t ioc_pa4_sel;                           // 0x400D4010
	uint32_t ioc_pa5_sel;                           // 0x400D4014
	uint32_t ioc_pa6_sel;                           // 0x400D4018
	uint32_t ioc_pa7_sel;                           // 0x400D401C
	uint32_t ioc_pb0_sel;                           // 0x400D4020
	uint32_t ioc_pb1_sel;                           // 0x400D4024
	uint32_t ioc_pb2_sel;                           // 0x400D4028
	uint32_t ioc_pb3_sel;                           // 0x400D402C
	uint32_t ioc_pb4_sel;                           // 0x400D4030
	uint32_t ioc_pb5_sel;                           // 0x400D4034
	uint32_t ioc_pb6_sel;                           // 0x400D4038
	uint32_t ioc_pb7_sel;                           // 0x400D403C
	uint32_t ioc_pc0_sel;                           // 0x400D4040
	uint32_t ioc_pc1_sel;                           // 0x400D4044
	uint32_t ioc_pc2_sel;                           // 0x400D4048
	uint32_t ioc_pc3_sel;                           // 0x400D404C
	uint32_t ioc_pc4_sel;                           // 0x400D4050
	uint32_t ioc_pc5_sel;                           // 0x400D4054
	uint32_t ioc_pc6_sel;                           // 0x400D4058
	uint32_t ioc_pc7_sel;                           // 0x400D405C
	uint32_t ioc_pd0_sel;                           // 0x400D4060
	uint32_t ioc_pd1_sel;                           // 0x400D4064
	uint32_t ioc_pd2_sel;                           // 0x400D4068
	uint32_t ioc_pd3_sel;                           // 0x400D406C
	uint32_t ioc_pd4_sel;                           // 0x400D4070
	uint32_t ioc_pd5_sel;                           // 0x400D4074
	uint32_t ioc_pd6_sel;                           // 0x400D4078
	uint32_t ioc_pd7_sel;                           // 0x400D407C
	#define IOC_OVERRIDE_DIS                        (0x0)
	#define IOC_OVERRIDE_ANA                        (0x1)
	#define IOC_OVERRIDE_PDE                        (0x2)
	#define IOC_OVERRIDE_PUE                        (0x4)
	#define IOC_OVERRIDE_OE                         (0x8)
	uint32_t ioc_pa0_over;                          // 0x400D4080
	uint32_t ioc_pa1_over;                          // 0x400D4084
	uint32_t ioc_pa2_over;                          // 0x400D4088
	uint32_t ioc_pa3_over;                          // 0x400D408C
	uint32_t ioc_pa4_over;                          // 0x400D4090
	uint32_t ioc_pa5_over;                          // 0x400D4094
	uint32_t ioc_pa6_over;                          // 0x400D4098
	uint32_t ioc_pa7_over;                          // 0x400D409C
	uint32_t ioc_pb0_over;                          // 0x400D40A0
	uint32_t ioc_pb1_over;                          // 0x400D40A4
	uint32_t ioc_pb2_over;                          // 0x400D40A8
	uint32_t ioc_pb3_over;                          // 0x400D40AC
	uint32_t ioc_pb4_over;                          // 0x400D40B0
	uint32_t ioc_pb5_over;                          // 0x400D40B4
	uint32_t ioc_pb6_over;                          // 0x400D40B8
	uint32_t ioc_pb7_over;                          // 0x400D40BC
	uint32_t ioc_pc0_over;                          // 0x400D40C0
	uint32_t ioc_pc1_over;                          // 0x400D40C4
	uint32_t ioc_pc2_over;                          // 0x400D40C8
	uint32_t ioc_pc3_over;                          // 0x400D40CC
	uint32_t ioc_pc4_over;                          // 0x400D40D0
	uint32_t ioc_pc5_over;                          // 0x400D40D4
	uint32_t ioc_pc6_over;                          // 0x400D40D8
	uint32_t ioc_pc7_over;                          // 0x400D40DC
	uint32_t ioc_pd0_over;                          // 0x400D40E0
	uint32_t ioc_pd1_over;                          // 0x400D40E4
	uint32_t ioc_pd2_over;                          // 0x400D40E8
	uint32_t ioc_pd3_over;                          // 0x400D40EC
	uint32_t ioc_pd4_over;                          // 0x400D40F0
	uint32_t ioc_pd5_over;                          // 0x400D40F4
	uint32_t ioc_pd6_over;                          // 0x400D40F8
	uint32_t ioc_pd7_over;                          // 0x400D40FC
	#define IOC_PA0 								0
	#define IOC_PA1 								1
	#define IOC_PA2 								2
	#define IOC_PA3 								3
	#define IOC_PA4 								4
	#define IOC_PA5 								5
	#define IOC_PA6 								6
	#define IOC_PA7 								7
	#define IOC_PB0 								8
	#define IOC_PB1 								9
	#define IOC_PB2 								10
	#define IOC_PB3 								11
	#define IOC_PB4 								12
	#define IOC_PB5 								13
	#define IOC_PB6 								14
	#define IOC_PB7 								15
	#define IOC_PC0 								16
	#define IOC_PC1 								17
	#define IOC_PC2 								18
	#define IOC_PC3 								19
	#define IOC_PC4 								20
	#define IOC_PC5 								21
	#define IOC_PC6 								22
	#define IOC_PC7 								23
	#define IOC_PD0 								24
	#define IOC_PD1 								25
	#define IOC_PD2 								26
	#define IOC_PD3 								27
	#define IOC_PD4 								28
	#define IOC_PD5 								29
	#define IOC_PD6 								30
	#define IOC_PD7 								31
	uint32_t ioc_uartrxd_uart0;                     // 0x400D4100
	uint32_t ioc_uartcts_uart1;                     // 0x400D4104
	uint32_t ioc_uartrxd_uart1;                     // 0x400D4108
	uint32_t ioc_clk_ssi_ssi0;                      // 0x400D410C
	uint32_t ioc_ssirxd_ssi0;                       // 0x400D4110
	uint32_t ioc_ssifssin_ssi0;                     // 0x400D4114
	uint32_t ioc_clk_ssiin_ssi0;                    // 0x400D4118
	uint32_t ioc_clk_ssi_ssi1;                      // 0x400D411C
	uint32_t ioc_ssirxd_ssi1;                       // 0x400D4120
	uint32_t ioc_ssifssin_ssi1;                     // 0x400D4124
	uint32_t ioc_clk_ssiin_ssi1;                    // 0x400D4128
	uint32_t ioc_i2cmssda;                          // 0x400D412C
	uint32_t ioc_i2cmsscl;                          // 0x400D4130
	uint32_t ioc_gpt0ocp1;                          // 0x400D4134
	uint32_t ioc_gpt0ocp2;                          // 0x400D4138
	uint32_t ioc_gpt1ocp1;                          // 0x400D413C
	uint32_t ioc_gpt1ocp2;                          // 0x400D4140
	uint32_t ioc_gpt2ocp1;                          // 0x400D4144
	uint32_t ioc_gpt2ocp2;                          // 0x400D4148
	uint32_t ioc_gpt3ocp1;                          // 0x400D414C
	uint32_t ioc_gpt3ocp2;                          // 0x400D4150
} iocmux_t;

#define IOCMUX									((volatile iocmux_t *)(IOCMUX_BASE))
#define GPIO_A 									((volatile gpio_t *)(GPIO_A_BASE)) 
#define GPIO_B									((volatile gpio_t *)(GPIO_B_BASE))
#define GPIO_C									((volatile gpio_t *)(GPIO_C_BASE))
#define GPIO_D									((volatile gpio_t *)(GPIO_D_BASE))


#endif