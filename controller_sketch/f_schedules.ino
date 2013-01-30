/********************************************
  Check Schedules
********************************************/

void checkSchedules(DateTime checkTime, boolean debugMe){

  for(int i=0;i<maxSchedules;i++){
    checkSchedule(config.schedules[i],checkTime,debugMe);
  }
}

void checkSchedule(struct Schedule &checkingSchedule,DateTime checkTime, boolean debugMe){

  if(checkingSchedule.type==0){
    //schedule off
    //return;
  }else if(checkingSchedule.type==1){
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
  
    //check if to turn on
    for(int i=0;i<7;i++){
      if(checkingSchedule.timerStartWeekdays[i]==weekday){
      //day matches
      for(int j=0;j<24;j++){
        if(checkingSchedule.timerStartHours[j]==hour){
          //hour matches
          for(int k=0;k<60;k++){
            if(checkingSchedule.timerStartMinutes[k]==minute){
              //minute matches
              //turn schedule on
              run = 1;
  
            }
          }
  
        }
      }   

    }
    }
    
    if(run==1){
      checkingSchedule.isRunning = 1;
      //checkingSchedule.statusRunStarted = checkTime.unixtime();
      //checkingSchedule.statusLastRunMinute = minute;
      //addScheduleLog(checkingSchedule,checkTime,"starting","timer");
      turnScheduleZonesOn(checkingSchedule,checkTime,debug);
    }else{
      checkingSchedule.isRunning = 0;
      //checkingSchedule.statusRunStarted = 0;
      //addScheduleLog(checkingSchedule,checkTime,"ending","timer");
      turnScheduleZonesOff(checkingSchedule,checkTime,debug);
    }

}

int checkScheduleSoilMoisture(struct Schedule &checkingSchedule, DateTime checkTime){

  int soilMoisture = 0;
  
  //loop through each sensor on schedule
  for(int i=0;i<maxSensors;i++){
    int thisSensorNum = checkingSchedule.sensors[i];
    if(thisSensorNum>0){
      //sensor specfied
      if(config.sensors[thisSensorNum-1].type==1){
        //is a soil moisture sensor;
        //fix-me: just uses the value of the last soil moisture sensor
        soilMoisture = config.sensors[thisSensorNum-1].statusValue;
      }
    }
   }
        
  //compare
  if(soilMoisture >= checkingSchedule.valueMax){
    //moisture high enough, turn off
    turnScheduleZonesOff(checkingSchedule,checkTime,debug);
  }else if(soilMoisture <= checkingSchedule.valueMin){
    //moisture above target, turn on
    turnScheduleZonesOn(checkingSchedule,checkTime,debug);
  }else{
    //not sure, so turn off
    turnScheduleZonesOff(checkingSchedule,checkTime,debug);
  }
  
  

}

int checkScheduleTemperature(struct Schedule &checkingSchedule, DateTime checkTime){
  
  float temperature = 0;
  
  //loop through each sensor on schedule
  for(int i=0;i<maxSensors;i++){
    int thisSensorNum = checkingSchedule.sensors[i];
    if(thisSensorNum>0){
      //sensor specfied
      if(config.sensors[thisSensorNum-1].type==2 || config.sensors[thisSensorNum-1].type==3){
        //is a soil moisture sensor;
        //fix-me: just uses the value of the last soil moisture sensor
        temperature = config.sensors[thisSensorNum-1].statusValue;
      }
    }
   }
     
    
  //compare
  if(temperature >= checkingSchedule.valueMax){
    //moisture high enough, turn off
    turnScheduleZonesOff(checkingSchedule,checkTime,debug);
  }else if(temperature <= checkingSchedule.valueMin){
    //moisture above target, turn on
    turnScheduleZonesOn(checkingSchedule,checkTime,debug);
  }else{
    //not sure, so turn off
    turnScheduleZonesOff(checkingSchedule,checkTime,debug);
  }
  
}


