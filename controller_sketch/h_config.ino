void pushStatus(){
}

void checkConfig(){
}


/*void loadConfig(boolean debugMe) {
  // To make sure there are settings, and they are YOURS!
  // If nothing is found it will use the default settings.
  if(debugMe==true){
    Serial.print("|| Config...");
  }
  if (EEPROM.read(CONFIG_START + 0) == CONFIG_VERSION[0] &&
      EEPROM.read(CONFIG_START + 1) == CONFIG_VERSION[1] &&
      EEPROM.read(CONFIG_START + 2) == CONFIG_VERSION[2]){
    for (unsigned int t=0; t<sizeof(config); t++){
      *((char*)&config + t) = EEPROM.read(CONFIG_START + t);
    }
    if(debugMe==true){
      Serial.println("OK");
    }
  }else{
    if(debugMe==true){
      Serial.println("FAIL (version mismatch)");
    }
  }
}

void saveConfig() {
  for (unsigned int t=0; t<sizeof(config); t++)
    EEPROM.write(CONFIG_START + t, *((char*)&config + t));
}*/
