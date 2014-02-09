  
//#if defined(USESERIALMONITOR) 

FLASH_STRING(memString,"[DEBUG] [MEMORY] ");
FLASH_STRING(banner,"|| Fat Rabbit Farm - Garden Controller  ||");
FLASH_STRING(bannerBreak,"==========================================");

void printDateTimeToSerial(time_t t){
    Serial.print(year(t), DEC);
    Serial.print('/');
    if(month(t) < 10) Serial.print("0");
    Serial.print(month(t), DEC);
    Serial.print('/');
    if(day(t) < 10) Serial.print("0");
    Serial.print(day(t), DEC);
    Serial.print(' ');
    if(hour(t) < 10) Serial.print("0");
    Serial.print(hour(t), DEC);
    Serial.print(':');
    if(minute(t) < 10) Serial.print("0");
    Serial.print(minute(t), DEC);
    Serial.print(':');
    if(second(t) < 10) Serial.print("0");
    Serial.print(second(t), DEC);

}

/*void printDateTimeToSerial(DateTime now){
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

}*/

void printCommandLineAvailable(){

  //Serial.println("You may enter commands now. All commands must be terminated with a < (left-carrot).");

}

void printAvailableMemory(){
      memString.print(Serial);
      Serial.println(memoryFree());
}
  
void printBanner(){
      printBreak();
      banner.print(Serial);Serial.println();
      printBreak();
}

void printBreak(){
    bannerBreak.print(Serial);Serial.println();
}

//#endif

int availableMemory() 
{
  int size = 4096;
  byte *buf;
  while ((buf = (byte *) malloc(--size)) == NULL);
  free(buf);
  return size;
} 

extern unsigned long __bss_end;
extern void *__brkval;

int memoryFree()
{
  //  long myValue;
  int freeValue;
  freeValue = 0;

  if ((unsigned long)__brkval == 0)
  { 
    freeValue = ((unsigned long)&freeValue) - ((unsigned long)&__bss_end);
    //       Serial.println("here and there");
  }
  else
  { 
    freeValue = ((unsigned long)&freeValue) - ((unsigned long)__brkval);
    //      Serial.println("here  2");
  }

  return freeValue;

}//end memoryFree()


char* getIdFromParams(char* params){
  
  return getParamByName(params,fieldId);
  
}

char* getParamByName(char* originalParams, const char* paramName){
  
  //fix-me: put a defined buffer size here for stability
  char params[maxBufferSize];
  strcpy(params,originalParams);
  
  char* param = strtok(params,"&");
  
  while(param > 0){
    
    char* thisParamName = strtok(param,"=");
    
    if(strstr(paramName,thisParamName)!=0){
    
      char* paramValue= strtok(NULL,"=");  
      return paramValue;
      
    }
    
    
    param = strtok(NULL,"&");
    
  }
  
  return "";
  
}

