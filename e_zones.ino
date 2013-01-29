void safetyOff(DateTime checkTime){
  /*
  //turn off if RTC failed to load a proper time
  
  for(int i=0;i<maxZones;i++){
    
    if(config.zones[i].type==0){
      
      //turn off if zone is set to off
      turnZoneOff(config.zones[i],checkTime,0);
    }else if(config.zones[i].statusRun==1 && (checkTime.unixtime()>=(config.zones[i].safetyOffAfterMinutes*60+config.zones[i].statusRunStarted))){
      //turn off if time exceeds safety off
      turnZoneOff(config.zones[i],checkTime,1);
      config.zones[i].statusSafetyOff = 1;
    }
    
  }*/
  
  //turn off if zone run time has exceeded safety off minutes
}


void turnScheduleZonesOn(struct Schedule &thisSchedule, DateTime onTime, boolean debugMe){
  for(int i=0;i<maxZones;i++){
    if(thisSchedule.zones[i]>0){
      if(config.zones[(thisSchedule.zones[i]-1)].statusRun==0){
        turnZoneOn(config.zones[(thisSchedule.zones[i]-1)],onTime,1);
      }else{
        turnZoneOn(config.zones[(thisSchedule.zones[i]-1)],onTime,0);
      }
    }
  }
}

void turnScheduleZonesOff(struct Schedule &thisSchedule, DateTime offTime){
  for(int i=0;i<maxZones;i++){
    if(thisSchedule.zones[i]>0){
      if(config.zones[(thisSchedule.zones[i]-1)].statusRun==1){
        turnZoneOff(config.zones[(thisSchedule.zones[i]-1)],offTime,1);
      }else{
        turnZoneOff(config.zones[(thisSchedule.zones[i]-1)],offTime,0);
      }
    }
  }
}

void turnZoneOn(struct Zone &thisZone, DateTime onTime,int logMe){
  
  if(thisZone.statusSafetyOff==1){
    addErrorLog(onTime,"could not turn on zone because it was turned off as a safety percaution","");
    return;
  }
  
  thisZone.statusRun = 1;
  thisZone.statusRunStarted = onTime.unixtime();

  digitalWrite(thisZone.pin,HIGH);
  
  if(logMe==1){
    addZoneLog(thisZone,onTime,"on","");
  }
}

void turnZoneOff(struct Zone &thisZone, DateTime offTime, int logMe){
  
  digitalWrite(thisZone.pin,LOW);
  thisZone.statusRun = 0;
  thisZone.statusRunStarted = 0;
  
  if(logMe==1){
    addZoneLog(thisZone,offTime,"off","");
  }
}


