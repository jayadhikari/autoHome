
#include "button.h"

const int ButtonInterruptPin  =         D6 ;
boolean buttonPressed=0;
extern boolean buttonFlag=0;
static volatile long waitTime=0;

void handleInterrupt() 
{     
    if(digitalRead(ButtonInterruptPin) == LOW)  
    {//button has internl pull up so if pressed connects to ground and becomes low      
      if( ((millis() - waitTime) >20) && buttonPressed == 1 )
      {
        buttonPressed =0;
        buttonFlag =1;
        enableIRRx();
        Serial.println("Button press Detected,enabled learn mode"); 
      }
      else if(!buttonPressed)
      {
        buttonPressed = 1;
        waitTime = millis();
      }
      else
      {
           
          
      }      
    }
    else
    {//if pin state is high then button is not pressed
      buttonPressed =0;
    }
}

void buttonInit(void)
{
  pinMode(ButtonInterruptPin, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(ButtonInterruptPin), handleInterrupt, CHANGE); 
}
