/********************************************
  Initialization Functions
********************************************/

void initRtc(boolean debugMe){

  //initialize clock
  if(debugMe){
    Serial.print("|| RTC...");
  }
  
  Wire.begin();
  RTC.begin();
  
  if(debugMe){
    Serial.print("OK (");
    Serial.print(getDateTime(getLocalTime()));
    Serial.print(")");
    Serial.println();
  }

}

//fix-me: better place for this?
DateTime getLocalTime(){
  return DateTime(RTC.now().unixtime() + (config.utcOffset*60*60));
}

void initEthernet(boolean debugMe){

  //initialize ethernet
  if(debugMe){
    Serial.print("|| TCP...");
  }
  
  if(debugMe==false || (debugMe=true && debugNoEthernet==false)){

    if(config.dhcp==true){
      ethernetHasConfig = Ethernet.begin(config.hardwareMac);
    }else{
      ethernetHasConfig = 1;
      Ethernet.begin(config.hardwareMac,config.clientAddress,config.clientGateway,config.clientNetmask);
    }
    
    if(ethernetHasConfig==1){
        
        //show ip
        if(debugMe){
          Serial.print("OK (");
          Serial.print(getIp());
          Serial.print(")");
          Serial.println();
        }
        
        //initialize udp
        Udp.begin(udpPort);
        if(debugMe){
          Serial.print("|| UDP...");
          Serial.print("OK");
          Serial.println();
        }
        
        //initialize webserver
        webserver.setDefaultCommand(&htmlStatus);
        webserver.addCommand("status.html", &htmlStatus);
        webserver.addCommand("sensor-log.csv", &sensorLog);
        webserver.addCommand("zone-log.csv", &zoneLog);
        webserver.begin();
        if(debugMe){
          Serial.print("|| WEB...");
          Serial.print("OK");
          Serial.println();
        }
        
      }else{
        if(debugMe){
          Serial.println("FAIL");
        }
      }
  
  }else{
      if(debugMe){
         Serial.println("OFF");
      }
  }
    
}

void initNtp(boolean debugMe){

  //initialize NTP
  if(debugMe){
    Serial.print("|| NTP...");
  }
  
  if(ethernetHasConfig==1){
    
    sendNTPpacket(config.ntpServer);
    delay(5000);
    unsigned long timeSyncedUnix = readNTPpacket();
    if(timeSyncedUnix<1){
      if(debugMe){
        Serial.print("FAIL (invalid time)");
      }
    }else{
      RTC.adjust(DateTime(timeSyncedUnix)); //adjusts RTC time to NTP time
      DateTime timeLocal = DateTime(timeSyncedUnix + (config.utcOffset*60*60));
      if(debugMe){
          Serial.print("OK (");
          Serial.print("Synced to ");
          Serial.print(getDateTime(timeLocal));
          Serial.print(")");
      }   
    }
    
  }else{
    if(debugMe){
      Serial.print("FAIL (no ethernet)");
    }
  }
  if(debugMe){
    Serial.println();
  }

}

void initSd(boolean debugMe){
  
  //initialize SD
  if(debugMe){
    Serial.print("|| SD ...");
  }
  pinMode(hardwareSelect, OUTPUT);

  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    if(debugMe){
      Serial.println("FAIL (unable to init)");
    }
  } else {
    if(debugMe){
      Serial.println("OK, card information:");
      Serial.print("type=");
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
      Serial.print("volume=");
    }
    if (!volume.init(card)) {
      if(debugMe){
        Serial.println("unformatted");
      }
    }else{
      // print the type and size of the first FAT-type volume
       root.openRoot(volume);
      if(debugMe){
        uint32_t volumesize;
        Serial.print("FAT ");
        Serial.println(volume.fatType(), DEC);
      
        volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
        volumesize *= volume.clusterCount();       // we'll have a lot of clusters
        volumesize *= 512;                            // SD card blocks are always 512 bytes
        volumesize /= 1024;
        volumesize /= 1024;
        Serial.print("size(MB) = ");
        Serial.println(volumesize);
      
        Serial.println("Files: ");
        
        // list all files in the card with date and size
        root.ls(LS_R | LS_DATE | LS_SIZE);
      
      }
      
      SD.begin(chipSelect);
    }
    
  }
  
}

void initSensors(boolean debugMe){
  for(int i=0;i>maxSensors;i++){
    initSensor(config.sensors[i],debugMe);
  }
}

void initSensor(struct Sensor &thisSensor,boolean debugMe){

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

void initZones(boolean debugMe){
  for(int i=0;i<maxZones;i++){
    initZone(config.zones[i],debugMe);
  }
}

void initZone(struct Zone &thisZone,boolean debugMe){
  
  if(thisZone.type==0){
    //sensor off
  }else if(thisZone.type==1){
    pinMode(thisZone.pin, OUTPUT);
  }
  
}


