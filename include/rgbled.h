#ifndef CC2538_RGBLED_H_
#define CC2538_RGBLED_H_

#include "ssim.h"
#include "udma.h"
#include <stdint.h>


#define WS2812B_HIGH				(0x6)
#define WS2812B_LOW					(0x4)

typedef enum
{
	XFER_SINGLE,
	XFER_BUFFER
} xfertype_t;

typedef struct
{
	uint16_t numled;					/*	NUMBER OF LEDs	*/
	uint8_t leddatasize;
	uint32_t *dataptr;					/*	POINTER TO DATA	*/
	xfertype_t xfertype;				/*	SINGLE COLOR OR BUFFER 	*/
	uint8_t xfercomplete;				/*	COMPLETION FLAG	*/
	uint16_t xfer_c;					/*	TRANSFER COUNTER	*/
	uint16_t xferqty;					/* 	QUANTITY OF TRANSFERS	*/
	uint32_t xfersize;					/* 	TOTAL TRANSFER SIZE (BYTES)	*/
	uint32_t xferaddr;					/*	ADDRESS FOR CURRENT DATA 	*/
	uint16_t xferbs;					/*	SIZE OF EACH TRANSFER 	*/
	uint8_t dma_txch;
	uint8_t dma_rxch;
	uint32_t dma_txchcfg;
	uint32_t dma_rxchcfg;
	volatile udmach_t *dma_txpri;
	volatile udmach_t *dma_txalt;
	volatile udmach_t *dma_rxpri;
	volatile udmach_t *dma_rxalt;
	volatile ssi_t *ssi;
	uint8_t ssiint;
}rgbdev_t;


rgbdev_t *cc2538RGBInit(volatile ssi_t *ssi, uint32_t numled, uint8_t leddatasize, xfertype_t xfertype, void *data, uint32_t frequency);
void cc2538RGBRender(rgbdev_t *rgbdev);
void cc2538WS2812Bgetrgb(uint8_t R, uint8_t G, uint8_t B, void *data);
void cc2538apa102(uint8_t R, uint8_t G, uint8_t B, void *data);

#endif


