/**

For more information, check readme.txt

File structure:
controller_sketch  = variable definition, setup() and loop()
a_web_pages        = web page functions used by Webduino. Must be included before Webduino init() or throws errors
c_init             = contains *init() functions used in setup()
d_sensors          = checkSensors() and related functions. Support for additional sensor types may be added here.
e_zones            = functions for turning zones on/off
f_schedules        = checkSchedules() and related functions. Support for additional schedule types may be added here.
g_log              = functions used for logging to SD
h_config           = functions for read/write EEPROM config
i_display          = various functions used for fomatting text
z_ntp              = ntp functions

Required components:
1. RTC - Chronodot (https://www.adafruit.com/products/255)

Supported components:
1. Temperature Sensors:
  a. DHT22 (https://www.adafruit.com/products/393) 
2. Analog Soil Moisture Sensors:
  a. Check Ebay, or build a zinc nail sensor
  
For non-standard libraries copy submodules included under FatRabbitGarden/libraries/ into your Arduino IDE libraries/ folder.
**/
#include <SPI.h> 
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Base64.h> //Base64 by 
#include <WebServer.h> //Webduino by sirleech
#include <Wire.h> 
#include <Chronodot.h> //Chronodot by Stephanie-Maks
#include <EEPROM.h>
#include <SD.h>
#include <DHT.h> //DHT by AdaFruit

//#define DEBUG

//rtc variable
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
const char* scheduleLogFileName = "schedule.log";
const char* sensorLogFileName = "sensor.log";
const char* zoneLogFileName = "zone.log";
const char* errorLogFileName = "error.log";

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

/*
WARNING, increasing these will allow you to configure more 
schedules, zones and sensors, but increase the RAM and EEPROM 
usage. Be careful if increases these that you stay within your
systems limits, or stability issue will occur.
*/
const int maxSchedules = 3; 
const int maxZones = 3; 
const int maxSensors = 3;

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
  unsigned long statusRunStarted;
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
  unsigned long statusLastChecked;
  unsigned long statusLastLogged;
};

#define CONFIG_VERSION "1v1"
#define CONFIG_START 32
struct Config{
  char version[4];
  String adminUsername;
  String adminPassword;
  unsigned long utcOffset;
  boolean hasEthernet;
  boolean dhcp;
  IPAddress clientAddress;
  IPAddress clientNetmask;
  IPAddress clientGateway;
  byte hardwareMac[8];
  boolean ntp;
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
  true,
  (0,0,0,0),
  (0, 0, 0, 0),
  (0, 0, 0, 0),
  {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 },
  false,
  IPAddress(64, 236, 96, 53) //time.nist.gov
};

P(startOpen) = "<!DOCTYPE HTML><html><head><meta http-equiv=\"refresh\" content=\"300\"/><title>Status - Fat Rabbit Garden</title></head>";
P(startOpenAdmin) = "<!DOCTYPE HTML><html><head><title>Configuration - Fat Rabbit Garden</title></head>";
P(style) = "<style> .mainSection{} section{width:400px;margin:auto;} article{margin:5px;padding:5px;} #general article{border:2px solid #000000;} #schedules article{border:2px solid #000000;} #sensors article{border:2px solid #000000;} #zones article{border:2px solid #000000;}</style>";
P(startClose) = "<body style=\"text-align:center;\">";
P(header) = "<header><h1>Garden Controller</h1><a href=\"/config\">Configuration</a></header>";
P(headerAdmin) = "<header><h1>Garden Controller</h1></header>";
P(generalSectionOpen) = "<h2>General</h2><section id=\"general\" class=\"mainSection\">Date/Time: ";
P(sensorSectionOpen) = "<h2>Sensors</h2><section id=\"sensors\" class=\"mainSection\">";
P(sensorSectionClose) = "</section><a href=\"/sensor-log.csv\" download=\"sensor-log.csv\">Donwload Sensor Log</a>";
P(zoneSectionOpen) = "<h2>Zones</h2><section id=\"zones\" class=\"mainSection\">";
P(zoneSectionClose) = "</section><a href=\"/zone-log.csv\" download=\"zone-log.csv\">Download Zone Log</a>";
P(scheduleSectionOpen) = "<h2>Schedules</h2><section id=\"schedules\" class=\"mainSection\">";
P(sectionEnd) = "</section>";
P(articleOpen) = "<article>";
P(articleClose) = "</article>";
      
// the setup routine runs once when you press reset:
void setup() {

  Serial.begin(9600);
    
  #if defined DEBUG
    Serial.println(memoryFree());
    Serial.println("==========================================");
    Serial.println("|| Fat Rabbit Farm - Garden Controller  ||");
    Serial.println("==========================================");
  #endif
  
  //initialize configuration
  //loadConfig();
  
  config.adminUsername = "admin";
  config.adminPassword = "fatrabbit";
  
  config.dhcp = false;
  config.clientAddress = IPAddress(192,168,2,99);
  config.clientNetmask = IPAddress(255,255,255,0);
  config.clientGateway = IPAddress(192,168,2,1);
  config.ntp=false;
  
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
  config.sensors[0].frequencyCheckSeconds = 5;
  config.sensors[0].frequencyLogSeconds = 300;
  
  config.sensors[1].name="Soil Temperature";
  config.sensors[1].type = 0;
  config.sensors[1].pin = 3;
  config.sensors[1].frequencyCheckSeconds = 5;
  config.sensors[1].frequencyLogSeconds = 300;

  config.sensors[2].name="Soil Moisture";
  config.sensors[2].type = 1;
  config.sensors[2].pin = 1;
  config.sensors[2].frequencyCheckSeconds = 5;
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
     
  initRtc();
  initEthernet();
  initSd();
  initNtp(config.ntp);
  initSensors();
  initZones();
    
  #if defined DEBUG 
    Serial.println(memoryFree());
    Serial.println("==========================================");
  #endif
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
  checkSensors(timeLocal);
    
  //check schedules
  checkSchedules(timeLocal);

  //display settings
  //pushDisplay();
  
  webserver.processConnection();
  
}


