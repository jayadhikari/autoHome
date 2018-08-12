/*
Released under Creative Commons Attribution 4.0
by bitluni 2016
https://creativecommons.org/licenses/by/4.0/
Attribution means you can use it however you like as long you
mention that it's base on my stuff.
I'll be pleased if you'd do it by sharing http://youtube.com/bitlunislab
*/

#include <Arduino.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
//#include "fauxmoESP.h"
//#include "credentials.h"
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
//#include "DHT11.h"

#include "IRTxRx.h"
#include "button.h"
#include "FauXMo.h"
#include "IRApplicationCode.h"


#include "time.h"
#include "myEeprom.h"

#include "RF.h"
//#include "DHT.h"
//#define PIR_FEATURE

#define LED                             2
#define MAX_DEVICE_TO_SAVE (5)

#define DEVICE_ON (1)
#define DEVICE_OFF (0)

const int RECV_PIN = D5;


IRrecv irrecv(RECV_PIN, CAPTURE_BUFFER_SIZE, TIMEOUT, true);
IRsend irsend(D3);  // An IR LED is controlled by GPIO pin 4 (D3)
/***NTP init***/
WiFiUDP ntpUDP;//for NTP
NTPClient timeClient(ntpUDP);
/***NTP init end***/
//#define DEBUG
MDNSResponder mdns;

extern boolean buttonFlag;

static int volumeLevel=0;
static long unsigned int checkTime =0,firstCommandTime=0,sysSecTick=0,SONYCommandTime=0;

boolean sclearnMode = 0,sendIRMusicCommandflag=0,sendIRTvCommandflag=0,sendIRCommandflag=0;
boolean sendMusicFunctionCommandflag=0,MusicVolumeflag=0,MusicVolumeDownflag=0;
boolean sendSONYCommands;//sony IR commands require send 3 IR commands for the same function

_EEPaddress controlCmd[1];
//_EEPaddress EEPROMLoc ;

//stChanID stCh;
_savedTime detectTime,checkTimeout;
// the current address in the EEPROM (i.e. which byte
// we're going to write to next)


boolean triggeredLightOn=false;

#ifdef PIR_FEATURE  
int LIGHTS_OFF_TIMEOUT = 5;
boolean humanPresent=false;
#endif

const char* ssid = "Yugoslavia";
const char* password = "trintin";

const int RF_OSC = 200;

int mainLight = 4;
int channelNum =0;
int timeNTP=0;

typedef enum deviceNumber{BULB,TUBE,BED_BULB, BED_TUBE,FAN,NONE};

deviceNumber deviceID = NONE;

ESP8266WebServer server(80);

int deviceCount=0;
typedef struct Info
{
    int devId;
    char channelNo;
    int scheduleFromTime;
    int scheduleToTime; 
    int Status; 
     
}deviceInfo;

deviceInfo infoSchedule[5];
const int pinNumbers[] = {D0, D1, D2, D3, D4, D5, D6, D7, D8};

//http://192.168.0.100/time?D=4&t=200&id=28015&on=1&channel=0&timeTo=300&timeFr=120


void handleRoot() {
  String message = "<html><head></head><body style='font-family: sans-serif; font-size: 12px'>";
  message += "Set up the RF code and press on/off to generate and call the corrensponding url:<br><br>";
  message += "<form id='data' action='/rf' target='response'>";
  message += "Output pin D<input name='D' type='number' value='4' style='width:40px' min='0' max='8'> ";
  message += "pulse<input name='t' type='number' value='200' style='width:60px' min='0' max='1000'>µs ";
  message += "ID<input name='id' type='number' value='28013' style='width:80px' min='0' max='1048575'> ";
  message += "<input name='on' type='hidden' value='1'>";
  message += "channel<input name='channel' type='number' value='0' style='width:40px' min='0' max='3'> ";
  message += "<script>function send(on){ document.getElementsByName('on')[0].value = on; document.getElementById('data').submit();}; function loaded(){ var a = document.getElementById('target'); a.innerHTML = a.href = document.getElementById('response').contentWindow.location; };</script>";
  message += "<input type='button' value='on' onclick='send(1)'><input type='button' value='off' onclick='send(0)'>";
  message += "</form><br>";
  message += "<iframe id='response' name='response' style='display:none' onload='loaded()'></iframe>URL constucted and called: <a href='#' id='target'>...</a><br><br>";
 // message += "have fun -<a </body></html>";
  //message += "have fun -<a /*href='http://youtube.com/bitlunislab'>bitluni*/</a></body></html>";
  server.send(200, "text/html", message);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

int getArgValue(String name)
{
  for (uint8_t i = 0; i < server.args(); i++)
    if(server.argName(i) == name)
      return server.arg(i).toInt();
  return -1;
}
//http://192.168.0.100/time?D=4&t=200&id=28015&on=1&channel=0&timeTo=300&timeFr=120
void handleTime()
{
  int id = getArgValue("id");
  int ch = getArgValue("channel");
  int on = getArgValue("on");
  int timeTo = getArgValue("timeTo");
  int timeFrom = getArgValue("timeFr");

  if(deviceCount < MAX_DEVICE_TO_SAVE)
  {
    infoSchedule[deviceCount].devId = id;
    infoSchedule[deviceCount].channelNo = ch;
    infoSchedule[deviceCount].scheduleFromTime = timeFrom;
    infoSchedule[deviceCount].scheduleToTime = timeTo;
    infoSchedule[deviceCount].Status = DEVICE_OFF;
    deviceCount++;//increase device count   
  }

  server.send(10, "text/plain", "OK TIME");
  Serial.print("timeTo=");
  Serial.println(timeTo);
  Serial.print("timeFrom=");
  Serial.println(timeFrom);
}
void handleRf()
{	
	int pin = getArgValue("D");
	int t = getArgValue("t");
	if(t == -1) 
	  t = RF_OSC;
   
	int id = getArgValue("id");
	int ch = getArgValue("channel");
	int on = getArgValue("on");
	/*String out = "rf D";
	out += pin;
	out += " ";
	out += t;
	out += " ";
	out += id;
	out += " ";
	out += ch;
	out += " ";
	out += on;*/
	pinMode(pinNumbers[pin], OUTPUT);
 
 if(pin == 4)
 {
    /* mainLight = pin;
     stCh.channelID = id;
     channelNum = 1 << (ch + 1);  

      triggeredLightOn = false;//if operated through app/web then reenable local control through sensor
     
     writeEEPROM((unsigned char)mainLight,ADDR_MAIN_LIGHT); 
     writeWordToEEPROM(stCh.channelID);      
     
     writeEEPROM((unsigned char)channelNum,addrChannelNum);  
     */
 }
	for(int i = 0; i < 5; i++)
		rfWriteCode(pinNumbers[pin], t, id, (1 << (ch + 1)) | (on > 0? 1: 0));
    
	server.send(2, "text/plain", "OK");	
}

void DrawingRoomBulb(void)
{
     //http://192.168.0.103/rf?D=4&t=200&id=28013&on=1&channel=0  
   for(int i = 0; i < 5; i++)
      rfWriteCode(D4, 200, 28013, (1 << (0 + 1)) | (1 > 0? 1: 0));
}
void DrawingRoomTube(void)
{
     //http://192.168.0.103/rf?D=4&t=200&id=28013&on=1&channel=0  
   for(int i = 0; i < 5; i++)
      rfWriteCode(D4, 200, 28013, (1 << (1 + 1)) | (1 > 0? 1: 0));
}
void BedRoomBulb(void)
{
     //http://192.168.0.103/rf?D=4&t=200&id=28013&on=1&channel=0  
   for(int i = 0; i < 5; i++)
      rfWriteCode(D4, 200, 28015, (1 << (0 + 1)) | (1 > 0? 1: 0));
}
void BedRoomTube(void)
{
     //http://192.168.0.103/rf?D=4&t=200&id=28013&on=1&channel=0  
   for(int i = 0; i < 5; i++)
      rfWriteCode(D4, 200, 28015, (1 << (1 + 1)) | (1 > 0? 1: 0));
}
void fan(void)
{
     //http://192.168.0.103/rf?D=4&t=200&id=28013&on=1&channel=0  
   for(int i = 0; i < 5; i++)
      rfWriteCode(D4, 200, 28015, (1 << (2 + 1)) | (1 > 0? 1: 0));
}
void AllLights(void)
{
  switch(deviceID)
  {
    case BULB:
          DrawingRoomBulb();
          deviceID = TUBE;
           
          break;
    case TUBE:      
          DrawingRoomTube();
          deviceID = BED_BULB;
          break;
    case BED_BULB:
          BedRoomBulb();
          deviceID = BED_TUBE;
          break;
    case BED_TUBE:
          BedRoomTube();
          deviceID = NONE;
          break;
  }
}
void checkDeviceSchedule(int timeNTP)
{//check if any device falls inside scheduling time
      if(deviceCount)
      {//if theres any device which needs scheduling
        for(int count=0; count < deviceCount;count++)
        {
          if(infoSchedule[count].Status == DEVICE_ON)
          {//if device is ON see if it has to be turned off            
              if(timeNTP > infoSchedule[count].scheduleToTime)
              {//ON till time is lesser than current time, then turn device OFF 
                  for(int i = 0; i < 5; i++)
                    rfWriteCode(D4, RF_OSC, infoSchedule[count].devId, (1 << (infoSchedule[count].channelNo + 1)) | 1);

                  Serial.println("Turning OFF device");
                  infoSchedule[count].Status = DEVICE_OFF;
              }
          }
          else if(infoSchedule[count].Status == DEVICE_OFF)
          {//if NTP time falls between device schedule time and device is off, turn on device
             if((infoSchedule[count].scheduleFromTime <= timeNTP) && (infoSchedule[count].scheduleToTime >timeNTP))     
             {
              Serial.println("Turning ON device");
                for(int i = 0; i < 5; i++)
                    rfWriteCode(D4, RF_OSC, infoSchedule[count].devId, (1 << (infoSchedule[count].channelNo + 1)) | 1);
                infoSchedule[count].Status = DEVICE_ON;
             }
             
          }
        }
      }
}
//This program get executed when interrupt is occures i.e.change of input state

void setup(void)
{
  //ESP.wdtDisable();
  //ESP.wdtEnable(WDTO_8S);
  //irsend.begin();
  Serial.begin(115200); 
  //DHTInit();   
 // DHT dht(DHTPIN, DHTTYPE);
 // dht.begin(); 
  
  eepromSetup();  
  initTime();
  initIR();//init IR send recieve  
 // buttonInit();
  
  #ifdef PIR_FEATURE
  setupPIRSensor();
  #endif
  
 readSavedValues(); 
    
  pinMode(mainLight, OUTPUT);//main light
  pinMode(D2, OUTPUT);//LED light
  //pinMode(D3, OUTPUT);//IR Send
   
  WiFi.begin(ssid, password);
  Serial.println("..");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  if(mdns.begin("mySmartHome",WiFi.localIP()))
  {
    Serial.println("mDNS server started..");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/rf", handleRf); 
  server.on("/time", handleTime);  
  server.onNotFound(handleNotFound);

  server.begin();  
  Serial.println("HTTP server started");
  MDNS.addService("http","tcp",80);
   
    // You can enable or disable the library at any moment
    // Disabling it will prevent the devices from being discovered and switched

   fauxmoInit();

   sysSecTick = millis();//timer to check system time
   timeClient.begin();
 
}

void loop(void)
{
  //updateTime();
 // ESP.wdtFeed();
  digitalWrite(D2, LOW);
  server.handleClient();
  digitalWrite(D2, HIGH);

  handleFauxmp();

  
  checkTime = millis();

  if((checkTime - sysSecTick)  > 2000)
  {//execute IR commands after hey are recieved from alexa

    handleIRCommandSend();        
    SONYCommandTime = sysSecTick = checkTime;
    timeClient.update();
    //Serial.println(timeClient.getFormattedTime());
    timeNTP = (timeClient.getHours()*60)+330+timeClient.getMinutes();
    Serial.println(timeNTP);
    checkDeviceSchedule(timeNTP);  
    
  //  getTempValues();
  }
  else if(sendSONYCommands && (checkTime - SONYCommandTime )  > 5)
  {
       handleIRCommandSend(); 
       SONYCommandTime = checkTime;
  }

  if((checkTime - firstCommandTime) > 2000)
  {  
      AllLights();//handle all devices switching in this
      firstCommandTime = checkTime;
  }
  if(sclearnMode || buttonFlag)
  {      
    if(learn("MUSIC","power") == true)//parameters to pass 1.device name(eg music,TV,AC), 2.CommandName(volume,power,etc)
    {
        sclearnMode = 0;//stop checking for IR messages when not in learn mode
        buttonFlag = 0;
        disableIRRx(); 
        Serial.println("Learn mode Exited"); 
    }    
    
  }

}
