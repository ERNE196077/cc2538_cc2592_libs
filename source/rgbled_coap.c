#include <openthread/coap.h>
#include <openthread/thread_ftd.h>
#include <openthread/ip6.h>
#include <string.h>
#include <openthread/cli.h>
#include "../include/rgbled.h"
#include "../include/rgbled_coap.h"
#include "../include/gptimer.h"

uint8_t renderflag = 0;

led_t user_preset;

const led_t default_presets[10] = 
{
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0}
};

led_t presets[10] = 
{
    {255,128,2},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0}
};
const coaprgbledcfg_t defaultcfg = 
{
    50,
    0,
    300,
    RENDER_PRESET,
    NULL,
    &(presets[0]),
    NULL
};

coaprgbledcfg_t currentcfg =
{
    50,
    0,
    300,
    RENDER_PRESET,
    NULL,
    &(presets[0]),
    NULL
};

otInstance *cInstance;

extern void GPTIMER3AIntHandler(void);

void GPTIMER3AIntHandler(void)
{
    cc2538GPTimerStopNVIC(GPTIMER3, A);
    GPTIMER3->icr |= GPTIMER_ICR_TATOCINT_TIMEOUTINT;
    renderflag = 1;

    cc2538GPTimerStartNVIC(GPTIMER3, A);
}


static otError getpayload(otMessage *aMessage, void *buffer, uint16_t len)
{
    
    uint16_t length = otMessageGetLength(aMessage) - otMessageGetOffset(aMessage);
    otCliUartOutputFormat("PL Len: %i\r\n", length);
    otCliUartOutputFormat("ExpLen: %i\r\n", len);
    /* If the requested length is different than the payload length this could not be the needed payload */
    if((len != length) || length == 0)
        return OT_ERROR_PARSE;



    otMessageRead(aMessage, otMessageGetOffset(aMessage), (uint8_t *)buffer, len);
    otCliUartOutputFormat("Payload:");
    int i; 
    for (i = 0 ; i < length ; i++)
        otCliUartOutputFormat("%x",((uint8_t *)(buffer))[i]);
    otCliUartOutputFormat("\r\n");
    return OT_ERROR_NONE;
}

static otError sendResponse(otCoapHeader *aHeader, const otMessageInfo *aMessageInfo, otCoapCode responseCode, void *buf, uint8_t bufsize)
{
    otCoapHeader responseHeader;
    otMessage *responseMessage;
    otError error = OT_ERROR_NONE;
    
    if ((buf != NULL) && (bufsize > 0))
        responseCode = OT_COAP_CODE_CONTENT;

    otCoapHeaderInit(&responseHeader, OT_COAP_TYPE_ACKNOWLEDGMENT, responseCode);
    otCoapHeaderSetMessageId(&responseHeader, otCoapHeaderGetMessageId(aHeader));
    otCoapHeaderSetToken(&responseHeader, otCoapHeaderGetToken(aHeader), otCoapHeaderGetTokenLength(aHeader));

    responseMessage = otCoapNewMessage(cInstance, &responseHeader);
    if(responseMessage == NULL) 
        error = OT_ERROR_NO_BUFS;
    else
    {
        if ((buf != NULL) && (bufsize > 0))
        {
            otCoapHeaderSetPayloadMarker(&responseHeader);
            otMessageAppend(responseMessage, (uint8_t *)buf, bufsize);
            otCliUartOutputFormat("RespPayload:");
            int i; 
            for (i = 0 ; i < bufsize ; i++)
                otCliUartOutputFormat("%x",((uint8_t *)(buf))[i]);
            otCliUartOutputFormat("\r\n");
            otCliUartOutputFormat("bufsize: %i\r\n",bufsize);
            

        }
        error = otCoapSendResponse(cInstance, responseMessage, aMessageInfo);
    }
    return error;
}


void cbkrcvSetup(void *aContext, otCoapHeader *aHeader, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
    otIp6Address ip = aMessageInfo->mSockAddr;
    otCoapCode responseCode = OT_COAP_CODE_RESPONSE_MIN;     /* Response 200 */
    coaprgbledsetup_t setup;
    uint32_t rgbdevcfg = RGBLEDCFG_XFERSSI_SSI1;
    uint32_t frequency = 2100000;
    
    switch(otCoapHeaderGetCode(aHeader))
    {
        case OT_COAP_CODE_POST:
            otCliUartOutputFormat(
                "Message from: %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x\n\r",
                ip.mFields.m16[0],ip.mFields.m16[1],ip.mFields.m16[2],ip.mFields.m16[3],ip.mFields.m16[4],ip.mFields.m16[5],ip.mFields.m16[6],ip.mFields.m16[7]);
            otCliUartOutputFormat(
                "Message from: %x\n\r",
                aContext);
            otCliUartOutputFormat(
                "Message from: %x\r\n",
                aMessage);

            /* Verify and get payload for configuration */
            if(getpayload(aMessage, (uint8_t *)(&setup), sizeof(setup)))
                responseCode = OT_COAP_CODE_BAD_REQUEST;
            else
            {   
                /* If device is configured deconfigure it */
                if(currentcfg.rgbdev)
                {
                    cc2538GPTimer32BitStop(GPTIMER3);
                    rgbledTerminate(currentcfg.rgbdev);
                }

                /* Setup rgbled and save configuration */
                switch(setup.ledtype)
                {
                    case LEDTYPE_WS2812B:
                        rgbdevcfg &= RGBLEDCFG_LEDTYPE_WS2812B;
                        frequency = 2100000;
                        break;

                    case LEDTYPE_APA102:
                        rgbdevcfg &= RGBLEDCFG_LEDTYPE_APA102;
                        frequency = 800000;
                        break;
                }

                /* Usethe current render to display */
                switch(currentcfg.currentrender)
                {
                    default:
                    case RENDER_PRESET:
                    case RENDER_USER:
                        rgbdevcfg &= RGBLEDCFG_XFERTYPE_SINGLE;
                        break;
                    case RENDER_SCENE:
                        rgbdevcfg &= RGBLEDCFG_XFERTYPE_BUFFER;
                        break;
                }  

                /* Initialize rgbled device */
                currentcfg.refreshrate = setup.refreshrate;
                currentcfg.rgbdev = rgbledInit(rgbdevcfg, (uint16_t)setup.lednum, frequency);   
                cc2538GPTimer32BitInit(GPTIMER3, currentcfg.refreshrate);
                cc2538GPTimer32BitStart(GPTIMER3);
            }
            break;

        default:
            responseCode = OT_COAP_CODE_BAD_OPTION;
            break;
    }

    sendResponse(aHeader, aMessageInfo, responseCode, NULL, 0);
}


void cbkrcvOff (void *aContext, otCoapHeader *aHeader, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
    otIp6Address ip = aMessageInfo->mSockAddr;
    otCoapCode responseCode = OT_COAP_CODE_RESPONSE_MIN;     /* Response 200 */
    
    switch(otCoapHeaderGetCode(aHeader))
    {
        case OT_COAP_CODE_POST:
            otCliUartOutputFormat(
                "Message from: %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x\n\r",
                ip.mFields.m16[0],ip.mFields.m16[1],ip.mFields.m16[2],ip.mFields.m16[3],ip.mFields.m16[4],ip.mFields.m16[5],ip.mFields.m16[6],ip.mFields.m16[7]);
            otCliUartOutputFormat(
                "Message from: %x\n\r",
                aContext);
            otCliUartOutputFormat(
                "Message from: %x\r\n",
                aMessage);

            /* Stop Timer and turn off LEDs */
            cc2538GPTimer32BitStop(GPTIMER3);
            if(currentcfg.rgbdev)
            {
                rgbledRenderColor(0, 0, 0, currentcfg.rgbdev);
            }

            break;

        default:
            responseCode = OT_COAP_CODE_BAD_OPTION;
            break;
    }

    sendResponse(aHeader, aMessageInfo, responseCode, NULL, 0);
}

void cbkrcvOn (void *aContext, otCoapHeader *aHeader, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
    otIp6Address ip = aMessageInfo->mSockAddr;
    otCoapCode responseCode = OT_COAP_CODE_RESPONSE_MIN;     /* Response 200 */
    
    switch(otCoapHeaderGetCode(aHeader))
    {
        case OT_COAP_CODE_POST:
            otCliUartOutputFormat(
                "Message from: %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x\n\r",
                ip.mFields.m16[0],ip.mFields.m16[1],ip.mFields.m16[2],ip.mFields.m16[3],ip.mFields.m16[4],ip.mFields.m16[5],ip.mFields.m16[6],ip.mFields.m16[7]);
            otCliUartOutputFormat(
                "Message from: %x\n\r",
                aContext);
            otCliUartOutputFormat(
                "Message from: %x\r\n",
                aMessage);

            if(currentcfg.rgbdev)
            {
                /* Usethe current render to display */
                switch(currentcfg.currentrender)
                {
                    default:
                    case RENDER_PRESET:
                    case RENDER_USER:
                        rgbledRenderColor(currentcfg.currentpreset->r, 
                                        currentcfg.currentpreset->g, 
                                        currentcfg.currentpreset->b, 
                                        currentcfg.rgbdev);
                        break;
                    case RENDER_SCENE:
                        /* CODE FOR RENDER SCENE */
                        break;
                }
                cc2538GPTimer32BitStart(GPTIMER3);
            }
            else
            {
                responseCode = OT_COAP_CODE_PRECONDITION_FAILED;  
            }
            break;

        default:
            responseCode = OT_COAP_CODE_BAD_OPTION;
            break;
    }

    sendResponse(aHeader, aMessageInfo, responseCode, NULL, 0);
}

void cbkrcvScene (void *aContext, otCoapHeader *aHeader, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
    otIp6Address ip = aMessageInfo->mSockAddr;
    otCoapCode responseCode = OT_COAP_CODE_RESPONSE_MIN;     /* Response 200 */
    
    switch(otCoapHeaderGetCode(aHeader))
    {
        case OT_COAP_CODE_POST:
            otCliUartOutputFormat(
                "Message from: %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x\n\r",
                ip.mFields.m16[0],ip.mFields.m16[1],ip.mFields.m16[2],ip.mFields.m16[3],ip.mFields.m16[4],ip.mFields.m16[5],ip.mFields.m16[6],ip.mFields.m16[7]);
            otCliUartOutputFormat(
                "Message from: %x\n\r",
                aContext);
            otCliUartOutputFormat(
                "Message from: %x\r\n",
                aMessage);
            
            if(currentcfg.rgbdev)
            {
                currentcfg.currentrender = RENDER_SCENE;
                /* CODE TO RENDER SCENE */
            }
            else
            {
                responseCode = OT_COAP_CODE_PRECONDITION_FAILED;  
            }

            break;

        default:
            responseCode = OT_COAP_CODE_BAD_OPTION;
            break;
    }

    sendResponse(aHeader, aMessageInfo, responseCode, NULL, 0);
}


void cbkrcvUser (void *aContext, otCoapHeader *aHeader, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
    otIp6Address ip = aMessageInfo->mSockAddr;
    otCoapCode responseCode = OT_COAP_CODE_RESPONSE_MIN;     /* Response 200 */
    led_t *userled = NULL;
    uint8_t userledsize = 0;

    otCliUartOutputFormat(
        "Message from: %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x\n\r",
        ip.mFields.m16[0],ip.mFields.m16[1],ip.mFields.m16[2],ip.mFields.m16[3],ip.mFields.m16[4],ip.mFields.m16[5],ip.mFields.m16[6],ip.mFields.m16[7]);
    otCliUartOutputFormat(
        "Message from: %x\n\r",
        aContext);
    otCliUartOutputFormat(
        "Message from: %x\r\n",
        aMessage);

    switch(otCoapHeaderGetCode(aHeader))
    {
        case OT_COAP_CODE_POST:
            if(currentcfg.rgbdev)
            {
                currentcfg.currentrender = RENDER_USER;
                currentcfg.currentpreset = &user_preset;
            }
            else
            {
                responseCode = OT_COAP_CODE_PRECONDITION_FAILED;  
            }
            break;

        case OT_COAP_CODE_GET:
            userled = &user_preset;
            userledsize = sizeof(led_t);
            break;

        default:
            responseCode = OT_COAP_CODE_BAD_OPTION;
            break;
    }

    sendResponse(aHeader, aMessageInfo, responseCode, userled, userledsize);
}

void cbkrcvPreset (void *aContext, otCoapHeader *aHeader, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
    otIp6Address ip = aMessageInfo->mSockAddr;
    otCoapCode responseCode = OT_COAP_CODE_RESPONSE_MIN;     /* Response 200 */
    uint8_t choosepreset;
    
    typedef struct 
    {
        uint8_t choosepreset;
        led_t colorpreset;
    } presetdata_t;
    presetdata_t presetdata;
    presetdata_t *buf = NULL;
    uint8_t bufsize = 0;

    uint32_t _buf = 0xFFFFFFFF;

    otCliUartOutputFormat(
        "Message from: %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x\n\r",
        ip.mFields.m16[0],ip.mFields.m16[1],ip.mFields.m16[2],ip.mFields.m16[3],ip.mFields.m16[4],ip.mFields.m16[5],ip.mFields.m16[6],ip.mFields.m16[7]);
    otCliUartOutputFormat(
        "Message from: %x\n\r",
        aContext);
    otCliUartOutputFormat(
        "Message from: %x\r\n",
        aMessage);

    switch(otCoapHeaderGetCode(aHeader))
    {
        case OT_COAP_CODE_POST:
            
            if(getpayload(aMessage, (uint8_t *)(&choosepreset), sizeof(choosepreset)))
                responseCode = OT_COAP_CODE_BAD_REQUEST;
            else
            {   
                if(currentcfg.rgbdev)
                {
                    currentcfg.currentrender = RENDER_PRESET;
                    currentcfg.presetnumber = choosepreset;
                    currentcfg.currentpreset = &(presets[choosepreset]);
                }
                else
                {
                    responseCode = OT_COAP_CODE_PRECONDITION_FAILED;  
                }
            }
            break;

        case OT_COAP_CODE_GET:
            presetdata.choosepreset = currentcfg.presetnumber;
            presetdata.colorpreset = presets[currentcfg.presetnumber];
            buf = &presetdata;
            bufsize = sizeof(presetdata_t);
            break;

        default:
            responseCode = OT_COAP_CODE_BAD_OPTION;
            break;
    }
    buf += 4;
    bufsize = 6;
    sendResponse(aHeader, aMessageInfo, responseCode, &_buf, bufsize);
}

void cbkrcvSetFadetime (void *aContext, otCoapHeader *aHeader, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
    otIp6Address ip = aMessageInfo->mSockAddr;
    otCoapCode responseCode = OT_COAP_CODE_RESPONSE_MIN;     /* Response 200 */
    uint8_t fadetime;

    switch(otCoapHeaderGetCode(aHeader))
    {
        case OT_COAP_CODE_POST:
            otCliUartOutputFormat(
                "Message from: %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x\n\r",
                ip.mFields.m16[0],ip.mFields.m16[1],ip.mFields.m16[2],ip.mFields.m16[3],ip.mFields.m16[4],ip.mFields.m16[5],ip.mFields.m16[6],ip.mFields.m16[7]);
            otCliUartOutputFormat(
                "Message from: %x\n\r",
                aContext);
            otCliUartOutputFormat(
                "Message from: %x\r\n",
                aMessage);
            
            if(getpayload(aMessage, &fadetime, sizeof(fadetime)))
                responseCode = OT_COAP_CODE_BAD_REQUEST;
            else
            {   
                currentcfg.fadetime = fadetime;
            }
            break;

        default:
            responseCode = OT_COAP_CODE_BAD_OPTION;
            break;
    }

    sendResponse(aHeader, aMessageInfo, responseCode, NULL, 0);
}


void cbkrcvStop (void *aContext, otCoapHeader *aHeader, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
    otIp6Address ip = aMessageInfo->mSockAddr;
    otCoapCode responseCode = OT_COAP_CODE_RESPONSE_MIN;     /* Response 200 */
    
    switch(otCoapHeaderGetCode(aHeader))
    {
        case OT_COAP_CODE_POST:
            otCliUartOutputFormat(
                "Message from: %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x\n\r",
                ip.mFields.m16[0],ip.mFields.m16[1],ip.mFields.m16[2],ip.mFields.m16[3],ip.mFields.m16[4],ip.mFields.m16[5],ip.mFields.m16[6],ip.mFields.m16[7]);
            otCliUartOutputFormat(
                "Message from: %x\n\r",
                aContext);
            otCliUartOutputFormat(
                "Message from: %x\r\n",
                aMessage);
            
            /* If device is configured deconfigure it */
            if(currentcfg.rgbdev)
            {
                cc2538GPTimer32BitStop(GPTIMER3);
                rgbledRenderColor(0, 0, 0, currentcfg.rgbdev);
                rgbledTerminate(currentcfg.rgbdev);
            }

            break;

        default:
            responseCode = OT_COAP_CODE_BAD_OPTION;
            break;
    }

    sendResponse(aHeader, aMessageInfo, responseCode, NULL, 0);
}

void cbkrcvSetPreset (void *aContext, otCoapHeader *aHeader, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
    otIp6Address ip = aMessageInfo->mSockAddr;
    otCoapCode responseCode = OT_COAP_CODE_RESPONSE_MIN;     /* Response 200 */
    uint32_t buffer;
    uint8_t *dataptr = (uint8_t *)(&buffer);

    switch(otCoapHeaderGetCode(aHeader))
    {
        case OT_COAP_CODE_POST:
            otCliUartOutputFormat(
                "Message from: %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x\n\r",
                ip.mFields.m16[0],ip.mFields.m16[1],ip.mFields.m16[2],ip.mFields.m16[3],ip.mFields.m16[4],ip.mFields.m16[5],ip.mFields.m16[6],ip.mFields.m16[7]);
            otCliUartOutputFormat(
                "Message from: %x\n\r",
                aContext);
            otCliUartOutputFormat(
                "Message from: %x\r\n",
                aMessage);
            
            if(getpayload(aMessage, &buffer, sizeof(buffer)))
                responseCode = OT_COAP_CODE_BAD_REQUEST;
            else
            {   
                presets[dataptr[3]].r = dataptr[0];
                presets[dataptr[3]].g = dataptr[1];
                presets[dataptr[3]].b = dataptr[2];
            }
            break;

        default:
            responseCode = OT_COAP_CODE_BAD_OPTION;
            break;
    }

    sendResponse(aHeader, aMessageInfo, responseCode, NULL, 0);
}


otCoapResource ledResSetup = {RGBLED_URI_SETUP, &cbkrcvSetup, NULL, NULL};
otCoapResource ledResStop = {RGBLED_URI_STOP, &cbkrcvStop, NULL, NULL};
otCoapResource ledResOff = {RGBLED_URI_OFF, &cbkrcvOff, NULL, NULL};
otCoapResource ledResOn = {RGBLED_URI_ON, &cbkrcvOn, NULL, NULL};
otCoapResource ledResScene = {RGBLED_URI_SCENE, &cbkrcvScene, NULL, NULL};
otCoapResource ledResUser = {RGBLED_URI_USER, &cbkrcvUser, NULL, NULL};
otCoapResource ledResPreset = {RGBLED_URI_PRESET, &cbkrcvPreset, NULL, NULL};
otCoapResource ledResSetFadetime = {RGBLED_URI_SET_FADETIME, &cbkrcvSetFadetime, NULL, NULL};
otCoapResource ledResSetPreset = {RGBLED_URI_SET_PRESET, &cbkrcvSetPreset, NULL, NULL};


void rgbledCoapInit(otInstance *sInstance)
{	
	int result = 0;
	cInstance = sInstance;

	result = otCoapStart(sInstance, OT_DEFAULT_COAP_PORT);
	if(result == OT_ERROR_NONE)
	{
        result = otCoapAddResource(sInstance, &ledResSetup);
        result = otCoapAddResource(sInstance, &ledResStop);
		result = otCoapAddResource(sInstance, &ledResOff);
        result = otCoapAddResource(sInstance, &ledResOn);
        result = otCoapAddResource(sInstance, &ledResScene);
        result = otCoapAddResource(sInstance, &ledResUser);
        result = otCoapAddResource(sInstance, &ledResPreset);
        result = otCoapAddResource(sInstance, &ledResSetFadetime);
        result = otCoapAddResource(sInstance, &ledResSetPreset);
		if(result == OT_ERROR_NONE)
		{
			otCliUartOutputFormat("Resources for rgbled added\n\r");

		}
	}
}


void rgbledCoapProcess(void)
{   

}

/*
void rgbledCoapSend(otInstance *sInstance, const char *ip)
{	
	otIp6Address address;
	otCoapHeader sendHeader;
	otMessage *sendMessage;
	otCoapCode coapCode = OT_COAP_CODE_POST;
	otCoapType coapType = OT_COAP_TYPE_NON_CONFIRMABLE;
	otMessageInfo sendMsgInfo;

	otIp6AddressFromString(ip, &address);
    otCoapHeaderInit(&sendHeader, OT_COAP_TYPE_NON_CONFIRMABLE, OT_COAP_CODE_POST);
    otCoapHeaderGenerateToken(&sendHeader, OT_COAP_MAX_TOKEN_LENGTH);
    otCoapHeaderAppendUriPathOptions(&sendHeader, RGBLED_URI_USER);
    sendMessage = otCoapNewMessage(sInstance, &sendHeader);
    
    otCliUartOutputFormat(
                "Message to: %04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x\n\r",
                address.mFields.m16[0],address.mFields.m16[1],address.mFields.m16[2],address.mFields.m16[3],address.mFields.m16[4],address.mFields.m16[5],address.mFields.m16[6],address.mFields.m16[7]);    

	memset(&sendMsgInfo, 0, sizeof(sendMsgInfo));
    sendMsgInfo.mPeerAddr    = address;
    sendMsgInfo.mPeerPort    = OT_DEFAULT_COAP_PORT;
    sendMsgInfo.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;

    if ((coapType == OT_COAP_TYPE_CONFIRMABLE) || (coapCode == OT_COAP_CODE_GET))
    {
        otCoapSendRequest(sInstance, sendMessage, &sendMsgInfo, &ledHandlerResponse, NULL);
    }
    else
    {
        otCoapSendRequest(sInstance, sendMessage, &sendMsgInfo, NULL, NULL);
    }
    otCliUartOutputFormat("Message Sent!\n\r");
}*/