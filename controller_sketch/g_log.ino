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

void addScheduleLog(struct Schedule &loggingSchedule, DateTime logTime, String logSubject, String logMessage){

  if(debug){
    Serial.print("[log] [SCHEDULE] ");
    //Serial.print(loggingSchedule.name);
    Serial.print(", ");
    Serial.print(logSubject);
    Serial.print(" , write=");
  }

  File loggingFile = SD.open(scheduleLogFileName, FILE_WRITE);

  if (loggingFile) {
    //String dataString = getDateTime(logTime)+","+loggingSchedule.name+","+loggingSchedule.statusRun+","+logSubject+","+logMessage;
    String dataString = getDateTime(logTime)+","+loggingSchedule.isRunning+","+logSubject+","+logMessage;
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
  
}

void addSensorLog(struct Sensor &loggingSensor, DateTime logTime, String logSubject, String logMessage){

  if(debug){
    Serial.print("[log] [SENSOR] ");
    //Serial.print(loggingSensor.name);
    Serial.print(", ");
    Serial.print(loggingSensor.statusValue);
    Serial.print(",");
    Serial.print(loggingSensor.statusValue2);
    Serial.print(", ");
    Serial.print(logSubject);
    Serial.print(", write=");
  }
  
  File loggingFile = SD.open(sensorLogFileName, FILE_WRITE);

  if (loggingFile) {
    String dataString = getDateTime(logTime)+","+loggingSensor.statusValue+","+loggingSensor.statusValue2+","+logSubject+","+logMessage;
    //String dataString = getDateTime(logTime)+","+loggingSensor.name+","+loggingSensor.statusValue+","+loggingSensor.statusValue2+","+logSubject+","+logMessage;
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
}

void addZoneLog(struct Zone &loggingZone, DateTime logTime, String logSubject, String logMessage){

  if(debug){
    Serial.print("[log] [ZONE] ");
    //Serial.print(loggingZone.name);
    Serial.print(" ");
    Serial.print(logSubject);
    Serial.print(", write=");
  }
  
  File loggingFile = SD.open(zoneLogFileName, FILE_WRITE);

  if (loggingFile) {
    String dataString = getDateTime(logTime)+","+loggingZone.isRunning+","+loggingZone.statusRunStarted+","+loggingZone.statusRunBySchedule+","+logSubject+","+logMessage;
    //String dataString = getDateTime(logTime)+","+loggingZone.name+","+loggingZone.statusRun+","+loggingZone.statusRunStarted+","+loggingZone.statusRunType+","+loggingZone.statusRunBySchedule+","+logSubject+","+logMessage;
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
}

