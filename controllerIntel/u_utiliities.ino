  
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
      Serial.print("[DEBUG] [MEMORY] ");
      Serial.println(memoryFree());
    #endif
}
  
void printBanner(){
  #if defined(USESERIALMONITOR) 
      printBreak();
      Serial.println("|| Gnome - Controller  ||");
      printBreak();
  #endif
}

void printBreak(){
  #if defined(USESERIALMONITOR) 
    Serial.println("==========================================");
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

void getIdsFromValue(int resultArray[],int arrayLength, String value,char delimitter=',',boolean shiftValues=false){
  
    int thisPosition;
    String thisParam;
    int lastPosition = 0;
        
     //cycle through this parameter by comma delimited
     while(thisPosition >= 0 && lastPosition < arrayLength){
         
        thisPosition = value.indexOf(delimitter);
      
        if(thisPosition != -1){
            thisParam = value.substring(0,thisPosition);
            value = value.substring(thisPosition+1, value.length());
        }else{
           if(value.length() > 0){
             thisParam = value; 
           }else{
             continue;
           }
        }
                
        resultArray[lastPosition] = thisParam.toInt();
        if(shiftValues==true){
          //Serial.println("shifting up one");
          resultArray[lastPosition]++;  
        }
        lastPosition++;
     }

}

