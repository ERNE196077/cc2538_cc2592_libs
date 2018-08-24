#ifndef CC2538_WS2812B_H_
#define CC2538_WS2812B_H_

#include "ssim.h"


#define WS2812B_HIGH				(0x6)
#define WS2812B_LOW					(0x4)

typedef enum
{
	WS2812B_SINGLE,
	WS2812B_BUFFER
} ws2812b_xfertype_t;


void cc2538WS2812BInit(volatile ssi_t *SSI, uint32_t numled, void *data, ws2812b_xfertype_t xfer_type);
void cc2538WS2812BRender(volatile ssi_t *SSI);

#endif


