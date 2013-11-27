/************************************
LOG functions
************************************/

FLASH_STRING(stringLogSchedule,"[log] [SCHEDULE] ");
FLASH_STRING(stringLogSensor,"[log] [SENSOR] ");
FLASH_STRING(stringLogZone,"[log] [ZONE] ");
FLASH_STRING(stringLogDelimitter,", ");
FLASH_STRING(stringLogWrite,", write=");


void addScheduleLog(struct Schedule &loggingSchedule, unsigned long logTime, String logSubject, String logMessage){

  String thisLog = "";
  thisLog += "schedule";
  thisLog += ",";
  thisLog += logTime;
  thisLog += ",";
  thisLog += loggingSchedule.name;
  thisLog += ",";
  thisLog += loggingSchedule.isRunning;
  thisLog += ",";
  thisLog += logSubject;
  thisLog += ",";
  thisLog += logMessage;
  
  #if defined DEBUG  
    stringLogZone.print(Serial);
    Serial.println(thisLog);
  #endif
  
  sendCommand("data:log?"+thisLog+">");
  
}


void addSensorLog(struct Sensor &loggingSensor, unsigned long logTime, String logSubject, String logMessage){

  String thisLog = "";
  thisLog += "sensor";
  thisLog += ",";
  thisLog += logTime;
  thisLog += ",";
  thisLog += loggingSensor.name;
  thisLog += ",";
  thisLog += loggingSensor.statusValue;
  thisLog += ",";
  thisLog += loggingSensor.statusValue2;
  thisLog += ",";
  thisLog += logSubject;
  thisLog += ",";
  thisLog += logMessage;
  
  #if defined DEBUG  
    stringLogSensor.print(Serial);
    Serial.println(thisLog);
  #endif
  
  sendCommand("data:log?"+thisLog+">");
  
}

void addZoneLog(struct Zone &loggingZone, unsigned long logTime, String logSubject, String logMessage){

  String thisLog = "";
  thisLog += "zone";
  thisLog += ",";
  thisLog += logTime;
  thisLog += ",";
  thisLog += loggingZone.name;
  thisLog += ",";
  thisLog += loggingZone.isRunning;
  thisLog += ",";
  thisLog += loggingZone.statusRunStarted;
  thisLog += ",";
  thisLog += loggingZone.statusRunBySchedule;
  thisLog += ",";
  thisLog += logSubject;
  thisLog += ",";
  thisLog += logMessage;
  
  #if defined DEBUG  
    stringLogZone.print(Serial);
    Serial.println(thisLog);
  #endif
  
  sendCommand("data:log?"+thisLog+">");
 
}


