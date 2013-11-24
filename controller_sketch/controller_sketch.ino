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
//#include <SPI.h> 
#include <SD.h>
#include <Wire.h> 
//#include <EEPROM.h>
//#include <Base64.h> //Base64 by adamvr
#include <Chronodot.h> //Chronodot by Stephanie-Maks
#include <DHT.h> //DHT by AdaFruit

//comment this out in production
#define DEBUG
//#define SETTIME

//if set to true, assumes that you will manually configure the config structure below
const boolean useManualConfiguration = true;

//RTC variable
const int useRtc = false;
boolean rtcHasConfig;
Chronodot RTC;

//NTP variables
const int NTP_PACKET_SIZE= 48;
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets 

/*
SD variables
On the Ethernet Shield, CS is pin 4. Note that even if it's not
used as the CS pin, the hardware CS pin (10 on most Arduino boards,
53 on the Mega) must be left as an output or the SD library
functions will not work.
*/
const boolean useSdBuffer = false;
const int chipSelect = 4;
const int hardwareSelect = 10; 
Sd2Card card;
SdVolume volume;
SdFile root;

/*
WARNING, increasing these will allow you to configure more 
schedules, zones and sensors, but increase the RAM and EEPROM 
usage. Be careful if increases these that you stay within your
systems limits, or stability issue will occur.
*/
const int maxSchedules = 3; 
const int maxZones = 3; 
const int maxSensors = 3;

//schedule structure, managed by config structure
struct Schedule{
  char* name;
  int type; //0=off, 1=timer, 2=soil moisture, 3=temperature
  int zones[maxZones]; //zone id, 0 to maxZones specified
  int zonesRunType; //0=series, 1=parallel
  int sensors[maxSensors]; //zone id, 0 to maxSensors specified
  int timerStartWeekdays[7]; //1-7
  int timerStartHours[24]; //1-24
  int timerStartMinutes[60];//1-60
  int valueMin; //will turn zones on when this value is reached by the specified sensors
  int valueMax; //will turn zones off when this value is reached by the specified sensors
  int isRunning; //0=no,1=yes
};

//zone structure, managed by config structure
struct Zone{
  char* name;
  int type; //0=off, 1=5v relay
  int pin;
  int safetyOffAfterMinutes;
  int isRunning; //0=off, 1=on
  unsigned long statusRunStarted;
  int statusRunBySchedule;
  int statusSafetyOff;
};

//sensor structure, managed by config structure
struct Sensor{
  char* name;
  int type; //0=off, 1=soil moisture (analog), 2=soil temperature(DS18B20), 3=air temperature (DHT22), 4=light
  int pin;
  int pin2;
  int frequencyCheckSeconds; //0=every loop
  int frequencyLogSeconds; //0=every log
  unsigned long statusValue;
  unsigned long statusValue2;
  unsigned long statusLastChecked;
  unsigned long statusLastLogged;
};

/**
This is the main structure that contains the complete configuration for the system.
**/
#define CONFIG_VERSION "2v2"
#define CONFIG_START 32
struct Config{
  char version[4];
  unsigned long utcOffset;
  Schedule schedules[maxSchedules];
  Zone zones[maxZones];
  Sensor sensors[maxSensors];
} config={
  CONFIG_VERSION,
};

// the setup routine runs once when you press reset:
void setup() {

  Serial.begin(9600);
        
  //#if defined(DEBUG)
    printMemoryFree();
    Serial.println("==========================================");
    Serial.println("|| Fat Rabbit Farm - Garden Controller  ||");
    Serial.println("==========================================");
  //#endif
  
  return;
  
  //initialize configuration
  if(useManualConfiguration==true){
    myManualConfig();
  }else{
    loadConfig();
  }
  
  if(useSdBuffer==true){
    initSd();
  }
  
  if(useRtc==true){
    initRtc();
  }
  
  initSensors();
  initZones();
    
  #if defined(DEBUG) 
    Serial.println("==========================================");
    printMemoryFree();
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
  checkSensors(timeLocal);
    
  //check schedules
  //checkSchedules(timeLocal);

  //display settings
  //pushDisplay();
  
  //push value
  //pushValues();
  
}

