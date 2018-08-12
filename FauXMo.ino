#include "fauxmoESP.h"
#include "credentials.h"
#include "FauXMo.h"

fauxmoESP fauxmo;
extern decode_results results;

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
        switch(device_id)
        {
            case 0:DrawingRoomTube();break;
            case 1:DrawingRoomBulb();break;   
            case 2:BedRoomTube();break;
            case 3:BedRoomBulb();break;
            case 4:fan();break;   
            case 5:deviceID =BULB;            
            firstCommandTime = millis();break;           
            case 6: sclearnMode=1;
                    enableIRRx();
                    memset(&results,0,sizeof(results));
            break;
            case 7: 
                    sendIRMusicCommandflag =1;
            break;
            case 8: 
                    sendIRTvCommandflag =1;
            break;
            case 9:
                    sendIRCommandflag =1;
            break;
            case 10:
                    sendMusicFunctionCommandflag =1;
            break;
            case 11:
                    MusicVolumeflag =1;
            break;
            case 12:
                    MusicVolumeDownflag =1;
            break;
        }
        
    });

    // Callback to retrieve current state (for GetBinaryState queries)
    fauxmo.onGetState([](unsigned char device_id, const char * device_name) 
    {
        return digitalRead(LED) == HIGH;
    });

}
