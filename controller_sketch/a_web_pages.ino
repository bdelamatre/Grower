
char* getUserPassInBase64(String user, String pass){

    String b;
    b += user;
    b += ":"; 
    b += pass;
    byte atm = 0;
    char tok[200];
    char encoded[200] = "";
    char o;
    
    atm = b.length(); 
    b.toCharArray(tok,atm+1);
    base64_encode(encoded, tok, atm);
     
    return encoded;
}

String displayYesNo(int i,boolean format=0){
  if(i==1){
    if(format==1){
      return "<span style=\"color:#00ff00\">Yes</span>";
    }else{
      return "yes";
    }
  }else{
    if(format==1){
      return "<span style=\"color:#ff0000\">No</span>";
    }else{
      return "no";
    }
  }
}

String displayWeekday(int i){
  if(i==0){
    return "Sun";
  }else if(i==1){
    return "Mon";
  }else if(i==2){
    return "Tue";
  }else if(i==3){
    return "Wed";
  }else if(i==4){
    return "Thu";
  }else if(i==5){
    return "Fri";
  }else if(i==6){
    return "Sat";
  }else{
    return "unk";
  }
}

/*String elementUtcOffset(String name, int value){
  String html = "<select name=\"utc_";
  html += name;
  html += " >";
  for(int i = -12;i<15;i++){
    html += "<option value=\"";
    html += i;
    html += "\"";
    if(value==i){
      html+= " selected";
    }
    html += ">";
    if(i>0){
      html += "+";
    }else{
      html += "-";
    }
    html += i;
    html +=" </option>";
  }
  return html;
}

String elementText(String name, String value){
  String html = "<input type=\"text\" name=\"text_";
  html += name;
  html += "\" value=\"";
  html += value;
  html += "\"/>";
  return html;
}

String elementInt(String name, int value){
  String html = "<input type=\"text\" name=\"int_";
  html += name;
  html += "\" value=\"";
  html += value;
  html += "\"/>";
  return html;
}

String elementIp(String name,IPAddress ip){
  String html = "<input type=\"text\" name=\"text_";
  html += name;
  html += "\" value=\"";
  html += ip[0];
  html += "\"/>";
  html += "<input type=\"text\" name=\"text_";
  html += name;
  html += "\" value=\"";
  html += ip[1];
  html += "\"/>";
  html += "<input type=\"text\" name=\"text_";
  html += name;
  html += "\" value=\"";
  html += ip[2];
  html += "\"/>";
  html += "<input type=\"text\" name=\"text_";
  html += name;
  html += "\" value=\"";
  html += ip[3];
  html += "\"/>";
  return html;
}

String elementCheckbox(String name, boolean checked){
  String html = "<input type=\"checkbox\" name=\"check_";
  html += name;
  html += " />";
  return html;
}

String elementScheduleType(String name, int type){
  String html = "<select name=\"type_";
  html += name;
  html += " >";
  for(int i = 0;i<4;i++){
    html += "<option value=\"";
    html += i;
    html += "\"";
    if(type==i){
      html+= " selected";
    }
    html += ">";
    if(i>0){
      html += "+";
    }else{
      html += "-";
    }
    //html += scheduleTypeDisplayName(i);
    html +=" </option>";
  }
  return html;
}

String elementZoneType(String name, int type){
  String html = "<select name=\"type_";
  html += name;
  html += " >";
  for(int i = 0;i<4;i++){
    html += "<option value=\"";
    html += i;
    html += "\"";
    if(type==i){
      html+= " selected";
    }
    html += ">";
    if(i>0){
      html += "+";
    }else{
      html += "-";
    }
    //html += zoneTypeDisplayName(i);
    html +=" </option>";
  }
  return html;
}

String elementZones(String name, int* type){
}

String elementZonesRunType(String name, int type){
}

String elementSensorType(String name, int type){
  String html = "<select name=\"type_";
  html += name;
  html += " >";
  for(int i = 0;i<5;i++){
    html += "<option value=\"";
    html += i;
    html += "\"";
    if(type==i){
      html+= " selected";
    }
    html += ">";
    if(i>0){
      html += "+";
    }else{
      html += "-";
    }
    //html += sensorTypeDisplayName(i);
    html +=" </option>";
  }
  return html;
}

String elementSensors(String name, int* type){
}

String elementTimerDays(String name, int* type){
}
String elementTimerHours(String name, int* type){
}

String elementTimerMinutes(String name, int* type){
}

#define NAMELEN 32
#define VALUELEN 32

/void htmlAdmin(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  
  if (server.checkCredentials("YWRtaW46YWRtaW4="))
  {
    server.httpSuccess();
    if(type == WebServer::POST){
      
      URLPARAM_RESULT rc;
      char name[NAMELEN]; 
      int  name_len;
      char value[VALUELEN];
      int value_len;
      
      //process post
      while (server.readPOSTparam(name, NAMELEN, value, VALUELEN))
      {
        
        
      }
    
      server.println("<h3>Saved!</h3>");  
      
    }
    
    if (type != WebServer::HEAD){
      
      server.println("<form type=\"post\">");
      //show form
      //admin username
      server.println(elementText("adminUsername",config.adminUsername));
      //admin password
      server.println(elementText("adminPassword",config.adminPassword));
      //utc offset
      server.println(elementUtcOffset("utcOffset",config.utcOffset));
      //dhcp?
      server.println(elementCheckbox("dhcp",config.dhcp));
      //ip address
      server.println(elementIp("clientAddress",config.clientAddress));
      //ip netmask
      server.println(elementIp("clientNetmask",config.clientNetmask));
      //ip gateway
      server.println(elementIp("clientGateway",config.clientGateway));
      //hardwaremac
      //ntpserver
      server.println(elementIp("ntpServer",config.ntpServer));
      //schedules
      for(int i=0;i<maxSchedules;i++){
        //schedule type
        String scheduleName = "schedule[";
        scheduleName += i;
        scheduleName += "]";
        server.println(elementScheduleType(scheduleName+"[\"type\"]",config.schedules[i].type));
        //name
        server.println(elementText(scheduleName+"[\"name\"]",config.schedules[i].name));
        //zones
        server.println(elementZones(scheduleName+"[\"zones\"]",config.schedules[i].zones));
        //zones run type
        server.println(elementZonesRunType(scheduleName+"[\"zonesRunType\"]",config.schedules[i].zonesRunType));
        //sensors
        server.println(elementSensors(scheduleName+"[\"sensors\"]",config.schedules[i].sensors));
        //timer days
        server.println(elementTimerDays(scheduleName+"[\"timerStartWeekdays\"]",config.schedules[i].timerStartWeekdays));
        //timer hours
        server.println(elementTimerHours(scheduleName+"[\"timerStartHours\"]",config.schedules[i].timerStartHours));
        //timer minutes
        server.println(elementTimerMinutes(scheduleName+"[\"timerStartMinutes\"]",config.schedules[i].timerStartMinutes));
        //valueMin
        server.println(elementInt(scheduleName+"[\"valueMin\"]",config.schedules[i].valueMin));
        //valueMax
        server.println(elementInt(scheduleName+"[\"valueMax\"]",config.schedules[i].valueMax));
      }
      //sensors
      for(int i=0;i<maxSensors;i++){
        String sensorName = "sensors[";
        sensorName += i;
        sensorName += "]";
        //type; //0=off, 1=soil moisture (analog), 2=soil temperature(DS18B20), 3=air temperature (DHT22), 4=light
        server.println(elementSensorType(sensorName+"[\"type\"]",config.sensors[i].type));
        //name;
        server.println(elementText(sensorName+"[\"name\"]",config.sensors[i].name));
        //pin;
        server.println(elementInt(sensorName+"[\"pin\"]",config.sensors[i].pin));
        //pin2;
        server.println(elementInt(sensorName+"[\"pin2\"]",config.sensors[i].pin2));
        //frequencyCheckSeconds; //0=every loop
        server.println(elementInt(sensorName+"[\"frequencyCheckSeconds\"]",config.sensors[i].frequencyCheckSeconds));
        //frequencyLogSeconds; //0=every log
        server.println(elementInt(sensorName+"[\"frequencyLogSeconds\"]",config.sensors[i].frequencyLogSeconds));
      }
      //zones
      for(int i=0;i<maxZones;i++){
        String zoneName = "zones[";
        zoneName += i;
        zoneName += "]";
        //int type; //0=off, 1=5v relay
        server.println(elementZoneType(zoneName+"[\"type\"]",config.zones[i].type));
        //name;
        server.println(elementText(zoneName+"[\"name\"]",config.zones[i].name));
        //pin;
        server.println(elementInt(zoneName+"[\"pin\"]",config.zones[i].pin));
        //safetyOffAfterMinutes;
        server.println(elementInt(zoneName+"[\"safetyOffAfterMinutes\"]",config.zones[i].safetyOffAfterMinutes));
      }
    }
      //server.println("<button type=\"submit\" />");
      //server.println("</form>");
    
  }
  else
  {
    server.httpUnauthorized();
  }
}*/

    
void htmlStatus(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  
  char* authentication = getUserPassInBase64(config.adminUsername,config.adminPassword);
  
  if (server.checkCredentials(authentication))
  {
  
    server.httpSuccess();
  
    if (type != WebServer::HEAD)
    {
  
      DateTime t = getLocalTime();
  
      P(startOpen) = "<!DOCTYPE HTML><html><head><meta http-equiv=\"refresh\" content=\"300\"/><title>Status - Fat Rabbit Garden</title></head>";
      server.printP(startOpen);
      P(style) = "<style> .mainSection{} section{width:400px;margin:auto;} article{margin:5px;padding:5px;} #general article{border:2px solid #000000;} #schedules article{border:2px solid #000000;} #sensors article{border:2px solid #000000;} #zones article{border:2px solid #000000;}</style>";
      server.printP(style);
      P(startClose) = "<body style=\"text-align:center;\">";
      server.printP(startClose);
      P(header) = "<header><h1>Fat Rabbit Farm - Garden Controller</h1></header>";
      server.printP(header);
      
      P(generalSectionOpen) = "<h2>General</h2><section id=\"general\" class=\"mainSection\">Date/Time: ";
      server.printP(generalSectionOpen);
      
      server.print(t.year(), DEC);
      server.print('/');
      if(t.month() < 10) server.print("0");
      server.print(t.month(), DEC);
      server.print('/');
      if(t.day() < 10) server.print("0");
      server.print(t.day(), DEC);
      server.print(' ');
      if(t.hour() < 10) server.print("0");
      server.print(t.hour(), DEC);
      server.print(':');
      if(t.minute() < 10) server.print("0");
      server.print(t.minute(), DEC);
      server.print(':');
      if(t.second() < 10) server.print("0");
      server.print(t.second(), DEC);
      
      server.print(" (UTC ");
      server.print(int(config.utcOffset));
      server.println(")");
      server.println("</section>");
      
      P(sensorSectionOpen) = "<h2>Sensors</h2><section id=\"sensors\" class=\"mainSection\">";
      server.printP(sensorSectionOpen);
      
      for(int i=0;i<maxSensors;i++){
        
        if(config.sensors[i].type>0){
          
          server.println("<article>");
        
          server.print("<b>");
          server.print(config.sensors[i].name);
          server.print("</b><br/>");
  
          server.print(sensorTypeDisplayName(config.sensors[i]));        
          
          server.print("<h2>");
          server.print(config.sensors[i].statusValue);
          if(config.sensors[i].type==1){
            //server.print("%");
          }else if(config.sensors[i].type==2 or config.sensors[i].type==3){
            server.print("F");
          }
          server.print("</h2>");
          
          if(config.sensors[i].type==3){
            server.print("<h3>");
            server.print(config.sensors[i].statusValue2);
            if(config.sensors[i].type==3){
              server.print("%");
            }
            server.print("</h3>");
          }
          
          server.print("</article>");
        }
        
      }
  
      P(sensorSectionClose) = "</section><a href=\"/sensor-log.csv\" download=\"sensor-log.csv\">Donwload Sensor Log</a>";
      server.printP(sensorSectionClose);
  
      P(zoneSectionOpen) = "<h2>Zones</h2><section id=\"zones\" class=\"mainSection\">";
      server.printP(zoneSectionOpen);
      
      for(int i=0;i<maxZones;i++){
        
        if(config.zones[i].type>0){
          
          server.print("<article>");
        
          server.print("<b>");
          server.print(config.zones[i].name);
          server.print("</b><br/>");
          
          server.print(zoneTypeDisplayName(config.zones[i]));
          
          server.print("<h2>");
          server.print(displayYesNo(config.zones[i].isRunning,1));
          server.print("</h2>");
        
          server.println("</article>");
        }
        
      }
      
      P(zoneSectionClose) = "</section><a href=\"/zone-log.csv\" download=\"zone-log.csv\">Download Zone Log</a>";
      server.printP(zoneSectionClose);
      
      P(scheduleSectionOpen) = "<h2>Schedules</h2><section id=\"schedules\" class=\"mainSection\">";
      server.printP(scheduleSectionOpen);
      
      for(int i=0;i<maxSchedules;i++){
        
        if(config.schedules[i].type>0){
          
          server.println("<article>");
        
          server.print("<b>");
          server.print(config.schedules[i].name);
          server.println("</b><br/>");
          
          server.print(scheduleTypeDisplayName(config.schedules[i]));
          
          server.print("<h2>");
          server.print(displayYesNo(config.schedules[i].isRunning,1));
          server.print("</h2>");
          
          server.println("<span style=\"font-weight:bold;\">Zones</span><br/>");
          for(int z=0;z<maxZones;z++){
            int thisZoneNum = config.schedules[i].zones[z];
            thisZoneNum--;
            if(config.zones[thisZoneNum].type>0){
              server.println(config.zones[thisZoneNum].type);
              //server.println(config.zones[thisZoneNum].type);
              //server.println("=");
              //server.println(config.zones[thisZoneNum].name);
              //server.println(config.zones[thisZoneNum].name);
              server.println("<br/>");
            }
          }
          server.println("<br/>");
          
          server.println("<span style=\"font-weight:bold;\">Sensors</span><br/>");
          for(int s=0;s<maxSensors;s++){
            int thisSensorNum = config.schedules[i].sensors[s];
            thisSensorNum--;
            if(config.sensors[thisSensorNum].type>0){
              server.println(config.sensors[thisSensorNum].name);
              server.println("<br/>");
            }
          }
          server.println("<br/>");
          
          //timer
          if(config.schedules[i].type==1){
            P(scheduleTimerTableOpen) = "<table style=\"margin:auto;\"><tr><td>Weekdays</td><td>Hours</td><td>Minutes</td></tr><tr><td valign=\"top\">";
            server.printP(scheduleTimerTableOpen);
            for(int w=0;w<7;w++){
             if(config.schedules[i].timerStartWeekdays[w]>=0){
               server.print(displayWeekday((config.schedules[i].timerStartWeekdays[w])));
               server.println("<br/>");
             }
            }
            P(tdCloseOpen) = "</td><td valign=\"top\">";
            server.printP(tdCloseOpen);
            for(int h=0;h<24;h++){
             if(config.schedules[i].timerStartHours[h]>=0){
               server.print(config.schedules[i].timerStartHours[h]);
               server.println("<br/>");
             }
            }
            server.printP(tdCloseOpen);
            for(int m=0;m<60;m++){
             if(config.schedules[i].timerStartMinutes[m]>=0){
               server.print(config.schedules[i].timerStartMinutes[m]+1);
               server.println("<br/>");
             }
            }
            P(scheduleTimerTableClose) = "</td></tr></table>";
            server.printP(scheduleTimerTableClose);
          //soil moisture
          }else if(config.schedules[i].type==2){
            server.println("Minimum: ");
            server.println(config.schedules[i].valueMin);
            server.println("<br/>");
            server.println("Maximum: ");
            server.println(config.schedules[i].valueMax);
            server.println("<br/>");
          //temperature
          }else if(config.schedules[i].type==3){
            server.println("Minimum: ");
            server.println(config.schedules[i].valueMin);
            server.println("<br/>");
            server.println("Maximum: ");
            server.println(config.schedules[i].valueMax);
            server.println("<br/>");
          }
        
          server.println("</article>");
        }
        
      }
  
      server.println("</section>");   
     
      P(footer) = "<footer>&copy;Fat Rabbit Farms</footer>";
      server.printP(footer);
      P(ending) = "</head></html>";
      server.printP(ending);
      
    }
    
        
  }
  else
  {
    server.httpUnauthorized();
  }
  
}

void sensorLog(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
    
  char* authentication = getUserPassInBase64(config.adminUsername,config.adminPassword);
  
  if (server.checkCredentials(authentication))
  {
    
      server.httpSuccess("application/csv");
    
      if (type != WebServer::HEAD)
      {
    
        File myFile = SD.open(sensorLogFileName, FILE_READ);
        int16_t c;
        while ((c = myFile.read()) >= 0) {
          server.print((char)c);
        }
        myFile.close();
        
      }
          
  }
  else
  {
    server.httpUnauthorized();
  }
}

void zoneLog(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
   
  char* authentication = getUserPassInBase64(config.adminUsername,config.adminPassword);
  
  if (server.checkCredentials(authentication))
  {
    
      server.httpSuccess("application/csv");
    
      if (type != WebServer::HEAD)
      {
        
        File myFile = SD.open(zoneLogFileName, FILE_READ);
        int16_t c;
        while ((c = myFile.read()) >= 0) {
          server.print((char)c);
        }
        myFile.close();
    
      }
          
  }
  else
  {
    server.httpUnauthorized();
  }
}

