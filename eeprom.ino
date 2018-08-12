/*
 * EEPROM Write
 *
 * Stores values read from analog input 0 into the EEPROM.
 * These values will stay in the EEPROM when the board is
 * turned off and may be retrieved later by another sketch.
 */

#include <EEPROM.h>

const int addrSSIDCountLoc =0;
const int addrPassCountLoc =1;
const int addrSSID =2;
const int addrPassword =22;

char incomingByte;
boolean saveSSID=0, savePass=0;
int hashCount=0;

void eepromSetup(void)
{
  EEPROM.begin(512);
}
void dummyRead()
{
      for(int i=0;i<40;i++)
      Serial.print(readEEPROM(i));
}
void readSavedValues()
{
  int count,count1;

  SSIDCount = readEEPROM(addrSSIDCountLoc);
  PassCount = readEEPROM(addrPassCountLoc);
  Serial.print("\n\rSSIDCount: ");
  Serial.println(SSIDCount,10);  
  Serial.print("PassCount: ");
  Serial.println(PassCount,10);  
  
  if((SSIDCount > 1 && SSIDCount <= 20) && (PassCount <=20 && PassCount > 1 ))
  {
    
    for(count=addrSSID,count1=0; count < (addrSSID+SSIDCount); count++,count1++)
    {   
      SSID_USER[count1] = readEEPROM(count);
    }
    SSID_USER[count1] = '\0';

    for(count=addrPassword,count1=0; count < (addrPassword+PassCount);  count++,count1++)
    {    
      Password[count1] = readEEPROM(count);
    }
    Password[count1] = '\0';

    
    
    Serial.print("SSID: ");  
    Serial.println(SSID_USER); 
    
    
    Serial.print("password: ");         
    Serial.println(Password); 
    
  }
  else  
  {
    Serial.println("SSID and Password not found..\n");   

    getCredentials();
  }
}
void getCredentials()
{
  boolean credentialsFound=0;
  Serial.println("please enter *SSID_NAME*PASSWORD*"); 
    while(!credentialsFound)
    {
          if (Serial.available() > 0) 
          {
              incomingByte = Serial.read(); // read the incoming byte:   
              if(incomingByte == '*')
              {
                  hashCount++;
                  if(!saveSSID && (hashCount == 1))
                  {
                     SSIDCount=0;
                     saveSSID=1;
                  }
                  else if(hashCount == 2)
                  {
                      SSID_USER[SSIDCount] = '\0';
                      savePass =1;
                      PassCount=0;
                      saveSSID =0;
                  }
                  else if(hashCount == 3)
                  {
                    Password[PassCount] = '\0';
                    saveSSID = 0;
                    savePass = 0;
                    hashCount =0;
                    
                    
                    Serial.print("SSID count: ");
                    Serial.println(SSIDCount,10);
                    Serial.print("SSID: ");          
                    Serial.println(SSID_USER);
                    Serial.print("password count: ");  
                    Serial.println(PassCount,10);                        
                    Serial.print("password: ");          
                    Serial.println(Password);

                    EEPROM.begin(512);
                    writeEEPROM(SSIDCount,addrSSIDCountLoc);       
                    writeEEPROM(PassCount,addrPassCountLoc);  

                    EEPROM.begin(512);
                    for(int i=0; i< SSIDCount;i++)
                      writeEEPROM(SSID_USER[i],addrSSID+i);  

                    EEPROM.begin(512);
                    for(int i=0; i< PassCount;i++) 
                      writeEEPROM(Password[i],addrPassword+i);                    
                    
                      credentialsFound=1;  
                  }
              } 
              else
              {
                    if(saveSSID)
                    {
                        SSID_USER[SSIDCount++] = incomingByte;
                    }  
                    else if(savePass)
                    {
                        Password[PassCount++] = incomingByte;
                    }
              }      
                     
          }  
    }
  
  
}
void writeEEPROM(unsigned char data,int address)
{
  // write the value to the appropriate byte of the EEPROM.
  // these values will remain there when the board is
  // turned off.
  EEPROM.write(address, data); 
  // save all changes to the flash. 
  EEPROM.commit();
 
  delay(100);
}

char readEEPROM(int address)
{
  // write the value to the appropriate byte of the EEPROM.
  // these values will remain there when the board is
  // turned off.
  delay(100);
  return(EEPROM.read(address)) ;
  
}
