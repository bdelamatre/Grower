
//html variables
P(startOpen) = "<!DOCTYPE HTML><html><head><meta http-equiv=\"refresh\" content=\"300\"/><title>Status - Fat Rabbit Garden</title></head>";
P(startOpenAdmin) = "<!DOCTYPE HTML><html><head><title>Configuration - Fat Rabbit Garden</title></head>";
P(style) = "<style type=\"text/css\"> .mainSection{} nav{margin:auto;} body{text-align:center;} header{width:425px;margin:auto;} footer{width:425px;margin:auto;} section{width:425px;margin:auto;} article{margin:5px;padding:5px;} #general article{border:2px solid #000000;} #schedules article{border:2px solid #000000;} #sensors article{border:2px solid #000000;} #zones article{border:2px solid #000000; label{display:block;}}</style>";
P(startClose) = "<body>";
P(header) = "<header><h1>Garden Controller Status</h1><a href=\"/config\">Configuration</a></header>";
P(headerAdmin) = "<header><h1>Garden Controller Configuration</h1><a href=\"/\">Back</a></header>";
P(headerAdminSub) = "<header><h1>Garden Controller Configuration</h1><a href=\"/config\">Back</a></header>";
P(generalSectionOpen) = "<section id=\"general\" class=\"mainSection\"><h2>General</h2>";
P(sensorSectionOpen) = "<section id=\"sensors\" class=\"mainSection\"><h2>Sensors</h2>";
P(sensorSectionClose) = "<a href=\"/sensor-log.csv\" download=\"sensor-log.csv\">Donwload Sensor Log</a></section>";
P(zoneSectionOpen) = "<section id=\"zones\" class=\"mainSection\"><h2>Zones</h2>";
P(zoneSectionClose) = "<a href=\"/zone-log.csv\" download=\"zone-log.csv\">Download Zone Log</a></section>";
P(scheduleSectionOpen) = "<section id=\"schedules\" class=\"mainSection\"><h2>Schedules</h2>";
P(sectionClose) = "</section>";
P(articleOpen) = "<article>";
P(articleClose) = "</article>";      
P(footer) = "<footer>&copy;Fat Rabbit Farms<br/><a href=\"https://github.com/bdelamatre/FatRabbitGarden\">GIT Source</a></footer>";
P(ending) = "</head></html>";
P(formOpen) = "<form method=\"post\" action=\"/config\">";
P(formClose) = "</form>";
P(submitButton) = "<input type=\"submit\" />";
P(br) = "<br/>";
P(configNav) = "<nav><a href=\"/config-general\">General</a><br/><a href=\"/config-sensors\">Sensors</a><br/><a href=\"/config-zones\">Zones</a><br/><a href=\"/config-schedules\">Schedules</a></nav>";

char* getUserPassInBase64(String user, String pass){
  
  //fix-me: this could be way better
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


P(labelOpen) = "<label>";
P(labelClose) = "</label>";

void label(WebServer &server, char* name){
  server.printP(labelOpen);
  server.print(name);
  server.printP(labelClose);
}


P(elementTextOpen) = "<input type=\"text\" name=\"text_";
P(attributeBracket) = "\"";
P(elementRequired) = " required=\"true\"";
P(elementValueOpen) = " value=\"";
P(elementClose) = "/>";

void elementText(WebServer &server, String name, char* value, boolean required){
  server.printP(elementTextOpen);
  server.print(name);
  server.printP(attributeBracket);
  server.printP(elementValueOpen);
  server.print(value);
  server.printP(attributeBracket);
  //if(required==true){
  //  server.printP(elementRequired);
  //}
  server.printP(elementClose);
}

P(elementIntOpen) = "<input type=\"number\" name=\"int_";

void elementInt(WebServer &server, String name, int value){
  server.printP(elementIntOpen);
  server.print(name);
  server.printP(attributeBracket);
  server.printP(elementValueOpen);
  server.print(value);
  server.printP(attributeBracket);
  //if(required==true){
  //  server.printP(elementRequired);
  //}
  server.printP(elementClose);
}

P(elementIpOpen) = "<input type=\"number\" size=\"3\" maxlength=\"3\" min=\"0\" max=\"255\" name=\"ip_";

void elementIp(WebServer &server, String name, IPAddress ip){
  
  server.printP(elementIpOpen);
  server.print(name);
  server.print("_0");
  server.printP(attributeBracket);
  server.printP(elementValueOpen);
  server.print(ip[0]);
  server.printP(attributeBracket);
  //if(required==true){
  //  server.printP(elementRequired);
  //}
  server.printP(elementClose);
  
  server.printP(elementIpOpen);
  server.print(name);
  server.print("_1");
  server.printP(attributeBracket);
  server.printP(elementValueOpen);
  server.print(ip[1]);
  server.printP(attributeBracket);
  //if(required==true){
  //  server.printP(elementRequired);
  //}
  server.printP(elementClose);
  
  server.printP(elementIpOpen);
  server.print(name);
  server.print("_2");
  server.printP(attributeBracket);
  server.printP(elementValueOpen);
  server.print(ip[2]);
  server.printP(attributeBracket);
  //if(required==true){
  //  server.printP(elementRequired);
  //}
  server.printP(elementClose);
  
  server.printP(elementIpOpen);
  server.print(name);
  server.print("_3");
  server.printP(attributeBracket);
  server.printP(elementValueOpen);
  server.print(ip[3]);
  server.printP(attributeBracket);
  //if(required==true){
  //  server.printP(elementRequired);
  //}
  server.printP(elementClose);
}

P(elementCheckboxOpen) = "<input type=\"radio\" name=\"check_";
P(elementChecked) = "checked=\"true\"";

void elementCheckbox(WebServer &server, String name, boolean checked){
  
  server.printP(elementCheckboxOpen);
  server.print(name);
  server.printP(attributeBracket);
  server.printP(elementValueOpen);
  server.print("Yes");
  server.printP(attributeBracket);
  if(checked==true){
    server.printP(elementChecked);
  }
  server.printP(elementClose);
  server.print("Yes");
  
  server.printP(elementCheckboxOpen);
  server.print(name);
  server.printP(attributeBracket);
  server.printP(elementValueOpen);
  server.print("No");
  server.printP(attributeBracket);
  if(checked==true){
    server.printP(elementChecked);
  }
  server.printP(elementClose);
  server.print("No");
}

P(elementSelectTypeOpen) = "<select name=\"type_";
P(elementSelectTypeOpenEnd) = "\" >";
P(elementSelectClose) = "</select>";
P(elementSelectOptionOpen) = "<option value=\"";
P(elementSelectOptionSelected) = " selected";
P(elementSelectOptionClose) = "</option>";

void elementScheduleType(WebServer &server, String name, int type){
  server.printP(elementSelectTypeOpen);
  server.print(name);
  server.printP(elementSelectTypeOpenEnd);
  for(int i = 0;i<4;i++){
    server.printP(elementSelectOptionOpen);
    server.print(i);
    server.printP(attributeBracket);
    if(type==i){
      server.printP(elementSelectOptionSelected);
    }
    server.print(">");
    server.print(scheduleTypeDisplayName(i));
    server.printP(elementSelectOptionClose);
  }
  server.printP(elementSelectClose);
}

void elementZoneType(WebServer &server, String name, int type){
  server.printP(elementSelectTypeOpen);
  server.print(name);
  server.printP(elementSelectTypeOpenEnd);
  for(int i = 0;i<4;i++){
    server.printP(elementSelectOptionOpen);
    server.print(i);
    server.printP(attributeBracket);
    if(type==i){
      server.printP(elementSelectOptionSelected);
    }
    server.print(">");
    server.print(zoneTypeDisplayName(i));
    server.printP(elementSelectOptionClose);
  }
  server.printP(elementSelectClose);
}

void elementSensorType(WebServer &server, String name, int type){
  server.printP(elementSelectTypeOpen);
  server.print(name);
  server.printP(elementSelectTypeOpenEnd);
  for(int i = 0;i<4;i++){
    server.printP(elementSelectOptionOpen);
    server.print(i);
    server.printP(attributeBracket);
    if(type==i){
      server.printP(elementSelectOptionSelected);
    }
    server.print(">");
    server.print(sensorTypeDisplayName(i));
    server.printP(elementSelectOptionClose);
  }
  server.printP(elementSelectClose);
}

void elementZones(WebServer &server, struct Schedule &checkingSchedule, String name, int* type){
  for(int i=0;i<maxZones;i++){
    if(config.zones[i].type>0){
      boolean checked = false;
      for(int c=0;c<maxZones;c++){
        if(checkingSchedule.zones[c]>0 && i==(checkingSchedule.zones[c]+1)){
          checked = true;
        }
      }
      String name = String(name+"_"+i);
      char nameC[64];
      name.toCharArray(nameC,name.length());
      elementCheckbox(server,nameC,checked);
      label(server,config.zones[i].name);
      server.printP(br);
    }
  }
}

void elementZonesRunType(WebServer &server, String name, int type){
  //String html = "";
  //return html;
}

void elementSensors(WebServer &server, struct Schedule &checkingSensor, String name, int* type){
  for(int i=0;i<maxSensors;i++){
    if(config.sensors[i].type>0){
      boolean checked = false;
      for(int c=0;c<maxSensors;c++){
        if(checkingSensor.sensors[c]>0 && i==(checkingSensor.sensors[c]+1)){
          checked = true;
        }
      }
      String name = String(name+"_"+i);
      char nameC[64];
      name.toCharArray(nameC,name.length());
      elementCheckbox(server,nameC,checked);
      label(server,config.sensors[i].name);
      server.printP(br);
    }
  }
}

void elementTimerDays(WebServer &server, String name, int* type){
  //String html = "";
  //return html;
}

void elementTimerHours(WebServer &server, String name, int* type){
  //String html = "";
  //return html;
}

void elementTimerMinutes(WebServer &server, String name, int* type){
  ///String html = "";
  //return html;
}

#define NAMELEN 64
#define VALUELEN 64

void htmlConfig(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
    
  char* authentication = getUserPassInBase64(config.adminUsername,config.adminPassword);

  if (server.checkCredentials(authentication))
  {
    
    server.httpSuccess();
    
    server.printP(startOpenAdmin);
    server.printP(style);
    server.printP(startClose);
    server.printP(headerAdmin);
    server.printP(br);
    
    if(type == WebServer::POST){
      
      URLPARAM_RESULT rc;
      char name[NAMELEN]; 
      int  name_len;
      char value[VALUELEN];
      int value_len;
      
      //process post
      //fix-me: incredibly hacked to get this to work like I want
      while (server.readPOSTparam(name, NAMELEN, value, VALUELEN))
      {
        
         //convert to string for parse functions
         String nameS = String(name);
         
         //get first _ to determine field type
         int ss1 = nameS.indexOf("_");
         String type = nameS.substring(0,ss1);
         
         //remove type from the raw name
         nameS = nameS.substring(ss1+1);
         
         //check if is an array
         int ss2 = nameS.indexOf("_");
         
         String ending;
         
         //int ss3 = -1;
         //int structureArrayPosition = 0;
         //String secondaryName = "";
         
         if(ss2>-1){
           ending = nameS.substring(ss2+1);
           nameS = nameS.substring(0,ss2);
           
           //check if this is an array as well
           /*ss3 = ending.indexOf("_");
           
           if(ss3>-1){
             
             //important to convert this to an integer since it is variable
             structureArrayPosition = "";

             secondaryName = ending.substring(ss3+1);
                   
           }*/
           
         }
         
         server.print(type);
         server.print(",");
         server.print(nameS);
         if(ss2>-1){
           server.print(",");
           server.print(ending);
         }
         server.print("=");
         server.print(value);
         server.println("<br/>");
         
         if(nameS=="adminUsername"){
           config.adminUsername = value;
         }else if(nameS=="adminPassword"){
           config.adminPassword = value;
         }else if(nameS=="utcOffset"){
           config.utcOffset = strtoul(value, NULL, 10);
         }else if(nameS=="dhcp"){
           if(String(value)=="Yes"){
             config.dhcp = true;
           }else{
             config.dhcp = false;
           }
         }else if(nameS=="clientAddress"){
           if(ending=="0"){
             config.clientAddress[0] = strtoul(value, NULL, 10);
           }else if(ending=="1"){
             config.clientAddress[1] = strtoul(value, NULL, 10);
           }else if(ending=="2"){
             config.clientAddress[2] = strtoul(value, NULL, 10);
           }else if(ending=="3"){
             config.clientAddress[3] = strtoul(value, NULL, 10);
           }
         }else if(nameS=="clientNetmask"){
           if(ending=="0"){
             config.clientNetmask[0] = strtoul(value, NULL, 10);
           }else if(ending=="1"){
             config.clientNetmask[1] = strtoul(value, NULL, 10);
           }else if(ending=="2"){
             config.clientNetmask[2] = strtoul(value, NULL, 10);
           }else if(ending=="3"){
             config.clientNetmask[3] = strtoul(value, NULL, 10);
           }
         }else if(nameS=="clientGateway"){
           if(ending=="0"){
             config.clientGateway[0] = strtoul(value, NULL, 10);
           }else if(ending=="1"){
             config.clientGateway[1] = strtoul(value, NULL, 10);
           }else if(ending=="2"){
             config.clientGateway[2] = strtoul(value, NULL, 10);
           }else if(ending=="3"){
             config.clientGateway[3] = strtoul(value, NULL, 10);
           }
         }else if(nameS=="clientDns"){
           if(ending=="0"){
             config.clientDns[0] = strtoul(value, NULL, 10);
           }else if(ending=="1"){
             config.clientDns[1] = strtoul(value, NULL, 10);
           }else if(ending=="2"){
             config.clientDns[2] = strtoul(value, NULL, 10);
           }else if(ending=="3"){
             config.clientDns[3] = strtoul(value, NULL, 10);
           }
         }else if(nameS=="ntp"){
           if(String(value)=="Yes"){
             config.ntp = true;
           }else{
             config.ntp = false;
           }
         }else if(nameS=="ntpServer"){
           if(ending=="0"){
             config.ntpServer[0] = strtoul(value, NULL, 10);
           }else if(ending=="1"){
             config.ntpServer[1] = strtoul(value, NULL, 10);
           }else if(ending=="2"){
             config.ntpServer[2] = strtoul(value, NULL, 10);
           }else if(ending=="3"){
             config.ntpServer[3] = strtoul(value, NULL, 10);
           }
         }else if(nameS=="sensors"){
           for(int b=0;b<maxSensors;b++){
             //server.println("checking sensor: ");
             //server.println(b);
             //server.println(" against ");
             String checkAgainst = String(b)+"_type";
             //server.println(checkAgainst);
             //server.println("<br/>");
             if(String(String(b)+"_type")==ending){
               config.sensors[b].type = strtoul(value, NULL, 10);
             }else if(String(String(b)+"_name")==ending){
               config.sensors[b].name = value;
             }else if(String(String(b)+"_pin")==ending){
               config.sensors[b].pin = strtoul(value, NULL, 10);
             }else if(String(String(b)+"_pin2")==ending){
               config.sensors[b].pin2 = strtoul(value, NULL, 10);
             }else if(String(String(b)+"_frequencyCheckSeconds")==ending){
               config.sensors[b].frequencyCheckSeconds = strtoul(value, NULL, 10);
             }else if(String(String(b)+"_frequencyLogSeconds")==ending){
               config.sensors[b].frequencyLogSeconds = strtoul(value, NULL, 10);
             }
           }
           
         }else if(nameS=="zones"){
         }else if(nameS=="schedules"){
         }
         
         
      }
    
      //saveConfig();
    
      server.println("<h2>Configuration Saved!</h2>");  
      
    }
    
    if (type != WebServer::HEAD){
      
      server.printP(configNav);
      server.printP(br);
      
    }
    
    server.printP(footer);
    server.printP(ending);
    
  }else{
    server.httpUnauthorized();
  }
}

void htmlConfigGeneral(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
    
  char* authentication = getUserPassInBase64(config.adminUsername,config.adminPassword);

  if (server.checkCredentials(authentication))
  {
    server.httpSuccess();
    
    server.printP(startOpenAdmin);
    server.printP(style);
    server.printP(startClose);
    server.printP(headerAdminSub);
    
    if (type != WebServer::HEAD){
      
      server.printP(formOpen);
      server.printP(generalSectionOpen);
      server.printP(articleOpen);
      
      //show form
      //admin username
      label(server,"Username");
      elementText(server,"adminUsername",config.adminUsername,true);
      server.printP(br);
      
      //admin password
      label(server,"Password");
      elementText(server,"adminPassword",config.adminPassword,true);
      server.printP(br);
      //utc offset
      label(server,"UTC Offset");
      elementInt(server,"utcOffset",config.utcOffset);
      server.printP(br);
      //dhcp?
      label(server,"Use DHCP?");
      elementCheckbox(server,"dhcp",config.dhcp);
      server.printP(br);
      //ip address
      label(server,"Static Address");
      elementIp(server,"clientAddress",config.clientAddress);
      server.printP(br);
      //ip netmask
      label(server,"Static Netmask");
      elementIp(server,"clientNetmask",config.clientNetmask);
      server.printP(br);
      //ip gateway
      label(server,"Static Gateway");
      elementIp(server,"clientGateway",config.clientGateway);
      server.printP(br);
      //ip gateway
      label(server,"Static DNS");
      elementIp(server,"clientDns",config.clientDns);
      server.printP(br);
      //hardwaremac
      //ntpserver
      label(server,"Use NTP?");
      elementCheckbox(server,"ntp",config.ntp);
      server.printP(br);
      label(server,"NTP Server");
      elementIp(server,"ntpServer",config.ntpServer);
      server.printP(br);
      
      server.printP(articleClose);
      server.printP(sectionClose);
      
      server.printP(br);
      server.printP(submitButton);
      server.printP(br);
      server.printP(br);
      server.printP(formClose);
      
    }
    
    server.printP(footer);
    server.printP(ending);
    
  }else{
    server.httpUnauthorized();
  }
}

void htmlConfigZones(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
    
  char* authentication = getUserPassInBase64(config.adminUsername,config.adminPassword);

  if (server.checkCredentials(authentication))
  {
    server.httpSuccess();
    
    server.printP(startOpenAdmin);
    server.printP(style);
    server.printP(startClose);
    server.printP(headerAdminSub);
      
    if (type != WebServer::HEAD){
     
      server.printP(formOpen);
      server.printP(zoneSectionOpen);
      //zones
      for(int i=0;i<maxZones;i++){
        
        server.printP(articleOpen);
        
        String zoneName;
        zoneName += "zones_"+String(i)+"_";
        //int type; //0=off, 1=5v relay
        label(server,"Zone Type");
        elementZoneType(server,zoneName+"type",config.zones[i].type);
        server.printP(br);
        //name;
        label(server,"Zone Name");
        elementText(server,zoneName+"name",config.zones[i].name,true);
        server.printP(br);
        //pin;
        label(server,"Zone Pin");
        elementInt(server,zoneName+"pin",config.zones[i].pin);
        server.printP(br);
        //safetyOffAfterMinutes;
        label(server,"Safety Off (Minutes)");
        elementInt(server,zoneName+"safetyOffAfterMinutes",config.zones[i].safetyOffAfterMinutes);
        server.printP(br);
        
        server.printP(articleClose);
      }
      server.printP(sectionClose);
      
      server.printP(br);
      server.printP(submitButton);
      server.printP(br);
      server.printP(br);
      server.printP(formClose);
      
    }
    
    server.printP(footer);
    server.printP(ending);
    
  }else{
    server.httpUnauthorized();
  }
}

void htmlConfigSensors(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
    
  char* authentication = getUserPassInBase64(config.adminUsername,config.adminPassword);

  if (server.checkCredentials(authentication))
  {
    server.httpSuccess();
    
    server.printP(startOpenAdmin);
    server.printP(style);
    server.printP(startClose);
    server.printP(headerAdminSub);
    
    if (type != WebServer::HEAD){
      
      server.printP(formOpen);
      server.printP(sensorSectionOpen);
      
      //sensors
      for(int i=0;i<maxSensors;i++){
        
        server.printP(articleOpen);
        
        String sensorName = "sensors_"+String(i)+"_";
        //type; //0=off, 1=soil moisture (analog), 2=soil temperature(DS18B20), 3=air temperature (DHT22), 4=light
        label(server,"Sensor Type");
        elementSensorType(server,sensorName+"type",config.sensors[i].type);
        server.printP(br);
        //name;
        label(server,"Sensor Name");
        elementText(server,sensorName+"name",config.sensors[i].name,true);
        server.printP(br);
        //pin;
        label(server,"Sensor Pin");
        elementInt(server,sensorName+"pin",config.sensors[i].pin);
        server.printP(br);
        //pin2;
        label(server,"Sensor Pin2");
        elementInt(server,sensorName+"pin2",config.sensors[i].pin2);
        server.printP(br);
        //frequencyCheckSeconds; //0=every loop
        label(server,"Check Frequency (Seconds)");
        elementInt(server,sensorName+"frequencyCheckSeconds",config.sensors[i].frequencyCheckSeconds);
        server.printP(br);
        //frequencyLogSeconds; //0=every log
        label(server,"Log Frequency (Seconds)");
        elementInt(server,sensorName+"frequencyLogSeconds",config.sensors[i].frequencyLogSeconds);
        server.printP(br);
      
        server.printP(articleClose);
      
      }
      server.printP(sectionClose);
      
      server.printP(br);
      server.printP(submitButton);
      server.printP(br);
      server.printP(br);
      server.printP(formClose);
      
    }
    
    server.printP(footer);
    server.printP(ending);
    
  }else{
    server.httpUnauthorized();
  }
}

void htmlConfigSchedules(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
    
  char* authentication = getUserPassInBase64(config.adminUsername,config.adminPassword);

  if (server.checkCredentials(authentication))
  {
    server.httpSuccess();
    
    server.printP(startOpenAdmin);
    server.printP(style);
    server.printP(startClose);
    server.printP(headerAdminSub);
    
    if (type != WebServer::HEAD){
      
      server.printP(formOpen);
      
      server.printP(scheduleSectionOpen);
      
      //schedules
      for(int i=0;i<maxSchedules;i++){
      
        server.printP(articleOpen);
      
        String scheduleName = "schedule_"+String(i)+"_";
        //schedule type
        label(server,"Schedule Type");
        elementScheduleType(server,scheduleName+"[type]",config.schedules[i].type);
        server.printP(br);
        //name
        label(server,"Schedule Name");
        elementText(server,scheduleName+"[name]",config.schedules[i].name,true);
        server.printP(br);
        //zones
        label(server,"Zones");
        elementZones(server,config.schedules[i],scheduleName+"[zones]",config.schedules[i].zones);
        server.printP(br);
        //zones run type
        label(server,"Zone Run Type");
        elementZonesRunType(server,scheduleName+"[zonesRunType]",config.schedules[i].zonesRunType);
        server.printP(br);
        //sensors
        label(server,"Sensors");
        elementSensors(server,config.schedules[i],scheduleName+"[sensors]",config.schedules[i].sensors);
        server.printP(br);
        //timer days
        label(server,"Run Weekdays");
        elementTimerDays(server,scheduleName+"[timerStartWeekdays]",config.schedules[i].timerStartWeekdays);
        server.printP(br);
        //timer hours
        label(server,"Run Hours");
        elementTimerHours(server,scheduleName+"[timerStartHours]",config.schedules[i].timerStartHours);
        server.printP(br);
        //timer minutes
        label(server,"Run Minutes");
        elementTimerMinutes(server,scheduleName+"[timerStartMinutes]",config.schedules[i].timerStartMinutes);
        server.printP(br);
        //valueMin
        label(server,"Minimum Value");
        elementInt(server,scheduleName+"[valueMin]",config.schedules[i].valueMin);
        server.printP(br);
        //valueMax
        label(server,"Maximum Value");
        elementInt(server,scheduleName+"[valueMax]",config.schedules[i].valueMax);
        server.printP(br);
        
        server.printP(articleClose);
      }
      server.printP(sectionClose);
      
      server.printP(br);
      server.printP(submitButton);
      server.printP(br);
      server.printP(br);
      server.printP(formClose);
      
    }
    
    server.printP(footer);
    server.printP(ending);
    
  }else{
    server.httpUnauthorized();
  }
}


void htmlStatus(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  
  Serial.println(config.adminUsername);
  Serial.println(config.adminPassword);
  char* authentication = getUserPassInBase64(config.adminUsername,config.adminPassword);
  Serial.println(authentication);
  
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
  
          server.print(sensorTypeDisplayName(config.sensors[i].type));        
          
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
          
          server.print(zoneTypeDisplayName(config.zones[i].type));
          
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
          
          server.print(scheduleTypeDisplayName(config.schedules[i].type));
          
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
  
      server.printP(sectionClose);
     
      server.printP(footer);
      server.printP(ending);
      
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
        
        File myFile = SD.open("zone.log", FILE_READ);
        int16_t c;
        while ((c = myFile.read()) >= 0) {
          server.print((char)c);
        }
        myFile.close();
    
      }
          
  }else{
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
    
        File myFile = SD.open("sensor.log", FILE_READ);
        int16_t c;
        while ((c = myFile.read()) >= 0) {
          server.print((char)c);
        }
        myFile.close();
        
      }
          
  }else{
    server.httpUnauthorized();
  }
}


