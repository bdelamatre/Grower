/**

@author Byron DeLamatre <byron@delamatre.com>
@url https://github.com/bdelamatre/Grower
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
* t_tests            = commands and functions used for testing the device
* u_utilities        = various utility functions  

about:
This firmware has been designed to run on multiple types of Arduino compatabile devices by configuring the options below. This ranges from
how you are communicating with the Grower server (through another device using Serial or direclty of Ethernet) to how many and what kind
of sensors and RTC devices you wish to support. You can configure all of these options below, but be aware that you can cause instability
if you are not sure what you are doing.
  
**/

/*********************************************************** 

Libraries

For non-standard libraries copy submodules included under Grower/libraries/ into your Arduino IDE libraries/ folder. 

***********************************************************/

/** Read/Write to EEPROM **/
#include <EEPROM.h>

/** Read and write to flash **/
#include <Flash.h>

/** Read/Write to SD card **/
//#include <SD.h>

/** Communicate with I2C/TWI devices, such as some sensors and RTCs **/
#include <Wire.h> 

/** Time functions  **/
#include <Time.h>

/** Support for DS1307 RTC **/
#include <DS1307RTC.h>

/** Support for DHT sensors **/
#include <DHT.h> //DHT by AdaFruit

/** Support for Software Serial interfaces, useful when working with a limited number of serial interfaces **/
//#include <SoftwareSerial.h> //DHT by AdaFruit

/** Support for SPI, needed if using an Ethernet shield **/
//#include <SPI.h>

/** Support for Wiznet Ethernet **/
//#include <Ethernet.h>

/** Wiznet library that provides additional configuration options for Ethernet **/
//#include <utility/w5100.h>


/*********************************************************** 

General Options

***********************************************************/

/** the command buffer size **/
const int maxBufferSize = 1512; //best
//const int maxBufferSize = 1024; //good
//const int maxBufferSize = 256; //light weight mode (not recommended)

/**
WARNING, increasing these will allow you to configure more 
schedules, zones and sensors, but increase the RAM and EEPROM 
usage. Be careful if increasing these that you stay within your
system limits, or stability issue will occur.
**/
const int maxSchedules = 5;        //how many schedules should this device support
const int maxZones = 5;            //how many zones should this device support
const int maxSensors = 6;          //how many sensors should this device support
const int maxNameLength = 7;       //how many characters can the identifying name of a zone, sensor or schedule
const int maxParamNameLength = 11; //how many characters can a parameter be
const int sensorCheckDelay = 5;    //how often (seconds) to poll sensors for data. Setting this lower can cause performance issues but increase real-time monitoring.
const int heartBeatDelay = 2;      //how often (seconds) to send a heartbeat
const int heartBeatDelayWait = 28; //how long to wait for a hearbeat response before going offline
const int timeSyncDelay = 60;      //how long to wait to receive a time sync response

#define SETTIME                    //manualy set system time, useful for debugging without an RTC or NTP access
#define STOPFORTIMESYNC false
//#define MANUALCONFIG             //instead of using EEPROM, override the configuration manually in loadManualConfig()
#define CONFIG_VERSION "2v2"       //when reading from EEPROM, identify saved configurations with this version
#define CONFIG_START 32            //position in EEPROM to start writing the configuration


/*********************************************************** 

Module - Serial Monitor

Will print useful information on specified serial interface. This is useful for debugging. 

***********************************************************/

#define USE_MODULE_SERIALMONITOR true

#define SERIALMONITOR Serial
#define SERIALMONITOR_BAUD_RATE 115200
#define DEBUG true //will output basic debugging, must be enabled for all other debugging to work
#define DEBUGETHERNET false
#define DEBUGSENSORS true
#define DEBUGSCHEDULE true
#define DEBUGCONFIG true
#define DEBUGMEM true
#define DEBUGHEARTBEAT true
#define DEBUGTIMESYNC true


/*********************************************************** 

Module - CLI on Serial Monitor

Allows manual interaction with the CLI while monitoring the Serial interface

***********************************************************/

#define USE_MODULE_CLIONSERIALMONITOR true

//do not edit this
#if USE_MODULE_SERIALMONITOR == true && USE_MODULE_CLIONSERIALMONITOR == true
  int commandBufferPositionMonitor = 0;
  boolean commandBufferReadyToProcessMonitor = false;
  char commandBufferMonitor[maxBufferSize]; 
#endif


/*********************************************************** 

Module - Ethernet Communication

Device will send and receive commands over Ethernet

***********************************************************/

#define USE_MODULE_ETHERNETCOM false

//system variables, do not edit this
#if USE_MODULE_ETHERNETCOM == true
  int commandBufferPositionEthernet = 0;
  boolean commandBufferReadyToProcessEthernet = false;
  char commandBufferEthernet[maxBufferSize];
#endif

//define client
#if USE_MODULE_ETHERNETCOM == true  
  EthernetClient client;
#endif


/*********************************************************** 

Module - Serial Communication

Device will send and receive commands over Serial interface defined for communication

***********************************************************/

#define USE_MODULE_SERIALCOM true

#define SERIALCOM Serial1 // Teensy++ or Arduino devices with multiple Serial interfaces
//#define SERIALCOM Serial // ArduinoUno and devices with a single Serial interface
#define SERIALCOM_BAUD_RATE 115200

//system variables, do not edit this
#if USE_MODULE_SERIALCOM == true
  int commandBufferPositionSerial = 0;
  boolean commandBufferReadyToProcessSerial = false;
  char commandBufferSerial[maxBufferSize];
#endif


/*********************************************************** 

Module - Serial Communication - Software Serial

If using the SERIALCOM module, you can specify whether SERIALCOM should use the SoftwareSerial library. This is useful when you don't have enough serial interfaces

***********************************************************/

#define USE_MODULE_SERIALCOM_SOFTWARESERIAL false
//SoftwareSerial softSerial(8, 9); // RX on 8, TX on 9


/*********************************************************** 

Module - Serial Communication - Software Serial

If using a DS1307 RTC

***********************************************************/

#define USE_MODULE_RTCDS1307 false
//#define DS1307_ADDRESS 0x68 //Wire address for the DS1307 RTC


/*********************************************************** 

Module - SD

If using an SD to buffer communications 

***********************************************************/

#define USE_MODULE_SD false

/*
On the Ethernet Shield, CS is pin 4. Note that even if it's not
used as the CS pin, the hardware CS pin (10 on most Arduino boards,
53 on the Mega) must be left as an output or the SD library
functions will not work.
*/
#if USE_MODULE_SD == true
  const int chipSelect = 4;
  const int hardwareSelect = 14; //Goldilocks
  //const int hardwareSelect = 10;  //Arduino Ethernet Shield R3
  Sd2Card card;
  SdVolume volume;
  SdFile root;
#endif


/*********************************************************** 

Module - Watchdog

Use the AVR built-in watchdog timer

***********************************************************/

#define USE_MODULE_WATCHDOG false       //use the watchdog timer

#if USE_MODULE_WATCHDOG == true
  /** Watchdog timer **/
  #include <avr/wdt.h>
#endif


/*********************************************************** 

Global Variables

Do not edit these for any reason

***********************************************************/

/** commands supported **/
const char commandStringSystemInfo[]          = "s:info";      //get system information
const char commandStringSystemHeartbeat[]     = "s:hb";        //send a system heartbeat
const char commandStringSystemRestart[]       = "s:restart";   //software restart of the device
const char commandStringSystemReinit[]        = "s:init";      //reinitialize the system without a restart
const char commandStringConfigSetTime[]       = "c:time";      //sets the system time. will save to RTC if present.
const char commandStringConfigSaveAsId[]      = "c:save-as";   //sets the id of the configuration. This will be used by the server to determine which config the device is using.
const char commandStringConfigSave[]          = "c:save";      //save the current configuration
const char commandStringConfigResetDefault[]  = "c:reset";     //resets the configuration by zeroing out the EEPROM
const char commandStringConfigZone[]          = "c:z";         //configure a zone using a string of parameters
const char commandStringConfigZoneReset[]     = "c:z-reset";   //resets a zone by id
const char commandStringConfigSensor[]        = "c:s";         //configure a sensor using a string of parameters
const char commandStringConfigSensorReset[]   = "c:s-reset";   //resets a sensor by id
const char commandStringConfigSchedule[]      = "c:sc";        //configure a schedule using a string of parameters
const char commandStringConfigScheduleReset[] = "c:sc-reset";  //resets a schedule by id
const char commandStringDataLogReceived[]     = "d:received";  //indicate that data was received and logged by the server
const char commandStringTestFactory[]         = "t:factory";   //
const char commandStringTestHeartbeat[]       = "t:hb";        //
const char commandStringTestTime[]            = "t:time";      //
const char commandStringTestRTC[]             = "t:rtc";       //
const char commandStringTestSD[]              = "t:sd";        //
const char commandStringTestZones[]           = "t:zones";     //
const char commandStringTestLoadConfig[]      = "t:config";    //

boolean configInProgress = false;    //will stop various processes while set to true

unsigned long sensorCheckLast = 0;   //the last time we checked for sensor data
boolean heartBeatOnline = false;     //set to true when a heartbeat response is received within the heartBeatDelayWait period
boolean heartBeatInProgress = false; //if we are in the middle of waiting for a heartbeat response
time_t heartBeatSent;                //when the current heartbeat was sent
time_t heartBeatLast;                //when was the last time we successfully received a heartbeat

boolean timeSyncInProgress = false;
boolean timeSynced = false;
time_t timeSyncedDateTime;

//schedule structure, managed by config structure
struct Schedule{
  char name[maxNameLength];
  int type;                      //0=off, 1=timer, 2=soil moisture, 3=temperature
  int zones[maxZones];           //zone id, 0 to maxZones specified
  int zonesRunType;              //0=series, 1=parallel
  int sensors[maxSensors];       //zone id, 0 to maxSensors specified
  char timerStartWeekdays[8];    //1-7
  char timerStartHours[25];      //1-24
  char timerStartMinutes[61];    //1-60
  //char timerStartSeconds[60];  //1-60
  int valueMin;                  //will turn zones on when this value is reached by the specified sensors
  int valueMax;                  //will turn zones off when this value is reached by the specified sensors
  int isRunning;                 //0=no,1=yes
};

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

//This is the main structure that contains the complete configuration for the system.
struct ConfigStore{
  char version[4];
  unsigned long utcOffset;
  char configId[11];
  char deviceId[33];
  char apiKey[13];
  #if USE_MODULE_ETHERNETCOM == true
    char server[255];
    unsigned int serverPort;
    byte mac[6];
    boolean dhcp;
    IPAddress address;
  #endif
  Schedule schedules[maxSchedules];
  Zone zones[maxZones];
  Sensor sensors[maxSensors];
} configStore={
  CONFIG_VERSION,
  -6,
  "0",
  "prop",
  "0",
  #if USE_MODULE_ETHERNETCOM == true
  "grower.io",
  8080,
  {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 },
  true,
  #endif
};

void(* restart) (void) = 0; //declare reset function @ address 0\


// the setup routine runs once when you press reset:
void setup() {
    
  //start wire
  Wire.begin();
  
  #if USE_MODULE_SERIALMONITOR == true
    //start serial connection for debugging output
    SERIALMONITOR.begin(SERIALMONITOR_BAUD_RATE);
  #endif
  
  #if USE_MODULE_SERIALCOM == true
    #if USE_MODULE_SERIALCOM_SOFTWARESERIAL == true  
      //start software serial port for server communication
      softSerial.begin(SERIALCOM_BAUD_RATE);
    #else
      //start serial port for server communication
      SERIALCOM.begin(SERIALCOM_BAUD_RATE);
    #endif
  #endif
  
  #if DEBUG == true && USE_MODULE_SERIALMONITOR == true  
    printAvailableMemory();
    //glamour banner
    printBanner();
  #endif
  
  loadConfig(); //load configuration from EEPROM
  #if USE_MODULE_ETHERNETCOM == true
    initEthernet(); //if using Ethernet for communication
  #endif
  #if USE_MODULE_SD == true
    initSd(); //if using an SD card for buffering communications
  #endif
  initRtc(); //iniatilize RTC
  initController(); //initalize sensors, zones and schedule
  
  #if DEBUG ==  true && USE_MODULE_SERIALMONITOR == true 
    printBreak();
    printAvailableMemory();  
  #endif
  
  #if USE_MODULE_SERIALMONITOR == true && USE_MODULE_CLIONSERIALMONITOR == true
    printCommandLineAvailable();
  #endif

  #if USE_MODULE_WATCHDOG == true
    //watchdog, 8 seconds
    wdt_enable(WDTO_8S);
  #endif

}

FLASH_STRING(stringTimeSync,"[TIME] initial sync");
FLASH_STRING(stringHeartBeatOffline,"[HEARTBEAT] [OFFLINE]");

//  loop - runs over and over again forever:
void loop(){
      
  #if USE_MODULE_ETHERNETCOM == true
      //read Ethernet interface and process incoming commands
      readEthernetToBuffer(commandBufferEthernet,commandBufferPositionEthernet,commandBufferReadyToProcessEthernet);   
  #endif
  
  #if USE_MODULE_SERIALCOM == true
    #if USE_MODULE_SERIALCOM_SOFTWARESERIAL == true
      //read SoftwareSerial interface and process incoming commands
      readSerialToBuffer(softSerial,commandBufferSerial,commandBufferPositionSerial,commandBufferReadyToProcessSerial);
    #else
      //read Serial interface and process incoming commands
      readSerialToBuffer(SERIALCOM,commandBufferSerial,commandBufferPositionSerial,commandBufferReadyToProcessSerial);
    #endif
  #endif
  
  #if USE_MODULE_SERIALMONITOR == true && USE_MODULE_CLIONSERIALMONITOR == true
    //if we are using serial monitor, give it it's own command buffer
    readSerialToBuffer(SERIALMONITOR,commandBufferMonitor,commandBufferPositionMonitor,commandBufferReadyToProcessMonitor);
  #endif
  
  //if we are in configuration mode, prevent running the reset of the script
  //fix-me: we might be able to add something more elegant than relying on the watchdog timer
  if(configInProgress==true){
     return;
  }
  
  //fix-me: should make this so it isn't dependent on wdt_reset
  //we can't do anything else until the time is synced
  if(timeSynced==false && timeSyncInProgress==true){
     #if STOPFORTIMESYNC == true
       return;
     #endif
  }else if(timeSynced==false){
    //go ahead and sync time
    #if USE_MODULE_SERIALMONITOR == true
      stringTimeSync.print(SERIALMONITOR);
      SERIALMONITOR.println();
    #endif
    sendCommand("c:time");
    SERIALMONITOR.println("Attempting initial time sync");
    return;
  }
    
  #if USE_MODULE_WATCHDOG == true
    wdt_reset();
  #endif
  
  //heartbeat determines if the controller is online or offline
  //if the timer has timed out and it is time to send the next heartbeat
  if(((now()-heartBeatSent)>=heartBeatDelay)){
                    
      //send heartbeat
      char buildCommand[32] = "s:hb?config=" ;
      strcat(buildCommand,configStore.configId);
      sendCommand(buildCommand);
      
  }
  
  //check if the controller is offline
  if(heartBeatOnline == true && now()-heartBeatLast>=(heartBeatDelay+heartBeatDelayWait)){

    #if USE_MODULE_SERIALMONITOR == true
      if(heartBeatOnline==true){
        stringHeartBeatOffline.print(SERIALMONITOR);
      }
    #endif
              
    heartBeatOnline = false;
    
  }
  
  //this prevents sensors from being checked on every pass
  if(timeSynced==true && (now()-sensorCheckLast)>sensorCheckDelay){
    
    //go ahead and reset the sensor check style
    sensorCheckLast = now();
    
    //time has been synced and we can continue with controller functions
    time_t timeLocal = getCurrentTime();
    
    //safety turn off
    //safetyOff(timeLocal);
  
    //check sensors
    checkSensors(timeLocal);
      
    //check schedules
    checkSchedules(timeLocal);
  
    //display settings
    //pushDisplay();
    
  }
  
  //serveHttp();
  
}
