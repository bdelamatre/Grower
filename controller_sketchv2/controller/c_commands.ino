
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
FLASH_STRING(stringUnrecognizedTime,"Unrecognized command: ");

void executeCommand(String command, String params){
  
  stringExecuting.print(Serial);
  Serial.print(command);
  stringWith.print(Serial);
  Serial.println(params);
  
  if(command=="config:set-time<"){
      //convert the param to a unixtime int and call command
      commandConfigSetTime(params.toInt());
  }else if(command=="config:save<"){
      //convert the param to a unixtime int and call command
      commandConfigSave(params);
  }else if(command=="config:reset-default<"){
      //convert the param to a unixtime int and call command
      commandConfigResetDefault(params);
  }else if(command=="system:restart<"){
      //convert the param to a unixtime int and call command
      commandSystemReset(params);
  }else if(command=="data:log-received<"){
      //convert the param to a unixtime int and call command
      commandDataLogReceived(params);
  }else if(command=="config:zone<"){
      //convert the param to a unixtime int and call command
      commandConfigZone(params);
  }else{
    stringUnrecognizedTime.print(Serial);
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
    
  if(zoneId<1 || zoneId>maxZones){
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
      
      if(name=="name"){
        Serial.print("Changing name from ");
        Serial.print(configStore.zones[zoneId].name);
        Serial.print(" to ");
        value.toCharArray(configStore.zones[zoneId].name,value.length()+1);
        Serial.println(configStore.zones[zoneId].name);
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
  
  /*struct Zone{
    char* name;
    int type; //0=off, 1=5v relay
    int pin;
    int safetyOffAfterMinutes;
    int isRunning; //0=off, 1=on
    unsigned long statusRunStarted;
    int statusRunBySchedule;
    int statusSafetyOff;
  };*/

