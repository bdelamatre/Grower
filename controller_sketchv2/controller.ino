/**

For more information, including setup, check readme.md

project structure:
* controller_sketch  = variable definition, setup() and loop()
* a_init             = contains *init() functions used in setup()
* b_config           = functions for read/write EEPROM config
* d_sensors          = checkSensors() and related functions. Support for additional sensor types may be added here.
* e_zones            = functions for turning zones on/off
* f_schedules        = checkSchedules() and related functions. Support for additional schedule types may be added here.
* g_log              = functions used for logging to SD
* i_display          = various functions used for fomatting text
* z_ntp              = ntp functions

required components:
* RTC - Chronodot (https://www.adafruit.com/products/255)

supported components:
* Temperature Sensors:
  * DHT22 (https://www.adafruit.com/products/393) 
* Analog Soil Moisture Sensors:
  * Check Ebay, or build a zinc nail sensor
  
**/

/** 
For non-standard libraries copy submodules included under FatRabbitGarden/libraries/ into your Arduino IDE libraries/ folder. 
**/
#include <SD.h>
#include <Wire.h> 
#include <Chronodot.h> //Chronodot by Stephanie-Maks
#include <SoftwareSerial.h>

//comment this out in production
#define DEBUG
//#define SETTIME

/*
SD variables
On the Ethernet Shield, CS is pin 4. Note that even if it's not
used as the CS pin, the hardware CS pin (10 on most Arduino boards,
53 on the Mega) must be left as an output or the SD library
functions will not work.
*/
const boolean useSdBuffer = true;
const int chipSelect = 4;
const int hardwareSelect = 10; 
Sd2Card card;
SdVolume volume;
SdFile root;

//RTC variable
const int useRtc = false;
boolean rtcHasConfig;
Chronodot RTC;

SoftwareSerial impSerial(8, 9); // RX on 8, TX on 9

String inputCommand = "";
String outputCommand = ""; 

// the setup routine runs once when you press reset:
void setup() {

  Serial.begin(19200);
  impSerial.begin(19200);
  
  inputCommand.reserve(200);
  outputCommand.reserve(200);

  #if defined(DEBUG)
    Serial.println("==========================================");
    Serial.println("|| Fat Rabbit Farm - Garden Controller  ||");
    Serial.println("==========================================");
  #endif
  
  if(useSdBuffer==true){
    initSd();
  }
  
  if(useRtc==true){
    initRtc();
  }
    
  #if defined(DEBUG) 
    Serial.println("==========================================");
  #endif
  
}

// the loop routine runs over and over again forever:

// the loop routine runs over and over again forever:
void loop(){
  
  
  //get local time in loop()
  DateTime timeLocal = getLocalTime();
  
  //safety turn off
  //safetyOff(timeLocal);

  //push status (every 10 seconds)
  //pushStatus();

  //pull config (every 10 seconds)
  //checkConfig();

  //check sensors
  //checkSensors(timeLocal);
    
  //check schedules
  //checkSchedules(timeLocal);

  //display settings
  //pushDisplay();
  
  //push value
  //pushValues();
  
  // Send data from the software serial
  //if (impSerial.available())
    //Serial.write(impSerial.read());  // to the hardware serial
 // Send data from the hardware serial
  //if (Serial.available())
    //impSerial.write(Serial.read());  // to the software serial
  
  //build and send commands to impSerial
  //while (Serial.available()) {
  if (Serial.available()){
    char inChar = (char)Serial.read(); 
    inputCommand += inChar;
    //done building command
    if (inChar == '>') {
      if(inputCommand==">"){
        //empty command
        inputCommand = "";
      }else{
        sendCommand(inputCommand);
        inputCommand = "";
      }
    }
  }

}

void executeCommand(String command, String query){
  
  switch(command){
      case 'configure-device:set-time':
        commandConfigureDeviceSetTime(Query);
      default:
        Serial.println("Unrecognized command: "+command);
  } 
  
}

void sendCommand(String command){
  
  Serial.print("Sending command: ");
  Serial.println(command);
  int commandLength = command.length();
  int i = 0;
  while(i<commandLength){
    char sendChar = command.charAt(i);
    impSerial.write(byte(sendChar));
    i++;
  }
  
}
