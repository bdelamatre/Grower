
const char commandStringSystemHeartbeat[]     = "system:heartbeat";
const char commandStringSystemRestart[]       = "system:restart";
const char commandStringSystemReinit[]        = "system:reinit";
const char commandStringConfigSaveAsId[]      = "config:save-as-id";
const char commandStringConfigSave[]          = "config:save";
const char commandStringConfigResetDefault[]  = "config:reset-default";
const char commandStringConfigSetTime[]       = "config:set-time";
const char commandStringDataLogReceived[]     = "data:log-received";
const char commandStringConfigZone[]          = "config:zone";
const char commandStringConfigZoneReset[]     = "config:zone-reset";
const char commandStringConfigSensor[]        = "config:sensor";
const char commandStringConfigSensorReset[]   = "config:sensor-reset";
const char commandStringConfigSchedule[]      = "config:schedule";
const char commandStringConfigScheduleReset[] = "config:schedule-reset";
const char commandStringHelp[]                = "help";


//FLASH_STRING(stringSyncInProgress,"Time sync in progress");



FLASH_STRING(stringSendingCommand,"[TX] ");

void receiveCommand(HardwareSerial &serial, String &commandBuffer){
      
    while(serial.available()){
      char inChar = (char)serial.read(); 
      commandBuffer += inChar;
      //Serial.println(commandBuffer);
      //done building command
      if (inChar == '>') {
        //if not an empty command
        if(commandBuffer!=">"){
          //send the command
          sendCommand(commandBuffer);
        }
        //than empty the command
        commandBuffer = "";
      }else if (inChar == '<') {
        if(commandBuffer!="<"){
          //empty command, ignore
          //has paramaters
          if(commandBuffer.indexOf("?")>=0){
            executeCommand(commandBuffer.substring(0,commandBuffer.indexOf("?"))+"<"
                          ,commandBuffer.substring(commandBuffer.indexOf("?")+1,commandBuffer.length()-1));
          //doesn't have parameters
          }else{
            executeCommand(commandBuffer,"");
          }
        }
        commandBuffer = "";
      }
    }  

}

void sendCommand(String thisCommand){
      
  //these commands need special processing
  if(thisCommand=="config:set-time>"){
    
    timeSyncInProgress = true;
    #if defined(DEBUGTIMESYNCD)
      stringSendingCommand.print(Serial);
      Serial.println(thisCommand);
    #endif
  }else if(thisCommand.indexOf("system:heartbeat")>-1){

    heartBeatInProgress = true;
    heartBeatSent = millis();
    #if defined(DEBUGHEARTBEAT)
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
   
  int commandLength = command.length();
  char commandString[commandLength];
  command.toCharArray(commandString, commandLength);
    
  boolean isHeartbeat = false;
  boolean isTimesync = false;
  boolean isUnrecognized = false;
    
 //Serial.print("command=");
 //Serial.println(commandString);   
    
 //Serial.println(commandStringConfigSaveAsId);
 //int test = strcmp(commandString,commandStringConfigSaveAsId);
 //Serial.println(test);
 
  //set time from unixtime
  if(strcmp(commandString,commandStringSystemHeartbeat)==0){
      commandSystemHeartbeat(params.toInt());
      isHeartbeat = true;
  //confirm that a data log was received
  }else if(strcmp(commandString,commandStringConfigSetTime)==0){
      commandConfigSetTime(params.toInt());
      isTimesync = true;
  //save configuration to EEPROM
  }else if(strcmp(commandString,commandStringConfigSaveAsId)==0){
      //Serial.println("save as id");
      commandConfigSaveAsId(params);
  }else if(strcmp(commandString,commandStringConfigSave)==0){
      commandConfigSave(params);
  //reset configuration to default (nullify EEPROM)
  }else if(strcmp(commandString,commandStringConfigResetDefault)==0){
      commandConfigResetDefault(params);
  //restart the controller
  }else if(strcmp(commandString,commandStringSystemRestart)==0){
      Serial.println("restarting system...");
      commandSystemRestart(params);
  //confirm that a data log was received
  }else if(strcmp(commandString,commandStringSystemReinit)==0){
      commandSystemReinit(params);
  //confirm that a data log was received
  }else if(strcmp(commandString,commandStringDataLogReceived)==0){
      commandDataLogReceived(params);
  //configure a zone
  }else if(strcmp(commandString,commandStringConfigZone)==0){
      commandConfigZone(params);
  //override a zone on
  }else if(strcmp(commandString,commandStringConfigZoneReset)==0){
      commandConfigZoneReset(params);
  //override a zone on
  //}else if(strcmp(commandString,commandStringSystemHeartbeat)){
  //    commandTestZones(params);
  }else if(strcmp(commandString,commandStringConfigSensor)==0){
      commandConfigSensor(params);
  }else if(strcmp(commandString,commandStringConfigSensorReset)==0){
      commandConfigSensorReset(params);
  }else if(strcmp(commandString,commandStringConfigSchedule)==0){
      commandConfigSchedule(params);
  }else if(strcmp(commandString,commandStringConfigScheduleReset)==0){
      commandConfigScheduleReset(params);
  }else if(strcmp(commandString,commandStringHelp)==0){
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
    Serial.println(commandString);
    
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

void(* restart) (void) = 0; //declare reset function @ address 0

FLASH_STRING(stringRestarting,"Restarting system. ");

void commandSystemRestart(String params){
  stringRestarting.print(Serial);
  Serial.println();
  restart();
}

FLASH_STRING(stringReinit,"Reinitializing controller.");

void commandSystemReinit(String params){
  stringReinit.print(Serial);
  Serial.println();
  initController();
}


FLASH_STRING(stringHeartbeatOnline,"[HEARTBEAT] [ONLINE]");

void commandSystemHeartbeat(int value){
  
      //#if defined(DEBUGHEARTBEAT)
      if(heartBeatOnline==false){
        stringHeartbeatOnline.print(Serial);
        Serial.println();
      }
      //#endif
  
    //indicate that the time has been synced and set the datetime
    heartBeatInProgress = false;
    heartBeatLast = millis();
    heartBeatOnline = true;
    
}


void commandHelp(){
  
    /*Serial.println();
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
    Serial.println();*/
   
}

FLASH_STRING(stringServerLogReceived,"Server received log: ");

void commandDataLogReceived(String logId){
  stringServerLogReceived.print(Serial);
  Serial.println(logId);
}



void commandTestZones(String params){
  
}
