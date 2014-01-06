/**

@author Byron DeLamatre <byron@delamatre.com>
@url https://github.com/bdelamatre/FatRabbitGarden
@about A controller designed for managing and monitoring your garden. Check readme.md

project structure:
* controller         = variable definition, setup() and loop()
* a_init             = contains *init() functions used in setup()
* b_config           = functions for read/write EEPROM config
* c_commands         = commands for controlling the system
* d_sensors          = checkSensors() and related functions. Support for additional sensor types may be added here.
* e_zones            = functions for turning zones on/off
* f_schedules        = checkSchedules() and related functions. Support for additional schedule types may be added here.
* g_logs             = functions used for logging
* u_utilities        = various utility functions
  
**/

/** 
For non-standard libraries copy submodules included under FatRabbitGarden/libraries/ into your Arduino IDE libraries/ folder. 
**/
#include <avr/wdt.h>
#include <EEPROM.h>
#include <SD.h>
#include <Wire.h> 
#include <Chronodot.h> //Chronodot by Stephanie-Maks
#include <Flash.h>
#include <DHT.h> //DHT by AdaFruit
#include <SoftwareSerial.h> //DHT by AdaFruit

//#define SENSORONLY

#define USESERIALMONITOR
#define USESERIALCOM
//#define USESOFTWARESERIAL
//#define USESD

#define CLIONMONITOR
#define DEBUG
//#define DEBUGSENSORS
//#define DEBUGCONFIG
//#define DEBUGMEM
//#define DEBUGHEARTBEAT
//#define DEBUGTIMESYNC
//#define SETTIME
//#define MANUALCONFIG

//SoftwareSerial softSerial(8, 9); // RX on 8, TX on 9

#define CONFIG_VERSION "1v1"
#define CONFIG_START 512

const char commandStringSystemHeartbeat[]     = "s:hb";
const char commandStringSystemRestart[]       = "s:restart";
const char commandStringSystemReinit[]        = "s:init";
const char commandStringConfigSetTime[]       = "c:time";
const char commandStringConfigSaveAsId[]      = "c:save-as";
const char commandStringConfigSave[]          = "c:save";
const char commandStringConfigResetDefault[]  = "c:reset";
const char commandStringConfigZone[]          = "c:z";
const char commandStringConfigZoneReset[]     = "c:z-reset";
const char commandStringConfigSensor[]        = "c:s";
const char commandStringConfigSensorReset[]   = "c:s-reset";
const char commandStringConfigSchedule[]      = "c:sc";
const char commandStringConfigScheduleReset[] = "c:sc-reset";
const char commandStringDataLogReceived[]     = "d:received";

boolean configInProgress = false;

const int sensorCheckDelay = 1000;
unsigned long sensorCheckLast = 0;

const int heartBeatDelay = 1500;
const int heartBeatDelayWait = 18500;
boolean heartBeatOnline = false;
boolean heartBeatInProgress = false;
unsigned long heartBeatSent = 0;
unsigned long heartBeatLast = 0;

/*
SD variables
On the Ethernet Shield, CS is pin 4. Note that even if it's not
used as the CS pin, the hardware CS pin (10 on most Arduino boards,
53 on the Mega) must be left as an output or the SD library
functions will not work.
*/
#if defined(USESD)
const int chipSelect = 4;
const int hardwareSelect = 14; //Goldilocks
//const int hardwareSelect = 10;  //Arduino Ethernet Shield R3
Sd2Card card;
SdVolume volume;
SdFile root;
#endif

//RTC variable
boolean timeSyncInProgress = false;
boolean timeSynced = false;
int timeAtSync;
DateTime timeSyncedDateTime;

//the command buffer
const int maxBufferSize = 2048;

int commandBufferPosition = 0;
boolean commandBufferReadyToProcess = false;
char commandBuffer[maxBufferSize];

#if defined(USESERIALMONITOR) && defined(CLIONMONITOR)
  int commandBufferPositionMonitor = 0;
  boolean commandBufferReadyToProcessMonitor = false;
  char commandBufferMonitor[maxBufferSize]; 
#endif

/*
WARNING, increasing these will allow you to configure more 
schedules, zones and sensors, but increase the RAM and EEPROM 
usage. Be careful if increasing these that you stay within your
system limits, or stability issue will occur.
*/
const int maxSchedules = 6; 
const int maxZones = 6; 
const int maxSensors = 6;
const int maxNameLength = 16;
const int maxParamNameLength = 16;

#if !defined(SENSORONLY)
//schedule structure, managed by config structure
struct Schedule{
  char name[maxNameLength];
  int type; //0=off, 1=timer, 2=soil moisture, 3=temperature
  int zones[maxZones]; //zone id, 0 to maxZones specified
  int zonesRunType; //0=series, 1=parallel
  int sensors[maxSensors]; //zone id, 0 to maxSensors specified
  boolean timerStartWeekdays[7]; //1-7
  boolean timerStartHours[24]; //1-24
  boolean timerStartMinutes[60];//1-60
  boolean timerStartSeconds[60];//1-60
  int valueMin; //will turn zones on when this value is reached by the specified sensors
  int valueMax; //will turn zones off when this value is reached by the specified sensors
  int isRunning; //0=no,1=yes
};
//#endif

//#if !defined(SENSORONLY)
//zone structure, managed by config structure
struct Zone{
  char name[maxNameLength];
  int type; //0=off, 1=5v relay
  int pin;
  int safetyOffAfterMinutes;
  int overrideOn; //null=n/a,0=off, 1=on
  int isRunning; //0=off, 1=on
  unsigned long statusRunStarted;
  int statusRunBySchedule;
  int statusSafetyOff;
};
#endif

//sensor structure, managed by config structure
struct Sensor{
  char name[maxNameLength];
  int type; //0=off, 1=soil moisture (analog), 2=soil temperature(DS18B20), 3=air temperature (DHT22), 4=light  
  int pin;
  int pin2;
  int frequencyCheckSeconds; //0=every loop
  int frequencyLogSeconds; //0=every log
  //int enabled;
  unsigned long statusValue;
  unsigned long statusValue2;
  unsigned long statusLastChecked;
  unsigned long statusLastLogged;
};

/**
This is the main structure that contains the complete configuration for the system.
**/
struct ConfigStore{
  char version[4];
  char configId[11];
  unsigned long utcOffset;
  #if !defined(SENSORONLY)
  Schedule schedules[maxSchedules];
  Zone zones[maxZones];
  #endif
  Sensor sensors[maxSensors];
} configStore={
  CONFIG_VERSION,
  0,
  -6,
};

void(* restart) (void) = 0; //declare reset function @ address 0\

// the setup routine runs once when you press reset:
void setup() {
  
  //watchdog, 8 seconds
  wdt_enable(WDTO_8S);
  
  //if debug serial, we will output debug statements to serial
  #if defined(USESERIALMONITOR)
    Serial.begin(19200);
  #endif
  
  #if defined(USESERIALCOM)
    #if defined(USESOFTWARESERIAL)  
      softSerial.begin(19200);
    #else
      Serial1.begin(19200);
    #endif
  #endif
  
  #if defined(DEBUG)
    printAvailableMemory();
    printBanner();
  #endif
  
  loadConfig();
  #if defined(USESD)
    initSd();
  #endif
  initRtc();
  initController();
    
  #if defined(DEBUG) 
    printBreak();
    printAvailableMemory();  
  #endif
  
  printCommandLineAvailable();
  
}

//  loop - runs over and over again forever:
void loop(){
      
  #if defined(USESERIALCOM)
    #if defined(USESOFTWARESERIAL)  
      readSerialToBuffer(softSerial,commandBuffer,commandBufferPosition,commandBufferReadyToProcess);
    #else
      readSerialToBuffer(Serial1,commandBuffer,commandBufferPosition,commandBufferReadyToProcess);
    #endif
  #endif
  
  //process the command buffer
  if(commandBufferReadyToProcess==true){
    //go ahead and reset the watchdog if the buffer is ready to process
    wdt_reset();
    //process commands in the buffer
    processBuffer(commandBuffer);
    //fix-me: this shouldn't be necessary because of strtok
    //reset the memory of the buffer
    memset(commandBuffer,0,maxBufferSize);
    //reset the buffer position
    commandBufferPosition = 0;
    //do not process the buffer again
    commandBufferReadyToProcess = false;
  }

  #if defined(USESERIALMONITOR) && defined(CLIONMONITOR)
    //if we are using serial monitor, give it it's own command buffer
    readSerialToBuffer(Serial,commandBufferMonitor,commandBufferPositionMonitor,commandBufferReadyToProcessMonitor);
    
    //same as above, but with other buffer
    if(commandBufferReadyToProcessMonitor==true){
      wdt_reset();
      processBuffer(commandBufferMonitor);
      memset(commandBufferMonitor,0,maxBufferSize);
      commandBufferPositionMonitor = 0;
      commandBufferReadyToProcessMonitor = false;
    }
  #endif
  
  //if we are in configuration mode, prevent running the reset of the script
  //fix-me: we might be able to add something more elegant than relying on the watchdog timer
  if(configInProgress==true){
    return;
  }
  
  //if no commands are being received and we are in config mode for 8 seconds, reset
  wdt_reset();
  
  //heartbeat determines if the controller is online or offline
  //if the timer has timed out and it is time to send the next heartbeat
  if(((millis()-heartBeatSent)>=heartBeatDelay)){
                    
      //send heartbeat
      char buildCommand[32] = "s:hb?config=" ;
      strcat(buildCommand,configStore.configId);
      sendCommand(buildCommand);
      
  //heartbeat sent, but we haven't received a response for awhile
  }
  
  //check if the controller is offline
  if(heartBeatOnline == true && millis()-heartBeatLast>=(heartBeatDelay+heartBeatDelayWait)){

    #if defined(USERSERIALMONITOR)
      if(heartBeatOnline==true){
        Serial.println("[HEARTBEAT] [OFFLINE]");
      }
    #endif
              
    heartBeatOnline = false;
  }
  
  //we can't do anything else until the time is synced
  if(timeSynced==false && timeSyncInProgress==false){
      sendCommand("c:time");
  }
  
  //this prevents sensors from being checked on every pass
  if(timeSynced==true && (millis()-sensorCheckLast)>sensorCheckDelay){
    sensorCheckLast = millis();
    
    //time has been synced and we can continue with controller functions
    DateTime timeLocal = getLocalTime();
    
    //Serial.println(timeLocal.unixtime());
    //Serial.println("check sensors");
    
    
    //safety turn off
    //safetyOff(timeLocal);
  
    //check sensors
    checkSensors(timeLocal);
      
    //check schedules
    //checkSchedules(timeLocal);
  
    //display settings
    //pushDisplay();
    
  }
  
}
