  
#if defined(USESERIALMONITOR) 
FLASH_STRING(memString,"[DEBUG] [MEMORY] ");
FLASH_STRING(banner,"|| Fat Rabbit Farm - Garden Controller  ||");
FLASH_STRING(bannerBreak,"==========================================");
#endif

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

void printCommandLineAvailable(){

  Serial.println("You may enter commands now. All commands must be terminated with a < (left-carrot).");

}

void printAvailableMemory(){
    #if defined(DEBUGMEM)
      memString.print(Serial);
      Serial.println(memoryFree());
    #endif
}
  
void printBanner(){
  #if defined(USESERIALMONITOR) 
      printBreak();
      banner.print(Serial);Serial.println();
      printBreak();
  #endif
}

void printBreak(){
  #if defined(USESERIALMONITOR) 
    bannerBreak.print(Serial);Serial.println();
  #endif
}

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


/*unsigned long getIdFromParams(String params){
  
  String value = getParamByName(params,"id");
  //Serial.print("id value is ");
  //Serial.println(value);
  unsigned long idValue = value.toInt();
  
  return idValue;
  
}*/

/*String getParamByName(String params, String paramName){
  
  int commaPosition = 0;
  String param;
  
  //loop through all the parameters
  while(commaPosition >= 0){
    
      commaPosition = params.indexOf('&');

      if(commaPosition != -1){
          param = params.substring(0,commaPosition);
          params = params.substring(commaPosition+1, params.length());
      }else{ 
         if(params.length() > 0)
           param = params; 
      }
      
      //get the paramater value
      String value = param.substring(param.indexOf("=")+1);
     
      if(param.startsWith(paramName)){
        return value;
      }
      
      
   }
   
   return "";
  
}*/



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
