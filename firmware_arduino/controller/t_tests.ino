/**

Perform a set of tests that are helpful when manufacturing devices

**/
void commandTestFactory(char* params){

  SERIALMONITOR.println("[TEST] Starting factory test");
  
  //clear the configuration
  SERIALMONITOR.println("[TEST] resetting default configuration");
  commandConfigResetDefault("");
  //print system information
  SERIALMONITOR.println("[TEST] printing system information");
  commandSystemInformation("");
  //perform heartbeat test
  commandTestHeartbeat("");
  //perform time test
  commandTestTime("");
  //perform rtc test
  commandTestRTC("");
  //perform sd test
  commandTestSD("");
  //perform zones test
  commandTestZones("");
  //load test configuration
  commandTestLoadFactoryConfiguration("");
  //save configuration
  SERIALMONITOR.println("[TEST] saving configuration");
  commandConfigSave("");
  //print system information
  SERIALMONITOR.println("[TEST] printing system information");
  commandSystemInformation("");
  
  SERIALMONITOR.println("[TEST] Ending factory test");
  
}

void commandTestHeartbeat(char* params){
}

void commandTestTime(char* params){
}

void commandTestRTC(char* params){
}

void commandTestSD(char* params){
}

void commandTestZones(char* params){
}

void commandTestLoadFactoryConfiguration(char* params){
}
