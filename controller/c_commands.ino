/***

Commands

***/

//FLASH_STRING(stringSyncInProgress,"Time sync in progress");
FLASH_STRING(stringSendingCommand,"[TX] ");

void receiveCommand(HardwareSerial &serial){

    while(serial.available()){
      char inChar = (char)serial.read(); 
      sendCommandBuffer += inChar;
      //done building command
      if (inChar == '>') {
        //if not an empty command
        if(sendCommandBuffer!=">"){
          //send the command
          sendCommand(sendCommandBuffer);
        }
        //than empty the command
        sendCommandBuffer = "";
      }else if (inChar == '<') {
        if(sendCommandBuffer!="<"){
          //empty command, ignore
          //has paramaters
          if(sendCommandBuffer.indexOf("?")>=0){
            executeCommand(sendCommandBuffer.substring(0,sendCommandBuffer.indexOf("?"))+"<"
                          ,sendCommandBuffer.substring(sendCommandBuffer.indexOf("?")+1,sendCommandBuffer.length()-1));
          //doesn't have parameters
          }else{
            executeCommand(sendCommandBuffer,"");
          }
        }
        sendCommandBuffer = "";
      }
    }  

}

void sendCommand(String thisCommand){
  
  
  //these commands need special processing
  if(thisCommand=="config:set-time>"){
    timeSyncInProgress = true;
    #if defined(DEBUGHEARTBEAT)
      stringSendingCommand.print(Serial);
      Serial.println(thisCommand);
    #endif
  }else if(thisCommand=="system:heartbeat>"){
    heartBeatInProgress = true;
    heartBeatSent = millis();
    #if defined(DEBUGTIMESYNC)
      stringSendingCommand.print(Serial);
      Serial.println(thisCommand);
    #endif
  }else{
    stringSendingCommand.print(Serial);
    Serial.println(thisCommand);
  }
  
  //write the command to impee byte-by-byte
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
  
  boolean isHeartbeat = false;
  boolean isTimesync = false;
  boolean isUnrecognized = false;
  
  //set time from unixtime
  if(command=="system:heartbeat<"){
      commandSystemHeartbeat(params.toInt());
      isHeartbeat = true;
  //confirm that a data log was received
  }else if(command=="config:set-time<"){
      commandConfigSetTime(params.toInt());
      isTimesync = true;
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
  }else if(command=="help<"){
      commandHelp();
  }else{
    isUnrecognized = true;
  }
  
  if(isHeartbeat==true){
  
    #if defined(DEBUGHEARTBEAT)
      stringExecuting.print(Serial);
      Serial.print(command);
      stringWith.print(Serial);
      Serial.println(params);
    #endif
    
  }else if(isTimesync==true){
  
    #if defined(DEBUGTIMESYNC)
      stringExecuting.print(Serial);
      Serial.print(command);
      stringWith.print(Serial);
      Serial.println(params);
    #endif
    
  }else if(isUnrecognized==true){
  
    stringUnrecognizedCommand.print(Serial);
    Serial.println(command);
    
  }else{
    
    stringExecuting.print(Serial);
    Serial.print(command);
    stringWith.print(Serial);
    Serial.println(params);
    
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
  
      //#if defined(DEBUGHEARTBEAT)
      if(heartBeatOnline==false){
        Serial.println("[HEARTBEAT] [ONLINE]");
      }
      //#endif
  
    //indicate that the time has been synced and set the datetime
    heartBeatInProgress = false;
    heartBeatLast = millis();
    heartBeatOnline = true;
    
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

void commandHelp(){
  
    Serial.println();
    Serial.println("Commands Available:");
    Serial.println("    config:reset-default   - reset EEPROM");
    Serial.println("    config:save            - save the running config to EEPROM");
    Serial.println("    config:sensor          - configure a sensor");
    Serial.println("    config:schedule        - configure a schedule");
    Serial.println("    config:set-time        - sync device time");
    Serial.println("    config:zone            - configure a zone");
    Serial.println("    data:log-received      - indicate that a data log was received");
    Serial.println("    system:heartbeat       - indicate that a heartbeat was received");
    Serial.println("    system:restart         - reboot the system");
    Serial.println("    test:zones             - runs a cycle that turns zones on and off");
    Serial.println();
   
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
      commaPosition = params.indexOf('&');

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
    
      commaPosition = params.indexOf('&');

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
      commaPosition = params.indexOf('&');

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
        
          int thisPosition;
          String thisParam;
          
           //cycle through this parameter by comma delimited
           while(thisPosition >= 0){
              thisPosition = thisParam.indexOf(',');
        
              if(thisPosition != -1){
                  thisParam = value.substring(0,thisPosition);
                  value = value.substring(thisPosition+1, value.length());
              }else{ 
                 if(params.length() > 0)
                   thisParam = value; 
              }
              
               configStore.schedules[scheduleId].zones[thisPosition] = value.toInt();
              
           }
           
           while(thisPosition < maxZones){
             configStore.schedules[scheduleId].zones[thisPosition] = 0;
             thisPosition++;
           }
        
      }else if(name=="zonesRunType"){
        configStore.schedules[scheduleId].zonesRunType = value.toInt();
      }else if(name=="sensors"){
        
          int thisPosition;
          String thisParam;
          
           //cycle through this parameter by comma delimited
           while(thisPosition >= 0){
              thisPosition = thisParam.indexOf(',');
        
              if(thisPosition != -1){
                  thisParam = value.substring(0,thisPosition);
                  value = value.substring(thisPosition+1, value.length());
              }else{ 
                 if(params.length() > 0)
                   thisParam = value; 
              }
              
               configStore.schedules[scheduleId].sensors[thisPosition] = value.toInt();
              
           }
           
           while(thisPosition < maxSensors){
             configStore.schedules[scheduleId].sensors[thisPosition] = 0;
             thisPosition++;
           }
           
      }else if(name=="timerStartWeekdays"){
        //cycle through weekdays
        for(int i=0;i<7;i++){
           configStore.schedules[scheduleId].timerStartWeekdays[i] = int(value.charAt(i));
        }
      }else if(name=="timerStartHours"){
        //cycle through hours
        for(int i=0;i<24;i++){
           configStore.schedules[scheduleId].timerStartHours[i] = int(value.charAt(i));
        }
      }else if(name=="timerStartMinutes"){
        //cycle through minutes
        for(int i=0;i<60;i++){
           configStore.schedules[scheduleId].timerStartMinutes[i] = int(value.charAt(i));
        }
      }else if(name=="timerStartSeconds"){
        //cycle through seconds
        for(int i=0;i<60;i++){
           configStore.schedules[scheduleId].timerStartSeconds[i] = int(value.charAt(i));
        }
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
      commaPosition = params.indexOf('&');

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
