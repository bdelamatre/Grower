/********************************************
  Initialization Functions
********************************************/


void initEthernet(){

  Serial.print("|| Ethernet...");
    
  boolean ethernetStarted = false;
  if(configStore.dhcp==false){
    Serial.print("Manual...");
    Ethernet.begin(configStore.mac,configStore.address);
    ethernetStarted = true;
    Serial.print(Ethernet.localIP());
  }else{
    Serial.print("DHCP...");
    if(Ethernet.begin(configStore.mac)!=0){
      ethernetStarted = true;
      Serial.print(Ethernet.localIP());
    }else{
      Serial.print("FAIL");
    }
  }
  
  Serial.println();
  Serial.print("|| Web Server...");
  if(ethernetStarted==true){
    
    W5100.setRetransmissionTime(0x07D0);
    W5100.setRetransmissionCount(4);
    
    //server.begin();
    Serial.print("OK");
  }else{
    Serial.print("FAIL (no ethernet)");
  }
  Serial.println();

}

#if defined(DEBUG) && defined(USESERIALMONITOR)  
  FLASH_STRING(stringOk,"OK");
  FLASH_STRING(stringSpaces,"  ");
  FLASH_STRING(stringBannerItemSpace,"||    ");
  FLASH_STRING(stringBannerSubitemSpace,"||    ");
    
  FLASH_STRING(stringInitSd,"|| SD ...");
  FLASH_STRING(stringInitSdFail,"FAIL (unable to init)");  
  FLASH_STRING(stringInitSdCardInfo,"card information:");  
#endif
  
#if defined(USESD)
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

#if defined(DEBUG) && defined(USESERIALMONITOR) 
FLASH_STRING(stringInitRtc,"|| RTC...");
FLASH_STRING(stringNotAvailable,"NOT AVAILABLE");  
FLASH_STRING(stringManuallySettingTime,"Manually setting to compile time...");  
#endif

void initRtc(){

  //initialize clock
  #if defined(DEBUG) && defined(USESERIALMONITOR)  
    stringInitRtc.print(Serial);
  #endif
  
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  
  if (timeStatus() != timeSet){ 
     Serial.print("UNAVAILABLE");
  }else{
     Serial.print("FOUND");
     timeSyncedDateTime = now();
  }
  
  #if defined(DEBUG) && defined(USESERIALMONITOR)  
    stringOk.print(Serial);
    Serial.print("(");  
    printDateTimeToSerial(timeSyncedDateTime);
    Serial.print(")");
    Serial.println();
  #endif
 
  //Wire.begin();
  //RTC.begin();
    
  //if not available, nothing to do
  /*if(!RTC.isrunning()){
    #if defined(DEBUG)
      stringNotAvailable.print(Serial);
      Serial.println();
    #endif
  }else{*/
    
    //RTC is available, lets go ahead and indicate that the time is synced
    //timeSynced = true;
    //timeSyncedDateTime =  getLocalTime();
    
      /*#if defined(SETTIME)
          #if defined(DEBUG) && defined(USESERIALMONITOR)  
            stringManuallySettingTime.print(Serial);
          #endif
          // following line sets the RTC to the date & time this sketch was compiled
          //RTC.adjust(DateTime(__DATE__, __TIME__));
      #endif*/
    
    
  //}
   
}

#if defined(DEBUG) && defined(USESERIALMONITOR)  
FLASH_STRING(stringErrorSync,"error: time hasn't been synced yet");
#endif

//fix-me: better place for this?
time_t getLocalTime(){
  
    //if time hasn't been synced yet, than there is nothing to do.
    if(timeSynced==false){
      #if defined(DEBUG) && defined(USESERIALMONITOR)  
        stringErrorSync.print(Serial);
        Serial.println();
      #endif
    }else{
    
      //otherwise, if RTC isn't running use the synced time
      //if(!RTC.isrunning()){
        
          int secondsSinceSync = (millis()/1000) - timeAtSync;
                    
          //resync every 5 minutes
          if(timeSyncInProgress==false && secondsSinceSync>(timeSyncDelay/1000)){
            //and initiate sync again
            sendCommand("c:time>");
          }
          
          //get the last synced time
          return timeSyncedDateTime+secondsSinceSync;
          
      //or get the synced time directly from the RTC
      //}else{
        //return DateTime(RTC.now().unixtime() + (config.utcOffset*60*60));
        //return DateTime(RTC.now().unixtime());
      //}

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
  #if defined(DEBUG)
    stringInitSensors.print(Serial);
    Serial.println();
  #endif
  for(int i=0;i<maxSensors;i++){
    #if defined(DEBUG)
      stringBannerSubitemSpace.print(Serial);
      Serial.print(i);
      Serial.print(" - ");
    #endif
    initSensor(configStore.sensors[i]);
  }
}

void initSensor(struct Sensor &thisSensor){
 
  
  if(thisSensor.type==0){
    //zone off
    #if defined(DEBUG)
      Serial.print("Disabled");
    #endif
  }else if(thisSensor.type==1){
    //soil moisture (analog)
    pinMode(thisSensor.pin, INPUT);
    #if defined(DEBUG)
      Serial.print("Analog");
    #endif
  }else if(thisSensor.type==2){
    //DS18B20
    pinMode(thisSensor.pin, INPUT);
    #if defined(DEBUG)
      Serial.print("DS18B20");
    #endif
  }else if(thisSensor.type==3){
    //dht22
    pinMode(thisSensor.pin, INPUT);
    #if defined(DEBUG)
      Serial.print("DHT22");
    #endif
  }else if(thisSensor.type==4){
    //light
    pinMode(thisSensor.pin, INPUT);
    #if defined(DEBUG)
      Serial.print("Light");
    #endif
  }
  
  #if defined(DEBUG)
    Serial.print(" - ");
    Serial.print(thisSensor.name);
    
    if(thisSensor.type>0){
      Serial.print(" (");
      Serial.print("pin=");
      Serial.print(thisSensor.pin);
      Serial.print(", ");
      Serial.print("pin2=");
      Serial.print(thisSensor.pin2);
      Serial.print(", ");
      Serial.print("frequencyCheckSeconds=");
      Serial.print(thisSensor.frequencyCheckSeconds);
      Serial.print(", ");
      Serial.print("frequencyLogSeconds=");
      Serial.print(thisSensor.frequencyLogSeconds);
      Serial.print(")");
    }
    
    Serial.println();
  #endif

}

#if !defined(SENSORONLY)

#if defined(DEBUG) && defined(USESERIALMONITOR)  
FLASH_STRING(stringInitZones,"|| Initializing zones:");
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
      Serial.print(" - ");
    #endif
    initZone(configStore.zones[i]);
  }
}

void initZone(struct Zone &thisZone){

  if(thisZone.type==0){
    //sensor off
    #if defined(DEBUG)
      Serial.print("Disabled");
    #endif
  }else if(thisZone.type==1){
    #if defined(DEBUG)
      Serial.print("5V Relay");
    #endif
    pinMode(thisZone.pin, OUTPUT);
  }
 
  #if defined(DEBUG)
    Serial.print(" - ");
    Serial.print(thisZone.name);
    Serial.println();
  #endif
  
}

#if defined(DEBUG) && defined(USESERIALMONITOR)  
FLASH_STRING(stringInitSchedules,"|| Initializing schedules:");
#endif

void initSchedules(){
  #if defined(DEBUG)
    stringInitSchedules.print(Serial);
    Serial.println();
  #endif
  for(int i=0;i<maxSchedules;i++){
    #if defined(DEBUG)
      stringBannerSubitemSpace.print(Serial);
      Serial.print(i);
      Serial.print(" - ");
    #endif
    initSchedule(configStore.schedules[i]);
  }
}

void initSchedule (struct Schedule &thisSchedule){
    
  if(thisSchedule.type==0){
    //sensor off
    #if defined(DEBUG)
      Serial.print("Disabled");
    #endif
  }else if(thisSchedule.type==1){
    #if defined(DEBUG)
      Serial.print("Timer");
    #endif
  }else if(thisSchedule.type==2){
    #if defined(DEBUG)
      Serial.print("Soil Moisture");
    #endif
  }else if(thisSchedule.type==3){
    #if defined(DEBUG)
      Serial.print("Temperature");
    #endif
  }
  
  //0=off, 1=timer, 2=soil moisture, 3=temperature
  
  #if defined(DEBUG)
    if(thisSchedule.type!=0){
      Serial.print(" - ");
      Serial.print(thisSchedule.name);
      Serial.print(" (sensors=");
      for(int i=0;i<maxSensors;i++){
        int thisSensorId = thisSchedule.sensors[i];
        if(thisSensorId > -1){
          //we subtract one because this is the actual ID
          Serial.print(configStore.sensors[thisSensorId].name);
          Serial.print(",");
        }
      }
        
      Serial.print(" zones=");
      for(int i=0;i<maxZones;i++){
        int thisZoneId = thisSchedule.zones[i];
        //Serial.println(thisZoneId);
        if(thisZoneId > -1){
          //we subtract one because this is the actual ID
          Serial.print(configStore.zones[thisZoneId].name);
          Serial.print(",");
        }
      }
      
      Serial.print(")");
    
    }
    Serial.println();
  #endif
  
}
#endif

