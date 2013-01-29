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

String getDateTime(DateTime t){
  return getDate(t)+" "+getTime(t);
}

String getDate(DateTime t){
  String value;
  value += String(t.day());
  value += "/";
  value += String(t.month());
  value += "/";
  value += String(t.year());
  return value;
}

String getTime(DateTime t){
  String value;
  value += String(t.hour());
  value += ":";
  value += String(t.minute());
  value += ":";
  value += String(t.second());
  return value;
}

unsigned long getChronodotTempF(DateTime t){
  return t.tempF();
}

String getIp(){
  // print the value of each byte of the IP address:
  String ip;
  ip += Ethernet.localIP()[0];
  ip += "."; 
  ip += Ethernet.localIP()[1];
  ip += "."; 
  ip += Ethernet.localIP()[2];
  ip += "."; 
  ip += Ethernet.localIP()[3];
  return ip;
}

/*String printConfig(){

  Serial.print("Version=");
  Serial.println(config.version);
  for(int i=0;i<maxSchedules;i++){
    Serial.print("Schedule #");
    Serial.println(i);
    Serial.print("name=");
    Serial.println(config.schedules[i].name);
  }
  for(int i=0;i<maxSensors;i++){
    Serial.print("Sensor #");
    Serial.println(i);
    Serial.print("name=");
    Serial.println(config.sensors[i].name);
  }
  for(int i=0;i<maxZones;i++){
    Serial.print("Zone #");
    Serial.println(i);
    Serial.print("name=");
    Serial.println(config.zones[i].name);

  }

}*/

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




