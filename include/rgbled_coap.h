#ifndef CC2538_RGBLEDCOAP_H_
#define CC2538_RGBLEDCOAP_H_

#include <openthread/coap.h>
#include <openthread/thread_ftd.h>
#include "rgbled.h"


#define RGBLED_URI_SETUP                    "res/rgbled/setup"
#define RGBLED_URI_STOP		                "res/rgbled/stop"
#define RGBLED_URI_OFF                      "res/rgbled/off"
#define RGBLED_URI_ON                       "res/rgbled/on"
#define RGBLED_URI_SCENE                    "res/rgbled/mode/scene"
#define RGBLED_URI_USER                     "res/rgbled/mode/user"
#define RGBLED_URI_PRESET                   "res/rgbled/mode/preset"
#define RGBLED_URI_SET_FADETIME             "res/rgbled/setting/fadetime"
#define RGBLED_URI_SET_PRESET               "res/rgbled/setting/preset"


typedef void (* rgbledanim)(led_t *leds);

typedef enum
{
	RENDER_SCENE,
	RENDER_PRESET,
	RENDER_USER	
}currentrender_t;

typedef struct 
{
	uint8_t fadetime;
	uint8_t presetnumber;
	uint16_t refreshrate;
	currentrender_t currentrender;
	rgbledanim *currentanim;
	led_t *currentpreset;
	rgbdev_t *rgbdev;
} coaprgbledcfg_t;

typedef struct 
{
	ledtype_t ledtype;
	uint32_t lednum;
	uint8_t refreshrate;
}coaprgbledsetup_t;



void cbkrcvSetup(void *aContext, otCoapHeader *aHeader, otMessage *aMessage, const otMessageInfo *aMessageInfo);
void cbkrcvOff (void *aContext, otCoapHeader *aHeader, otMessage *aMessage, const otMessageInfo *aMessageInfo);
void cbkrcvScene (void *aContext, otCoapHeader *aHeader, otMessage *aMessage, const otMessageInfo *aMessageInfo);
void cbkrcvUser (void *aContext, otCoapHeader *aHeader, otMessage *aMessage, const otMessageInfo *aMessageInfo);
void cbkrcvPreset (void *aContext, otCoapHeader *aHeader, otMessage *aMessage, const otMessageInfo *aMessageInfo);
void cbkrcvSetFadetime (void *aContext, otCoapHeader *aHeader, otMessage *aMessage, const otMessageInfo *aMessageInfo);
void cbkrcvStop (void *aContext, otCoapHeader *aHeader, otMessage *aMessage, const otMessageInfo *aMessageInfo);
void cbkrcvSetPreset (void *aContext, otCoapHeader *aHeader, otMessage *aMessage, const otMessageInfo *aMessageInfo);
void rgbledCoapInit(otInstance *sInstance);
void rgbledCoapProcess(void);
//void rgbledCoapSend(otInstance *sInstance, const char *ip);


#endif