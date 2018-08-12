#ifndef _MY_EEPROM_H_
#define _MY_EEPROM_H_

typedef union EEPaddress
{
  learntCmd commandIR;
  uint8_t addrLoc[sizeof(learntCmd)];
}_EEPaddress;
typedef union 
{
   
  struct 
  {
       char IDHigh;
       char IDLow;
  }un;  
  int channelID;
}stChanID;



#endif
