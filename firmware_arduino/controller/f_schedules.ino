/***

Check Schedules

***/

void checkSchedules(time_t checkTime){

  for(int i=0;i<maxSchedules;i++){
    checkSchedule(configStore.schedules[i],checkTime);
  }
}

void checkSchedule(struct Schedule &checkingSchedule,time_t checkTime){

  if(checkingSchedule.type==0){
    return;
  }
  
  #if defined(DEBUGSCHEDULE)
    SERIALMONITOR.print("checking schedule ");
    SERIALMONITOR.print(checkingSchedule.name);
    SERIALMONITOR.print("");
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

void checkScheduleTimer(struct Schedule &checkingSchedule,time_t checkTime){

  int run = 0;
  
  time_t adjustedTime = checkTime;
  adjustedTime =  adjustedTime + (configStore.utcOffset*60*60);
   
  int thisWeekday = weekday(adjustedTime);
  int thisHour = hour(adjustedTime);
  int thisMinute = minute(adjustedTime);
 
  /*SERIALMONITOR.print("days=");
  SERIALMONITOR.println(checkingSchedule.timerStartWeekdays);
  SERIALMONITOR.print(",hours=");
  SERIALMONITOR.println(checkingSchedule.timerStartHours);
  SERIALMONITOR.print(",minutes=");
  SERIALMONITOR.println(checkingSchedule.timerStartMinutes);
  SERIALMONITOR.println();
      
 
  SERIALMONITOR.println();
  SERIALMONITOR.print("unix before=");
  SERIALMONITOR.println(checkTime);
  SERIALMONITOR.print("unix after");
  SERIALMONITOR.println(adjustedTime);
  SERIALMONITOR.println();
  SERIALMONITOR.print("trying to match:");
  SERIALMONITOR.print(" d=");
  SERIALMONITOR.print(thisWeekday);
  SERIALMONITOR.print(" h=");
  SERIALMONITOR.print(thisHour);
  SERIALMONITOR.print(" m=");
  SERIALMONITOR.print(thisMinute);
  SERIALMONITOR.println();
  SERIALMONITOR.print(" day match ");
  SERIALMONITOR.print(checkingSchedule.timerStartWeekdays[thisWeekday]);
  SERIALMONITOR.print("=");*/
  
  //check if to turn on
  if(checkingSchedule.timerStartWeekdays[thisWeekday-1]==49){
    //SERIALMONITOR.print(thisWeekday);
    //SERIALMONITOR.print(", hour match=");
    //day matches
    if(checkingSchedule.timerStartHours[thisHour-1]==49){
      //SERIALMONITOR.print(thisHour);
      //SERIALMONITOR.print(", minute match=");
      //hour matches
      if(checkingSchedule.timerStartMinutes[thisMinute-1]==49){
        //minute matches
        //turn schedule on
        run = 1;
        //SERIALMONITOR.print(thisMinute);
        //SERIALMONITOR.print(" success!");

      }

    }

  }
  
  SERIALMONITOR.println();
  
  //SERIALMONITOR.println();
  
  int wasRunning = checkingSchedule.isRunning;
    
  if(run==1){
    checkingSchedule.isRunning = 1;
    //checkingSchedule.statusRunStarted = checkTime.unixtime();
    //checkingSchedule.statusLastRunMinute = minute;
    turnScheduleZonesOn(checkingSchedule,checkTime);
    //if(wasRunning==0){
      addScheduleLog(checkingSchedule,checkTime,"starting","timer");
    //}
  }else{
    checkingSchedule.isRunning = 0;
    //checkingSchedule.statusRunStarted = 0;
    turnScheduleZonesOff(checkingSchedule,checkTime);
    //if(wasRunning==1){
      addScheduleLog(checkingSchedule,checkTime,"ending","timer");
    //}
  }

}

int checkScheduleSoilMoisture(struct Schedule &checkingSchedule, time_t checkTime){

  
  int soilMoisture = 0;
  
  //loop through each sensor on schedule
  for(int i=0;i<maxSensors;i++){
    int thisSensorNum = checkingSchedule.sensors[i];
    if(thisSensorNum>-1){
      //sensor specfied
      //if(configStore.sensors[thisSensorNum].type==1){
        //is a soil moisture sensor;
        //fix-me: just uses the value of the last soil moisture senso)
        soilMoisture = configStore.sensors[thisSensorNum].statusValue;
      //}
    }
   }
   
  #if defined(DEBUGSCHEDULE)
    SERIALMONITOR.print("soil moisture=");
    SERIALMONITOR.println(soilMoisture);
  #endif
  
  int wasRunning = checkingSchedule.isRunning;
  
  //compare
  if(soilMoisture >= checkingSchedule.valueMax){
    //moisture high enough, turn off
    checkingSchedule.isRunning = 0;
    turnScheduleZonesOff(checkingSchedule,checkTime);
    //if(wasRunning==1){
      addScheduleLog(checkingSchedule,checkTime,"off","moisture low");
    //}
  }else if(soilMoisture <= checkingSchedule.valueMin){
    //moisture above target, turn on
    checkingSchedule.isRunning = 1;
    turnScheduleZonesOn(checkingSchedule,checkTime);
    //if(wasRunning==0){
      addScheduleLog(checkingSchedule,checkTime,"on","moisture high");
    //}
  }else{
    //not sure, so turn off
    checkingSchedule.isRunning = 0;
    turnScheduleZonesOff(checkingSchedule,checkTime);
    //if(wasRunning==1){
      addScheduleLog(checkingSchedule,checkTime,"off","unknown");
    //}
  }
  
}

int checkScheduleTemperature(struct Schedule &checkingSchedule, time_t checkTime){
  
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
    //if(wasRunning==1){
      addScheduleLog(checkingSchedule,checkTime,"off","temp low");
    //}
  }else if(temperature <= checkingSchedule.valueMin){
    //temp above target, turn on
    checkingSchedule.isRunning = 1;
    turnScheduleZonesOn(checkingSchedule,checkTime);
    //if(wasRunning==1){
      addScheduleLog(checkingSchedule,checkTime,"off","temp low");
    //}
  }else{
    //not sure, so turn off
    checkingSchedule.isRunning = 0;
    turnScheduleZonesOff(checkingSchedule,checkTime);
    //if(wasRunning==1){
      addScheduleLog(checkingSchedule,checkTime,"off","temp low");
    //}
  }
  
}



