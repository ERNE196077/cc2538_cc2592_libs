#ifndef CC2538_RGBLED_H_
#define CC2538_RGBLED_H_

#include "ssim.h"
#include "udma.h"
#include <stdint.h>


#define WS2812B_HIGH				0x6
#define WS2812B_LOW					0x4

typedef enum{
	XFERTYPE_SINGLE,
	XFERTYPE_BUFFER
}xfertype_t;

typedef enum{
	LEDTYPE_WS2812B,
	LEDTYPE_APA102
}ledtype_t;

typedef struct
{
	uint8_t g;
	uint8_t r;
	uint8_t b;
}led_t;

typedef struct
{
	unsigned int g0 : 1;
	unsigned int g1 : 1;
	unsigned int g2 : 1;
	unsigned int g3 : 1;
	unsigned int g4 : 1;
	unsigned int g5 : 1;
	unsigned int g6 : 1;
	unsigned int g7 : 1;
	unsigned int r0 : 1;
	unsigned int r1 : 1;
	unsigned int r2 : 1;
	unsigned int r3 : 1;
	unsigned int r4 : 1;
	unsigned int r5 : 1;
	unsigned int r6 : 1;
	unsigned int r7 : 1;
	unsigned int b0 : 1;
	unsigned int b1 : 1;
	unsigned int b2 : 1;
	unsigned int b3 : 1;
	unsigned int b4 : 1;
	unsigned int b5 : 1;
	unsigned int b6 : 1;
	unsigned int b7 : 1;
}leddata_t;

typedef struct
{
	uint8_t aux;
	uint8_t b;
	uint8_t g;
	uint8_t r;
}apa102pixel_t;

typedef struct
{
	unsigned int g2 : 1;
	unsigned int rsv1a : 1;
	unsigned int rsv1b : 1;
	unsigned int g1 : 1;
	unsigned int rsv2a : 1;
	unsigned int rsv2b : 1;
	unsigned int g0 : 1;
	unsigned int rsv3a : 1;
	unsigned int rsv3b : 1;
	unsigned int rsv3c : 1;
	unsigned int g4 : 1;
	unsigned int rsv4a : 1;
	unsigned int rsv4b : 1;
	unsigned int g3 : 1;
	unsigned int rsv5a : 1;
	unsigned int rsv5b : 1;
	unsigned int rsv5c : 1;
	unsigned int g7 : 1;
	unsigned int rsv6a : 1;
	unsigned int rsv6b : 1;
	unsigned int g6 : 1;
	unsigned int rsv7a : 1;
	unsigned int rsv7b : 1;
	unsigned int g5 : 1;
	unsigned int r2 : 1;
	unsigned int rsv8a : 1;
	unsigned int rsv8b : 1;
	unsigned int r1 : 1;
	unsigned int rsv9a : 1;
	unsigned int rsv9b : 1;
	unsigned int r0 : 1;
	unsigned int rsv10a : 1;
	unsigned int rsv10b : 1;
	unsigned int rsv10c : 1;
	unsigned int r4 : 1;
	unsigned int rsv11a : 1;
	unsigned int rsv11b : 1;
	unsigned int r3 : 1;
	unsigned int rsv12a : 1;
	unsigned int rsv12b : 1;
	unsigned int rsv12c : 1;
	unsigned int r7 : 1;
	unsigned int rsv13a : 1;
	unsigned int rsv13b : 1;
	unsigned int r6 : 1;
	unsigned int rsv14a : 1;
	unsigned int rsv14b : 1;
	unsigned int r5 : 1;
	unsigned int b2 : 1;
	unsigned int rsv15a : 1;
	unsigned int rsv15b : 1;
	unsigned int b1 : 1;
	unsigned int rsv16a : 1;
	unsigned int rsv16b : 1;
	unsigned int b0 : 1;
	unsigned int rsv17a : 1;
	unsigned int rsv17b : 1;
	unsigned int rsv17c : 1;
	unsigned int b4 : 1;
	unsigned int rsv18a : 1;
	unsigned int rsv18b : 1;
	unsigned int b3 : 1;
	unsigned int rsv19a : 1;
	unsigned int rsv19b : 1;
	unsigned int rsv19c : 1;
	unsigned int b7 : 1;
	unsigned int rsv20a : 1;
	unsigned int rsv20b : 1;
	unsigned int b6 : 1;
	unsigned int rsv21a : 1;
	unsigned int rsv21b : 1;
	unsigned int b5 : 1;
}ws2812b_t;


typedef struct
{
	uint8_t r[3];
	uint8_t g[3];
	uint8_t b[3];
}ws2812bpixel_t;

typedef struct
{
	unsigned int lednum 		: 14;
	unsigned int ledtype 		: 2;
	#define RGBLEDCFG_LEDTYPE_WS2812B	0x0
	#define RGBLEDCFG_LEDTYPE_APA102	(0x1 << 14)
	
	unsigned int leddatasize 	: 8;
	#define RGBLEDCFG_LEDDATASIZE(SIZE) ((SIZE) << 16)
	
	unsigned int xferssi 		: 1;
	#define RGBLEDCFG_XFERSSI_SSI0 		0x0
	#define RGBLEDCFG_XFERSSI_SSI1 		(0x1 << 24)
	
	unsigned int xfertype 		: 1;
	#define	RGBLEDCFG_XFERTYPE_SINGLE	0x0
	#define	RGBLEDCFG_XFERTYPE_BUFFER	(0x1 << 25)
	
	unsigned int startframe 	: 1;
	#define	RGBLEDCFG_STARTFRAME		(0x1 << 26)
	
	unsigned int startframesize : 2;
	#define	RGBLEDCFG_STARTFRAMESIZE_1 	(0x0 << 27)
	#define	RGBLEDCFG_STARTFRAMESIZE_2 	(0x1 << 27)
	#define	RGBLEDCFG_STARTFRAMESIZE_3 	(0x2 << 27)
	#define	RGBLEDCFG_STARTFRAMESIZE_4 	(0x3 << 27)
	
	unsigned int endframe 		: 1;
	#define	RGBLEDCFG_ENDFRAME			(0x1 << 29)
	
	unsigned int endframesize 	: 2;
	#define	RGBLEDCFG_ENDFRAMESIZE_1 	(0x0 << 30)
	#define	RGBLEDCFG_ENDFRAMESIZE_2 	(0x1 << 30)
	#define	RGBLEDCFG_ENDFRAMESIZE_3 	(0x2 << 30)
	#define	RGBLEDCFG_ENDFRAMESIZE_4 	(0x3 << 30)
	
	uint32_t sframe;
	uint32_t eframe;
} rgbledcfg_t;


typedef struct
{
	unsigned int xfercomplete 	: 1;
	unsigned int xferbs			: 10;
	unsigned int dma_txch 		: 7;
	unsigned int dma_rxch 		: 7;
	unsigned int ssiint			: 7;
	uint16_t xfer_c;
	uint16_t xferqty;
	uint32_t xfersize;
	uint32_t dma_txchcfg;
	uint32_t dma_rxchcfg;
	uint32_t *dataptr;					/*	POINTER TO DATA	*/
	uint8_t counter;
	volatile udmach_t *dma_txpri;
	volatile udmach_t *dma_txalt;
	volatile udmach_t *dma_rxpri;
	volatile udmach_t *dma_rxalt;
	volatile ssi_t *ssi;
} rgbledctrl_t;


typedef struct
{
	rgbledcfg_t config;	
	rgbledctrl_t control;	
	void *leds;
}rgbdev_t;


rgbdev_t *rgbledInit(uint32_t config, uint16_t lednum, uint32_t frequency);
void rgbledTerminate(rgbdev_t *rgbdev);
void rgbledRender(rgbdev_t *rgbdev);
void rgbledRenderColor(uint8_t r, uint8_t g, uint8_t b, rgbdev_t *rgbdev);

#endif
