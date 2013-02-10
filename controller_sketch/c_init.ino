/********************************************
  Initialization Functions
********************************************/
        
void initRtc(){

  //initialize clock
  #if defined DEBUG  
    Serial.print("|| RTC...");
  #endif
  
  Wire.begin();
  RTC.begin();
  
  #if defined SETTIME
  if (! RTC.isrunning()) {
    Serial.print("Manually setting...");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  #endif
  
  
  #if defined DEBUG 
    if(!RTC.isrunning()){
      rtcHasConfig = false;
      Serial.println("NOT AVAILABLE");
    }else{
      rtcHasConfig = true;
      Serial.print("OK (");  
      printDateTimeToSerial(getLocalTime());
      Serial.print(")");
      Serial.println();
    }
  #endif

}

//fix-me: better place for this?
DateTime getLocalTime(){
  if(rtcHasConfig==false){
    sendNTPpacket(config.ntpServer);
    delay(1000);
    unsigned long timeSyncedUnix = readNTPpacket();
    if(timeSyncedUnix<1){
    }else{
      return DateTime(timeSyncedUnix + (config.utcOffset*60*60));
    }
  }else{
    
    return DateTime(RTC.now().unixtime() + (config.utcOffset*60*60));
  }
}

void initEthernet(){

  //initialize ethernet
  #if defined DEBUG  
    Serial.print("|| TCP...");
  #endif
  
  if(config.hasEthernet){

    if(config.dhcp==true){
      ethernetHasConfig = Ethernet.begin(config.hardwareMac);
    }else{
      ethernetHasConfig = 1;
      Ethernet.begin(config.hardwareMac,config.clientAddress,config.clientDns,config.clientGateway,config.clientNetmask);
    }
    
    if(ethernetHasConfig==1){
        
        //show ip
        #if defined DEBUG  
          Serial.print("OK (");
          printIpToSerial(Ethernet.localIP());
          Serial.print(")");
          Serial.println();
        #endif;
        
        //initialize udp
        Udp.begin(udpPort);
        #if defined DEBUG  
          Serial.print("|| UDP...");
          Serial.print("OK");
          Serial.println();
        #endif
        
        //initialize webserver
        webserver.setDefaultCommand(&htmlStatus);
        webserver.addCommand("config", &htmlConfig);
        webserver.addCommand("config-general", &htmlConfigGeneral);
        webserver.addCommand("config-sensors", &htmlConfigSensors);
        webserver.addCommand("config-zones", &htmlConfigZones);
        webserver.addCommand("config-schedules", &htmlConfigSchedules);
        webserver.addCommand("zone-log.csv", &zoneLog);
        webserver.addCommand("sensor-log.csv", &sensorLog);
        webserver.begin();
        
        #if defined DEBUG  
          Serial.print("|| WEB...");
          Serial.print("OK");
          Serial.println();
        #endif

    }
        
  }else{
    #if defined DEBUG  
      Serial.println("DISABLED");
    #endif
  }
    
}

void initNtp(){

  //initialize NTP
  #if defined DEBUG  
    Serial.print("|| NTP (");
    printIpToSerial(config.ntpServer);
    Serial.print(")...");
  #endif
  
  if(config.ntp==false){
    #if defined DEBUG  
      Serial.println("DISABLED");
    #endif
  }else if(config.ntp==true && ethernetHasConfig==1){
    
    sendNTPpacket(config.ntpServer);
    delay(1000);
    unsigned long timeSyncedUnix = readNTPpacket();
    if(timeSyncedUnix<1){
      #if defined DEBUG  
        Serial.print("FAIL (invalid time) ");
        Serial.println(timeSyncedUnix);
      #endif
    }else{
      RTC.adjust(DateTime(timeSyncedUnix)); //adjusts RTC time to NTP time
      DateTime timeLocal = DateTime(timeSyncedUnix + (config.utcOffset*60*60));
      #if defined DEBUG  
          Serial.print("OK (");
          Serial.print("Synced to ");
          printDateTimeToSerial(timeLocal);
          Serial.println(")");
      #endif  
    }
    
  }else{
    #if defined DEBUG  
      Serial.println("FAIL (no ethernet)");
    #endif
  }

}

void initSd(){
  
  //initialize SD
  #if defined DEBUG  
    Serial.print("|| SD ...");
  #endif
  pinMode(hardwareSelect, OUTPUT);

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    #if defined DEBUG
      Serial.println("FAIL (unable to init)");
    #endif
  } else {
    #if defined DEBUG
      Serial.println("OK, card information:");
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
      #if defined DEBUG  
        Serial.println("unformatted");
      #endif
    }else{
      // print the type and size of the first FAT-type volume
      root.openRoot(volume);
      #if defined DEBUG
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

void initSensors(){
  for(int i=0;i>maxSensors;i++){
    initSensor(config.sensors[i]);
  }
}

void initSensor(struct Sensor &thisSensor){

  if(thisSensor.type==0){
    //zone off
  }else if(thisSensor.type==1){
    pinMode(thisSensor.pin, INPUT);
  }else if(thisSensor.type==2){
    //DS18B20
    pinMode(thisSensor.pin, INPUT);
  }else if(thisSensor.type==3){
    //dht22
    pinMode(thisSensor.pin, INPUT);
  }else if(thisSensor.type==4){
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


