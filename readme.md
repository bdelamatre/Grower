Fat Rabbit Garden Controller
=========

v.01 1/28/2013
---------

A configurable controller that uses sensors, zones and schedules with a web interface for monitoring.


Hardware Requirements:
* Arduino with > 75Kb of flash memmory (Mega 2560, Due)
* Ethernet Shield with micro SD (https://www.adafruit.com/products/201)
* Chronodot RTC (https://www.adafruit.com/products/255)

Supported Sensors:
* DHT22 (temperature & humidity) (https://www.adafruit.com/products/393) 
* Any analog soil moisture sensor (hygrometer) (check Ebay)

Recommended Hardware:
* 24vac transformer, for standard 24v irrigation solenoid (http://www.radioshack.com/product/index.jsp?productId=2102703&filterName=Type&filterValue=Transformers)
* Relay board, for controlling irrigation solenoids. I do NOT recommend these for large AC loads like grow lights, heaters, etc. (http://www.sainsmart.com/arduino-compatibles/module/relay.html)
* Opto22 DC Controler Solid State Relays, for controlling power outlets or other large AC loads (http://www.opto22.com/site/pr_selector.aspx?cid=3&qs=10061016#1053)
* Nema 3r or 4 enclosure, especially for any high voltage equipment. (Check Ebay)
* Terminal blocks for making wire connections, or whatever you prefer

Software Setup:
* Modify the config structure in init for your desired settings
* Using the Arduino IDE upload the controller sketch
* An Internet connection is required for NTP

Software To-do:
* Read/Write configuration from EEPROM
* Add administration capability to web interface