
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

String elementUtcOffset(String name, int value){
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
    if(value==i){
      html+= " selected";
    }
    html += ">";
    if(i>0){
      html += "+";
    }else{
      html += "-";
    }
    html += scheduleTypeDisplayName(i);
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
    if(value==i){
      html+= " selected";
    }
    html += ">";
    if(i>0){
      html += "+";
    }else{
      html += "-";
    }
    html += zoneTypeDisplayName(i);
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
    if(value==i){
      html+= " selected";
    }
    html += ">";
    if(i>0){
      html += "+";
    }else{
      html += "-";
    }
    html += sensorTypeDisplayName(i);
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

void adminCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  
  //fix-me: a bit lengthy to get a base64 encoded char*
  char* concatC;
  String concatS;
  char* credentialC;
  concatS += config.adminUsername;
  concatS += ":";
  concatS += config.adminPassword;
  concatS.toCharArray(concatC,concatS.length());
  base64_encode(credentialC,concatC,sizeof(concatC));
  
  /* if the user has requested this page using the following credentials
   * username = user
   * password = user
   * display a page saying "Hello User"
   *
   * the credentials have to be concatenated with a colon like
   * username:password
   * and encoded using Base64 - this should be done outside of your Arduino
   * to be easy on your resources
   *
   * in other words: "dXNlcjp1c2Vy" is the Base64 representation of "user:user"
   *
   * if you need to change the username/password dynamically please search
   * the web for a Base64 library */
  if (server.checkCredentials(credentialC))
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
      server.println("<button type=\"submit\" />");
      server.println("</form>");
    
  }
  else
  {
    /* send a 401 error back causing the web browser to prompt the user for credentials */
    server.httpUnauthorized();
  }
}

    
void htmlStatus(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  server.httpSuccess();

  if (type != WebServer::HEAD)
  {

    DateTime t = getLocalTime();

    P(startOpen) = "<!DOCTYPE HTML><html><head><meta http-equiv=\"refresh\" content=\"10\"/><title>Status - Fat Rabbit Garden</title></head>";
    server.printP(startOpen);
    P(style) = "<style>.mainSection{} #general{} #schedules{} #sensors{} #zones{}</style>";
    server.printP(style);
    P(startClose) = "<body style=\"text-align:center;\">";
    server.printP(startClose);
    P(header) = "<header><h1>Fat Rabbit Farm - Garden Controller</h1></header>";
    server.printP(header);
    
    server.println("<h2>General</h2>");
    server.println("<section id=\"general\" class=\"mainSection\">");
    server.print("Date/Time: ");
    server.print(getDateTime(t));
    server.print(" (UTC ");
    server.print(config.utcOffset);
    server.println(")");
    server.println("</section>");
    
    server.println("<h2>Sensors</h2>");
    server.println("<a href=\"/sensor-log.csv\" download=\"sensor-log.csv\">Donwload Log</a>");
    server.println("<section id=\"sensors\" class=\"mainSection\">");
    
    for(int i=0;i<maxSensors;i++){
      
      if(config.sensors[i].type>0){
        
        server.println("<article>");
      
        server.print("<b>");
        server.print(config.sensors[i].name);
        server.print("</b>");

        server.print(sensorTypeDisplayName(config.sensors[i]));        
        
        server.print("<h2>");
        server.print(config.sensors[i].statusValue);
        if(config.sensors[i].type==1){
          server.print("%");
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

    server.print("</section>");

    server.print("<h2>Zones</h2>");
    server.print("<a href=\"/zone-log.csv\" download=\"zone-log.csv\">Download Log</a>");
    server.print("<section id=\"zones\" class=\"mainSection\">");
    
    for(int i=0;i<maxZones;i++){
      
      if(config.zones[i].type>0){
        
        server.print("<article>");
      
        server.print("<b>");
        server.print(config.zones[i].name);
        server.print("</b>");
        
        server.print(zoneTypeDisplayName(config.zones[i]));
        
        server.print("<h2>");
        server.print(displayYesNo(config.zones[i].isRunning,1));
        server.print("</h2>");
        //server.print("Safety Off? ");
        //server.print(displayYesNo(config.zones[i].statusSafetyOff));
        //server.print("<br/>");
      
        server.println("</article>");
      }
      
    }

    server.println("</section>");  
   
   
    server.println("<h2>Schedules</h2>");
    //server.println("<a href=\"/schedule-log.csv\" download=\"schedule-log.csv\">Download Log</a>");
    server.println("<section id=\"schedules\" class=\"mainSection\">");
    
    for(int i=0;i<maxSchedules;i++){
      
      if(config.schedules[i].type>0){
        
        server.println("<article>");
      
        server.print("<b>");
        server.print(config.schedules[i].name);
        server.println("</b>");
        
        server.print(scheduleTypeDisplayName(config.schedules[i]));
        
        server.print("<h2>");
        server.print(displayYesNo(config.schedules[i].isRunning,1));
        server.print("</h2>");
        
        server.println("<span style=\"font-weight:bold;\">Zones</span><br/>");
        for(int z=0;z<maxZones;z++){
          int thisZoneNum = config.schedules[i].zones[z];
          if(thisZoneNum>0){
            server.println(config.zones[thisZoneNum].name);
            server.println("<br/>");
          }
        }
        
        server.println("<span style=\"font-weight:bold;\">Sensors</span><br/>");
        for(int s=0;s<maxSensors;s++){
          int thisSensorNum = config.schedules[i].sensors[s];
          if(thisSensorNum>0){
            server.println(config.sensors[thisSensorNum].name);
            server.println("<br/>");
          }
        }
        
        //timer
        if(config.schedules[i].type==1){
          server.println("<table>");
          server.println("<tr>");
          server.println("<td>Weekdays</td>");
          server.println("<td>Hours</td>");
          server.println("<td>Minutes</td>");
          server.println("</tr>");
          server.println("<tr>");
          server.println("<td>");
          for(int w=0;w<7;w++){
           if(config.schedules[i].timerStartWeekdays[w]>=0){
             server.print(displayWeekday((config.schedules[i].timerStartWeekdays[w]+1)));
             server.println("<br/>");
           }
          }
          server.println("</td>");
          server.println("<td>");
          for(int h=0;h<24;h++){
           if(config.schedules[i].timerStartHours[h]>=0){
             server.print(config.schedules[i].timerStartHours[h]+1);
             server.println("<br/>");
           }
          }
          server.println("</td>");
          server.println("<td>");
          for(int m=0;m<60;m++){
           if(config.schedules[i].timerStartMinutes[m]>=0){
             server.print(config.schedules[i].timerStartMinutes[m]+1);
             server.println("<br/>");
           }
          }
          server.println("</td>");
          server.println("</tr>");
          server.println("</table>");
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

void sensorLog(WebServer &server, WebServer::ConnectionType type, char *, bool)
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

void zoneLog(WebServer &server, WebServer::ConnectionType type, char *, bool)
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

/*void statusHtml(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  server.httpSuccess();

  if (type != WebServer::HEAD)
  {
    
    File myFile = SD.open("status.html", FILE_READ);
    int16_t c;
    while ((c = myFile.read()) >= 0) {
      server.print((char)c);
    }
    myFile.close();

  }
  
}*/

