
/**
Generic handler for sending commands out the Electric Imp
**/


//FLASH_STRING(stringSyncInProgress,"Time sync in progress");
FLASH_STRING(stringSendingCommand,"Sending command: ");

void sendCommand(String thisCommand){
  
  stringSendingCommand.print(Serial);
  Serial.println(thisCommand);
  
  //fix-me: probably a better way to handle this
  if(thisCommand=="config:set-time>"){
    //stringSyncInProgress.print(Serial);
    //Serial.println();
    timeSyncInProgress = true;
  }
  
  int commandLength = thisCommand.length();
  int i = 0;
  while(i<commandLength){
    char sendChar = thisCommand.charAt(i);
    impSerial.write(byte(sendChar));
    i++;
  }
  
  #if defined(DEBUG)
    printAvailableMemory();
  #endif
}


/**
Generic handler for executing commands received from the Electric Imp
**/

FLASH_STRING(stringExecuting,"Received command: ");
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

DateTime commandConfigSetTime(unsigned long int timeunix){
    
    //stringSettingTime.print(Serial);
    //Serial.println(timeunix);
  
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


