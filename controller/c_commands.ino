const char sendTermination = '>';
const char receiveTermination = '<';

#if USE_MODULE_SERIALMONITOR == true
FLASH_STRING(stringSendingCommand,"[TX] ");
FLASH_STRING(stringExecuting,"[RX] ");
FLASH_STRING(stringWith," ,paramater= ");
FLASH_STRING(stringUnrecognizedCommand,"[ERROR] Unrecognized command: ");
FLASH_STRING(stringBufferOver,"buffer overflowing");
#endif
  

#if USE_MODULE_ETHERNETCOM == true  
void readEthernetToBuffer(char* commandBuffer, int &bufferPosition, boolean &readyToProcess){
  
  if((bufferPosition-2)>=maxBufferSize){
    #if USE_MODULE_SERIALMONITOR == true
      stringBufferOver.print(SERIALMONITOR);
    #endif
    //memset(commandBuffer,0,maxBufferSize);
    //fix-me: we need to do something better here
    restart();
    return;
  }
      
  boolean currentLineIsBlank = true;    
  boolean foundHeader = false;
  boolean foundBody = false;

  while(client && client.connected()){
    
    if(client.available()){
  
        char c = client.read();
        
        #if DEBUGETHERNET == true
          SERIALMONITOR.print(c);
        #endif
        
        if(c == '\n' && currentLineIsBlank==true){
          
          if(foundHeader==false){
            foundHeader=true;
          }else if(foundBody==false){
            foundBody=true;
            client.stop();
            break;
          }else{
            client.stop();
            break;
          }
          
        }
        
        if(foundHeader==true && c != '\n'){
                  
          //done building command
          if (c == '^') {
            readyToProcess = true;
            client.stop();
            break;
          }else{
            //SERIALMONITOR.print(c);
            commandBuffer[bufferPosition] = c;
            bufferPosition++;
          }
          
        }
        
        if (c == '\n') {
          currentLineIsBlank = true;
        }else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
        
      }
   
    }
  
  //process the command buffer
  if(readyToProcess==true){
    #if USE_MODULE_WATCHDOG == true
      //go ahead and reset the watchdog if the buffer is ready to process
      wdt_reset();
    #endif
    //process commands in the buffer
    processBuffer(commandBuffer);
    //fix-me: this shouldn't be necessary because of strtok
    //reset the memory of the buffer
    memset(commandBuffer,0,maxBufferSize);
    //reset the buffer position
    bufferPosition = 0;
    //do not process the buffer again
    readyToProcess = false;
  }
  
} 
#endif
  
  
/*void readSerialToBuffer(SoftwareSerial &serial, char* commandBuffer, int &bufferPosition, boolean &readyToProcess){
  
  if((bufferPosition-2)>=maxBufferSize){
    #if USE_MODULE_SERIALMONITOR == true
      SERIALMONITOR.println("buffer overflowing");
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
  
}*/  

//void readSerialToBuffer(HardwareSerial &serial, char* commandBuffer, int &bufferPosition, boolean &readyToProcess){
void readSerialToBuffer(Stream &serial, char* commandBuffer, int &bufferPosition, boolean &readyToProcess){
  
  if((bufferPosition-2)>=maxBufferSize){
    #if USE_MODULE_SERIALMONITOR == true
      stringBufferOver.print(SERIALMONITOR);
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
      break;
    }else{
      commandBuffer[bufferPosition] = inChar;
      bufferPosition++;
    }
  }  
  
    //process the command buffer
  if(readyToProcess==true){
    
    #if USE_MODULE_WATCHDOG == true
      //go ahead and reset the watchdog if the buffer is ready to process
      wdt_reset();
    #endif
    
    //process commands in the buffer
    processBuffer(commandBuffer);
    //fix-me: this shouldn't be necessary because of strtok
    //reset the memory of the buffer
    memset(commandBuffer,0,maxBufferSize);
    //reset the buffer position
    bufferPosition = 0;
    //do not process the buffer again
    readyToProcess = false;
  }
  
  
}
  




void processBuffer(char* commandBuffer){
  
   //SERIALMONITOR.print("process=");
   //SERIALMONITOR.println(commandBuffer);
     
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
    
    //timeSyncSent = millis();
    timeSyncInProgress = true;
    
    #if DEBUGTIMESYNC == true
      stringSendingCommand.print(SERIALMONITOR);
      SERIALMONITOR.println(thisCommand);
    #endif
  }else if(strstr(thisCommand,commandStringSystemHeartbeat)!=0){

    heartBeatSent = now();
    heartBeatInProgress = true;
      
    #if DEBUGHEARTBEAT == true
      stringSendingCommand.print(SERIALMONITOR);
      SERIALMONITOR.println(thisCommand);
    #endif
    
  }else{
    #if USE_MODULE_SERIALMONITOR == true
      stringSendingCommand.print(SERIALMONITOR);
      SERIALMONITOR.println(thisCommand);
    #endif
  } 
    
  #if USE_MODULE_SERIALCOM == true
  #if USESOFTWARESERIAL == true  
    softSerial.write(thisCommand);
    softSerial.write(">");
  #else
    SERIALCOM.write(thisCommand);
    SERIALCOM.write(">");
  #endif
  #endif
  
  #if USE_MODULE_ETHERNETCOM == true
    
    #if DEBUGETHERNET == true
      SERIALMONITOR.print("connecting...");
    #endif
    
    if (client.connect(configStore.server, configStore.serverPort)) {
      
      #if DEBUGETHERNET == true
        SERIALMONITOR.println("connected");
      #endif

      // Make a HTTP request:
      client.println("POST /service HTTP/1.1");
      client.print("Host: ");
      client.println(configStore.server);
      client.println("Connection: close");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(3+strlen(configStore.deviceId)+9+strlen(thisCommand));
      client.println();
      client.print("id=");
      client.print(configStore.deviceId);
      client.print("&command=");
      for(int i=0;i<strlen(thisCommand);i++){
        if(thisCommand[i]=='&'){
          client.print("%26");
        }else{
          client.print(thisCommand[i]);
        }
      }
      
      delay(10);
      
      readEthernetToBuffer(commandBufferEthernet,commandBufferPositionEthernet,commandBufferReadyToProcessEthernet);

    }else{
      client.stop();
      #if DEBUGETHERNET == true
        SERIALMONITOR.println("failed to connect");
      #endif
    }
  #endif
 
  #if DEBUG ==  true && USE_MODULE_SERIALMONITOR == true
    printAvailableMemory();
  #endif
  
}

char specials[] = "&"; ///* String containing chars you want encoded */

static char hex_digit(char c)
{  return "01234567890ABCDEF"[c & 0x0F];
}

char* urlencode(char* dst,char* src)
{  char *d = dst;
   char c;
   while (c = *src++)
   {  if (strchr(specials,c))
      {  *d++ = '%';
         *d++ = hex_digit(c >> 4);
         *d++ = hex_digit(c);
      }
      else *d++ = c;
   }
   return dst;
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
  //set the current time on the system
  }else if(strcmp(command,commandStringConfigSetTime)==0){
      commandConfigSetTime(atol(params));
      isTimesync = true;
  //print the current system information
  }if(strcmp(command,commandStringSystemInfo)==0){
      commandSystemInformation(params);
  //set the id of the next saved configuration
  }else if(strcmp(command,commandStringConfigSaveAsId)==0){
      //SERIALMONITOR.println("save as id");
      commandConfigSaveAsId(params);
  //save configuration to EEPROM
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
  #if SENSORONLY == false
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
  }else if(strcmp(command,commandStringTestFactory)==0){
      commandTestFactory(params);
  }else if(strcmp(command,commandStringTestHeartbeat)==0){
      commandTestHeartbeat(params);
  }else if(strcmp(command,commandStringTestTime)==0){
      commandTestTime(params);
  }else if(strcmp(command,commandStringTestRTC)==0){
      commandTestRTC(params);
  }else if(strcmp(command,commandStringTestSD)==0){
      commandTestSD(params);
  }else if(strcmp(command,commandStringTestZones)==0){
      commandTestZones(params);
  }else if(strcmp(command,commandStringTestLoadConfig)==0){
      commandTestLoadFactoryConfiguration(params);
  }else{
    isUnrecognized = true;
  }
    
  if(isHeartbeat==true){
  
    #if DEBUGHEARTBEAT == true
      stringExecuting.print(SERIALMONITOR);
      SERIALMONITOR.print(command);
      stringWith.print(SERIALMONITOR);
      SERIALMONITOR.println(params);
    #endif
    
  }else if(isTimesync==true){
  
    #if DEBUGTIMESYNC == true
      stringExecuting.print(SERIALMONITOR);
      SERIALMONITOR.print(command);
      stringWith.print(SERIALMONITOR);
      SERIALMONITOR.println(params);
    #endif
    
  }else if(isUnrecognized==true){
  
    #if USE_MODULE_SERIALMONITOR == true
      stringUnrecognizedCommand.print(SERIALMONITOR);
      SERIALMONITOR.println(command);
    #endif
    
  }else{
    
    #if USE_MODULE_SERIALMONITOR == true
      stringExecuting.print(SERIALMONITOR);
      SERIALMONITOR.print(command);
      stringWith.print(SERIALMONITOR);
      SERIALMONITOR.println(params);
    #endif
    
  }

  #if DEBUG ==  true && USE_MODULE_SERIALMONITOR == true
    printAvailableMemory();
  #endif

}

#if USE_MODULE_SERIALMONITOR == true
FLASH_STRING(stringRestarting,"Restarting system. ");
#endif

void commandSystemRestart(char* params){
  #if USE_MODULE_SERIALMONITOR == true
    stringRestarting.print(SERIALMONITOR);
    SERIALMONITOR.println();
  #endif
  restart();
}

#if USE_MODULE_SERIALMONITOR == true
FLASH_STRING(stringReinit,"Reinitializing controller.");
#endif

void commandSystemInformation(char* params){
  
  
  
}

void commandSystemReinit(char* params){
  #if USE_MODULE_SERIALMONITOR == true
    stringReinit.print(SERIALMONITOR);
    SERIALMONITOR.println();
  #endif
  initController();
}


#if USE_MODULE_SERIALMONITOR == true
FLASH_STRING(stringHeartbeatOnline,"[HEARTBEAT] [ONLINE]");
#endif

void commandSystemHeartbeat(char* value){
    
    if(heartBeatInProgress==false){
      #if DEBUGHEARTBEAT == true
        SERIALMONITOR.println("ignored out of sync heartbeat");
      #endif
      return;
    }
  
    #if DEBUGHEARTBEAT == true
    if(heartBeatOnline==false){
      stringHeartbeatOnline.print(SERIALMONITOR);
      SERIALMONITOR.println();
    }
    #endif
  
    //indicate that the time has been synced and set the datetime
    heartBeatLast = now();
    heartBeatOnline = true;
    heartBeatInProgress = false;
    
}

#if USE_MODULE_SERIALMONITOR == true
FLASH_STRING(stringServerLogReceived,"Server received log: ");
#endif

void commandDataLogReceived(char* logId){
  #if USE_MODULE_SERIALMONITOR == true
    stringServerLogReceived.print(SERIALMONITOR);
    SERIALMONITOR.println(logId);
  #endif
}

