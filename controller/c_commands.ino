const char sendTermination = '>';
const char receiveTermination = '<';

#if defined(USESERIALMONITOR)
FLASH_STRING(stringSendingCommand,"[TX] ");
FLASH_STRING(stringExecuting,"[RX] ");
FLASH_STRING(stringWith," ,paramater= ");
FLASH_STRING(stringUnrecognizedCommand,"[ERROR] Unrecognized command: ");
#endif
  
void readSerialToBuffer(SoftwareSerial &serial, char* commandBuffer, int &bufferPosition, boolean &readyToProcess){
  
  if((bufferPosition-2)>=maxBufferSize){
    #if defined(USESERIALMONITOR)
      Serial.println("buffer overflowing");
    #endif
    //memset(commandBuffer,0,maxBufferSize);
    //fix-me: we need to do something better here
    restart();
    return;
  }

  char inChar;
  
  while(serial.available()>0){
    
    inChar = serial.read(); 
        
    //done building command
    if (inChar == '^') {
      readyToProcess = true;
      return;
    }else{
      commandBuffer[bufferPosition] = inChar;
      bufferPosition++;
    }
  }  
  
}  

void readSerialToBuffer(HardwareSerial &serial, char* commandBuffer, int &bufferPosition, boolean &readyToProcess){
  
  if((bufferPosition-2)>=maxBufferSize){
    #if defined(USESERIALMONITOR)
      Serial.println("buffer overflowing");
    #endif
    //memset(commandBuffer,0,maxBufferSize);
    //fix-me: we need to do something better here
    restart();
    return;
  }

  char inChar;
  
  while(serial.available()>0){
    
    inChar = serial.read(); 
    
    //done building command
    if (inChar == '^') {
      readyToProcess = true;
      return;
    }else{
      commandBuffer[bufferPosition] = inChar;
      bufferPosition++;
    }
  }  
  
}
  




void processBuffer(char* commandBuffer){
  
   //Serial.print("process=");
   //Serial.println(commandBuffer);
     
    char * thisCommand = strtok_r(commandBuffer,"<",&commandBuffer);
    while (commandBuffer != NULL)
    {
      
      if(strchr(thisCommand,'?')!=0){
        executeCommand(strtok(thisCommand,"?"),strtok(NULL,"?"));
      //doesn't have parameters
      }else{
        executeCommand(thisCommand,"");
      }
      
      thisCommand = strtok_r(NULL,"<",&commandBuffer);
    }
    
}

void sendCommand(char* thisCommand){
        
  //these commands need special processing
  if(strstr(thisCommand,commandStringConfigSetTime)!=0){
    
    timeSyncInProgress = true;
    #if defined(DEBUGTIMESYNC)
      stringSendingCommand.print(Serial);
      Serial.println(thisCommand);
    #endif
  }else if(strstr(thisCommand,commandStringSystemHeartbeat)!=0){

    heartBeatSent = millis();
    heartBeatInProgress = true;
      
    #if defined(DEBUGHEARTBEAT)
      stringSendingCommand.print(Serial);
      Serial.println(thisCommand);
    #endif
    
  }else{
    #if defined(USESERIALMONITOR)
      stringSendingCommand.print(Serial);
      Serial.println(thisCommand);
    #endif
  }
    
  //Serial.print("sending=");
  //Serial.println(thisCommand);  
    
  #if defined(USESERIALCOM)
  #if defined(USESOFTWARESERIAL)  
    softSerial.write(thisCommand);
    softSerial.write(">");
  #else
    Serial1.write(thisCommand);
    Serial1.write(">");
  #endif
  #endif
 
  #if defined(DEBUG)
    printAvailableMemory();
  #endif
  
}


/**
Generic handler for executing commands received from the Electric Imp
**/


void executeCommand(char* command, char* params){
    
  boolean isHeartbeat = false;
  boolean isTimesync = false;
  boolean isUnrecognized = false;
    
  //set time from unixtime
  if(strcmp(command,commandStringSystemHeartbeat)==0){
      commandSystemHeartbeat(params);
      isHeartbeat = true;
  //confirm that a data log was received
  }else if(strcmp(command,commandStringConfigSetTime)==0){
      commandConfigSetTime(atol(params));
      isTimesync = true;
  //save configuration to EEPROM
  }else if(strcmp(command,commandStringConfigSaveAsId)==0){
      //Serial.println("save as id");
      commandConfigSaveAsId(params);
  }else if(strcmp(command,commandStringConfigSave)==0){
      commandConfigSave(params);
  //reset configuration to default (nullify EEPROM)
  }else if(strcmp(command,commandStringConfigResetDefault)==0){
      commandConfigResetDefault(params);
  //restart the controller
  }else if(strcmp(command,commandStringSystemRestart)==0){
      commandSystemRestart(params);
  //confirm that a data log was received
  }else if(strcmp(command,commandStringSystemReinit)==0){
      commandSystemReinit(params);
  //confirm that a data log was received
  }else if(strcmp(command,commandStringDataLogReceived)==0){
      commandDataLogReceived(params);
  //configure a zone
  #if !defined(SENSORONLY)
  }else if(strcmp(command,commandStringConfigZone)==0){
      commandConfigZone(params);
  //override a zone on
  }else if(strcmp(command,commandStringConfigZoneReset)==0){
      commandConfigZoneReset(params);
  }else if(strcmp(command,commandStringConfigSchedule)==0){
      commandConfigSchedule(params);
  }else if(strcmp(command,commandStringConfigScheduleReset)==0){
      commandConfigScheduleReset(params);
  #endif
  }else if(strcmp(command,commandStringConfigSensor)==0){
      commandConfigSensor(params);
  }else if(strcmp(command,commandStringConfigSensorReset)==0){
      commandConfigSensorReset(params);
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
  
    #if defined(USESERIALMONITOR)
      stringUnrecognizedCommand.print(Serial);
      Serial.println(command);
    #endif
    
  }else{
    
    #if defined(USESERIALMONITOR)
      stringExecuting.print(Serial);
      Serial.print(command);
      stringWith.print(Serial);
      Serial.println(params);
    #endif
    
  }

  #if defined(DEBUG)
    printAvailableMemory();
  #endif

}

#if defined(USESERIALMONITOR)
FLASH_STRING(stringRestarting,"Restarting system. ");
#endif

void commandSystemRestart(char* params){
  #if defined(USESERIALMONITOR)
    stringRestarting.print(Serial);
    Serial.println();
  #endif
  restart();
}

#if defined(USESERIALMONITOR)
FLASH_STRING(stringReinit,"Reinitializing controller.");
#endif

void commandSystemReinit(char* params){
  #if defined(USESERIALMONITOR)
    stringReinit.print(Serial);
    Serial.println();
  #endif
  initController();
}


#if defined(USESERIALMONITOR)
FLASH_STRING(stringHeartbeatOnline,"[HEARTBEAT] [ONLINE]");
#endif

void commandSystemHeartbeat(char* value){
  
    if(heartBeatInProgress==false){
      #if defined(DEBUGHEARTBEAT)
        Serial.println("ignored out of sync heartbeat");
      #endif
      return;
    }
  
    #if defined(DEBUGHEARTBEAT)
    if(heartBeatOnline==false){
      stringHeartbeatOnline.print(Serial);
      Serial.println();
    }
    #endif
  
    //indicate that the time has been synced and set the datetime
    heartBeatLast = millis();
    heartBeatOnline = true;
    heartBeatInProgress = false;
    
}

#if defined(USESERIALMONITOR)
FLASH_STRING(stringServerLogReceived,"Server received log: ");
#endif

void commandDataLogReceived(char* logId){
  #if defined(USESERIALMONITOR)
    stringServerLogReceived.print(Serial);
    Serial.println(logId);
  #endif
}
