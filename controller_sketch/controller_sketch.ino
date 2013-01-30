/**
Required components:
1. RTC - Chronodot (https://www.adafruit.com/products/255)

Supported components:
1. Temperature Sensors:
  a. DHT22 (https://www.adafruit.com/products/393) 
2. Analog Soil Moisture Sensors:
  a. Check Ebay, or build a zinc nail sensor
  

**/

/**
For non-standard libraries copy submodules included under FatRabbitGarden/libraries/ into your Arduino IDE libraries/ folder.
**/
#include <SPI.h> 
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Base64.h> //Base64 by 
#include <WebServer.h> //Webduino by sirleech
#include "avr/pgmspace.h" // new include
#include <Wire.h> 
#include <Chronodot.h> //Chronodot by Stephanie-Maks
#include <EEPROM.h>
#include <SD.h>
#include <DHT.h> //DHT by AdaFruit

/**

To-Do:
> load/save config from EEPROM

**/

boolean debug = false;
boolean debugNoEthernet = false;

//increasing this will increase the EEPROM usage.
//3,3,3 = <1000
//12,12,12 = <4000
const int maxSchedules = 3; 
const int maxZones = 3; 
const int maxSensors = 3;

//RTC
Chronodot RTC;

//ntp variables
const int NTP_PACKET_SIZE= 48;
byte ntpPacketBuffer[ NTP_PACKET_SIZE];

//ethernet tcp variables
int ethernetHasConfig = 0;

//ethernet udp variables
EthernetUDP Udp;
unsigned int udpPort = 8888;

//web server variables
#define PREFIX ""
WebServer webserver(PREFIX, 80);

// SD variables
// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 4;
const int hardwareSelect = 53;
Sd2Card card;
SdVolume volume;
SdFile root;
char* scheduleLogFileName = "schedule.log";
char* sensorLogFileName = "sensor.log";
char* zoneLogFileName = "zone.log";
char* errorLogFileName = "error.log";

struct Schedule{
  String name;
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

struct Zone{
  String name;
  int type; //0=off, 1=5v relay
  int pin;
  int safetyOffAfterMinutes;
  int isRunning; //0=off, 1=on
  int statusRunStarted;
  int statusRunBySchedule;
  int statusSafetyOff;
};

struct Sensor{
  String name;
  int type; //0=off, 1=soil moisture (analog), 2=soil temperature(DS18B20), 3=air temperature (DHT22), 4=light
  int pin;
  int pin2;
  int frequencyCheckSeconds; //0=every loop
  int frequencyLogSeconds; //0=every log
  unsigned long statusValue;
  unsigned long statusValue2;
  int statusLastChecked;
  int statusLastLogged;
};

#define CONFIG_VERSION "1v1"
#define CONFIG_START 32
struct Config{
  char version[4];
  String adminUsername;
  String adminPassword;
  unsigned long utcOffset;
  boolean dhcp;
  IPAddress clientAddress;
  IPAddress clientNetmask;
  IPAddress clientGateway;
  byte hardwareMac[8];
  IPAddress ntpServer;
  Schedule schedules[maxSchedules];
  Zone zones[maxZones];
  Sensor sensors[maxSensors];
} 
config={
  CONFIG_VERSION,
  "admin",
  "admin",
  -6,
  true,
  (0,0,0,0),
  (0, 0, 0, 0),
  (0, 0, 0, 0),
  {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 },
  IPAddress(64, 236, 96, 53) //time.nist.gov
};

// the setup routine runs once when you press reset:
void setup() {

  if(debug){
    // initialize serial communication at 9600 bits per second:
    Serial.begin(9600);
    
    //fancy header
    Serial.println("==========================================");
    Serial.println("|| Fat Rabbit Farm - Garden Controller  ||");
    Serial.println("==========================================");
  }
  
  //initialize configuration
  //loadConfig();
  
  config.clientAddress = IPAddress(192,168,2,99);
  config.clientNetmask = IPAddress(255,255,255,255);
  config.clientGateway = IPAddress(192,168,2,1);
  
  config.zones[0].name="Heater Outlet";
  config.zones[0].type = 1;
  config.zones[0].pin = 5;
  
  config.zones[1].name="Misting System";
  config.zones[1].type = 1;
  config.zones[1].pin = 4;
  config.zones[1].safetyOffAfterMinutes = 120;
  
  config.zones[2].name="Light Outlet";
  config.zones[2].type = 1;
  config.zones[2].pin = 6;
  
  config.sensors[0].name = "Bench Temperature & Humidity";
  config.sensors[0].type = 3;
  config.sensors[0].pin = 2;
  config.sensors[0].frequencyLogSeconds = 300;
  
  config.sensors[1].name="soil temperature";
  config.sensors[1].type = 0;
  config.sensors[1].pin = 3;
  config.sensors[1].frequencyLogSeconds = 300;

  config.sensors[2].name="Soil Moisture";
  config.sensors[2].type = 1;
  config.sensors[2].pin = 1;
  config.sensors[2].frequencyLogSeconds = 300;
  
  //heat schedule - temperature
  config.schedules[0].name="Heaters";
  config.schedules[0].type=3;
  config.schedules[0].zones[0] = 1;
  config.schedules[0].valueMin = 76;
  config.schedules[0].valueMax = 78;
  config.schedules[0].sensors[0] = 1;
  
  //watering schedule - moisture level
  config.schedules[1].name="Misting (Watering)";
  config.schedules[1].type=0;
  config.schedules[1].zones[0] = 2;
  config.schedules[1].valueMin = 500;
  config.schedules[1].valueMax = 600;
  config.schedules[1].sensors[0] = 3;
  
  //light schedule - timer
  config.schedules[2].name="Grow Light";
  config.schedules[2].type=1;
  config.schedules[2].zones[0] = 3;
  config.schedules[2].timerStartWeekdays[0] = 0;
  config.schedules[2].timerStartWeekdays[1] = 1;
  config.schedules[2].timerStartWeekdays[2] = 2;
  config.schedules[2].timerStartWeekdays[3] = 3;
  config.schedules[2].timerStartWeekdays[4] = 4;
  config.schedules[2].timerStartWeekdays[5] = 5;
  config.schedules[2].timerStartWeekdays[6] = 6;
  config.schedules[2].timerStartHours[0] = -1;
  config.schedules[2].timerStartHours[1] = -1;
  config.schedules[2].timerStartHours[2] = -1;
  config.schedules[2].timerStartHours[3] = -1;
  config.schedules[2].timerStartHours[4] = -1;
  config.schedules[2].timerStartHours[5] = -1;
  config.schedules[2].timerStartHours[6] = 6;
  config.schedules[2].timerStartHours[7] = 7;
  config.schedules[2].timerStartHours[8] = 8;
  config.schedules[2].timerStartHours[9] = 9;
  config.schedules[2].timerStartHours[10] = 10;
  config.schedules[2].timerStartHours[11] = 11;
  config.schedules[2].timerStartHours[12] = 12;
  config.schedules[2].timerStartHours[13] = 13;
  config.schedules[2].timerStartHours[14] = 14;
  config.schedules[2].timerStartHours[15] = 15;
  config.schedules[2].timerStartHours[16] = 16;
  config.schedules[2].timerStartHours[17] = 17;
  config.schedules[2].timerStartHours[18] = 18;
  config.schedules[2].timerStartHours[19] = 19;
  config.schedules[2].timerStartHours[20] = 20;
  config.schedules[2].timerStartHours[21] = 21;
  config.schedules[2].timerStartHours[22] = 22;
  config.schedules[2].timerStartHours[23] = 23;
  for(int m=0;m<60;m++){
    config.schedules[2].timerStartMinutes[m] = m;
  }
    
  initRtc(debug);
  initEthernet(debug);
  initSd(debug);
  initNtp(debug);
  initSensors(debug);
  initZones(debug);
  loadConfig(debug);
 
  Serial.println("==========================================");
}

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
  checkSensors(timeLocal,debug);
    
  //check schedules
  checkSchedules(timeLocal,debug);

  //display settings
  //pushDisplay();
  
  webserver.processConnection();
  
}


