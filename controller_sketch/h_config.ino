void pushStatus(){
}

void checkConfig(){
}


void loadConfig() {
  // To make sure there are settings, and they are YOURS!
  // If nothing is found it will use the default settings.
  #if defined DEBUG
    Serial.print("|| Config...");
  #endif
  if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2]){
    for (unsigned int t=0; t<sizeof(config); t++){
      //Serial.println(EEPROM.read(CONFIG_START + t));
      *((char*)&config + t) = EEPROM.read(CONFIG_START + t);
    }
    #if defined DEBUG
      Serial.println("OK");
    #endif
  }else{
    #if defined DEBUG
      Serial.println("FAIL (version mismatch)");
    #endif
  }
}

void saveConfig() {
  for (unsigned int t=0; t<sizeof(config); t++)
    EEPROM.write(CONFIG_START + t, *((char*)&config + t));
}

void myManualConfig(){

  config.adminUsername = "admin";
  config.adminPassword = "fatrabbit";
  
  //config.cameraImgUrl = "http://192.168.2.98/image.jpg";
  //config.cameraAdminUrl = "http://192.168.2.98";
  
  config.dhcp = true;
  config.clientAddress = IPAddress(192,168,2,99);
  config.clientNetmask = IPAddress(255,255,255,0);
  config.clientGateway = IPAddress(192,168,2,1);
  config.clientDns = IPAddress(192,168,2,1);
  config.ntp=true;
  
  config.zones[0].name="Heater Outlet";
  config.zones[0].type = 1;
  config.zones[0].pin = 5;
  
  config.zones[1].name="Misting System";
  config.zones[1].type = 1;
  config.zones[1].pin = 4;
  config.zones[1].safetyOffAfterMinutes = 120;
  
  config.zones[2].name="Light Outlet";
  config.zones[2].type = 1;
  config.zones[2].pin = 6;
  
  config.sensors[0].name = "Bench Temperature & Humidity";
  config.sensors[0].type = 3;
  config.sensors[0].pin = 2;
  config.sensors[0].frequencyCheckSeconds = 5;
  config.sensors[0].frequencyLogSeconds = 300;
  
  config.sensors[1].name="Soil Temperature";
  config.sensors[1].type = 0;
  config.sensors[1].pin = 3;
  config.sensors[1].frequencyCheckSeconds = 5;
  config.sensors[1].frequencyLogSeconds = 300;

  config.sensors[2].name="Soil Moisture";
  config.sensors[2].type = 1;
  config.sensors[2].pin = 1;
  config.sensors[2].frequencyCheckSeconds = 5;
  config.sensors[2].frequencyLogSeconds = 300;
  
  //heat schedule - temperature
  config.schedules[0].name="Heaters";
  config.schedules[0].type=3;
  config.schedules[0].zones[0] = 1;
  config.schedules[0].valueMin = 76;
  config.schedules[0].valueMax = 78;
  config.schedules[0].sensors[0] = 1;
  
  //watering schedule - moisture level
  config.schedules[1].name="Misting (Watering)";
  config.schedules[1].type=0;
  config.schedules[1].zones[0] = 2;
  config.schedules[1].valueMin = 500;
  config.schedules[1].valueMax = 600;
  config.schedules[1].sensors[0] = 3;
  
  //light schedule - timer
  config.schedules[2].name="Grow Light";
  config.schedules[2].type=1;
  config.schedules[2].zones[0] = 3;
  config.schedules[2].timerStartWeekdays[0] = 0;
  config.schedules[2].timerStartWeekdays[1] = 1;
  config.schedules[2].timerStartWeekdays[2] = 2;
  config.schedules[2].timerStartWeekdays[3] = 3;
  config.schedules[2].timerStartWeekdays[4] = 4;
  config.schedules[2].timerStartWeekdays[5] = 5;
  config.schedules[2].timerStartWeekdays[6] = 6;
  config.schedules[2].timerStartHours[0] = -1;
  config.schedules[2].timerStartHours[1] = -1;
  config.schedules[2].timerStartHours[2] = -1;
  config.schedules[2].timerStartHours[3] = -1;
  config.schedules[2].timerStartHours[4] = -1;
  config.schedules[2].timerStartHours[5] = -1;
  config.schedules[2].timerStartHours[6] = 6;
  config.schedules[2].timerStartHours[7] = 7;
  config.schedules[2].timerStartHours[8] = 8;
  config.schedules[2].timerStartHours[9] = 9;
  config.schedules[2].timerStartHours[10] = 10;
  config.schedules[2].timerStartHours[11] = 11;
  config.schedules[2].timerStartHours[12] = 12;
  config.schedules[2].timerStartHours[13] = 13;
  config.schedules[2].timerStartHours[14] = 14;
  config.schedules[2].timerStartHours[15] = 15;
  config.schedules[2].timerStartHours[16] = 16;
  config.schedules[2].timerStartHours[17] = 17;
  config.schedules[2].timerStartHours[18] = 18;
  config.schedules[2].timerStartHours[19] = 19;
  config.schedules[2].timerStartHours[20] = 20;
  config.schedules[2].timerStartHours[21] = 21;
  config.schedules[2].timerStartHours[22] = 22;
  config.schedules[2].timerStartHours[23] = 23;
  for(int m=0;m<60;m++){
    config.schedules[2].timerStartMinutes[m] = m;
  }

  
}
