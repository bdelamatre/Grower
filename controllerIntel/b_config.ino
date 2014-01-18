
const char blankName[maxNameLength] = "               "; 

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

void saveConfig() {
  
    #if defined(USESERIALMONITOR)  
      Serial.print("Writing ");
      Serial.print(sizeof(configStore));
      Serial.print("...);
    #endif
    
    //EEPROM.writeBlock(CONFIG_START, configStore);
    for (unsigned int t=0; t<sizeof(configStore); t++)
        EEPROM.write(CONFIG_START + t, *((char*)&configStore + t));


    configInProgress=false;
    
    #if defined(USESERIALMONITOR)  
      Serial.println(" done!");
      Serial.println("config mode ended");
    #endif
    
}

void resetDefaultConfig(){
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 4096; i++)
    EEPROM.write(i, 0);
}



void commandConfigSave(char* logId){
  saveConfig();
  #if defined(USESERIALMONITOR)  
    Serial.println("Current configuration saved to EEPROM.");
  #endif
}


void commandConfigSaveAsId(char* params){
    
  #if defined(USESERIALMONITOR)  
    Serial.print("Configuration ID changed from ");
    Serial.print(configStore.configId);
    Serial.print(" to ");
  #endif
  
  char* id = getParamByName(params,fieldId);
  strcpy(configStore.configId,id);
  configInProgress = true;
  
  #if defined(USESERIALMONITOR)  
    Serial.println(id);
    Serial.println("config mode started");
  #endif
  
}

void commandConfigResetDefault(char* params){
  #if defined(USESERIALMONITOR)
    Serial.println("Resetting to default.");
  #endif
  resetDefaultConfig();
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
    /*if (RTC.isrunning()) {
      Serial.print("Adjusting RTC to ");
      printDateTimeToSerial(thisDateTime);
      Serial.println();
      RTC.adjust(thisDateTime); 
    }*/
    
    #if defined(USESERIALMONITOR)  
      Serial.print("Time synced to ");
      printDateTimeToSerial(thisDateTime);
      Serial.println();
    #endif
    
    return thisDateTime;
}

void commandConfigZone(char* params){
  
  //get id
  int zoneId = atoi(getIdFromParams(params));
  
  if(zoneId<0 || zoneId>maxZones){
    #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
      Serial.print(zoneId);
      Serial.print(" isn't a valid id ");
      Serial.println();
    #endif
    return;
  }else{
    #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
      Serial.print("Updating config for id ");
      Serial.println(zoneId);
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
        #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
          Serial.print(name);
          Serial.println(" invalid property name");
        #endif
        continue;
      }
      
      #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
        Serial.print("Changed ");
        Serial.print(name);
        Serial.print(" to ");
        Serial.println(value);
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
     
      #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
        Serial.print("resetting id ");
        Serial.println(thisId);
      #endif
      
      strncpy(configStore.zones[thisId].name,blankName,maxNameLength);
      configStore.zones[thisId].type = 0;
      configStore.zones[thisId].pin = -1;
      configStore.zones[thisId].safetyOffAfterMinutes = 0;
      configStore.zones[thisId].overrideOn = -1;
    }else{
      
      #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
        Serial.print(thisId);
        Serial.println(" invalid");
      #endif
      
    }
    
  }
    
}

void commandConfigSensor(char* params){
  
  //get id
  //int sensorId = getIdFromParams(params);
  int sensorId = atoi(getIdFromParams(params));

  if(sensorId<0 || sensorId>maxSensors){
    #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
      Serial.print(sensorId);
      Serial.print(" isn't a valid id ");
      Serial.println();
    #endif
    return;
  }else{
    #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
      Serial.print("Updating config for id ");
      Serial.println(sensorId);
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
        #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
          Serial.print(name);
          Serial.println(" invalid property name");
        #endif
        continue;
      }
      
      #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
        Serial.print("Changing ");
        Serial.print(name);
        Serial.print(" to ");
        Serial.println(value);
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
      
      #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
        Serial.print("resetting id ");
        Serial.println(thisId);
      #endif
      
      strncpy(configStore.sensors[thisId].name,blankName,maxNameLength);
      configStore.sensors[thisId].type = 0;
      configStore.sensors[thisId].pin = -1;
      configStore.sensors[thisId].pin2 = -1;
      configStore.sensors[thisId].frequencyCheckSeconds = 99;
      configStore.sensors[thisId].frequencyLogSeconds = 99;
      
    }else{
      
      #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
        Serial.print(thisId);
        Serial.println(" invalid");
      #endif
      
    }
    
  }
      
}

void commandConfigSchedule(char* params){
  
  //get id
  int scheduleId = atoi(getIdFromParams(params));
    
  if(scheduleId<0 || scheduleId>maxZones){
    #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
      Serial.print(scheduleId);
      Serial.print(" isn't a valid id ");
      Serial.println();
    #endif
    return;
  }else{
    #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
      Serial.print("Updating config for id ");
      Serial.println(scheduleId);
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
            
            #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
               Serial.print(" set zone position ");
               Serial.print(i);
             #endif
              
            if(zoneIds[i]>-1){
                configStore.schedules[scheduleId].zones[i] = zoneIds[i];
                
                #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
                  Serial.print(" to zone id ");
                  Serial.print(configStore.schedules[scheduleId].zones[i]);
                #endif
            }else{
               configStore.schedules[scheduleId].zones[i] = -1;
               
              #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
                 Serial.print(" to null ");
               #endif
            }
            
            #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
              Serial.print(" from ");
              Serial.print(zoneIds[i]); 
              Serial.println();
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
            
             #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
               Serial.print(" set sensor position ");
               Serial.print(i);
             #endif
             
            if(sensorIds[i]>-1){
                configStore.schedules[scheduleId].sensors[i] = sensorIds[i];
                
                #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
                  Serial.print(" set to sensor id ");
                  Serial.print(configStore.schedules[scheduleId].sensors[i]);
                #endif
            }else{
               configStore.schedules[scheduleId].sensors[i] = -1;
               
               #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
                 Serial.print(" to null ");
               #endif
            }
            
            #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
              Serial.print(" from ");
              Serial.print(sensorIds[i]);     
              Serial.println();
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
      }else if(strcmp(name,fieldTimerStartSeconds)==0){
        //cycle through seconds
        for(int i=0;i<60;i++){
           configStore.schedules[scheduleId].timerStartSeconds[i] = int(value[i]);
        }
      }else if(strcmp(name,fieldValueMin)==0){
        configStore.schedules[scheduleId].valueMin = atoi(value);
      }else if(strcmp(name,fieldValueMax)==0){ 
        configStore.schedules[scheduleId].valueMax = atoi(value);
      }else{
        #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
          Serial.print(name);
          Serial.println(" invalid property name");
        #endif
        continue;
      }
   
      #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
        Serial.print("Changing ");
        Serial.print(name);
        Serial.print(" to ");
        Serial.println(value);
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
      
        #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
          Serial.print("resetting id ");
          Serial.println(thisId);
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
        for(int i=0;i<60;i++){
           configStore.schedules[thisId].timerStartSeconds[i] = 0;
        }
        configStore.schedules[thisId].valueMin = 0;
        configStore.schedules[thisId].valueMax = 0;
    }else{
      
      #if defined(USESERIALMONITOR) && defined(DEBUGCONFIG)
        Serial.print(thisId);
        Serial.println(" invalid");
      #endif
      
    }
    
  }
  
  
}

