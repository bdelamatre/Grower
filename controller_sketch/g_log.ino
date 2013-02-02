/************************************
LOG functions
************************************/

/*void addErrorLog(DateTime logTime, String logSubject, String logMessage){
  
  if(debug){
    Serial.print("[log] [ERROR] ");
    Serial.print(logSubject);
    Serial.print(", logging...");
  }
  
  File loggingFile = SD.open(errorLogFileName, FILE_WRITE);

  if (loggingFile) {
    String dataString = getDateTime(logTime) + ","+logSubject+","+logMessage;
    loggingFile.println(dataString);
    loggingFile.close();
    if(debug){
      Serial.println("ok");
    }
  }  
  else {
    if(debug){
      Serial.println("fail");
    }
  }

}*/

void addScheduleLog(struct Schedule &loggingSchedule, int logTime, String logSubject, String logMessage){

 #if defined DEBUG  
    Serial.print("[log] [SCHEDULE] ");
    Serial.print(loggingSchedule.name);
    Serial.print(", ");
    Serial.print(logSubject);
    Serial.print(" , write=");
  #endif

  File loggingFile = SD.open(scheduleLogFileName, FILE_WRITE);

  if (loggingFile) {
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
  }
  
}

void addSensorLog(struct Sensor &loggingSensor, int logTime, String logSubject, String logMessage){

 #if defined DEBUG  
    Serial.print("[log] [SENSOR] ");
    Serial.print(loggingSensor.name);
    Serial.print(", ");
    Serial.print(loggingSensor.statusValue);
    Serial.print(",");
    Serial.print(loggingSensor.statusValue2);
    Serial.print(", ");
    Serial.print(logSubject);
    Serial.print(", write=");
  #endif
  
  File loggingFile = SD.open(sensorLogFileName, FILE_WRITE);

  if (loggingFile) {
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
    #if defined DEBUG  
      Serial.println("ok");
    #endif
  }  
  else {
    #if defined DEBUG  
      Serial.println("fail");
    #endif
  }
}

void addZoneLog(struct Zone &loggingZone, int logTime, String logSubject, String logMessage){

  #if defined DEBUG  
    Serial.print("[log] [ZONE] ");
    Serial.print(loggingZone.name);
    Serial.print(" ");
    Serial.print(logSubject);
    Serial.print(", write=");
  #endif
  
  File loggingFile = SD.open(zoneLogFileName, FILE_WRITE);

  if (loggingFile) {
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
  }
}

