#ifndef WLAN_H_INCLUDED
#define WLAN_H_INCLUDED

#include "Arduino.h"
#ifdef ESP8266
  #include "ESP8266WiFi.h"
#endif
#ifdef ESP32
  #include "WiFi.h"
#endif


void wlan_setup();
void wlan_loop();


#endif


#ifdef NEVER_TRY_TO_COMPILE

For reference:

void setupMode();  --> 
boolean restoreConfig();  --> 
boolean checkWlanConnection();  --> 
void setupMode();  --> 


void startWebServer();  --> Start webserver with configuration pages
 - would run to configure WLAN when in "setup mode" (as AP
 - would run when in STA mode to support a subset of web configs

String makePage(String, String);  --> 
 - 

String urlDecode(String);  --> 

String deviceID();  --> 

boolean wlanConnect(const char *,const char *);  --> Connect to a WLAN
 - Start the ESP functions to attempt WLAN connection
 - Does not block, so simply starts the process (see checkWlanConnection)

checkWlanConnection()
 - Continually check esp wlan status for 'connected'
 - update our own statas to 'associated'
 - get the IP address
 - timesout if connection not achived in tineout time of 15 seconds
 
try_wlan_connection(index)
 - calls wlanConnect (to initiate connection) 
 - then checkWlanConnection (to monitor for success)
 - for one of two profiles in turn


boolean restoreConfig();  --> 



** "standard" module functions **

boolean setup_wlan();  --> 
void loop_wlan();  --> 



#endif