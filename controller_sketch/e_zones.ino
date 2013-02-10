
void turnScheduleZonesOn(struct Schedule &thisSchedule, DateTime onTime){
  for(int i=0;i<maxZones;i++){
    if(thisSchedule.zones[i]>0){
      if(config.zones[(thisSchedule.zones[i]-1)].isRunning==0){
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
      if(config.zones[(thisSchedule.zones[i]-1)].isRunning==1){
        turnZoneOff(config.zones[(thisSchedule.zones[i]-1)],offTime,1);
      }else{
        turnZoneOff(config.zones[(thisSchedule.zones[i]-1)],offTime,0);
      }
    }
  }
}

void turnZoneOn(struct Zone &thisZone, DateTime onTime,int logMe){
  
  /*if(thisZone.statusSafetyOff==1){
    addErrorLog(onTime,"could not turn on zone because it was turned off as a safety percaution","");
    return;
  }*/
  
  thisZone.isRunning = 1;
  thisZone.statusRunStarted = onTime.unixtime();

  digitalWrite(thisZone.pin,HIGH);
  
  if(logMe==1){
    addZoneLog(thisZone,onTime.unixtime(),"on","");
  }
}

void turnZoneOff(struct Zone &thisZone, DateTime offTime, int logMe){
  
  digitalWrite(thisZone.pin,LOW);
  thisZone.isRunning = 0;
  thisZone.statusRunStarted = 0;
  
  if(logMe==1){
    addZoneLog(thisZone,offTime.unixtime(),"off","");
  }
}



