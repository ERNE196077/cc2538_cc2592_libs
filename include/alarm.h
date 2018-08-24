#ifndef CC2538_ALARM_H_
#define CC2538_ALARM_H_

#define NVIC_ST_RELOAD                          0xE000E014  // SysTick Reload Value Register
#define NVIC_ST_CTRL                            0xE000E010  // SysTick Control and Status
#define NVIC_ST_CTRL_COUNT                      0x00010000  // Count Flag
#define NVIC_ST_CTRL_CLK_SRC                    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN                      0x00000002  // Interrupt Enable
#define NVIC_ST_CTRL_ENABLE                     0x00000001  // Enable

void us_delay(uint32_t delayTicks);

#endif