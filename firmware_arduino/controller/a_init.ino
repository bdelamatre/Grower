/********************************************
  Initialization Functions
********************************************/

#if DEBUG ==  true && USE_MODULE_SERIALMONITOR == true
  //we store strings that are used for debugging in flash to save on memmory
  FLASH_STRING(stringOk,"OK");
  FLASH_STRING(stringSpaces,"  ");
  FLASH_STRING(stringBannerItemSpace,"||    ");
  FLASH_STRING(stringBannerSubitemSpace,"||    ");
  FLASH_STRING(stringDashSpace," - ");
  FLASH_STRING(stringComma,",");
  FLASH_STRING(stringBracketLeft,"(");
  FLASH_STRING(stringBrackerRight,")");
#endif

#if USE_MODULE_ETHERNETCOM == true

  #if DEBUG ==  true && USE_MODULE_SERIALMONITOR == true  
    FLASH_STRING(stringInitEthernet,"|| Ethernet...");  
    FLASH_STRING(stringManual,"Manual...");  
    FLASH_STRING(stringDhcp,"DHCP...");  
    FLASH_STRING(stringFail,"FAIL");
  #endif
  
  void initEthernet(){
  
    #if DEBUG == true && USE_MODULE_SERIALMONITOR == true  
      stringInitEthernet.print(SERIALMONITOR);
    #endif
      
    boolean ethernetStarted = false;
    if(configStore.dhcp==false){
      #if DEBUG == true && USE_MODULE_SERIALMONITOR == true  
        stringManual.print(SERIALMONITOR);
      #endif    
      Ethernet.begin(configStore.mac,configStore.address);
      ethernetStarted = true;
      SERIALMONITOR.print(Ethernet.localIP());
    }else{
      #if DEBUG == true && USE_MODULE_SERIALMONITOR == true  
        stringDhcp.print(SERIALMONITOR);
      #endif    
      if(Ethernet.begin(configStore.mac)!=0){
        ethernetStarted = true;
        SERIALMONITOR.print(Ethernet.localIP());
      }else{
        #if DEBUG == true && USE_MODULE_SERIALMONITOR == true  
          stringFail.print(SERIALMONITOR);
        #endif
      }
    }
    
    #if DEBUG == true && USE_MODULE_SERIALMONITOR == true  
      SERIALMONITOR.println();
    #endif
    
    //fix-me: add support to show settings using a web server
    //SERIALMONITOR.print("|| Web Server...");
    if(ethernetStarted==true){
      
      //W5100.setRetransmissionTime(0x07D0);
      //W5100.setRetransmissionCount(4);
      
      //server.begin();
      //SERIALMONITOR.print("OK");
    }else{
      //SERIALMONITOR.print("FAIL (no ethernet)");
    }
    //SERIALMONITOR.println();
  
  }
  
#endif
    
  
#if USE_MODULE_SD == true

#if DEBUG ==  true && USE_MODULE_SERIALMONITOR == true  
  FLASH_STRING(stringInitSd,"|| SD ...");
  FLASH_STRING(stringInitSdFail,"FAIL (unable to init)");  
  FLASH_STRING(stringInitSdCardInfo,"card information:");  
#endif

void initSd(){
  
  //initialize SD
  #if DEBUG == true && USE_MODULE_SERIALMONITOR == true  
      stringInitSd.print(SERIALMONITOR);
  #endif
  
   pinMode(hardwareSelect, OUTPUT);

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    #if DEBUG == true && USE_MODULE_SERIALMONITOR == true        
      stringInitSdFail.print(SERIALMONITOR);
      SERIALMONITOR.println();
    #endif
  } else {
    #if DEBUG ==  true && USE_MODULE_SERIALMONITOR == true
      stringOk.print(SERIALMONITOR);
      stringInitSdCardInfo.print(SERIALMONITOR);
      SERIALMONITOR.print("||  type=");
      switch(card.type()) {
        case SD_CARD_TYPE_SD1:
          SERIALMONITOR.println("SD1");
          break;
        case SD_CARD_TYPE_SD2:
          SERIALMONITOR.println("SD2");
          break;
        case SD_CARD_TYPE_SDHC:
          SERIALMONITOR.println("SDHC");
          break;
        default:
          SERIALMONITOR.println("Unknown");
      }
      SERIALMONITOR.print("||  volume=");
    #endif
    if (!volume.init(card)) {
    }else{
      // print the type and size of the first FAT-type volume
      root.openRoot(volume);
      #if DEBUG == true && USE_MODULE_SERIALMONITOR == true  
        uint32_t volumesize;
        SERIALMONITOR.print("FAT ");
        SERIALMONITOR.println(volume.fatType(), DEC);
        volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
        volumesize *= volume.clusterCount();       // we'll have a lot of clusters
        volumesize *= 512;                            // SD card blocks are always 512 bytes
        volumesize /= 1024;
        volumesize /= 1024;
        SERIALMONITOR.print("||  size(MB) = ");
        SERIALMONITOR.println(volumesize);
        SERIALMONITOR.println("||  Files: ");
        // list all files in the card with date and size
        root.ls(LS_R | LS_DATE | LS_SIZE);
      #endif
      
      SD.begin(chipSelect);
    }
    
  }
  
}
#endif

#if USE_MODULE_DS1307RTC == true

  #if USE_MODULE_SERIALMONITOR == true 
  FLASH_STRING(stringInitRtc,"|| RTC...");
  FLASH_STRING(stringNotAvailable,"NOT AVAILABLE");  
  FLASH_STRING(stringFound,"FOUND");  
  //FLASH_STRING(stringManuallySettingTime,"Manually setting to compile time...");  
  #endif
  
  void initRtc(){
  
    //initialize clock
    #if USE_MODULE_SERIALMONITOR == true 
      stringInitRtc.print(SERIALMONITOR);
    #endif
    
    setSyncProvider(RTC.get);   // the function to get the time from the RTC
    
    if (timeStatus() != timeSet){ 
      #if USE_MODULE_SERIALMONITOR == true 
         stringNotAvailable.print(SERIALMONITOR);
      #endif
    }else{
      #if USE_MODULE_SERIALMONITOR == true 
         stringFound.print(SERIALMONITOR);
      #endif
      timeSyncedDateTime = now();
    }
    
    #if DEBUG == true && USE_MODULE_SERIALMONITOR == true  
      stringOk.print(SERIALMONITOR);
      stringBracketLeft.print(SERIALMONITOR);
      printDateTimeToSerial(timeSyncedDateTime);
      SERIALMONITOR.print(")");
      SERIALMONITOR.println();
    #endif
     
  }

#endif

#if DEBUG ==  true && USE_MODULE_SERIALMONITOR == true  
FLASH_STRING(stringErrorSync,"error: time hasn't been synced yet");
#endif

//fix-me: better place for this?
time_t getCurrentTime(){
  
    //if time hasn't been synced yet, than there is nothing to do.
    if(timeSynced==false){
      #if DEBUG == true && USE_MODULE_SERIALMONITOR == true  
        stringErrorSync.print(SERIALMONITOR);
        SERIALMONITOR.println();
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
  #if SENSORONLY == false
  initZones();
  initSchedules();
  #endif

}

#if DEBUG ==  true && USE_MODULE_SERIALMONITOR == true  
FLASH_STRING(stringInitSensors,"|| Initializing sensors:");
#endif

void initSensors(){
  #if DEBUG ==  true && USE_MODULE_SERIALMONITOR == true
    stringInitSensors.print(SERIALMONITOR);
    SERIALMONITOR.println();
  #endif
  for(int i=0;i<maxSensors;i++){
    #if DEBUG ==  true && USE_MODULE_SERIALMONITOR == true
      stringBannerSubitemSpace.print(SERIALMONITOR);
      SERIALMONITOR.print(i);
      stringDashSpace.print(SERIALMONITOR);      
      stringDashSpace.print(SERIALMONITOR);
    #endif
    initSensor(configStore.sensors[i]);
  }
}

#if DEBUG ==  true && USE_MODULE_SERIALMONITOR == true  
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
    #if DEBUG == true
      stringDisabled.print(SERIALMONITOR);
    #endif
  }else if(thisSensor.type==1){
    //soil moisture (analog)
    pinMode(thisSensor.pin, INPUT);
    #if DEBUG == true
      stringAnalog.print(SERIALMONITOR);
    #endif
  }else if(thisSensor.type==2){
    //DS18B20
    pinMode(thisSensor.pin, INPUT);
    #if DEBUG == true
      stringDS18B20.print(SERIALMONITOR);
    #endif
  }else if(thisSensor.type==3){
    //dht22
    pinMode(thisSensor.pin, INPUT);
    #if DEBUG == true
      stringDHT22.print(SERIALMONITOR);
    #endif
  }else if(thisSensor.type==4){
    //light
    pinMode(thisSensor.pin, INPUT);
    #if DEBUG == true
      stringLight.print(SERIALMONITOR);
    #endif
  }
  
  #if DEBUG == true
    stringDashSpace.print(SERIALMONITOR);
    SERIALMONITOR.print(thisSensor.name);
    
    if(thisSensor.type>0){
      stringInitPin.print(SERIALMONITOR);
      SERIALMONITOR.print(thisSensor.pin);
      stringInitPin2.print(SERIALMONITOR);
      SERIALMONITOR.print(thisSensor.pin2);
      stringFreqCheck.print(SERIALMONITOR);
      SERIALMONITOR.print(thisSensor.frequencyCheckSeconds);
      stringFreqLog.print(SERIALMONITOR);
      SERIALMONITOR.print(thisSensor.frequencyLogSeconds);
      SERIALMONITOR.print(")");
    }
    
    SERIALMONITOR.println();
  #endif

}

#if DEBUG ==  true && USE_MODULE_SERIALMONITOR == true  
FLASH_STRING(stringInitZones,"|| Initializing zones:");
FLASH_STRING(stringInitRelay,"5V Relay");
#endif

void initZones(){
  #if DEBUG == true && USE_MODULE_SERIALMONITOR == true  
    stringInitZones.print(SERIALMONITOR);
    SERIALMONITOR.println();
  #endif
  for(int i=0;i<maxZones;i++){
    #if DEBUG == true && USE_MODULE_SERIALMONITOR == true  
      stringBannerSubitemSpace.print(SERIALMONITOR);
      SERIALMONITOR.print(i);
      stringDashSpace.print(SERIALMONITOR);
    #endif
    initZone(configStore.zones[i]);
  }
}

void initZone(struct Zone &thisZone){

  if(thisZone.type==0){
    //sensor off
    #if DEBUG == true
      stringDisabled.print(SERIALMONITOR);
    #endif
  }else if(thisZone.type==1){
    #if DEBUG == true
      stringInitRelay.print(SERIALMONITOR);
    #endif
    pinMode(thisZone.pin, OUTPUT);
  }
 
  #if DEBUG == true
    stringDashSpace.print(SERIALMONITOR);
    SERIALMONITOR.print(thisZone.name);
    SERIALMONITOR.println();
  #endif
  
}

#if DEBUG ==  true && USE_MODULE_SERIALMONITOR == true  
FLASH_STRING(stringInitSchedules,"|| Initializing schedules:");
FLASH_STRING(stringInitTimer,"Timer");
FLASH_STRING(stringInitMoisture,"Soil Moisture");
FLASH_STRING(stringInitTemperature,"Temperature");
FLASH_STRING(stringInitSchSensors," (sensors=");
FLASH_STRING(stringInitSchZones," zones=");
#endif

void initSchedules(){
  #if DEBUG ==  true && USE_MODULE_SERIALMONITOR == true
    stringInitSchedules.print(SERIALMONITOR);
    SERIALMONITOR.println();
  #endif
  for(int i=0;i<maxSchedules;i++){
    #if DEBUG ==  true && USE_MODULE_SERIALMONITOR == true
      stringBannerSubitemSpace.print(SERIALMONITOR);
      SERIALMONITOR.print(i);
      stringDashSpace.print(SERIALMONITOR);
    #endif
    initSchedule(configStore.schedules[i]);
  }
}

void initSchedule (struct Schedule &thisSchedule){
    
  if(thisSchedule.type==0){
    //sensor off
    #if DEBUG == true
      stringDisabled.print(SERIALMONITOR);
    #endif
  }else if(thisSchedule.type==1){
    #if DEBUG == true
      stringInitTimer.print(SERIALMONITOR);
    #endif
  }else if(thisSchedule.type==2){
    #if DEBUG == true
      stringInitMoisture.print(SERIALMONITOR);
    #endif
  }else if(thisSchedule.type==3){
    #if DEBUG == true
      stringInitTemperature.print(SERIALMONITOR);
    #endif
  }
  
  //0=off, 1=timer, 2=soil moisture, 3=temperature
  
  #if DEBUG == true
    if(thisSchedule.type!=0){
      stringDashSpace.print(SERIALMONITOR);
      SERIALMONITOR.print(thisSchedule.name); 
      stringInitSchSensors.print(SERIALMONITOR);
      for(int i=0;i<maxSensors;i++){
        int thisSensorId = thisSchedule.sensors[i];
        if(thisSensorId > -1){
          //we subtract one because this is the actual ID
          SERIALMONITOR.print(configStore.sensors[thisSensorId].name);
          stringComma.print(SERIALMONITOR);
        }
      }
        
      stringInitSchZones.print(SERIALMONITOR);
      for(int i=0;i<maxZones;i++){
        int thisZoneId = thisSchedule.zones[i];
        //SERIALMONITOR.println(thisZoneId);
        if(thisZoneId > -1){
          //we subtract one because this is the actual ID
          SERIALMONITOR.print(configStore.zones[thisZoneId].name);
          stringComma.print(SERIALMONITOR);
        }
      }
      
      SERIALMONITOR.print(",days=");
      SERIALMONITOR.print(thisSchedule.timerStartWeekdays);
      SERIALMONITOR.print(",hours=");
      SERIALMONITOR.print(thisSchedule.timerStartHours);
      SERIALMONITOR.print(",minutes=");
      SERIALMONITOR.print(thisSchedule.timerStartMinutes);
      
      SERIALMONITOR.print(")");
    
    }
    SERIALMONITOR.println();
  #endif
  
}

