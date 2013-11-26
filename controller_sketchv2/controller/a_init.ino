/********************************************
  Initialization Functions
********************************************/


FLASH_STRING(stringOk,"OK");


FLASH_STRING(stringInitImp,"|| IMP...");

void initElectricImp(){
  
    #if defined(DEBUG)
      stringInitImp.print(Serial);
    #endif
    
    //fix-me: we are just assuming everything is okay for now
    
    #if defined(DEBUG)  
      stringOk.print(Serial);
      Serial.println();
    #endif

}
  
FLASH_STRING(stringInitSd,"|| SD ...");
FLASH_STRING(stringInitSdFail,"FAIL (unable to init)");  
FLASH_STRING(stringInitSdCardInfo,"card information:");  
  
void initSd(){
  
  //initialize SD
  #if defined(DEBUG)  
      stringInitSd.print(Serial);
  #endif
  
  pinMode(hardwareSelect, OUTPUT);
  //pinMode(10, OUTPUT);

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    #if defined(DEBUG)
      stringInitSdFail.print(Serial);
      Serial.println();
    #endif
  } else {
    #if defined(DEBUG)
     
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
      #if defined(DEBUG)
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


FLASH_STRING(stringInitRtc,"|| RTC...");
FLASH_STRING(stringNotAvailable,"NOT AVAILABLE");  
FLASH_STRING(stringManuallySettingTime,"Manually setting to compile time...");  

void initRtc(){

  //initialize clock
  #if defined(DEBUG)  
    stringInitRtc.print(Serial);
  #endif
  
  Wire.begin();
  RTC.begin();
    
  //if not available, nothing to do
  if(!RTC.isrunning()){
    #if defined(DEBUG)
      stringNotAvailable.print(Serial);
      Serial.println();
    #endif
  }else{
    
    //RTC is available, lets go ahead and indicate that the time is synced
    timeSynced = true;
    timeSyncedDateTime =  getLocalTime();
    
      #if defined(SETTIME)
          #if defined(DEBUG)
            stringManuallySettingTime.print(Serial);
          #endif
          // following line sets the RTC to the date & time this sketch was compiled
          RTC.adjust(DateTime(__DATE__, __TIME__));
      #endif
    
    #if defined(DEBUG) 
      stringOk.print(Serial);
      Serial.print("(");  
      printDateTimeToSerial(timeSyncedDateTime);
      Serial.print(")");
      Serial.println();
    #endif
  }
   
}


FLASH_STRING(stringErrorSync,"error: time hasn't been synced yet");

//fix-me: better place for this?
DateTime getLocalTime(){
  
    //if time hasn't been synced yet, than there is nothing to do.
    if(timeSynced==false){
      #if defined(DEBUG)
        stringErrorSync.print(Serial);
        Serial.println();
      #endif
    }else{
    
      //otherwise, if RTC isn't running use the synced time
      if(!RTC.isrunning()){
        
          int secondsSinceSync = (millis()/1000) - timeAtSync;
          
          //resync every 5 minutes
          if(secondsSinceSync>(60*5)){
            //and initiate sync again
            sendCommand("config:set-time>");
          }
          
          //get the last synced time
          return DateTime(timeSyncedDateTime.unixtime()+secondsSinceSync);
          
      //or get the synced time directly from the RTC
      }else{
        //return DateTime(RTC.now().unixtime() + (config.utcOffset*60*60));
        return DateTime(RTC.now().unixtime());
      }

    }
  
}

FLASH_STRING(stringInitSensors,"Initializing sensors:");
FLASH_STRING(stringSpaces,"  ");

void initSensors(){
  #if defined(DEBUG)
    stringInitSensors.print(Serial);
    Serial.println();
  #endif
  for(int i=0;i>maxSensors;i++){
    initSensor(config.sensors[i]);
  }
}

void initSensor(struct Sensor &thisSensor){

  if(thisSensor.type==0){
    //zone off
  }else if(thisSensor.type==1){
    #if defined(DEBUG)
      stringSpaces.print(Serial);
      Serial.print(thisSensor.name);
    #endif
    pinMode(thisSensor.pin, INPUT);
  }else if(thisSensor.type==2){
    #if defined(DEBUG)
      stringSpaces.print(Serial);
      Serial.print(thisSensor.name);
    #endif
    //DS18B20
    pinMode(thisSensor.pin, INPUT);
  }else if(thisSensor.type==3){
    #if defined(DEBUG)
      stringSpaces.print(Serial);
      Serial.print(thisSensor.name);
    #endif
    //dht22
    pinMode(thisSensor.pin, INPUT);
  }else if(thisSensor.type==4){
    #if defined(DEBUG)
      stringSpaces.print(Serial);
      Serial.print(thisSensor.name);
    #endif
    pinMode(thisSensor.pin, INPUT);
  }

}

void initZones(){
  for(int i=0;i<maxZones;i++){
    initZone(config.zones[i]);
  }
}

void initZone(struct Zone &thisZone){
  
  if(thisZone.type==0){
    //sensor off
  }else if(thisZone.type==1){
    pinMode(thisZone.pin, OUTPUT);
  }
  
}

