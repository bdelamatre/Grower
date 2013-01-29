
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
    /*P(sectionStart) = "<section>";
    P(sectionEnd) = "</section>";
    P(tableOpen) = "<table>";
    P(tableEnd) = "</table>";
    P(trOpen) = "<tr>";
    P(trEnd) = "</tr>";
    P(tdOpen) = "<td>";
    P(tdEnd) = "</td>";*/
    
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
        server.print(displayYesNo(config.zones[i].statusRun,1));
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
        server.print(displayYesNo(config.schedules[i].statusRun,1));
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
          server.println(config.schedules[i].moistureMin);
          server.println("<br/>");
          server.println("Maximum: ");
          server.println(config.schedules[i].moistureMax);
          server.println("<br/>");
        //temperature
        }else if(config.schedules[i].type==3){
          server.println("Minimum: ");
          server.println(config.schedules[i].temperatureMin);
          server.println("<br/>");
          server.println("Maximum: ");
          server.println(config.schedules[i].temperatureMax);
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

