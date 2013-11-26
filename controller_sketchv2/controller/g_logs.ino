/************************************
LOG functions
************************************/

FLASH_STRING(stringLogSchedule,"[log] [SCHEDULE] ");
FLASH_STRING(stringLogSensor,"[log] [SENSOR] ");
FLASH_STRING(stringLogZone,"[log] [ZONE] ");
FLASH_STRING(stringLogDelimitter,", ");
FLASH_STRING(stringLogWrite,", write=");


void addScheduleLog(struct Schedule &loggingSchedule, unsigned long logTime, String logSubject, String logMessage){

 #if defined DEBUG  
    stringLogSchedule.print(Serial);
    Serial.print(loggingSchedule.name);
    stringLogDelimitter.print(Serial);
    Serial.print(logSubject);
    stringLogWrite.print(Serial);
  #endif

  File loggingFile = SD.open("schedule.log", FILE_WRITE);

  /*if (loggingFile) {
    loggingFile.print(logTime);
    loggingFile.print(",");
    loggingFile.print(+loggingSchedule.isRunning);
    loggingFile.print(",");
    loggingFile.print(logSubject);
    loggingFile.print(",");
    loggingFile.println(logMessage);
    loggingFile.close();
    #if defined DEBUG  
      Serial.println("ok");
    #endif
  }else {
    #if defined DEBUG  
      Serial.println("fail");
    #endif
  }*/
  
}

void addSensorLog(struct Sensor &loggingSensor, unsigned long logTime, String logSubject, String logMessage){

  #if defined DEBUG  
    stringLogSensor.print(Serial);
    Serial.print(loggingSensor.name);
    stringLogDelimitter.print(Serial);
    Serial.print(loggingSensor.statusValue);
    stringLogDelimitter.print(Serial);
    Serial.print(loggingSensor.statusValue2);
    stringLogDelimitter.print(Serial);
    Serial.print(logSubject);
    stringLogWrite.print(Serial);
  #endif
  
  File loggingFile = SD.open("sensor.log", FILE_WRITE);

  /*if (loggingFile) {
    loggingFile.print(logTime);
    loggingFile.print(",");
    loggingFile.print(loggingSensor.name);
    loggingFile.print(",");
    loggingFile.print(loggingSensor.statusValue);
    loggingFile.print(",");
    loggingFile.print(loggingSensor.statusValue2);
    loggingFile.print(",");
    loggingFile.print(logSubject);
    loggingFile.print(",");
    loggingFile.println(logMessage);
    loggingFile.close();
    #if defined DEBUG  
      Serial.println("ok");
    #endif
  }  
  else {
    #if defined DEBUG  
      Serial.println("fail");
    #endif
  }*/
}

void addZoneLog(struct Zone &loggingZone, unsigned long logTime, String logSubject, String logMessage){

  #if defined DEBUG  
    stringLogZone.print(Serial);
    Serial.print(loggingZone.name);
    stringLogDelimitter.print(Serial);
    Serial.print(logSubject);
    stringLogWrite.print(Serial);
  #endif
  
  File loggingFile = SD.open("zone.log", FILE_WRITE);

  /*if (loggingFile) {
    loggingFile.print(logTime);
    loggingFile.print(",");
    loggingFile.print(loggingZone.isRunning);
    loggingFile.print(",");
    loggingFile.print(loggingZone.statusRunStarted);
    loggingFile.print(",");
    loggingFile.print(loggingZone.statusRunBySchedule);
    loggingFile.print(",");
    loggingFile.print(logSubject);
    loggingFile.print(",");
    loggingFile.println(logMessage);
    loggingFile.close();
    #if defined DEBUG  
      Serial.println("ok");
    #endif
  }
  else {
    #if defined DEBUG  
      Serial.println("fail");
    #endif
  }*/
}


