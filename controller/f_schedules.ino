/***

Check Schedules

***/

#if !defined(SENSORONLY)

void checkSchedules(DateTime checkTime){

  for(int i=0;i<maxSchedules;i++){
    checkSchedule(configStore.schedules[i],checkTime);
  }
}

void checkSchedule(struct Schedule &checkingSchedule,DateTime checkTime){

  if(checkingSchedule.type==0){
    return;
  }
  
  #if defined(DEBUGSCHEDULE)
    Serial.print("checking schedule ");
    Serial.print(checkingSchedule.name);
    Serial.print("");
  #endif
   
  if(checkingSchedule.type==1){
    //timer
    checkScheduleTimer(checkingSchedule,checkTime);
  }else if(checkingSchedule.type==2){
    //soil moisture
    checkScheduleSoilMoisture(checkingSchedule,checkTime);
  }else if(checkingSchedule.type==3){
    //temperature
    checkScheduleTemperature(checkingSchedule,checkTime);
  }

}

void checkScheduleTimer(struct Schedule &checkingSchedule,DateTime checkTime){

  int run = 0;
  
  int weekday = checkTime.dayOfWeek();
  int hour = checkTime.hour();
  int minute = checkTime.minute();
 
  /*Serial.println();
  Serial.print("trying to match:");
  Serial.print(" d=");
  Serial.print(weekday);
  Serial.print(" h=");
  Serial.print(hour);
  Serial.print(" m=");
  Serial.print(minute);
  Serial.println();
  Serial.print(" day match ");
  Serial.print(checkingSchedule.timerStartWeekdays[weekday]);
  Serial.print("=");*/
  
  //check if to turn on
  if(checkingSchedule.timerStartWeekdays[weekday]==49){
   // Serial.print(weekday);
    //Serial.print(", hour match=");
    //day matches
    if(checkingSchedule.timerStartHours[hour]==49){
      //Serial.print(hour);
      //Serial.print(", minute match=");
      //hour matches
      if(checkingSchedule.timerStartMinutes[minute]==49){
        //minute matches
        //turn schedule on
        run = 1;
        //Serial.print(minute);
        //Serial.print(" success!");

      }

    }

  }
  
  //Serial.println();
  
  int wasRunning = checkingSchedule.isRunning;
    
  if(run==1){
    checkingSchedule.isRunning = 1;
    //checkingSchedule.statusRunStarted = checkTime.unixtime();
    //checkingSchedule.statusLastRunMinute = minute;
    turnScheduleZonesOn(checkingSchedule,checkTime);
    if(wasRunning==0){
      addScheduleLog(checkingSchedule,checkTime.unixtime(),"starting","timer");
    }
  }else{
    checkingSchedule.isRunning = 0;
    //checkingSchedule.statusRunStarted = 0;
    turnScheduleZonesOff(checkingSchedule,checkTime);
    if(wasRunning==1){
      addScheduleLog(checkingSchedule,checkTime.unixtime(),"ending","timer");
    }
  }

}

int checkScheduleSoilMoisture(struct Schedule &checkingSchedule, DateTime checkTime){

  
  int soilMoisture = 0;
  
  //loop through each sensor on schedule
  for(int i=0;i<maxSensors;i++){
    int thisSensorNum = checkingSchedule.sensors[i];
    if(thisSensorNum>-1){
      //sensor specfied
      if(configStore.sensors[thisSensorNum].type==1){
        //is a soil moisture sensor;
        //fix-me: just uses the value of the last soil moisture senso)
        soilMoisture = configStore.sensors[thisSensorNum].statusValue;
      }
    }
   }
   
  #if defined(DEBUGSCHEDULE)
    Serial.print("soil moisture=");
    Serial.println(soilMoisture);
  #endif
  
  int wasRunning = checkingSchedule.isRunning;
  
  //compare
  if(soilMoisture >= checkingSchedule.valueMax){
    //moisture high enough, turn off
    checkingSchedule.isRunning = 0;
    turnScheduleZonesOff(checkingSchedule,checkTime);
    if(wasRunning==1){
      addScheduleLog(checkingSchedule,checkTime.unixtime(),"off","moisture low");
    }
  }else if(soilMoisture <= checkingSchedule.valueMin){
    //moisture above target, turn on
    checkingSchedule.isRunning = 1;
    turnScheduleZonesOn(checkingSchedule,checkTime);
    if(wasRunning==0){
      addScheduleLog(checkingSchedule,checkTime.unixtime(),"on","moisture high");
    }
  }else{
    //not sure, so turn off
    checkingSchedule.isRunning = 0;
    turnScheduleZonesOff(checkingSchedule,checkTime);
    if(wasRunning==1){
      addScheduleLog(checkingSchedule,checkTime.unixtime(),"off","unknown");
    }
  }
  
}

int checkScheduleTemperature(struct Schedule &checkingSchedule, DateTime checkTime){
  
  float temperature = 0;
  
  //loop through each sensor on schedule
  for(int i=0;i<maxSensors;i++){
    int thisSensorNum = checkingSchedule.sensors[i];
    if(thisSensorNum>0){
      //sensor specfied
      if(configStore.sensors[thisSensorNum-1].type==2 || configStore.sensors[thisSensorNum-1].type==3){
        //is a soil moisture sensor;
        //fix-me: just uses the value of the last soil moisture sensor
        temperature = configStore.sensors[thisSensorNum-1].statusValue;
      }
    }
   }
     
  int wasRunning = checkingSchedule.isRunning;
  
  //compare
  if(temperature >= checkingSchedule.valueMax){
    //temp high enough, turn off
    checkingSchedule.isRunning = 0;
    turnScheduleZonesOff(checkingSchedule,checkTime);
    if(wasRunning==1){
      addScheduleLog(checkingSchedule,checkTime.unixtime(),"off","temp low");
    }
  }else if(temperature <= checkingSchedule.valueMin){
    //temp above target, turn on
    checkingSchedule.isRunning = 1;
    turnScheduleZonesOn(checkingSchedule,checkTime);
    if(wasRunning==1){
      addScheduleLog(checkingSchedule,checkTime.unixtime(),"off","temp low");
    }
  }else{
    //not sure, so turn off
    checkingSchedule.isRunning = 0;
    turnScheduleZonesOff(checkingSchedule,checkTime);
    if(wasRunning==1){
      addScheduleLog(checkingSchedule,checkTime.unixtime(),"off","temp low");
    }
  }
  
}

#endif


