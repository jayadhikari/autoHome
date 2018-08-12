/* 
 * //////////////////////////////////////////////////
 * //making sense of the Parallax PIR sensor's output
 * //////////////////////////////////////////////////
 *
 * Switches a LED according to the state of the sensors output pin.
 * Determines the beginning and end of continuous motion sequences.
 *
 * @author: Kristian Gohlke / krigoo (_) gmail (_) com / http://krx.at
 * @date:   3. September 2006 
 *
 * kr1 (cleft) 2006 
 * released under a creative commons "Attribution-NonCommercial-ShareAlike 2.0" license
 * http://creativecommons.org/licenses/by-nc-sa/2.0/de/
 *
 *
 * The Parallax PIR Sensor is an easy to use digital infrared motion sensor module. 
 * (http://www.parallax.com/detail.asp?product_id=555-28027)
 *
 * The sensor's output pin goes to HIGH if motion is present.
 * However, even if motion is present it goes to LOW from time to time, 
 * which might give the impression no motion is present. 
 * This program deals with this issue by ignoring LOW-phases shorter than a given time, 
 * assuming continuous motion is present during these phases.
 *  
 */

#//include<RF24.h>
/////////////////////////////
//VARS
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;        

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 5000;  

boolean lockLow = true;
boolean takeLowTime;  
boolean motionStatus=false;
boolean repeatUpdate=false;



int pirPin = 13;    //the digital pin connected to the PIR sensor's output
//int ledPin = 8;


/////////////////////////////
//SETUP
void setupPIRSensor()
{
  //Serial.begin(9600);
  pinMode(pirPin, INPUT);
 // pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);

  //give the sensor some time to calibrate
 // #ifdef DEBUG
  Serial.println("calibrating sensor ");
//  #endif
    for(int i = 0; i < calibrationTime; i++)
    {
     // #ifdef DEBUG
      Serial.print(".");
     // #endif
      delay(1000);
    }
  //  #ifdef DEBUG
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
   // #endif
    delay(50);
}

////////////////////////////
//LOOP
void setAvoidRepeatUpdate(boolean updateState)
{
    repeatUpdate = updateState;
}
boolean getAvoidRepeatUpdate(void)
{
    return repeatUpdate;  
}
void setMotionDetectionStatus(boolean statusDetect )
{
  motionStatus = statusDetect;
}
boolean getMotionDetectionStatus(void)
{
   return motionStatus;
}
void activateMotionDetection(void)
{

     if(digitalRead(pirPin) == HIGH && !getMotionDetectionStatus())
     {  
         setMotionDetectionStatus(true);    
     
     }

     if(digitalRead(pirPin) == LOW && getMotionDetectionStatus())
     {       
        setMotionDetectionStatus(false);
  
      }
}
