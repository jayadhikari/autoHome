#include <IRrecv.h>
#include "IRApplicationCode.h"
#include "myEeprom.h"

#define MAX_COMMAND   1


static int totalAvailableIRCommands=0;
long unsigned int IRTimeout = 10000;

decode_results results;  // Somewhere to store the results


void initCmdStr(void)
{
  for(int i=0; i<MAX_COMMAND;i++)
  {
    //controlCmd[i].command ="";
    //controlCmd[i].repeat=0;
    controlCmd[i].commandIR.count=0;
  }
}
/*
 * This function is called when the device needs to learn a new IR command
 * To learn new command Alexa is asked to put device in learn mode
 */
boolean learn(String dev,String cmdName)
{
    boolean returnVal=false; 
  //decode_results results;
  int totalCmd=0;
 // unsigned long int currentTime=0;
 // unsigned long int baseTime =0;

 // if(totalAvailableIRCommands)
 // {
   // Serial.println("2nd time\n");
 // }
 // loopOn =true;
 // baseTime = millis();
  /*
  for( totalCmd=0;totalCmd < MAX_COMMAND; totalCmd++)
  {
    
    if(controlCmd[totalCmd].command == "")
    {
      loopOn =true;
      controlCmd[totalCmd].command = dev+cmdName;

      totalAvailableIRCommands += 1;

     // if(totalAvailableIRCommands >=2)
      //  Serial.println("2nd time inside\n");
        
      if(totalAvailableIRCommands >=5)
        totalAvailableIRCommands = 1;

      baseTime = millis();  

      
      break;
      
       
      //Serial.print("command added= ");
      //Serial.println(controlCmd[totalCmd].command);
    }
  }*/
  
   /* currentTime = millis(); 
    if(( currentTime - baseTime) >= IRTimeout)
    {
        loopOn = false;
        Serial.print("FAIL");
    }*/
   // Serial.print("command Count= ");
   // Serial.println(totalCmd);

    
    if(irrecv.decode(&results)) 
    {
      //Serial.println(results.value);
     // if (results.decode_type == SONY) 
     // {
        //controlCmd[totalCmd].repeat=3;
     // }
    //  else
    //  {
    //    controlCmd[totalCmd].repeat=1;
    //  }    
     
        Serial.println(results.rawlen - 1);
        if(results.rawlen > 20)
        {
          
        controlCmd[totalCmd].commandIR.count = results.rawlen;
        for(int code = 1; code < results.rawlen; code++) 
        {
          controlCmd[totalCmd].commandIR.duration[code-1] = results.rawbuf[code] * RAWTICK;
          yield();  // Feed the WDT (again)
          //unsigned int number = 
         // Serial.print(",");
         // Serial.print(number);
          
        }
      //  saveCommand(controlCmd);
        yield();  // Feed the WDT (again)
        Serial.print("totalCmd= ");
        Serial.println(totalCmd);
        Serial.print("command= ");
       // Serial.println(controlCmd[totalCmd].command);
       // Serial.print("repeat= ");
       // Serial.println(controlCmd[totalCmd].repeat,HEX);
        Serial.print("buf count= ");
        Serial.println(controlCmd[totalCmd].commandIR.count);
        Serial.print("buffer= ");
        yield();  // Feed the WDT (again)
        for(int i=0;i<controlCmd[totalCmd].commandIR.count;i++)
        {
          Serial.println(controlCmd[totalCmd].commandIR.duration[i]);
          yield();  // Feed the WDT (again)
        }
        //loopOn = false;
        Serial.print("DONE");
        delay(100);
        returnVal =true;
      }
      irrecv.resume();
     
    }     
   
  return returnVal;
}  

void handleIRCommandSend(void)
{    

    static int commandSendCount=0;
    
    if(sendIRMusicCommandflag)
    { 
      sendSONYCommands = 1;     
      if(commandSendCount < 1)
        {
          commandSendCount +=1;
          sendMusicCommand();
          Serial.println("music on sent");
        }
        else
        {
            sendSONYCommands = 0;
            commandSendCount = 0;
            sendIRMusicCommandflag = 0;
        }
    }
    else if(sendIRTvCommandflag)
    {
      if(sendIRTvCommandflag)
      {
          sendIRTvCommandflag = sendTvCommand();
      }
    }
    else if(sendIRCommandflag)
    {
      sendIRCommandflag=0;
      sendIRCommand(controlCmd[0].commandIR.duration,controlCmd[0].commandIR.count);
    }
    else if(sendMusicFunctionCommandflag)
    {
        //sendMusicFunctionCommandflag=0; 
        sendSONYCommands = 1;       
        if(commandSendCount < 1)
        {
          commandSendCount +=1;
          sendMusicFunctionCommand();
        }
        else
        {
            sendSONYCommands = 0;
            commandSendCount = 0;
            sendMusicFunctionCommandflag = 0;
        }
    }
    else if(MusicVolumeDownflag)
    {
        volumeLevel += 1;
        if(volumeLevel <= 5)
        {
            sendMusicVolumeDownCommand();
        }
        else
        {
           volumeLevel=0;
           MusicVolumeDownflag =0;
        }
    }
    else if(MusicVolumeflag)
    {        //increase volume 5 levels
        volumeLevel += 1;
        if(volumeLevel <= 5)
        {
            sendMusicVolumeCommand();
        }
        else
        {
           volumeLevel=0;
           MusicVolumeflag =0;
        }
    }
  
}
