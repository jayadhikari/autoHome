#ifndef _MYEEPROM_H_
#define _MYEEPROM_H_


/*
typedef struct 
{
    //uint16_t addrMainLight ;
    //DataWord Channel;
   // uint16_t addrChannelIDL ;
    //uint16_t addrChannelNum ;
    uint16_t IRCommand[2][100];
  
}eepromData_;
*/
typedef union EEPaddress
{
  learntCmd commandIR;
  uint8_t addrLoc[sizeof(learntCmd)];
}_EEPaddress;

typedef union 
{
   
  struct 
  {
       uint8_t MSB;
       uint8_t LSB;
  }un;  
  uint16_t Word;
}DataWord;

uint8_t readEEPROM(uint16_t address);
void writeEEPROM(uint8_t data,uint16_t address);
void eepromSetup(void);
void readSavedValues(void);
void saveCommand(_EEPaddress*);
void writeWordToEEPROM(uint16_t cmdWord,uint16_t address);

#endif
