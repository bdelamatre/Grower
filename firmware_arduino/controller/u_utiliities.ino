  
//#if USE_MODULE_SERIALMONITOR == true 

FLASH_STRING(memString,"[DEBUG] [MEMORY AVAILABLE] ");
FLASH_STRING(banner,"|| Grower.io - Garden Controller        ||");
FLASH_STRING(bannerBreak,"==========================================");

void printDateTimeToSerial(time_t t){
    SERIALMONITOR.print(year(t), DEC);
    SERIALMONITOR.print('/');
    if(month(t) < 10) SERIALMONITOR.print("0");
    SERIALMONITOR.print(month(t), DEC);
    SERIALMONITOR.print('/');
    if(day(t) < 10) SERIALMONITOR.print("0");
    SERIALMONITOR.print(day(t), DEC);
    SERIALMONITOR.print(' ');
    if(hour(t) < 10) SERIALMONITOR.print("0");
    SERIALMONITOR.print(hour(t), DEC);
    SERIALMONITOR.print(':');
    if(minute(t) < 10) SERIALMONITOR.print("0");
    SERIALMONITOR.print(minute(t), DEC);
    SERIALMONITOR.print(':');
    if(second(t) < 10) SERIALMONITOR.print("0");
    SERIALMONITOR.print(second(t), DEC);

}

/*void printDateTimeToSerial(DateTime now){
    SERIALMONITOR.print(now.year(), DEC);
    SERIALMONITOR.print('/');
    if(now.month() < 10) SERIALMONITOR.print("0");
    SERIALMONITOR.print(now.month(), DEC);
    SERIALMONITOR.print('/');
    if(now.day() < 10) SERIALMONITOR.print("0");
    SERIALMONITOR.print(now.day(), DEC);
    SERIALMONITOR.print(' ');
    if(now.hour() < 10) SERIALMONITOR.print("0");
    SERIALMONITOR.print(now.hour(), DEC);
    SERIALMONITOR.print(':');
    if(now.minute() < 10) SERIALMONITOR.print("0");
    SERIALMONITOR.print(now.minute(), DEC);
    SERIALMONITOR.print(':');
    if(now.second() < 10) SERIALMONITOR.print("0");
    SERIALMONITOR.print(now.second(), DEC);

}*/

void printCommandLineAvailable(){
  SERIALMONITOR.println("You may enter commands now. All commands must be terminated with a < (left-carrot).");
}

void printAvailableMemory(){
      memString.print(SERIALMONITOR);
      SERIALMONITOR.println(memoryFree());
}
  
void printBanner(){
      printBreak();
      banner.print(SERIALMONITOR);SERIALMONITOR.println();
      printBreak();
}

void printBreak(){
    bannerBreak.print(SERIALMONITOR);SERIALMONITOR.println();
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
    //       SERIALMONITOR.println("here and there");
  }
  else
  { 
    freeValue = ((unsigned long)&freeValue) - ((unsigned long)__brkval);
    //      SERIALMONITOR.println("here  2");
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

