#ifndef _IRTXRX_H
#define _IRTXRX_H

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>

#define CAPTURE_BUFFER_SIZE 1024
#define MIN_UNKNOWN_SIZE 12

#if DECODE_AC
#define TIMEOUT 50U  // Some A/C units have gaps in their protocols of ~40ms.
                     // e.g. Kelvinator
                     // A value this large may swallow repeats of some protocols
#else  // DECODE_AC
#define TIMEOUT 15U  // Suits most messages, while not swallowing many repeats.
#endif  // DECODE_AC


typedef enum TVState_ 
{POWERON,UP1,TVOK,UP2,UP3,TVEND};



void initIR(void);
void enableIRRx(void);
void disableIRRx(void);
void sendIRCommand(uint16_t *rawD,int);
void sendMusicCommand(void);
boolean sendTvCommand(void);
void handleIRCommandSend(void);


#endif
