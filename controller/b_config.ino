
const char blankName[maxNameLength] = "     "; 

const char fieldId[] = "id";
const char fieldType[] = "type";
const char fieldName[] = "name";
const char fieldPin[] = "pin";
const char fieldSafetyOffAfterMinutes[] = "sftyOff";
const char fieldOverrideOn[] = "ovrdeOn";
const char fieldPin2[] = "pin2";
const char fieldFrequencyCheckSeconds[] = "frqCheck";
const char fieldFrequencyLogSeconds[] = "frqLog";
const char fieldZones[] = "z";
const char fieldSensors[] = "s";
const char fieldZonesRunType[] = "zRun";
const char fieldTimerStartWeekdays[] = "tw";
const char fieldTimerStartHours[] = "th";
const char fieldTimerStartMinutes[] = "tm";
const char fieldTimerStartSeconds[] = "ts";
const char fieldValueMin[] = "min";
const char fieldValueMax[] = "max";

String getDeviceId(){
  return configStore.deviceId;
}

void getIdsFromValue(int resultArray[],int arrayLength, String value,char delimitter=',',boolean shiftValues=false){
  
    int thisPosition;
    String thisParam;
    int lastPosition = 0;
        
     //cycle through this parameter by comma delimited
     while(thisPosition >= 0 && lastPosition < arrayLength){
         
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
          //SERIALMONITOR.println("shifting up one");
          resultArray[lastPosition]++;  
        }
        lastPosition++;
     }

}


FLASH_STRING(stringConfig,"|| Config (");
FLASH_STRING(stringDot,")...");
FLASH_STRING(stringBytes,"bytes...");
FLASH_STRING(stringOk2,"OK");
FLASH_STRING(stringIdEquals," ID=");
FLASH_STRING(stringDefault," DEFAULT");

void loadConfig(){ 
    
  #if DEBUG == true
    stringConfig.print(SERIALMONITOR);
    SERIALMONITOR.print(CONFIG_VERSION);
    stringDot.print(SERIALMONITOR);
  #endif
  
  if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2]){
    
    int configSize = sizeof(configStore);
    
    for (unsigned int t=0; t<configSize; t++){
      *((char*)&configStore + t) = EEPROM.read(CONFIG_START + t);
    }
    
    #if DEBUG == true
      SERIALMONITOR.print(configSize);
      stringBytes.print(SERIALMONITOR);
      stringOk2.print(SERIALMONITOR);
      stringIdEquals.print(SERIALMONITOR);
      SERIALMONITOR.println(configStore.configId);
    #endif
  }else{
    
    #if DEBUG == true
      stringDefault.print(SERIALMONITOR);
      SERIALMONITOR.println();
    #endif
  
  }
  
}

#if USE_MODULE_SERIALMONITOR == true  
FLASH_STRING(stringSaveConfigWriting,"Writing ");
FLASH_STRING(stringSaveConfigWritingCurrent,"Current configuration saved to EEPROM.");
FLASH_STRING(stringSaveConfigWritingDone," done!");
FLASH_STRING(stringConfigSaved,"Current configuration saved to EEPROM.");
FLASH_STRING(stringConfigSaveAsIdChanged,"Configuration ID changed from ");
FLASH_STRING(stringConfigSaveAsIdTo," to ");
FLASH_STRING(stringResettingDefault,"Resetting to default.");
FLASH_STRING(stringConfigModeEnded,"Config mode ended");
FLASH_STRING(stringConfigModeStarted,"Config mode started");
#endif

void saveConfig() {
  
    #if USE_MODULE_SERIALMONITOR == true  
      stringSaveConfigWriting.print(SERIALMONITOR);
      SERIALMONITOR.print(sizeof(configStore));
      stringSaveConfigWritingCurrent.print(SERIALMONITOR);
    #endif
    
    //EEPROM.writeBlock(CONFIG_START, configStore);
    for (unsigned int t=0; t<sizeof(configStore); t++)
        EEPROM.write(CONFIG_START + t, *((char*)&configStore + t));


    configInProgress=false;
    
    #if USE_MODULE_SERIALMONITOR == true  
      stringSaveConfigWritingDone.print(SERIALMONITOR);
      SERIALMONITOR.println();
      stringConfigModeEnded.print(SERIALMONITOR);
      SERIALMONITOR.println();
    #endif
    
}

void resetDefaultConfig(){
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 4096; i++)
    EEPROM.write(i, 0);
}



void commandConfigSave(char* logId){
  saveConfig();
  #if USE_MODULE_SERIALMONITOR == true  
    stringConfigSaved.print(SERIALMONITOR);
    SERIALMONITOR.println();
  #endif
}


void commandConfigSaveAsId(char* params){
    
  #if USE_MODULE_SERIALMONITOR == true  
    stringConfigSaveAsIdChanged.print(SERIALMONITOR);
    SERIALMONITOR.print(configStore.configId);
    stringConfigSaveAsIdTo.print(SERIALMONITOR);
  #endif
  
  char* id = getParamByName(params,fieldId);
  strcpy(configStore.configId,id);
  configInProgress = true;
  
  #if USE_MODULE_SERIALMONITOR == true  
    SERIALMONITOR.println(id);
    stringConfigModeStarted.print(SERIALMONITOR);
    SERIALMONITOR.println();
  #endif
  
}

void commandConfigResetDefault(char* params){
  #if USE_MODULE_SERIALMONITOR == true  
    stringResettingDefault.print(SERIALMONITOR);
    SERIALMONITOR.println();
  #endif
  resetDefaultConfig();
}

#if USE_MODULE_SERIALMONITOR == true  
//FLASH_STRING(stringSettingTime,"Setting time from unixtime ");
FLASH_STRING(stringAdjustingRTC,"Adjusting RTC to ");
FLASH_STRING(stringTimeSynced,"Time synced to ");
#endif

time_t commandConfigSetTime(unsigned long int timeunix){
    
    //build DateTime object from unix timestamp
    //DateTime thisDateTime = DateTime(timeunix);
    setTime(timeunix);
    time_t thisDateTime = now();
  
    //indicate that the time has been synced and set the datetime
    timeSyncInProgress = false;
    //timeAtSync = millis()/1000;
    timeSynced = true;
    timeSyncedDateTime = thisDateTime;
  
    //if the RTC is running, lets go ahead and adjust the clock
    #if USE_MODULE_DS1307RTC == true
    if (RTC.chipPresent()) {
      #if USE_MODULE_SERIALMONITOR == true  
        stringAdjustingRTC.print(SERIALMONITOR);
        printDateTimeToSerial(thisDateTime);
        SERIALMONITOR.println();
      #endif
      RTC.set(thisDateTime); 
    }
    #endif
    
    #if USE_MODULE_SERIALMONITOR == true  
      stringTimeSynced.print(SERIALMONITOR);
      printDateTimeToSerial(thisDateTime);
      SERIALMONITOR.println();
    #endif
    
    return thisDateTime;
}

#if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
FLASH_STRING(stringInvalidId," isn't a valid id ");
FLASH_STRING(stringUpdatingId,"Updating config for id ");
FLASH_STRING(stringResettingId,"Resetting config for id ");
FLASH_STRING(stringChanging,"Changing ");
FLASH_STRING(stringChanged,"Changed ");
FLASH_STRING(stringChangingTo," to ");
FLASH_STRING(stringInvalidProperty," invalid property name");
FLASH_STRING(stringInvalid," invalid");
//FLASH_STRING(stringResettingId," resetting id");
#endif

#if SENSORONLY == false
void commandConfigZone(char* params){
  
  //get id
  int zoneId = atoi(getIdFromParams(params));
  
  if(zoneId<0 || zoneId>maxZones){
    #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
      SERIALMONITOR.print(zoneId);
      stringInvalidId.print(SERIALMONITOR);
      SERIALMONITOR.println();
    #endif
    return;
  }else{
    #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
      stringUpdatingId.print(SERIALMONITOR);
      SERIALMONITOR.println(zoneId);
    #endif;
  } 
    
  char *param = strtok_r(params,"&",&params);
  
  while(param != NULL){
      
      char* name = strtok_r(param,"=",&param);
      char* value = strtok_r(NULL,"=",&param);  
  
      param = strtok_r(NULL,"&",&params);   
      
      if(strcmp(name,fieldName)==0){
        strncpy(configStore.zones[zoneId].name,value,maxNameLength);
      }else if(strcmp(name,fieldType)==0){
        configStore.zones[zoneId].type = atoi(value);
      }else if(strcmp(name,fieldPin)==0){
        configStore.zones[zoneId].pin = atoi(value);
      }else if(strcmp(name,fieldSafetyOffAfterMinutes)==0){
        configStore.zones[zoneId].safetyOffAfterMinutes = atoi(value);
      }else if(strcmp(name,fieldOverrideOn)==0){
        configStore.zones[zoneId].overrideOn = atoi(value);
      }else{
        #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
          SERIALMONITOR.print(name);
          stringInvalidProperty.print(SERIALMONITOR);
        #endif
        continue;
      }
      
      #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
        stringChanged.print(SERIALMONITOR);
        SERIALMONITOR.print(name);
        stringChangingTo.print(SERIALMONITOR);
        SERIALMONITOR.println(value);
      #endif
      
   }
   
}

void commandConfigZoneReset(char* params){

  char* ids = getIdFromParams(params);
  
  int zoneIds[maxZones];
  for(int i=0;i<maxZones;i++){
    zoneIds[i] = -1;
  }
  
  getIdsFromValue(zoneIds,maxZones,ids);
  
  for(int i=0;i<maxZones;i++){
    
   int thisId = zoneIds[i];
    
   if(thisId > -1 && thisId < maxZones){
     
      #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
        stringResettingId.print(SERIALMONITOR);
        SERIALMONITOR.println(thisId);
      #endif
      
      strncpy(configStore.zones[thisId].name,blankName,maxNameLength);
      configStore.zones[thisId].type = 0;
      configStore.zones[thisId].pin = -1;
      configStore.zones[thisId].safetyOffAfterMinutes = 0;
      configStore.zones[thisId].overrideOn = -1;
    }else{
      
      #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
        SERIALMONITOR.print(thisId);
        stringInvalid.print(SERIALMONITOR);
      #endif
      
    }
    
  }
    
}
#endif

void commandConfigSensor(char* params){
  
  //get id
  //int sensorId = getIdFromParams(params);
  int sensorId = atoi(getIdFromParams(params));

  if(sensorId<0 || sensorId>maxSensors){
    #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
      SERIALMONITOR.print(sensorId);
      stringInvalidId.print(SERIALMONITOR);
      SERIALMONITOR.println();
    #endif
    return;
  }else{
    #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
      stringUpdatingId.print(SERIALMONITOR);
      SERIALMONITOR.println(sensorId);
    #endif
  } 
  
  char *param = strtok_r(params,"&",&params);
  
  while(param != NULL){
      
      char* name = strtok_r(param,"=",&param);
      char* value = strtok_r(NULL,"=",&param);  
  
      param = strtok_r(NULL,"&",&params);   
      
      if(strcmp(name,fieldName)==0){
        strncpy(configStore.sensors[sensorId].name,value,maxNameLength);
      }else if(strcmp(name,fieldType)==0){
        configStore.sensors[sensorId].type = atoi(value);
      }else if(strcmp(name,fieldPin)==0){
        configStore.sensors[sensorId].pin = atoi(value);
      }else if(strcmp(name,fieldPin2)==0){
        configStore.sensors[sensorId].pin2 = atoi(value);
      }else if(strcmp(name,fieldFrequencyCheckSeconds)==0){
        configStore.sensors[sensorId].frequencyCheckSeconds = atoi(value);
      }else if(strcmp(name,fieldFrequencyLogSeconds)==0){
        configStore.sensors[sensorId].frequencyLogSeconds = atoi(value);
      }else{
        #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
          SERIALMONITOR.print(name);
          stringInvalidProperty.print(SERIALMONITOR);
        #endif
        continue;
      }
      
      #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
        stringChanging.print(SERIALMONITOR);
        SERIALMONITOR.print(name);
        stringChangingTo.print(SERIALMONITOR);
        SERIALMONITOR.println(value);
      #endif
      
   }   
  
}

void commandConfigSensorReset(char* params){
  
  char* ids = getIdFromParams(params);
  
  int sensorIds[maxSensors];
  
  for(int i=0;i<maxSensors;i++){
    sensorIds[i] = -1;
  }
  
  getIdsFromValue(sensorIds,maxSensors,ids);
  
  for(int i=0;i<maxSensors;i++){
          
    int thisId = sensorIds[i];
      
    if(thisId>=0 && thisId<maxSensors){
      
      #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
        stringResettingId.print(SERIALMONITOR);
        SERIALMONITOR.println(thisId);
      #endif
      
      strncpy(configStore.sensors[thisId].name,blankName,maxNameLength);
      configStore.sensors[thisId].type = 0;
      configStore.sensors[thisId].pin = -1;
      configStore.sensors[thisId].pin2 = -1;
      configStore.sensors[thisId].frequencyCheckSeconds = 99;
      configStore.sensors[thisId].frequencyLogSeconds = 99;
      
    }else{
      
      #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
        SERIALMONITOR.print(thisId);
        stringInvalid.print(SERIALMONITOR);
      #endif
      
    }
    
  }
      
}

void commandConfigSchedule(char* params){
  
  //get id
  int scheduleId = atoi(getIdFromParams(params));
    
  if(scheduleId<0 || scheduleId>maxZones){
    #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
      SERIALMONITOR.print(scheduleId);
      stringInvalidId.print(SERIALMONITOR);
      SERIALMONITOR.println();
    #endif
    return;
  }else{
    #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
      stringUpdatingId.print(SERIALMONITOR);
      SERIALMONITOR.println(scheduleId);
    #endif
  } 
  
  char *param = strtok_r(params,"&",&params);
  
  while(param != NULL){
      
      char* name = strtok_r(param,"=",&param);
      char* value = strtok_r(NULL,"=",&param);  
  
      param = strtok_r(NULL,"&",&params);   
      
      if(strcmp(name,fieldName)==0){
        strncpy(configStore.schedules[scheduleId].name,value,maxNameLength);
      }else if(strcmp(name,fieldType)==0){
        configStore.schedules[scheduleId].type = atoi(value);
      }else if(strcmp(name,fieldZones)==0){
        
          int zoneIds[maxZones];
          
          for(int i=0;i<maxZones;i++){
            zoneIds[i] = -1;
          }
          
          getIdsFromValue(zoneIds,maxZones,value);
          
          for(int i=0;i<maxZones;i++){
            
            #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
               SERIALMONITOR.print(" set zone position ");
               SERIALMONITOR.print(i);
             #endif
              
            if(zoneIds[i]>-1){
                configStore.schedules[scheduleId].zones[i] = zoneIds[i];
                
                #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
                  SERIALMONITOR.print(" to zone id ");
                  SERIALMONITOR.print(configStore.schedules[scheduleId].zones[i]);
                #endif
            }else{
               configStore.schedules[scheduleId].zones[i] = -1;
               
              #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
                 SERIALMONITOR.print(" to null ");
               #endif
            }
            
            #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
              SERIALMONITOR.print(" from ");
              SERIALMONITOR.print(zoneIds[i]); 
              SERIALMONITOR.println();
            #endif
            
          }
        
      }else if(strcmp(name,fieldZonesRunType)==0){
        configStore.schedules[scheduleId].zonesRunType = atoi(value);
      }else if(strcmp(name,fieldSensors)==0){
                
          int sensorIds[maxSensors];
          
          for(int i=0;i<maxSensors;i++){
            sensorIds[i] = -1;
          }
          
          getIdsFromValue(sensorIds,maxSensors,value);
          
          for(int i=0;i<maxSensors;i++){
            
             #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
               SERIALMONITOR.print(" set sensor position ");
               SERIALMONITOR.print(i);
             #endif
             
            if(sensorIds[i]>-1){
                configStore.schedules[scheduleId].sensors[i] = sensorIds[i];
                
                #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
                  SERIALMONITOR.print(" set to sensor id ");
                  SERIALMONITOR.print(configStore.schedules[scheduleId].sensors[i]);
                #endif
            }else{
               configStore.schedules[scheduleId].sensors[i] = -1;
               
               #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
                 SERIALMONITOR.print(" to null ");
               #endif
            }
            
            #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
              SERIALMONITOR.print(" from ");
              SERIALMONITOR.print(sensorIds[i]);     
              SERIALMONITOR.println();
            #endif
            
          }
           
      }else if(strcmp(name,fieldTimerStartWeekdays)==0){
        //cycle through weekdays
        strncpy(configStore.schedules[scheduleId].timerStartWeekdays,value,7);
        /*for(int i=0;i<7;i++){
           configStore.schedules[scheduleId].timerStartWeekdays[i] = int(value[i]);
        }*/
      }else if(strcmp(name,fieldTimerStartHours)==0){
        //cycle through hours
        strncpy(configStore.schedules[scheduleId].timerStartHours,value,24);
        /*for(int i=0;i<24;i++){
           configStore.schedules[scheduleId].timerStartHours[i] = int(value[i]);
        }*/
      }else if(strcmp(name,fieldTimerStartMinutes)==0){
        //cycle through minutes
        strncpy(configStore.schedules[scheduleId].timerStartMinutes,value,60);
        /*for(int i=0;i<60;i++){
           configStore.schedules[scheduleId].timerStartMinutes[i] = int(value[i]);
        }*/
      }
      /*else if(strcmp(name,fieldTimerStartSeconds)==0){
        //cycle through seconds
        for(int i=0;i<60;i++){
           configStore.schedules[scheduleId].timerStartSeconds[i] = int(value[i]);
        }
      }*/
      else if(strcmp(name,fieldValueMin)==0){
        configStore.schedules[scheduleId].valueMin = atoi(value);
      }else if(strcmp(name,fieldValueMax)==0){ 
        configStore.schedules[scheduleId].valueMax = atoi(value);
      }else{
        #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
          SERIALMONITOR.print(name);
          stringInvalidProperty.print(SERIALMONITOR);
        #endif
        continue;
      }
   
      #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
        stringChanging.print(SERIALMONITOR);
        SERIALMONITOR.print(name);
        stringChangingTo.print(SERIALMONITOR);
        SERIALMONITOR.println(value);
      #endif
   
   }
  
}

void commandConfigScheduleReset(char* params){
  
  char* value = getIdFromParams(params);
  
  int scheduleIds[maxSchedules];
  
  for(int i=0;i<maxSchedules;i++){
    scheduleIds[i] = -1;
  }
  
  getIdsFromValue(scheduleIds,maxSchedules,value);
  
  for(int i=0;i<maxSchedules;i++){
    
    int thisId = scheduleIds[i];
    
    if(thisId>=0 && thisId<maxSchedules){
      
        #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
        stringResettingId.print(SERIALMONITOR);
          SERIALMONITOR.println(thisId);
        #endif
      
        strncpy(configStore.schedules[thisId].name,blankName,maxNameLength);
        configStore.schedules[thisId].type = 0;
        int thisPosition = 0;
        while(thisPosition < maxZones){
          configStore.schedules[thisId].zones[thisPosition] = 0;
          thisPosition++;
        }
        thisPosition = 0;
        configStore.schedules[thisId].zonesRunType = 0;
        while(thisPosition < maxSensors){
           configStore.schedules[thisId].sensors[thisPosition] = 0;
           thisPosition++;
        }
        for(int i=0;i<7;i++){
           configStore.schedules[thisId].timerStartWeekdays[i] = 0;
        }
        for(int i=0;i<24;i++){
           configStore.schedules[thisId].timerStartHours[i] = 0;
        }
        for(int i=0;i<60;i++){
           configStore.schedules[thisId].timerStartMinutes[i] = 0;
        }
        /*for(int i=0;i<60;i++){
           configStore.schedules[thisId].timerStartSeconds[i] = 0;
        }*/
        configStore.schedules[thisId].valueMin = 0;
        configStore.schedules[thisId].valueMax = 0;
    }else{
      
      #if USE_MODULE_SERIALMONITOR == true && DEBUGCONFIG == true
        SERIALMONITOR.print(thisId);
        stringInvalid.print(SERIALMONITOR);
      #endif
      
    }
    
  }
  
}
