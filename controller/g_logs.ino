/************************************
LOG functions
************************************/

#if defined(USESERIALMONITOR)
FLASH_STRING(stringLogSchedule,"[LOG] [SCHEDULE] ");
FLASH_STRING(stringLogSensor,"[LOG] [SENSOR] ");
FLASH_STRING(stringLogZone,"[LOG] [ZONE] ");
FLASH_STRING(stringLogDelimitter,", ");
FLASH_STRING(stringLogWrite,", write=");
#endif

void addSensorLog(struct Sensor &loggingSensor, unsigned long logTime, String logSubject, String logMessage){

  String thisLog = "";
  thisLog += "d:log?";
  thisLog += "type=sensor";
  thisLog += "&time=";
  thisLog += logTime;
  thisLog += "&name=";
  thisLog += loggingSensor.name;
  thisLog += "&value=";
  thisLog += loggingSensor.statusValue;
  thisLog += "&value2=";
  thisLog += loggingSensor.statusValue2;
  thisLog += "&subject=";
  thisLog += logSubject;
  thisLog += "&message=";
  thisLog += logMessage;
  
  #if defined(USESERIALMONITOR) 
    stringLogSensor.print(Serial);
    Serial.println(thisLog);
  #endif
  
  char buffer[maxBufferSize];
  thisLog.toCharArray(buffer,maxBufferSize);
  
  sendCommand(buffer);
    
}

#if !defined(SENSORONLY)
  
void addScheduleLog(struct Schedule &loggingSchedule, unsigned long logTime, String logSubject, String logMessage){

  String thisLog = "";
  thisLog += "d:log?";
  thisLog += "type=schedule";
  thisLog += "&time=";
  thisLog += logTime;
  thisLog += "&name=";
  thisLog += loggingSchedule.name;
  thisLog += "&isRunning=";
  thisLog += loggingSchedule.isRunning;
  thisLog += "&subject=";
  thisLog += logSubject;
  thisLog += "&message=";
  thisLog += logMessage;
  
  #if defined(USESERIALMONITOR) 
    stringLogZone.print(Serial);
    Serial.println(thisLog);
  #endif
  
  char buffer[maxBufferSize];
  thisLog.toCharArray(buffer,maxBufferSize);
  
  sendCommand(buffer);
  
}

void addZoneLog(struct Zone &loggingZone, unsigned long logTime, String logSubject, String logMessage){

  String thisLog = "";
  thisLog += "d:log?";
  thisLog += "type=zone";
  thisLog += "&time=";
  thisLog += logTime;
  thisLog += "&name=";
  thisLog += loggingZone.name;
  thisLog += "&isRunning=";
  thisLog += loggingZone.isRunning;
  thisLog += "&started=";
  thisLog += loggingZone.statusRunStarted;
  thisLog += "&runBySchedule=";
  thisLog += loggingZone.statusRunBySchedule;
  thisLog += "&subject=";
  thisLog += logSubject;
  thisLog += "&message=";
  thisLog += logMessage;
  
  #if defined(USESERIALMONITOR) 
    stringLogZone.print(Serial);
    Serial.println(thisLog);
  #endif
  
  char buffer[maxBufferSize];
  thisLog.toCharArray(buffer,maxBufferSize);
  
  sendCommand(buffer);
  
}

#endif


