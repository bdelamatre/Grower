Fat Rabbit Garden Controller

v.01 1/28/2013

A configurable controller that uses sensors, zones and schedules with a web interface for monitoring.


Hardware Requirements:
1) Arduino with > 75Kb of flash memmory (Mega 2560, Due)
2) Ethernet Shield with micro SD (https://www.adafruit.com/products/201)
3) Chronodot RTC (https://www.adafruit.com/products/255)

Supported Sensors:
1) DHT22 (temperature & humidity) (https://www.adafruit.com/products/393) 
2) Any analog soil moisture sensor (hygrometer) (check Ebay)

Recommended Hardware:
1) 24vac transformer, for standard 24v irrigation solenoid (http://www.radioshack.com/product/index.jsp?productId=2102703&filterName=Type&filterValue=Transformers)
2) Relay board, for controlling irrigation solenoids. I do NOT recommend these for large AC loads like grow lights, heaters, etc. (http://www.sainsmart.com/arduino-compatibles/module/relay.html)
3) Opto22 DC Controler Solid State Relays, for controlling power outlets or other large AC loads (http://www.opto22.com/site/pr_selector.aspx?cid=3&qs=10061016#1053)
4) Nema 3r or 4 enclosure, especially for any high voltage equipment. (Check Ebay)
5) Terminal blocks for making wire connections, or whatever you prefer

Software Setup:
1) Modify the config structure in init for your desired settings
2) Using the Arduino IDE upload the controller sketch
3) An Internet connection is required for NTP

Software To-do:
1) Read/Write configuration from EEPROM
2) Add administration capability to web interface