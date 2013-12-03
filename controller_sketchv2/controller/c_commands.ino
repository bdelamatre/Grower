
/**
Generic handler for sending commands out the Electric Imp
**/

//FLASH_STRING(stringSyncInProgress,"Time sync in progress");
FLASH_STRING(stringSendingCommand,"[TX] ");

void sendCommand(String thisCommand){
  
  stringSendingCommand.print(Serial);
  Serial.println(thisCommand);
  
  //fix-me: probably a better way to handle this
  if(thisCommand=="config:set-time>"){
    timeSyncInProgress = true;
  }
  
  //fix-me: probably a better way to handle this
  if(thisCommand=="system:heartbeat>"){
    heartBeatInProgress = true;
    heartBeatSent = millis();
  }
  
  int commandLength = thisCommand.length();
  int i = 0;
  while(i<commandLength){
    char sendChar = thisCommand.charAt(i);
    Serial1.write(byte(sendChar));
    i++;
  }
  
  #if defined(DEBUG)
    printAvailableMemory();
  #endif
  
}


/**
Generic handler for executing commands received from the Electric Imp
**/

FLASH_STRING(stringExecuting,"[RX] ");
FLASH_STRING(stringWith," ,paramater= ");
FLASH_STRING(stringUnrecognizedCommand,"[ERROR] Unrecognized command: ");

void executeCommand(String command, String params){
  
  stringExecuting.print(Serial);
  Serial.print(command);
  stringWith.print(Serial);
  Serial.println(params);
  
  //set time from unixtime
  if(command=="config:set-time<"){
      commandConfigSetTime(params.toInt());
  //save configuration to EEPROM
  }else if(command=="config:save<"){
      commandConfigSave(params);
  //reset configuration to default (nullify EEPROM)
  }else if(command=="config:reset-default<"){
      commandConfigResetDefault(params);
  //restart the controller
  }else if(command=="system:restart<"){
      commandSystemReset(params);
  //confirm that a data log was received
  }else if(command=="system:heartbeat<"){
      commandSystemHeartbeat(params.toInt());
  //confirm that a data log was received
  }else if(command=="data:log-received<"){
      commandDataLogReceived(params);
  //configure a zone
  }else if(command=="config:zone<"){
      commandConfigZone(params);
  //override a zone on
  }else if(command=="test:zones<"){
      commandTestZones(params);
  }else if(command=="config:sensor<"){
      commandConfigSensor(params);
  }else if(command=="config:schedule<"){
      commandConfigSchedule(params);
  }else{
    stringUnrecognizedCommand.print(Serial);
    Serial.println(command);
  } 
                      
  #if defined(DEBUG)
    printAvailableMemory();
  #endif
  
}


//FLASH_STRING(stringSettingTime,"Setting time from unixtime ");
FLASH_STRING(stringAdjustingRTC,"Adjusting RTC to ");
FLASH_STRING(stringTimeSynced,"Time synced to ");

void commandConfigSave(String logId){
  saveConfig();
  Serial.println("Current configuration saved to EEPROM.");
}

void commandConfigResetDefault(String params){
  Serial.println("Resetting to default.");
  resetDefaultConfig();
}

void(* restart) (void) = 0; //declare reset function @ address 0

void commandSystemReset(String params){
  Serial.println("Restarting system.");
  restart();
}

void commandSystemHeartbeat(int value){
  
    //indicate that the time has been synced and set the datetime
    heartBeatInProgress = false;
    heartBeatLast = millis();
    heartBeatOnline = true;
    Serial.println("[HEARTBEAT] [ONLINE]");
    
}

DateTime commandConfigSetTime(unsigned long int timeunix){
  
    //build DateTime object from unix timestamp
    DateTime thisDateTime = DateTime(timeunix);
  
    //indicate that the time has been synced and set the datetime
    timeSyncInProgress = false;
    timeAtSync = millis()/1000;
    timeSynced = true;
    timeSyncedDateTime = thisDateTime;
  
    //if the RTC is running, lets go ahead and adjust the clock
    if (RTC.isrunning()) {
      stringAdjustingRTC.print(Serial);
      printDateTimeToSerial(thisDateTime);
      Serial.println();
      RTC.adjust(thisDateTime); 
    }
    
    stringTimeSynced.print(Serial);
    printDateTimeToSerial(thisDateTime);
    Serial.println();
    
    return thisDateTime;
}

void commandDataLogReceived(String logId){
  Serial.print("Server received log: ");
  Serial.println(logId);
}

void commandConfigZone(String params){

  //get id
  int zoneId = getIdFromParams(params);
    
  if(zoneId<0 || zoneId>maxZones){
    Serial.print(zoneId);
    Serial.println(" isn't a valid zone id");  
    return;
  }else{
    Serial.print("Updating config for zone ");
    Serial.println(zoneId);
  } 
  
  int commaPosition;
  String param;
  
  //
  while(commaPosition >= 0){
      commaPosition = params.indexOf(',');

      if(commaPosition != -1){
          param = params.substring(0,commaPosition);
          params = params.substring(commaPosition+1, params.length());
      }else{ 
         if(params.length() > 0)
           param = params; 
      }
     
      String name  = param.substring(0,param.indexOf("="));
      String value = param.substring(param.indexOf("=")+1);
      
      Serial.print("Changing ");
      Serial.print(name);
      Serial.print(" to ");
      Serial.println(value);
      
      if(name=="name"){
        value.toCharArray(configStore.zones[zoneId].name,32);
      }else if(name=="type"){
        configStore.zones[zoneId].type = value.toInt();
      }else if(name=="pin"){
        configStore.zones[zoneId].type = value.toInt();
      }else if(name=="safetyOffAfterMinutes"){
        configStore.zones[zoneId].type = value.toInt();
      }else if(name=="overrideOn"){
        configStore.zones[zoneId].type = value.toInt();
      }
      
   }
   
}

void commandTestZones(String params){
  
}

void commandConfigSensor(String params){
  
  //get id
  int sensorId = getIdFromParams(params);
    
  if(sensorId<0 || sensorId>maxZones){
    Serial.print(sensorId);
    Serial.println(" isn't a valid sensor id");  
    return;
  }else{
    Serial.print("Updating config for sensor ");
    Serial.println(sensorId);
  } 
  
  int commaPosition;
  String param;
  
  //
  while(commaPosition >= 0){
      commaPosition = params.indexOf(',');

      if(commaPosition != -1){
          param = params.substring(0,commaPosition);
          params = params.substring(commaPosition+1, params.length());
      }else{ 
         if(params.length() > 0)
           param = params; 
      }
     
      String name  = param.substring(0,param.indexOf("="));
      String value = param.substring(param.indexOf("=")+1);
      
      Serial.print("Changing ");
      Serial.print(name);
      Serial.print(" to ");
      Serial.println(value);
      
      if(name=="name"){
        value.toCharArray(configStore.sensors[sensorId].name,32);
      }else if(name=="type"){
        configStore.sensors[sensorId].type = value.toInt();
      }else if(name=="pin"){
        configStore.sensors[sensorId].pin = value.toInt();
      }else if(name=="pin2"){
        configStore.sensors[sensorId].pin2 = value.toInt();
      }else if(name=="frequencyCheckSeconds"){
        configStore.sensors[sensorId].frequencyCheckSeconds = value.toInt();
      }else if(name=="frequencyLogSeconds"){
        configStore.sensors[sensorId].frequencyLogSeconds = value.toInt();
      }
      
   }   
  
}

void commandConfigSchedule(String params){
  
  //get id
  int scheduleId = getIdFromParams(params);
    
  if(scheduleId<0 || scheduleId>maxZones){
    Serial.print(scheduleId);
    Serial.println(" isn't a valid schedule id");  
    return;
  }else{
    Serial.print("Updating config for schedule ");
    Serial.println(scheduleId);
  } 
  
  int commaPosition;
  String param;
  
  //
   while(commaPosition >= 0){
      commaPosition = params.indexOf(',');

      if(commaPosition != -1){
          param = params.substring(0,commaPosition);
          params = params.substring(commaPosition+1, params.length());
      }else{ 
         if(params.length() > 0)
           param = params; 
      }
     
      String name  = param.substring(0,param.indexOf("="));
      String value = param.substring(param.indexOf("=")+1);
      
      Serial.print("Changing ");
      Serial.print(name);
      Serial.print(" to ");
      Serial.println(value);
      
      if(name=="name"){
        value.toCharArray(configStore.schedules[scheduleId].name,32);
      }else if(name=="type"){
        configStore.schedules[scheduleId].type = value.toInt();
      }else if(name=="zones"){
        configStore.schedules[scheduleId].type = value.toInt();
      }else if(name=="zonesRunType"){
        configStore.schedules[scheduleId].zonesRunType = value.toInt();
      }else if(name=="sensors"){
        configStore.schedules[scheduleId].type = value.toInt();
      }else if(name=="timerStartWeekdays"){
        configStore.schedules[scheduleId].type = value.toInt();
      }else if(name=="timerStartHours"){
        configStore.schedules[scheduleId].type = value.toInt();
      }else if(name=="timerStartMinutes"){
        configStore.schedules[scheduleId].type = value.toInt();
      }else if(name=="valueMin"){
        configStore.schedules[scheduleId].valueMin = value.toInt();
      }else if(name=="valueMax"){
        configStore.schedules[scheduleId].valueMax = value.toInt();
      }
   
   }
  
}

int getIdFromParams(String params){
  
  String value = getParamByName(params,"id");
  int idValue = value.toInt();
  
  return idValue;
  
}

String getParamByName(String params, String paramName){
  
  int commaPosition;
  String param;
  
  //loop through all the parameters
  while(commaPosition >= 0){
      commaPosition = params.indexOf(',');

      if(commaPosition != -1){
          param = params.substring(0,commaPosition);
          params = params.substring(commaPosition+1, params.length());
      }else{ 
         if(params.length() > 0)
           param = params; 
      }
      
      //get the paramater value
      String value = param.substring(param.indexOf("=")+1);
      
      return value;
      
   }
   
   return "";
  
}
