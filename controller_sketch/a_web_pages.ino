
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


String label(String name){
  
  String html = "<label>";
  html += name;
  html += "</label>";
  return html;
}

String elementUtcOffset(String name, int value){
    String html;
    html += "<select name=\"utc_";
    html += name;
    html += "\" >";
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
  String html = "<input type=\"text\" size=\"3\" maxlength=\"3\" name=\"text_";
  html += name;
  html += "1\" value=\"";
  html += ip[0];
  html += "\"/>";
  
  html += "<input type=\"text\" size=\"3\" maxlength=\"3\" name=\"text_";
  html += name;
  html += "2\" value=\"";
  html += ip[1];
  html += "\"/>";
  
  html += "<input type=\"text\" size=\"3\" maxlength=\"3\" name=\"text_";
  html += name;
  html += "3\" value=\"";
  html += ip[2];
  html += "\"/>";
  
  html += "<input type=\"text\" size=\"3\" maxlength=\"3\" name=\"text_";
  html += name;
  html += "4\" value=\"";
  html += ip[3];
  html += "\"/>";
  return html;
}

String elementCheckbox(String name, boolean checked){
  String html = "<input type=\"checkbox\" name=\"check_";
  html += name;
  html += "\" />";
  return html;
}

String elementScheduleType(String name, int type){
  String html = "<select name=\"type_";
  html += name;
  html += "\" >";
  for(int i = 0;i<4;i++){
    html += "<option value=\"";
    html += i;
    html += "\"";
    if(type==i){
      html+= " selected";
    }
    html += ">";
    html += scheduleTypeDisplayName(config.schedules[i]);
    html +=" </option>";
  }
  return html;
}

String elementZoneType(String name, int type){
  String html = "<select name=\"type_";
  html += name;
  html += "\" >";
  for(int i = 0;i<4;i++){
    html += "<option value=\"";
    html += i;
    html += "\"";
    if(type==i){
      html+= " selected";
    }
    html += ">";
    html += zoneTypeDisplayName(config.zones[i]);
    html +=" </option>";
  }
  return html;
}

String elementZones(String name, int* type){
  String html = "";
  return html;
}

String elementZonesRunType(String name, int type){
  String html = "";
  return html;
}

String elementSensorType(String name, int type){
  String html = "<select name=\"type_";
  html += name;
  html += "\" >";
  for(int i = 0;i<5;i++){
    html += "<option value=\"";
    html += i;
    html += "\"";
    if(type==i){
      html+= " selected";
    }
    html += ">";
    html += sensorTypeDisplayName(config.sensors[i]);
    html +=" </option>";
  }
  return html;
}

String elementSensors(String name, int* type){
  String html = "";
  return html;
}

String elementTimerDays(String name, int* type){
  String html = "";
  return html;
}
String elementTimerHours(String name, int* type){
  String html = "";
  return html;
}

String elementTimerMinutes(String name, int* type){
  String html = "";
  return html;
}

#define NAMELEN 32
#define VALUELEN 32

void htmlAdmin(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
    
  char* authentication = getUserPassInBase64(config.adminUsername,config.adminPassword);

  if (server.checkCredentials(authentication))
  {
    server.httpSuccess();
    
    server.printP(startOpenAdmin);
    server.printP(style);
    server.printP(startClose);
    server.printP(headerAdmin);
      
    
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
      
      P(formStart) = "<form type=\"post\">";
      server.printP(formStart);
      
      P(br) = "<br/>";
      
      server.printP(generalSectionOpen);
      server.printP(articleOpen);
      
      //show form
      //admin username
      server.println(label("Username"));
      server.println(elementText("adminUsername",config.adminUsername));
      server.printP(br);
      //admin password
      server.println(label("Password"));
      server.println(elementText("adminPassword",config.adminPassword));
      server.printP(br);
      //utc offset
      server.println(label("UTC"));
      server.println(elementUtcOffset("utcOffset",config.utcOffset));
      server.printP(br);
      //dhcp?
      server.println(label("Use DHCP?"));
      server.println(elementCheckbox("dhcp",config.dhcp));
      server.printP(br);
      //ip address
      server.println(label("Static Address"));
      server.println(elementIp("clientAddress",config.clientAddress));
      server.printP(br);
      //ip netmask
      server.println(label("Static Netmask"));
      server.println(elementIp("clientNetmask",config.clientNetmask));
      server.printP(br);
      //ip gateway
      server.println(label("Static Gateway"));
      server.println(elementIp("clientGateway",config.clientGateway));
      server.printP(br);
      //hardwaremac
      //ntpserver
      server.println(label("Use NTP?"));
      server.println(elementCheckbox("ntp",config.ntp));
      server.printP(br);
      server.println(label("NTP Server"));
      server.println(elementIp("ntpServer",config.ntpServer));
      server.printP(br);
      
      server.printP(articleClose);
      server.printP(sectionEnd);
            
      server.printP(sensorSectionOpen);
      
      //sensors
      for(int i=0;i<maxSensors;i++){
        
        server.printP(articleOpen);
        
        String sensorName = "sensors[";
        sensorName += i;
        sensorName += "]";
        //type; //0=off, 1=soil moisture (analog), 2=soil temperature(DS18B20), 3=air temperature (DHT22), 4=light
        server.println(label("Sensor Type"));
        server.println(elementSensorType(sensorName+"[type]",config.sensors[i].type));
        server.printP(br);
        //name;
        server.println(label("Sensor Name"));
        server.println(elementText(sensorName+"[name]",config.sensors[i].name));
        server.printP(br);
        //pin;
        server.println(label("Sensor Pin"));
        server.println(elementInt(sensorName+"[pin]",config.sensors[i].pin));
        server.printP(br);
        //pin2;
        server.println(label("Sensor Pin2"));
        server.println(elementInt(sensorName+"[pin2]",config.sensors[i].pin2));
        server.printP(br);
        //frequencyCheckSeconds; //0=every loop
        server.println(label("Check Frequency (Seconds)"));
        server.println(elementInt(sensorName+"[frequencyCheckSeconds]",config.sensors[i].frequencyCheckSeconds));
        server.printP(br);
        //frequencyLogSeconds; //0=every log
        server.println(label("Log Frequency (Seconds)"));
        server.println(elementInt(sensorName+"[frequencyLogSeconds]",config.sensors[i].frequencyLogSeconds));
        server.printP(br);
      
        server.printP(articleClose);
      
      }
      server.printP(sectionEnd);
      server.printP(zoneSectionOpen);
      //zones
      for(int i=0;i<maxZones;i++){
        
        server.printP(articleOpen);
        
        String zoneName = "zones[";
        zoneName += i;
        zoneName += "]";
        //int type; //0=off, 1=5v relay
        server.println(label("Zone Type"));
        server.println(elementZoneType(zoneName+"[type]",config.zones[i].type));
        server.printP(br);
        //name;
        server.println(label("Zone Name"));
        server.println(elementText(zoneName+"[name]",config.zones[i].name));
        server.printP(br);
        //pin;
        server.println(label("Zone Pin"));
        server.println(elementInt(zoneName+"[pin]",config.zones[i].pin));
        server.printP(br);
        //safetyOffAfterMinutes;
        server.println(label("Safety Off (Minutes)"));
        server.println(elementInt(zoneName+"[safetyOffAfterMinutes]",config.zones[i].safetyOffAfterMinutes));
        server.printP(br);
        
        server.printP(articleClose);
      }
      server.printP(sectionEnd);
      
      server.printP(scheduleSectionOpen);
      
      //schedules
      for(int i=0;i<maxSchedules;i++){
      
        server.printP(articleOpen);
      
        //schedule type
        String scheduleName = "schedule[";
        scheduleName += i;
        scheduleName += "]";
        server.println(label("Schedule Type"));
        server.println(elementScheduleType(scheduleName+"[type]",config.schedules[i].type));
        server.printP(br);
        //name
        server.println(label("Schedule Name"));
        server.println(elementText(scheduleName+"[name]",config.schedules[i].name));
        server.printP(br);
        //zones
        server.println(label("Zones"));
        server.println(elementZones(scheduleName+"[zones]",config.schedules[i].zones));
        server.printP(br);
        //zones run type
        server.println(label("Zone Run Type"));
        server.println(elementZonesRunType(scheduleName+"[zonesRunType]",config.schedules[i].zonesRunType));
        server.printP(br);
        //sensors
        server.println(label("Sensors"));
        server.println(elementSensors(scheduleName+"[sensors]",config.schedules[i].sensors));
        server.printP(br);
        //timer days
        server.println(label("Run Weekdays"));
        server.println(elementTimerDays(scheduleName+"[timerStartWeekdays]",config.schedules[i].timerStartWeekdays));
        server.printP(br);
        //timer hours
        server.println(label("Run Hours"));
        server.println(elementTimerHours(scheduleName+"[timerStartHours]",config.schedules[i].timerStartHours));
        server.printP(br);
        //timer minutes
        server.println(label("Run Minutes"));
        server.println(elementTimerMinutes(scheduleName+"[timerStartMinutes]",config.schedules[i].timerStartMinutes));
        server.printP(br);
        //valueMin
        server.println(label("Minimum Value"));
        server.println(elementInt(scheduleName+"[valueMin]",config.schedules[i].valueMin));
        server.printP(br);
        //valueMax
        server.println(label("Maximum Value"));
        server.println(elementInt(scheduleName+"[valueMax]",config.schedules[i].valueMax));
        server.printP(br);
        
        server.printP(articleClose);
      }
      server.printP(sectionEnd);
      
      server.println("<button type=\"submit\" />");
      server.println("</form>");
    }
    
  }
  else
  {
    server.httpUnauthorized();
  }
}

void htmlStatus(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  
  char* authentication = getUserPassInBase64(config.adminUsername,config.adminPassword);
  
  if (server.checkCredentials(authentication))
  {
  
    server.httpSuccess();
  
    if (type != WebServer::HEAD)
    {
  
      DateTime t = getLocalTime();
  
      server.printP(startOpen);
      server.printP(style);
      server.printP(startClose);
      server.printP(header);
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
  
      server.printP(sensorSectionClose);
  
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
      
      server.printP(zoneSectionClose);
      
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
  
      server.printP(sectionEnd);
     
      P(footer) = "<footer>&copy;Fat Rabbit Farms<br/><a href=\"https://github.com/bdelamatre/FatRabbitGarden\">GIT Source</a></footer>";
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

