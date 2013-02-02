void pushDisplay(){
}

String sensorTypeDisplayName(struct Sensor &checkingSensor){
  //0=off, 1=soil moisture (analog), 2=soil temperature(DS18B20), 3=air temperature (DHT22), 4=light
  String d = "off";
  
  if(checkingSensor.type==1){
    d = "Moisture";
  }else if(checkingSensor.type==2){
    d = "Temperature (DS18B20)";
  }else if(checkingSensor.type==3){
    d = "Temperature (DHT22)";
  }else if(checkingSensor.type==4){
    d = "Light";
  }
  
  return d;
}

String zoneTypeDisplayName(struct Zone &checkingZone){
  //0=off, 1=5v relay
  String d = "off";
  
  if(checkingZone.type==1){
    d = "5v Relay";
  }
  
  return d;
}

String scheduleTypeDisplayName(struct Schedule &checkingSchedule){
  //0=off, 1=timer, 2=soil moisture, 3=temperature
  String d = "off";
  
  if(checkingSchedule.type==1){
    d = "Timer";
  }else if(checkingSchedule.type==2){
    d = "Soil Moisture";
  }else if(checkingSchedule.type==3){
    d = "Temperature";
  }
  
  return d;
}



void printDateTimeToSerial(DateTime now){
    Serial.print(now.year(), DEC);
    Serial.print('/');
    if(now.month() < 10) Serial.print("0");
    Serial.print(now.month(), DEC);
    Serial.print('/');
    if(now.day() < 10) Serial.print("0");
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    if(now.hour() < 10) Serial.print("0");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    if(now.minute() < 10) Serial.print("0");
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    if(now.second() < 10) Serial.print("0");
    Serial.print(now.second(), DEC);

}

int getChronodotTempF(DateTime t){
  return t.tempF();
}

void printIpToSerial(IPAddress thisIp){

  Serial.print(thisIp[0]);
  Serial.print("."); 
  Serial.print(thisIp[1]);
  Serial.print("."); 
  Serial.print(thisIp[2]);
  Serial.print("."); 
  Serial.print(thisIp[3]);

}

void printDirectory(File dir, int numTabs) {
  while(true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      //Serial.println("**nomorefiles**");
      break;
    }
    for (uint8_t i=0; i<numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs+1);
    } 
    else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
  }
}




