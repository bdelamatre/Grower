/***

Sensors

***/

void checkSensors(DateTime checkTime){
  for(int i=0;i<maxSensors;i++){
    checkSensor(configStore.sensors[i],checkTime);
  }
}

void checkSensor(struct Sensor &checkingSensor, DateTime checkTime){

  boolean logMe = false;
  
  if(checkingSensor.type==0){
    //sensor off
  }else{
    
      #if defined(DEBUGSENSORS)
        Serial.print(checkingSensor.name);
        Serial.print(", last checked at ");
        Serial.print(checkingSensor.statusLastChecked);
      #endif 
      
      if(checkingSensor.frequencyCheckSeconds>0 && checkingSensor.statusLastChecked>0){
        DateTime nextTime = DateTime(checkingSensor.statusLastChecked+checkingSensor.frequencyCheckSeconds);
        
        #if defined(DEBUGSENSORS)
          Serial.print(", next time in ");
          Serial.print(checkingSensor.frequencyCheckSeconds);
          Serial.print(" at a ");
          Serial.print(nextTime.unixtime());
        #endif
        
        if(checkTime.unixtime() < nextTime.unixtime() && checkingSensor.statusLastChecked > 0){
          
          #if defined(DEBUGSENSORS)
            Serial.println();
          #endif
          
          return;
        }
      }
          
      #if defined(DEBUGSENSORS)   
        Serial.println();
      #endif
      
      checkingSensor.statusLastChecked = checkTime.unixtime();
      
      if(checkingSensor.frequencyLogSeconds>0){
        DateTime nextTime = DateTime(checkingSensor.statusLastLogged+checkingSensor.frequencyLogSeconds);
        if(checkTime.unixtime() >= nextTime.unixtime() || checkingSensor.statusLastLogged < 1){
          logMe = true;
          checkingSensor.statusLastLogged = checkTime.unixtime();
        }
      }
      
      if(checkingSensor.type==1){
        //soil moisture
        checkSensorSoilMoisture(checkingSensor,checkTime,logMe);
      }else if(checkingSensor.type==2){
        //soil temperature
        checkSensorSoilTemperature(checkingSensor,checkTime,logMe);
      }else if(checkingSensor.type==3){
        //air temperature
        checkSensorAirTemperature(checkingSensor,checkTime,logMe);
      }else if(checkingSensor.type==4){
        //light
        checkSensorLight(checkingSensor,checkTime,logMe);
      }
      
      #if defined(DEBUGSENSORS)
        Serial.print(checkingSensor.name);
        Serial.print("=");
        Serial.print(checkingSensor.statusValue);
        if(checkingSensor.statusValue2){
          Serial.print(", ");
          Serial.print(checkingSensor.statusValue2);
        }
        Serial.println();
      #endif
      
  }

}

int checkSensorSoilMoisture(struct Sensor &checkingSensor,DateTime checkTime, boolean logMe){
          
  //smooth over .25s  
  int smoothed = 0;
  for(int i=0;i<25;i++){
    smoothed += analogRead(checkingSensor.pin);
    delay(10);
  }
  
  checkingSensor.statusValue = smoothed / 50;
    
  if(logMe==true){
      addSensorLog(checkingSensor,checkTime.unixtime(),"","");
  }
}

int checkSensorSoilTemperature(struct Sensor &checkingSensor,DateTime checkTime, boolean logMe){
    
  //fix-me: add support
  
  if(logMe==true){
      addSensorLog(checkingSensor,checkTime.unixtime(),"","");
  }
}

int checkSensorAirTemperature(struct Sensor &checkingSensor,DateTime checkTime, boolean logMe){
    
  DHT dht(checkingSensor.pin, DHT22);
 
  dht.begin();
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature(true);

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    
  }else{
    checkingSensor.statusValue = t; 
    checkingSensor.statusValue2 =  h;
  }
  
  if(logMe==true){
      addSensorLog(checkingSensor,checkTime.unixtime(),"","");
  }
}

int checkSensorLight(struct Sensor &checkingSensor,DateTime checkTime, boolean logMe){
    
  if(logMe==true){
      addSensorLog(checkingSensor,checkTime.unixtime(),"","");
  }
}

