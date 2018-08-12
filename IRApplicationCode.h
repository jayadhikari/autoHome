#ifndef _IRAPPLICATIONCODE_H_
#define _IRAPPLICATIONCODE_H_



typedef struct
{
  //String deviceName;
  //String command;
  //uint16_t repeat;
  uint16_t count;
  uint16_t duration[200];
}learntCmd;

void initCmdStr(void);
boolean learn(String dev,String cmdName);


#endif
