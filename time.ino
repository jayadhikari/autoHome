#include "time.h"


static long unsigned int baseTime =0;

_savedTime savedTime;
 
void initTime(void)
{
  savedTime = {0,0,0,0,0};
  baseTime = millis();
}
void updateTime(void)
{ 
  unsigned long int currentTime=0;

  currentTime = millis();
    
  if(( currentTime - baseTime) >= pause)
  {    
    savedTime.secondsCount += (currentTime - baseTime)/1000;
    #ifdef DEBUG
    Serial.print("secs=");
    Serial.println(savedTime.secondsCount);
    #endif
    baseTime = currentTime;     
  }
  if(savedTime.secondsCount == 60)
  {
    savedTime.secondsCount=0;
    savedTime.minuteCount++;  
    #ifdef DEBUG
    Serial.print("min=");
    Serial.println(savedTime.minuteCount);
    #endif
  }
  if(savedTime.minuteCount == 60)
  {
    savedTime.minuteCount=0;
    savedTime.hourCount++;  
  }
  if(savedTime.hourCount == 24)
  {
    savedTime.hourCount=0;
    savedTime.dayCount++;  
  }
  if(savedTime.dayCount == 365)
  {
     savedTime.yearCount++;  
  }
}

_savedTime getCurrentTime(void)
{
    return  savedTime;
  
}

