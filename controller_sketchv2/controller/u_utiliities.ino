
FLASH_STRING(memString,"Mem:");
FLASH_STRING(banner,"|| Fat Rabbit Farm - Garden Controller  ||");
FLASH_STRING(bannerBreak,"==========================================");

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

void printAvailableMemory(){
    #if defined(DEBUGMEM)
      memString.print(Serial);
      Serial.println(availableMemory());
    #endif
}

void printBanner(){
    printBreak();
    banner.print(Serial);Serial.println();
    printBreak();
}

void printBreak(){
    bannerBreak.print(Serial);Serial.println();
}

int availableMemory() 
{
  int size = 1024;
  byte *buf;
  while ((buf = (byte *) malloc(--size)) == NULL);
  free(buf);
  return size;
} 
