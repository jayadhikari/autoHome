/*
 * EEPROM Write
 *
 * Stores values read from analog input 0 into the EEPROM.
 * These values will stay in the EEPROM when the board is
 * turned off and may be retrieved later by another sketch.
 */

#include "myEeprom.h"
#include <EEPROM.h>

#define EEPROM_MAX_SIZE   (512)
#define IR_COMMAND_SIZE   (100)
#define LOC_COMMAND_0     (0)
#define LOC_COMMAND_1     (LOC_COMMAND_0 + IR_COMMAND_SIZE)

void eepromSetup(void)
{
  EEPROM.begin(512);
}

void readSavedValues(void)
{
  /*mainLight = readEEPROM(memory.addrMainLight);
  stCh.un.IDHigh = readEEPROM(memory.addrChannelIDH);
  stCh.un.IDLow = readEEPROM(memory.addrChannelIDL);
  channelNum = readEEPROM(memory.addrChannelNum);
  */
  //memset(&controlCmd,0x00,sizeof(controlCmd));
 
  for(uint16_t loc=0;loc<120;loc++)
  {
      controlCmd[0].addrLoc[loc] = readEEPROM(loc);
     // yield();  // Feed the WDT (again)
  }
  //yield();  // Feed the WDT (again)
  Serial.println("\r\nEEPROM Values:");
  for(int loc=0;loc<EEPROM_MAX_SIZE;loc++)
  {
      Serial.print(controlCmd[0].addrLoc[loc]);
      
      Serial.print(",");
      if(loc%100 == 0)
      Serial.print("\r\n");
  }
  yield();  // Feed the WDT (again)
  /*Serial.println("Read EEPROM values: "); 
  Serial.println(mainLight);
  Serial.println(stCh.channelID);
  Serial.println(channelNum);
    */  
  
}
void writeEEPROM(uint8_t data,uint16_t address)
{
  // write the value to the appropriate byte of the EEPROM.
  // these values will remain there when the board is
  // turned off.
  EEPROM.write(address, data); 
  // save all changes to the flash. 
  EEPROM.commit();
 
  delay(100);
}

uint8_t readEEPROM(uint16_t address)
{
  // write the value to the appropriate byte of the EEPROM.
  // these values will remain there when the board is
  // turned off.
  return(EEPROM.read(address)) ;
}
void writeWordToEEPROM(uint16_t cmdWord,uint16_t address)
{
     writeEEPROM((cmdWord >> 8),address);  
     writeEEPROM((cmdWord & 0xFF),address+1);  
}     
void saveCommand(_EEPaddress *ptrCmd)
{
    for(int count=0;count<ptrCmd->commandIR.count;count++)
    {
        writeWordToEEPROM(ptrCmd->commandIR.duration[count],(LOC_COMMAND_0+(count*2)));//for each adrees sent the api write to two locations , so multiple of 2
    }
  
}    
