/********************************************
  Initialization Functions
********************************************/

#if defined(DEBUG) && defined(USESERIALMONITOR)  
  FLASH_STRING(stringOk,"OK");
  FLASH_STRING(stringSpaces,"  ");
  FLASH_STRING(stringBannerItemSpace,"||    ");
  FLASH_STRING(stringBannerSubitemSpace,"||    ");
  
  FLASH_STRING(stringDashSpace," - ");
  FLASH_STRING(stringComma,",");
  FLASH_STRING(stringBracketLeft,"(");
  FLASH_STRING(stringBrackerRight,")");
#endif

#if defined(USEETHERNETCOM) 

#if defined(DEBUG) && defined(USESERIALMONITOR)  
  FLASH_STRING(stringInitEthernet,"|| Ethernet...");  
  FLASH_STRING(stringManual,"Manual...");  
  FLASH_STRING(stringDhcp,"DHCP...");  
  FLASH_STRING(stringFail,"FAIL");
#endif

void initEthernet(){

  #if defined(DEBUG) && defined(USESERIALMONITOR)  
    stringInitEthernet.print(Serial);
  #endif
    
  boolean ethernetStarted = false;
  if(configStore.dhcp==false){
    #if defined(DEBUG) && defined(USESERIALMONITOR)  
      stringManual.print(Serial);
    #endif    
    Ethernet.begin(configStore.mac,configStore.address);
    ethernetStarted = true;
    Serial.print(Ethernet.localIP());
  }else{
    #if defined(DEBUG) && defined(USESERIALMONITOR)  
      stringDhcp.print(Serial);
    #endif    
    if(Ethernet.begin(configStore.mac)!=0){
      ethernetStarted = true;
      Serial.print(Ethernet.localIP());
    }else{
      #if defined(DEBUG) && defined(USESERIALMONITOR)  
        stringFail.print(Serial);
      #endif
    }
  }
  
  Serial.println();
  //Serial.print("|| Web Server...");
  if(ethernetStarted==true){
    
    //W5100.setRetransmissionTime(0x07D0);
    //W5100.setRetransmissionCount(4);
    
    //server.begin();
    //Serial.print("OK");
  }else{
    //Serial.print("FAIL (no ethernet)");
  }
  //Serial.println();

}
#endif
    
  
#if defined(USESD)

#if defined(DEBUG) && defined(USESERIALMONITOR)  
  FLASH_STRING(stringInitSd,"|| SD ...");
  FLASH_STRING(stringInitSdFail,"FAIL (unable to init)");  
  FLASH_STRING(stringInitSdCardInfo,"card information:");  
#endif

void initSd(){
  
  //initialize SD
  #if defined(DEBUG) && defined(USESERIALMONITOR)  
      stringInitSd.print(Serial);
  #endif
  
   pinMode(hardwareSelect, OUTPUT);
  //pinMode(10, OUTPUT);

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    #if defined(DEBUG) && defined(USESERIALMONITOR)        
      stringInitSdFail.print(Serial);
      Serial.println();
    #endif
  } else {
    #if defined(DEBUG) && defined(USESERIALMONITOR)
      stringOk.print(Serial);
      stringInitSdCardInfo.print(Serial);
      Serial.print("||  type=");
      switch(card.type()) {
        case SD_CARD_TYPE_SD1:
          Serial.println("SD1");
          break;
        case SD_CARD_TYPE_SD2:
          Serial.println("SD2");
          break;
        case SD_CARD_TYPE_SDHC:
          Serial.println("SDHC");
          break;
        default:
          Serial.println("Unknown");
      }
      Serial.print("||  volume=");
    #endif
    if (!volume.init(card)) {
    }else{
      // print the type and size of the first FAT-type volume
      root.openRoot(volume);
      #if defined(DEBUG) && defined(USESERIALMONITOR)  
        uint32_t volumesize;
        Serial.print("FAT ");
        Serial.println(volume.fatType(), DEC);
        volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
        volumesize *= volume.clusterCount();       // we'll have a lot of clusters
        volumesize *= 512;                            // SD card blocks are always 512 bytes
        volumesize /= 1024;
        volumesize /= 1024;
        Serial.print("||  size(MB) = ");
        Serial.println(volumesize);
        Serial.println("||  Files: ");
        // list all files in the card with date and size
        root.ls(LS_R | LS_DATE | LS_SIZE);
      #endif
      
      SD.begin(chipSelect);
    }
    
  }
  
}
#endif

#if defined(USESERIALMONITOR) 
FLASH_STRING(stringInitRtc,"|| RTC...");
FLASH_STRING(stringNotAvailable,"NOT AVAILABLE");  
FLASH_STRING(stringFound,"FOUND");  
//FLASH_STRING(stringManuallySettingTime,"Manually setting to compile time...");  
#endif

void initRtc(){

  //initialize clock
  #if defined(USESERIALMONITOR) 
    stringInitRtc.print(Serial);
  #endif
  
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  
  if (timeStatus() != timeSet){ 
    #if defined(USESERIALMONITOR) 
       stringNotAvailable.print(Serial);
    #endif
  }else{
    #if defined(USESERIALMONITOR) 
       stringFound.print(Serial);
    #endif
    timeSyncedDateTime = now();
  }
  
  #if defined(DEBUG) && defined(USESERIALMONITOR)  
    stringOk.print(Serial);
    stringBracketLeft.print(Serial);
    printDateTimeToSerial(timeSyncedDateTime);
    Serial.print(")");
    Serial.println();
  #endif
   
}

#if defined(DEBUG) && defined(USESERIALMONITOR)  
FLASH_STRING(stringErrorSync,"error: time hasn't been synced yet");
#endif

//fix-me: better place for this?
time_t getCurrentTime(){
  
    //if time hasn't been synced yet, than there is nothing to do.
    if(timeSynced==false){
      #if defined(DEBUG) && defined(USESERIALMONITOR)  
        stringErrorSync.print(Serial);
        Serial.println();
      #endif
    }else{
      
      time_t currentTime = now();
      
      int secondsSinceSync = currentTime - timeSyncedDateTime;
                
      //resync every 5 minutes
      if(timeSyncInProgress==false && secondsSinceSync>timeSyncDelay){
        //and initiate sync again
        sendCommand("c:time");
      }
      
      //get the last synced time
      return currentTime;
  
    }
    
}


void initController(){
 
  initSensors();
  #if !defined(SENSORONLY)
  initZones();
  initSchedules();
  #endif

}

#if defined(DEBUG) && defined(USESERIALMONITOR)  
FLASH_STRING(stringInitSensors,"|| Initializing sensors:");
#endif

void initSensors(){
  #if defined(DEBUG) && defined(USESERIALMONITOR)
    stringInitSensors.print(Serial);
    Serial.println();
  #endif
  for(int i=0;i<maxSensors;i++){
    #if defined(DEBUG) && defined(USESERIALMONITOR)
      stringBannerSubitemSpace.print(Serial);
      Serial.print(i);
      stringDashSpace.print(Serial);      
      stringDashSpace.print(Serial);
    #endif
    initSensor(configStore.sensors[i]);
  }
}

#if defined(DEBUG) && defined(USESERIALMONITOR)  
FLASH_STRING(stringDisabled,"Disabled");
FLASH_STRING(stringAnalog,"Analog");
FLASH_STRING(stringDS18B20,"DS18B20");
FLASH_STRING(stringDHT22,"DHT22");
FLASH_STRING(stringLight,"Light");
FLASH_STRING(stringInitPin," (pin=");
FLASH_STRING(stringInitPin2,", pin2=");
FLASH_STRING(stringFreqCheck,", frequencyCheckSeconds=");
FLASH_STRING(stringFreqLog,", frequencyLogSeconds=");
#endif

void initSensor(struct Sensor &thisSensor){
 
  
  if(thisSensor.type==0){
    //zone off
    #if defined(DEBUG)
      stringDisabled.print(Serial);
    #endif
  }else if(thisSensor.type==1){
    //soil moisture (analog)
    pinMode(thisSensor.pin, INPUT);
    #if defined(DEBUG)
      stringAnalog.print(Serial);
    #endif
  }else if(thisSensor.type==2){
    //DS18B20
    pinMode(thisSensor.pin, INPUT);
    #if defined(DEBUG)
      stringDS18B20.print(Serial);
    #endif
  }else if(thisSensor.type==3){
    //dht22
    pinMode(thisSensor.pin, INPUT);
    #if defined(DEBUG)
      stringDHT22.print(Serial);
    #endif
  }else if(thisSensor.type==4){
    //light
    pinMode(thisSensor.pin, INPUT);
    #if defined(DEBUG)
      stringLight.print(Serial);
    #endif
  }
  
  #if defined(DEBUG)
    stringDashSpace.print(Serial);
    Serial.print(thisSensor.name);
    
    if(thisSensor.type>0){
      stringInitPin.print(Serial);
      Serial.print(thisSensor.pin);
      stringInitPin2.print(Serial);
      Serial.print(thisSensor.pin2);
      stringFreqCheck.print(Serial);
      Serial.print(thisSensor.frequencyCheckSeconds);
      stringFreqLog.print(Serial);
      Serial.print(thisSensor.frequencyLogSeconds);
      Serial.print(")");
    }
    
    Serial.println();
  #endif

}

#if defined(DEBUG) && defined(USESERIALMONITOR)  
FLASH_STRING(stringInitZones,"|| Initializing zones:");
FLASH_STRING(stringInitRelay,"5V Relay");
#endif

void initZones(){
  #if defined(DEBUG) && defined(USESERIALMONITOR)  
    stringInitZones.print(Serial);
    Serial.println();
  #endif
  for(int i=0;i<maxZones;i++){
    #if defined(DEBUG) && defined(USESERIALMONITOR)  
      stringBannerSubitemSpace.print(Serial);
      Serial.print(i);
      stringDashSpace.print(Serial);
    #endif
    initZone(configStore.zones[i]);
  }
}

void initZone(struct Zone &thisZone){

  if(thisZone.type==0){
    //sensor off
    #if defined(DEBUG)
      stringDisabled.print(Serial);
    #endif
  }else if(thisZone.type==1){
    #if defined(DEBUG)
      stringInitRelay.print(Serial);
    #endif
    pinMode(thisZone.pin, OUTPUT);
  }
 
  #if defined(DEBUG)
    stringDashSpace.print(Serial);
    Serial.print(thisZone.name);
    Serial.println();
  #endif
  
}

#if defined(DEBUG) && defined(USESERIALMONITOR)  
FLASH_STRING(stringInitSchedules,"|| Initializing schedules:");
FLASH_STRING(stringInitTimer,"Timer");
FLASH_STRING(stringInitMoisture,"Soil Moisture");
FLASH_STRING(stringInitTemperature,"Temperature");
FLASH_STRING(stringInitSchSensors," (sensors=");
FLASH_STRING(stringInitSchZones," zones=");
#endif

void initSchedules(){
  #if defined(DEBUG) && defined(USESERIALMONITOR)
    stringInitSchedules.print(Serial);
    Serial.println();
  #endif
  for(int i=0;i<maxSchedules;i++){
    #if defined(DEBUG) && defined(USESERIALMONITOR)
      stringBannerSubitemSpace.print(Serial);
      Serial.print(i);
      stringDashSpace.print(Serial);
    #endif
    initSchedule(configStore.schedules[i]);
  }
}

void initSchedule (struct Schedule &thisSchedule){
    
  if(thisSchedule.type==0){
    //sensor off
    #if defined(DEBUG)
      stringDisabled.print(Serial);
    #endif
  }else if(thisSchedule.type==1){
    #if defined(DEBUG)
      stringInitTimer.print(Serial);
    #endif
  }else if(thisSchedule.type==2){
    #if defined(DEBUG)
      stringInitMoisture.print(Serial);
    #endif
  }else if(thisSchedule.type==3){
    #if defined(DEBUG)
      stringInitTemperature.print(Serial);
    #endif
  }
  
  //0=off, 1=timer, 2=soil moisture, 3=temperature
  
  #if defined(DEBUG)
    if(thisSchedule.type!=0){
      stringDashSpace.print(Serial);
      Serial.print(thisSchedule.name); 
      stringInitSchSensors.print(Serial);
      for(int i=0;i<maxSensors;i++){
        int thisSensorId = thisSchedule.sensors[i];
        if(thisSensorId > -1){
          //we subtract one because this is the actual ID
          Serial.print(configStore.sensors[thisSensorId].name);
          stringComma.print(Serial);
        }
      }
        
      stringInitSchZones.print(Serial);
      for(int i=0;i<maxZones;i++){
        int thisZoneId = thisSchedule.zones[i];
        //Serial.println(thisZoneId);
        if(thisZoneId > -1){
          //we subtract one because this is the actual ID
          Serial.print(configStore.zones[thisZoneId].name);
          stringComma.print(Serial);
        }
      }
      
      Serial.print(",days=");
      Serial.print(thisSchedule.timerStartWeekdays);
      Serial.print(",hours=");
      Serial.print(thisSchedule.timerStartHours);
      Serial.print(",minutes=");
      Serial.print(thisSchedule.timerStartMinutes);
      
      Serial.print(")");
    
    }
    Serial.println();
  #endif
  
}

