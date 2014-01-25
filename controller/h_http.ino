

/*void serveHttp(){
 
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {

          httpPrintHeader(client);
          client.println("<h1>Garden Gnome</h1>");
          client.println("DHCP");
          client.println(configStore.dhcp);
          client.println("<br/>");
          client.println("IP Address: ");
          client.println(Ethernet.localIP());
          client.println("<br/>");
          client.println("Device ID: ");
          client.println(getDeviceId());
          client.println("<br/>");
          client.println("Gnome Server: ");
          client.println(configStore.server);
          client.print(", port=");
          client.println(configStore.serverPort);
          client.println("<br/>");
          client.println("API Key: ");
          client.println(configStore.apiKey);
          client.println("<br/>");
          client.println("Config:");
          client.println(configStore.configId);
          client.println("<br/>");
          httpPrintFooter(client);
          
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
 
}

void httpPrintHeader(EthernetClient client){
  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<head>");
  client.println("</head>");
  client.println("<body>");

}

void httpPrintFooter(EthernetClient client){
  
  client.println("</body>");
  client.println("</html>");

}*/
