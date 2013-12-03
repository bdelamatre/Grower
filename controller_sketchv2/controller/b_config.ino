void loadConfig() {

  #if defined DEBUG
    Serial.print("|| Config...");
    Serial.print(CONFIG_VERSION);
    Serial.print("...");
  #endif
  
  
  if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2]){
    
    int configSize = sizeof(configStore);
    Serial.print(configSize);
    Serial.print(" bytes...");
    
    for (unsigned int t=0; t<configSize; t++){
      *((char*)&configStore + t) = EEPROM.read(CONFIG_START + t);
    }
    
    #if defined DEBUG
      Serial.println("OK");
    #endif
  }else{
    defaultConfig();
  }
  
  
}

void saveConfig() {
    Serial.print("Writing ");
    Serial.print(sizeof(configStore));
    Serial.print(" bytes...");

    //EEPROM.writeBlock(CONFIG_START, configStore);
    for (unsigned int t=0; t<sizeof(configStore); t++)
        EEPROM.write(CONFIG_START + t, *((char*)&configStore + t));


    Serial.println("done!");
}

void resetDefaultConfig(){
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 4096; i++)
    EEPROM.write(i, 0);
}

void defaultConfig(){
    
  Serial.print("loading defaults ");
  
  //#if defined DEBUG
    //Serial.println("FAIL (version mismatch)");
    
  //#endif
  
  
  String z0n = "Heater Outlet";
  z0n.toCharArray(configStore.zones[0].name,32);
  //configStore.zones[0].name = "Heater Outlet";
  configStore.zones[0].type = 1;
  configStore.zones[0].pin = 5;
  
  String z1n = "Misting System";
  z1n.toCharArray(configStore.zones[1].name,32);
  //configStore.zones[1].name = "Misting System";
  configStore.zones[1].type = 1;
  configStore.zones[1].pin = 4;
  configStore.zones[1].safetyOffAfterMinutes = 120;
  
  String z2n = "Light Outlet";
  z2n.toCharArray(configStore.zones[2].name,32);
  //configStore.zones[2].name = "Light Outlet";
  configStore.zones[2].type = 1;
  configStore.zones[2].pin = 6;
  
  String s0n = "Bench Temperature & Humidity";
  s0n.toCharArray(configStore.sensors[0].name,32);
  //configStore.sensors[0].name = "Bench Temperature & Humidity";
  configStore.sensors[0].type = 0;
  configStore.sensors[0].pin = 2;
  configStore.sensors[0].frequencyCheckSeconds = 5;
  configStore.sensors[0].frequencyLogSeconds = 60;
  
  String s1n = "Soil Temperature";
  s1n.toCharArray(configStore.sensors[1].name,32);
  //configStore.sensors[1].name="Soil Temperature";
  configStore.sensors[1].type = 0;
  configStore.sensors[1].pin = 3;
  configStore.sensors[1].frequencyCheckSeconds = 5;
  configStore.sensors[1].frequencyLogSeconds = 60;

  String s2n = "Soil Moisture";
  s2n.toCharArray(configStore.sensors[2].name,32);
  //configStore.sensors[2].name="Soil Moisture";
  configStore.sensors[2].type = 1;
  configStore.sensors[2].pin = 1;
  configStore.sensors[2].frequencyCheckSeconds = 1;
  configStore.sensors[2].frequencyLogSeconds = 60;
  
  //heat schedule - temperature
  String x0n = "Heaters";
  x0n.toCharArray(configStore.schedules[0].name,32);
  //configStore.schedules[0].name="Heaters";
  configStore.schedules[0].type=3;
  configStore.schedules[0].zones[0] = 1;
  configStore.schedules[0].valueMin = 76;
  configStore.schedules[0].valueMax = 78;
  configStore.schedules[0].sensors[0] = 1;
  
  //watering schedule - moisture level
  String x1n = "Misting (Watering)";
  x1n.toCharArray(configStore.schedules[0].name,32);
  //configStore.schedules[1].name="Misting (Watering)";
  configStore.schedules[1].type=0;
  configStore.schedules[1].zones[0] = 2;
  configStore.schedules[1].valueMin = 500;
  configStore.schedules[1].valueMax = 600;
  configStore.schedules[1].sensors[0] = 3;
  
  //light schedule - timer
  String x2n = "Grow Lights";
  x2n.toCharArray(configStore.schedules[0].name,32);
  //configStore.schedules[2].name="Grow Light";
  configStore.schedules[2].type=1;
  configStore.schedules[2].zones[0] = 3;
  configStore.schedules[2].timerStartWeekdays[0] = 0;
  configStore.schedules[2].timerStartWeekdays[1] = 1;
  configStore.schedules[2].timerStartWeekdays[2] = 2;
  configStore.schedules[2].timerStartWeekdays[3] = 3;
  configStore.schedules[2].timerStartWeekdays[4] = 4;
  configStore.schedules[2].timerStartWeekdays[5] = 5;
  configStore.schedules[2].timerStartWeekdays[6] = 6;
  configStore.schedules[2].timerStartHours[0] = -1;
  configStore.schedules[2].timerStartHours[1] = -1;
  configStore.schedules[2].timerStartHours[2] = -1;
  configStore.schedules[2].timerStartHours[3] = -1;
  configStore.schedules[2].timerStartHours[4] = -1;
  configStore.schedules[2].timerStartHours[5] = -1;
  configStore.schedules[2].timerStartHours[6] = 6;
  configStore.schedules[2].timerStartHours[7] = 7;
  configStore.schedules[2].timerStartHours[8] = 8;
  configStore.schedules[2].timerStartHours[9] = 9;
  configStore.schedules[2].timerStartHours[10] = 10;
  configStore.schedules[2].timerStartHours[11] = 11;
  configStore.schedules[2].timerStartHours[12] = 12;
  configStore.schedules[2].timerStartHours[13] = 13;
  configStore.schedules[2].timerStartHours[14] = 14;
  configStore.schedules[2].timerStartHours[15] = 15;
  configStore.schedules[2].timerStartHours[16] = 16;
  configStore.schedules[2].timerStartHours[17] = 17;
  configStore.schedules[2].timerStartHours[18] = 18;
  configStore.schedules[2].timerStartHours[19] = 19;
  configStore.schedules[2].timerStartHours[20] = 20;
  configStore.schedules[2].timerStartHours[21] = 21;
  configStore.schedules[2].timerStartHours[22] = 22;
  configStore.schedules[2].timerStartHours[23] = 23;
  for(int m=0;m<60;m++){
    configStore.schedules[2].timerStartMinutes[m] = m;
  }
  Serial.print(sizeof(configStore));
  Serial.println(" bytes");
}
