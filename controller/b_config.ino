const char blankName[maxNameLength] = "blank                  "; 

const char fieldId[] = "id";
const char fieldType[] = "type";
const char fieldName[] = "name";
const char fieldPin[] = "pin";
const char fieldSafetyOffAfterMinutes[] = "safetyOffAfterMinutes";
const char fieldOverrideOn[] = "overrideOn";
const char fieldPin2[] = "pin2";
const char fieldFrequencyCheckSeconds[] = "frequencyCheckSeconds";
const char fieldFrequencyLogSeconds[] = "frequencyLogSeconds";
const char fieldZones[] = "zones";
const char fieldSensors[] = "sensors";
const char fieldZonesRunType[] = "zonesRunType";
const char fieldTimerStartWeekdays[] = "timerStartWeekdays";
const char fieldTimerStartHours[] = "timerStartHours";
const char fieldTimerStartMinutes[] = "timerStartMinutes";
const char fieldTimerStartSeconds[] = "timerStartSeconds";
const char fieldValueMin[] = "valueMin";
const char fieldValueMax[] = "valueMax";


void getIdsFromValue(int resultArray[],String value,char delimitter=',',boolean shiftValues=false){
  
    int maxLength = sizeof(resultArray);
    int thisPosition;
    String thisParam;
    int lastPosition = 0;
    
     //cycle through this parameter by comma delimited
     while(thisPosition >= 0 && thisPosition < maxLength){
         
        thisPosition = value.indexOf(delimitter);
      
        if(thisPosition != -1){
            thisParam = value.substring(0,thisPosition);
            value = value.substring(thisPosition+1, value.length());
        }else{
           if(value.length() > 0){
             thisParam = value; 
           }else{
             continue;
           }
        }
        
        resultArray[lastPosition] = thisParam.toInt();
        if(shiftValues==true){
          resultArray[lastPosition]++;  
        }
        lastPosition++;
     }
          

}

void loadConfig() {

  #if defined DEBUG
    Serial.print("|| Config (");
    Serial.print(CONFIG_VERSION);
    Serial.print(")...");
  #endif
  
  
  if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2]){
    
    int configSize = sizeof(configStore);
    
    for (unsigned int t=0; t<configSize; t++){
      *((char*)&configStore + t) = EEPROM.read(CONFIG_START + t);
    }
    
    
    #if defined DEBUG
    Serial.print(configSize);
    Serial.print(" bytes...");
      Serial.println("OK");
      Serial.print(" ID=");
      Serial.println(configStore.configId);
    #endif
  }
  
}


FLASH_STRING(stringSaveConfigWriting,"Writing ");
FLASH_STRING(stringSaveConfigWritingCurrent,"Current configuration saved to EEPROM.");
FLASH_STRING(stringSaveConfigWritingDone," done!");

void saveConfig() {
  
    stringSaveConfigWriting.print(Serial);
    Serial.print(sizeof(configStore));
    stringSaveConfigWritingCurrent.print(Serial);
    
    //EEPROM.writeBlock(CONFIG_START, configStore);
    for (unsigned int t=0; t<sizeof(configStore); t++)
        EEPROM.write(CONFIG_START + t, *((char*)&configStore + t));

    stringSaveConfigWritingDone.print(Serial);
    Serial.println();
    
    configInProgress=false;
    Serial.println("config mode ended");
}

void resetDefaultConfig(){
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 4096; i++)
    EEPROM.write(i, 0);
}


FLASH_STRING(stringConfigSaved,"Current configuration saved to EEPROM.");

void commandConfigSave(String logId){
  saveConfig();
  stringConfigSaved.print(Serial);
  Serial.println();
}

FLASH_STRING(stringConfigSaveAsIdChanged,"Configuration ID changed from ");
FLASH_STRING(stringConfigSaveAsIdTo," to ");

void commandConfigSaveAsId(String params){
  
  unsigned long id = getIdFromParams(params);
  
  stringConfigSaveAsIdChanged.print(Serial);
  Serial.print(configStore.configId);
  stringConfigSaveAsIdTo.print(Serial);
  
  configStore.configId = id;
  
  Serial.println(id);
  
  configInProgress = true;
  Serial.println("config mode started");
}


FLASH_STRING(stringResettingDefault,"Resetting to default.");

void commandConfigResetDefault(String params){
  stringResettingDefault.print(Serial);
  Serial.println();
  resetDefaultConfig();
}

//FLASH_STRING(stringSettingTime,"Setting time from unixtime ");
FLASH_STRING(stringAdjustingRTC,"Adjusting RTC to ");
FLASH_STRING(stringTimeSynced,"Time synced to ");

DateTime commandConfigSetTime(unsigned long int timeunix){
  
    //build DateTime object from unix timestamp
    DateTime thisDateTime = DateTime(timeunix);
  
    //indicate that the time has been synced and set the datetime
    timeSyncInProgress = false;
    timeAtSync = millis()/1000;
    timeSynced = true;
    timeSyncedDateTime = thisDateTime;
  
    //if the RTC is running, lets go ahead and adjust the clock
    /*if (RTC.isrunning()) {
      stringAdjustingRTC.print(Serial);
      printDateTimeToSerial(thisDateTime);
      Serial.println();
      RTC.adjust(thisDateTime); 
    }*/
    
    stringTimeSynced.print(Serial);
    printDateTimeToSerial(thisDateTime);
    Serial.println();
    
    return thisDateTime;
}

FLASH_STRING(stringInvalidId," isn't a valid id ");
FLASH_STRING(stringUpdatingId,"Updating config for id ");
FLASH_STRING(stringResettingId,"Resetting config for id ");
FLASH_STRING(stringChanging,"Changing ");
FLASH_STRING(stringChangingTo," to ");

void commandConfigZone(String params){

  //get id
  //int zoneId = getIdFromParams(params);
  unsigned long zoneId = getIdFromParams(params);

  if(zoneId<0 || zoneId>maxZones){
    #if defined(DEBUGCONFIG)    
      Serial.print(zoneId);
      stringInvalidId.print(Serial);
      Serial.println();
    #endif
    return;
  }else{
    #if defined(DEBUGCONFIG)    
      stringUpdatingId.print(Serial);
      Serial.println(zoneId);
    #endif;
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
     
      String nameString  = param.substring(0,param.indexOf("="));
      
      char name[maxNameLength];
      nameString.toCharArray(name,maxNameLength);
      
      String value = param.substring(param.indexOf("=")+1);
      
      #if defined(DEBUGCONFIG)    
        Serial.print("Changing ");
        Serial.print(name);
        Serial.print(" to ");
        Serial.println(value);
      #endif
      
      if(strcmp(name,fieldName)==0){
        value.toCharArray(configStore.zones[zoneId].name,32);
      }else if(strcmp(name,fieldType)==0){
        configStore.zones[zoneId].type = value.toInt();
      }else if(strcmp(name,fieldPin)==0){
        configStore.zones[zoneId].pin = value.toInt();
      }else if(strcmp(name,fieldSafetyOffAfterMinutes)==0){
        configStore.zones[zoneId].safetyOffAfterMinutes = value.toInt();
      }else if(strcmp(name,fieldOverrideOn)==0){
        configStore.zones[zoneId].overrideOn = value.toInt();
      }
      
   }
   
}

void commandConfigZoneReset(String params){

  params = getParamByName(params,"id");
  
  int zoneIds[maxZones];
  
  for(int i=0;i<maxZones;i++){
    zoneIds[i] = 0;
  }
  
  getIdsFromValue(zoneIds,params);
  
  for(int i=0;i<maxZones;i++){
    
   if(zoneIds[i]>0 && zoneIds[i]<maxZones){
      String tmpName = "";
      tmpName.toCharArray(configStore.zones[zoneIds[i]].name,maxNameLength);
      //strlcpy(configStore.zones[zoneIds[i]].name,blankName,maxNameLength);
      configStore.zones[zoneIds[i]].type = 0;
      configStore.zones[zoneIds[i]].pin = -1;
      configStore.zones[zoneIds[i]].safetyOffAfterMinutes = 0;
      configStore.zones[zoneIds[i]].overrideOn = -1;
    }
    
  }
    
}


void commandConfigSensor(String params){
  
  //get id
  //int sensorId = getIdFromParams(params);
  unsigned long sensorId = getIdFromParams(params);
    
  if(sensorId<0 || sensorId>maxSensors){
    #if defined(DEBUGCONFIG)    
      Serial.print(sensorId);
      stringInvalidId.print(Serial);
      Serial.println();
    #endif
    return;
  }else{
    #if defined(DEBUGCONFIG)    
      stringUpdatingId.print(Serial);
      Serial.println(sensorId);
    #endif
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
     
      String nameString  = param.substring(0,param.indexOf("="));
      
      char name[maxNameLength];
      nameString.toCharArray(name,maxNameLength);      String value = param.substring(param.indexOf("=")+1);
      
      #if defined(DEBUGCONFIG)    
        stringChanging.print(Serial);
        Serial.print(name);
        stringChangingTo.print(Serial);
        Serial.println(value);
      #endif
      
      if(strcmp(name,fieldName)==0){
        value.toCharArray(configStore.sensors[sensorId].name,32);
      }else if(strcmp(name,fieldType)==0){
        configStore.sensors[sensorId].type = value.toInt();
        Serial.println(configStore.sensors[sensorId].type);
      }else if(strcmp(name,fieldPin)==0){
        configStore.sensors[sensorId].pin = value.toInt();
      }else if(strcmp(name,fieldPin2)==0){
        configStore.sensors[sensorId].pin2 = value.toInt();
      }else if(strcmp(name,fieldFrequencyCheckSeconds)==0){
        configStore.sensors[sensorId].frequencyCheckSeconds = value.toInt();
      }else if(strcmp(name,fieldFrequencyLogSeconds)==0){
        configStore.sensors[sensorId].frequencyLogSeconds = value.toInt();
      }
      
   }   
  
}

void commandConfigSensorReset(String params){
  
  params = getParamByName(params,"id");
  
  int sensorIds[maxSensors];
  
  for(int i=0;i<maxSensors;i++){
    sensorIds[i] = 0;
  }
  
  getIdsFromValue(sensorIds,params);
  
  for(int i=0;i<maxSensors;i++){
    
    if(sensorIds[i]>0 && sensorIds[i]<maxSensors){
      String tmpName = "";
      tmpName.toCharArray(configStore.sensors[sensorIds[i]].name,maxNameLength);
      //strlcpy(configStore.sensors[sensorIds[i]].name,blankName,maxNameLength);
      configStore.sensors[sensorIds[i]].type = 0;
      configStore.sensors[sensorIds[i]].pin = -1;
      configStore.sensors[sensorIds[i]].pin2 = -1;
      configStore.sensors[sensorIds[i]].frequencyCheckSeconds = 99;
      configStore.sensors[sensorIds[i]].frequencyLogSeconds = 99;
    }
    
  }
      
}


void commandConfigSchedule(String params){
  
  //get id
  unsigned long scheduleId = getIdFromParams(params);
    
  if(scheduleId<0 || scheduleId>maxZones){
    #if defined(DEBUGCONFIG)    
      Serial.print(scheduleId);
      stringInvalidId.print(Serial);
      Serial.println();
    #endif
    return;
  }else{
    #if defined(DEBUGCONFIG)    
      stringUpdatingId.print(Serial);
      Serial.println(scheduleId);
    #endif
  } 
  
  int commaPosition;
  String param;
  
  while(commaPosition >= 0){
     
      //get position of first seperator
      commaPosition = params.indexOf('&');

      if(commaPosition != -1){
          param = params.substring(0,commaPosition);
          params = params.substring(commaPosition+1, params.length());
      }else{ 
         //if(params.length() > 0){
           param = params; 
         //}
      }
     
      String nameString  = param.substring(0,param.indexOf("="));
      
      char name[maxNameLength];
      nameString.toCharArray(name,maxNameLength);      String value = param.substring(param.indexOf("=")+1);
     
      #if defined(DEBUGCONFIG)    
        stringChanging.print(Serial);
        Serial.print(name);
        stringChangingTo.print(Serial);
        Serial.println(value);
      #endif
      
      if(strcmp(name,fieldName)==0){
        value.toCharArray(configStore.schedules[scheduleId].name,32);
      }else if(strcmp(name,fieldType)==0){
        configStore.schedules[scheduleId].type = value.toInt();
      }else if(strcmp(name,fieldZones)==0){
        
          int zoneIds[maxZones];
          
          for(int i=0;i<maxZones;i++){
            zoneIds[i] = 0;
          }
          
          getIdsFromValue(zoneIds,value,',',true);
          
          for(int i=0;i<maxZones;i++){
            
             #if defined(DEBUGCONFIG)    
               Serial.print(" set zone position ");
               Serial.print(i);
             #endif
              
            if(zoneIds[i]>0){
                configStore.schedules[scheduleId].zones[i] = zoneIds[i];
                
                #if defined(DEBUGCONFIG)    
                  Serial.print(" to zone id ");
                  Serial.print(configStore.schedules[scheduleId].zones[i]);
                #endif
            }else{
               configStore.schedules[scheduleId].zones[i] = -1;
               
              #if defined(DEBUGCONFIG)    
                 Serial.print(" to null ");
               #endif
            }
            
            #if defined(DEBUGCONFIG)    
              Serial.println();
            #endif
            
          }
        
      }else if(strcmp(name,fieldZonesRunType)==0){
        configStore.schedules[scheduleId].zonesRunType = value.toInt();
      }else if(strcmp(name,fieldSensors)==0){
                
          int sensorIds[maxSensors];
          
          for(int i=0;i<maxSensors;i++){
            sensorIds[i] = 0;
          }
          
          getIdsFromValue(sensorIds,value,',',true);
          
          for(int i=0;i<maxSensors;i++){
            
             #if defined(DEBUGCONFIG)    
               Serial.print(" set sensor position ");
               Serial.print(i);
             #endif
             
            if(sensorIds[i]>0){
                configStore.schedules[scheduleId].sensors[i] = sensorIds[i];
                
                #if defined(DEBUGCONFIG)    
                  Serial.print(" set to sensor id ");
                  Serial.print(configStore.schedules[scheduleId].sensors[i]);
                #endif
            }else{
               configStore.schedules[scheduleId].sensors[i] = -1;
               
              #if defined(DEBUGCONFIG)    
                 Serial.print(" to null ");
               #endif
            }
            
            #if defined(DEBUGCONFIG)    
              Serial.println();
            #endif
            
          }
           
      }else if(strcmp(name,fieldTimerStartWeekdays)==0){
        //cycle through weekdays
        for(int i=0;i<7;i++){
           configStore.schedules[scheduleId].timerStartWeekdays[i] = (int)value.charAt(i);
        }
      }else if(strcmp(name,fieldTimerStartHours)==0){
        //cycle through hours
        for(int i=0;i<24;i++){
           configStore.schedules[scheduleId].timerStartHours[i] = (int)value.charAt(i);
        }
      }else if(strcmp(name,fieldTimerStartMinutes)==0){
        //cycle through minutes
        for(int i=0;i<60;i++){
           configStore.schedules[scheduleId].timerStartMinutes[i] = (int)value.charAt(i);
        }
      }else if(strcmp(name,fieldTimerStartSeconds)==0){
        //cycle through seconds
        for(int i=0;i<60;i++){
           configStore.schedules[scheduleId].timerStartSeconds[i] = (int)value.charAt(i);
        }
      }else if(strcmp(name,fieldValueMin)==0){
        configStore.schedules[scheduleId].valueMin = value.toInt();
      }else if(strcmp(name,fieldValueMax)==0){ 
        configStore.schedules[scheduleId].valueMax = value.toInt();
      }
   
   }
  
}

void commandConfigScheduleReset(String params){
  
  String value = getParamByName(params,"id");
  
  int scheduleIds[maxSchedules];
  
  for(int i=0;i<maxSchedules;i++){
    scheduleIds[i] = 0;
  }
  
  getIdsFromValue(scheduleIds,value);
  
  for(int i=0;i<maxSchedules;i++){
    
    if(scheduleIds[i]>0 && scheduleIds[i]<maxSchedules){
        String tmpName = "";
        tmpName.toCharArray(configStore.schedules[scheduleIds[i]].name,maxNameLength);
        configStore.schedules[scheduleIds[i]].type = 0;
        int thisPosition = 0;
        while(thisPosition < maxZones){
          configStore.schedules[scheduleIds[i]].zones[thisPosition] = 0;
          thisPosition++;
        }
        thisPosition = 0;
        configStore.schedules[scheduleIds[i]].zonesRunType = 0;
        while(thisPosition < maxSensors){
           configStore.schedules[scheduleIds[i]].sensors[thisPosition] = 0;
           thisPosition++;
        }
        for(int i=0;i<7;i++){
           configStore.schedules[scheduleIds[i]].timerStartWeekdays[i] = 0;
        }
        for(int i=0;i<24;i++){
           configStore.schedules[scheduleIds[i]].timerStartHours[i] = 0;
        }
        for(int i=0;i<60;i++){
           configStore.schedules[scheduleIds[i]].timerStartMinutes[i] = 0;
        }
        for(int i=0;i<60;i++){
           configStore.schedules[scheduleIds[i]].timerStartSeconds[i] = 0;
        }
        configStore.schedules[scheduleIds[i]].valueMin = 0;
        configStore.schedules[scheduleIds[i]].valueMax = 0;
    }
    
  }
  
  
}


