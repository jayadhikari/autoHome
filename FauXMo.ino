#include "fauxmoESP.h"

#include "FauXMo.h"

fauxmoESP fauxmo;

bool tState;
extern decode_results results;

#define DEVICE_ON   (1)
#define DEVICE_OFF   (0)

void handleFauxmp(void)
{
  fauxmo.handle();
}
void addSmartDevices(void)
{
    // Add virtual devices
    fauxmo.addDevice("tube");
    fauxmo.addDevice("bulb");
    fauxmo.addDevice("bed tube"); // You can add more devices
    fauxmo.addDevice("bed bulb");
    fauxmo.addDevice("fan");
    fauxmo.addDevice("fancy light"); // You can add more devices
    fauxmo.addDevice("lights"); // You can add more devices
    
    fauxmo.addDevice("learn mode"); // You can add more devices
    fauxmo.addDevice("speaker"); // You can add more devices
    fauxmo.addDevice("TV"); // You can add more devices
    fauxmo.addDevice("Device"); // You can add more devices
    fauxmo.addDevice("function"); // You can add more devices
    fauxmo.addDevice("volume up"); // You can add more devices
    fauxmo.addDevice("volume down"); // You can add more devices
    
}

void fauxmoInit(void)
{
    fauxmo.enable(true);
    addSmartDevices();
    
    // fauxmoESP 2.0.0 has changed the callback signature to add the device_id, this WARRANTY
    // it's easier to match devices to action without having to compare strings.
    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state) 
    {
        Serial.printf("[MAIN] Device #%d (%s) state: %s\n", device_id, device_name, state ? "ON" : "OFF");
        //digitalWrite(LED, !state);
        tState =  state;      
        switch(device_id)
        {
            case 0:flag.bits.DrawingRoomTubeFlag =1;             
                //    state.bits.DrawingRoomTubeFlag = state;
            break;
            case 1:flag.bits.DrawingRoomBulbFlag =1; 
                 // state.bits.DrawingRoomBulbFlag = state;
            break;   
            case 2:flag.bits.BedRoomTubeFlag=1; 
                //  state.bits.BedRoomTubeFlag = state;
            break;
            case 3:flag.bits.BedRoomBulbFlag=1; 
                //  state.bits.BedRoomBulbFlag = state;
            break;
            case 4:flag.bits.fanFlag=1;
               //   state.bits.fanFlag = state;
            break;   
            
            case 5:flag.bits.fancyLightflag =1;
                //   state.bits.fancyLightflag = state;
            break;
            
            case 6:deviceID =BULB;            
            firstCommandTime = millis();
            break;           
            
            case 7: sclearnMode=1;
                    enableIRRx();
                    memset(&results,0,sizeof(results));
            break;
            case 8: 
                    sendIRMusicCommandflag =1;
            break;
            case 9: 
                    sendIRTvCommandflag =1;
            break;
            case 10:
                    sendIRCommandflag =1;
            break;
            case 11:
                    sendMusicFunctionCommandflag =1;
            break;
            case 12:
                    MusicVolumeflag =1;
            break;
            case 13:
                    MusicVolumeDownflag =1;
            break;
            
        }
        
    });

    // Callback to retrieve current state (for GetBinaryState queries)
    fauxmo.onGetState([](unsigned char device_id, const char * device_name) 
    {
      return (tState);
        //return (state.Byte & (1 << device_id));
    });

}

unsigned char getNewStatus(unsigned char Byte1 , unsigned char Byte2,CMD_SOURCE cmdSource,boolean req)
{//to differentiate between ON and OFF commands. Fauxmo handles the commands from echo. APP and web commands has to 
  //send a state bit to indicate if the command is for ON or OFF
  boolean stateReq;
  
  if(cmdSource == WEB_APP)
    stateReq = req;
  else
    stateReq = tState;   
     
    if( stateReq == DEVICE_OFF)
    {
        Byte1 = Byte1 & (~Byte2);
    }
    else
    {
      Byte1 = Byte1 |  Byte2;
    } 
    return Byte1;
}
void checkActionTobePerformed(void)
{//this routine will execute for a single device at a time, like when asking echo to operate a device  

  
    if(flag.bits.DrawingRoomTubeFlag )
    {    
      DrawingRoomBulb();        
    }
    else if(flag.bits.DrawingRoomBulbFlag)
    {    
      DrawingRoomTube();
    }
    else if(flag.bits.BedRoomTubeFlag )
    {    
      BedRoomTube();
    }
    else if(flag.bits.BedRoomBulbFlag )
    {
      BedRoomBulb();
    }
    else if(flag.bits.fanFlag)
    {
      fan();
    }
    else if(flag.bits.fancyLightflag )
    {
      fancyLight();
    }  
  
  
             
    Serial.print("\n\rflag.Byte = ");
    Serial.println(flag.Byte, HEX);
    Serial.print("state.Byte = ");
    Serial.println(state.Byte, HEX);

    state.Byte = getNewStatus(state.Byte,flag.Byte,ECHO,0);
    
    Serial.print("\n\rnew device status = ");
    Serial.println(state.Byte, HEX);
    saveDeviceStatus(state.Byte);      
      
 
    flag.Byte = 0;
  
}
void restoreState(void)
{
  if(flag.bits.DrawingRoomTubeFlag && !state.bits.DrawingRoomTubeFlag)
  {
    state.bits.DrawingRoomTubeFlag = 1;
    DrawingRoomBulb();        
  }
  else if(flag.bits.DrawingRoomBulbFlag && !state.bits.DrawingRoomBulbFlag)
  {
    state.bits.DrawingRoomBulbFlag = 1;
    DrawingRoomTube();
  }
  else if(flag.bits.BedRoomTubeFlag && !state.bits.BedRoomTubeFlag)
  {
    state.bits.BedRoomTubeFlag = 1;
    BedRoomTube();
  }
  else if(flag.bits.BedRoomBulbFlag && !state.bits.BedRoomBulbFlag)
  {
    state.bits.BedRoomBulbFlag = 1;
    BedRoomBulb();
  }
  else if(flag.bits.fanFlag && !state.bits.fanFlag)
  {
    state.bits.fanFlag = 1;
    fan();
  }
  else if(flag.bits.fancyLightflag && !state.bits.fancyLightflag)
  {
    state.bits.fancyLightflag = 1;
    fancyLight();
  }
  
}














