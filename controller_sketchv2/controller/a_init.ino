/********************************************
  Initialization Functions
********************************************/
  
 void initSd(){
  
  //initialize SD
  #if defined(DEBUG)  
    Serial.print("|| SD ...");
  #endif
  
  pinMode(hardwareSelect, OUTPUT);
  //pinMode(10, OUTPUT);

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    #if defined(DEBUG)
      Serial.println("FAIL (unable to init)");
    #endif
  } else {
    #if defined(DEBUG)
      Serial.println("OK.  card information:");
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

void initRtc(){

  //initialize clock
  #if defined(DEBUG)  
    Serial.print("|| RTC...");
  #endif
  
  Wire.begin();
  RTC.begin();
  
  #if defined(SETTIME)
    if (! RTC.isrunning()) {
      #if defined(DEBUG)
        Serial.print("Manually setting...");
      #endif
      // following line sets the RTC to the date & time this sketch was compiled
      RTC.adjust(DateTime(__DATE__, __TIME__));
    }
  #endif
    
  if(!RTC.isrunning()){
    rtcHasConfig = false;
    #if defined(DEBUG)
      Serial.println("NOT AVAILABLE");
    #endif
  }else{
    rtcHasConfig = true;
    #if defined(DEBUG)
      Serial.print("OK (");  
      printDateTimeToSerial(getLocalTime());
      Serial.print(")");
      Serial.println();
    #endif
  }
   
}

//fix-me: better place for this?
DateTime getLocalTime(){
  if(useRtc==false){
    int sinceStart = millis()/1000;
    return DateTime(sinceStart);
  }else{
    if(rtcHasConfig==false){
      /*sendNTPpacket(config.ntpServer);
      delay(1000);
      unsigned long timeSyncedUnix = readNTPpacket();
      if(timeSyncedUnix<1){
      }else{
        return DateTime(timeSyncedUnix + (config.utcOffset*60*60));
      }*/
    }else{
      //return DateTime(RTC.now().unixtime() + (config.utcOffset*60*60));
      return DateTime(RTC.now().unixtime());
    }
  }
}
