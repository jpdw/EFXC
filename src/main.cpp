


// Fundamentals
#include <Arduino.h>
#include "build_info.h"
#include "esp_funcs.h"
#include "hal.h"
#include "examples.h"
#include "switcher/onoff.h"
//#include "pwm.h"
#include "watchdog.h"
//#include "profile.h"
//#include "application.h"

// Networking - JIOT-Core's Core owns WLAN + MQTT (see Core::start()/handle())
#include "Core.h"
//#include "ota.h"
//#include "mdns_resolve.h"

Core core;

// Features - lights
//#include "fader.h"
//#include "lightning.h"
//#include "fade_around.h"
//#include "skull_eyes.h"

// Features - other
//#include "fogger.h"


#ifdef ESP32
  #include <esp32-hal-rgb-led.h>
  #include "esp32_funcs.h"
#endif

#ifdef ESP8266
#include "esp8266_funcs.h"
#endif 


// Legacy of some sort...
#ifdef ARDUINO_ESP8266_ESP_WROOM_02
  // File system size = 256KB
  FLASH_MAP_SETUP_CONFIG(FLASH_MAP_OTA_FS);
#endif
  #ifdef ARDUINO_ESP8266_D1_MINI
  // File system size = 2MB
  //FLASH_MAP_SETUP_CONFIG(FLASH_MAP_OTA_FS);
#endif




// put function declarations here:
//int myFunction(int, int);

void setup() {
  // Set-up serial
  Serial.begin(115200);
  Serial.println("main.spp: Setup start");

  serialPrintInfo();

  delay(5000);

  Serial.print("RGB_BUILTIN = ");
  Serial.println(RGB_BUILTIN);

#ifdef ESP8266
  Serial.print("RGB_RED   = ");
  Serial.println(RGB_RED);
  Serial.print("RGB_GREEN = ");
  Serial.println(RGB_GREEN);
  Serial.print("RGB_BLUE  = ");
  Serial.println(RGB_BLUE);
#endif

  Serial.print("RGB_BRIGHTNESS = ");
  Serial.println(RGB_BRIGHTNESS);

  // Green LED for 2 seconds to indicate start-up
  neopixelWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, 0);  // Green
  delay(2000);
  neopixelWrite(RGB_BUILTIN, 0, 0, 0);  // Off
  delay(2000);

  // Must be distinct from other Core-based devices on the same broker -
  // Mqtt's client_id is built from this, and a broker disconnects
  // whichever client already holds a duplicate id (seen live: this board
  // and JIOT-Core's own D1 Mini test both defaulted to "new node" and
  // kept fighting over the connection).
  core.setNodeName("EFXC");
  core.start();

  // Registers "outoff"/"test_slide" demo commands (see src/examples.cpp).
  // Safe to call even though setup_hardware() isn't wired up yet (#7,
  // deferred) - HwLedController::rgb()'s own guards (#1) make them a
  // no-op rather than a crash until the controller is actually configured.
  setup_examples();
  setup_switcher();

  Serial.println("main.spp: Setup finished");

}


// Non-blocking replacement for what used to be a chain of delay() calls in
// loop() below. Those delay()s (~3s total per pass) meant core.handle()
// only ran once every ~3s, which made JIOT-Core's AP-setup web server
// (and MQTT/scheduler) effectively unreachable - a delay() blocks
// everything, not just this animation. Same sequence/timing/labels as
// before, just advanced via millis() so it never blocks.
void ledDemoStep(){
  static uint8_t step = 0;
  static unsigned long nextStepAt = 0;
  if (millis() < nextStepAt){
    return;
  }

  switch (step){
    case 0:
      Serial.println("loop");
      nextStepAt = millis() + 100;
      break;
    case 1:
      digitalWrite(RGB_BUILTIN, HIGH);  // Turn the RGB LED white
      Serial.println("white");
      nextStepAt = millis() + 500;
      break;
    case 2:
      digitalWrite(RGB_BUILTIN, LOW);  // Turn the RGB LED off
      Serial.println("off");
      nextStepAt = millis() + 500;
      break;
    case 3:
      neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0);  // Red
      Serial.println("red");
      nextStepAt = millis() + 500;
      break;
    case 4:
      neopixelWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, 0);  // Green
      Serial.println("green");
      nextStepAt = millis() + 500;
      break;
    case 5:
      neopixelWrite(RGB_BUILTIN, 0, 0, RGB_BRIGHTNESS);  // Blue
      Serial.println("blue");
      nextStepAt = millis() + 500;
      break;
    case 6:
      neopixelWrite(RGB_BUILTIN, 0, 0, 0);  // Off / black
      nextStepAt = millis() + 500;
      break;
    case 7:
      neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0);  // Red
      Serial.println("red");
      nextStepAt = millis() + 250;
      break;
    case 8:
      neopixelWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, 0);  // Green
      Serial.println("green");
      nextStepAt = millis() + 250;
      break;
    case 9:
      neopixelWrite(RGB_BUILTIN, 0, 0, 0);  // Off / black
      nextStepAt = millis() + 250;
      break;
  }
  step = (step + 1) % 10;
}

void loop() {
  // core.handle() must run every pass, not gated behind the LED demo's
  // own timing - see ledDemoStep()'s comment above.
  core.handle();

  ledDemoStep();

  //watchdog_loop();
}
// put function definitions here:
//int myFunction(int x, int y) {
//  return x + y;
//}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////



// Whether to include capability to take directed "push" updates OTA
#define INCLUDE_OTA_PUSH



//##include <ESP8266WiFi.h>
#include <LittleFS.h>

#ifdef INCLUDE_OTA_PUSH
    #ifdef TARGET_esp8266
        #include <ESP8266mDNS.h>
    #endif
    #ifdef TARGET_esp32
        #include <ESPmDNS.h>
    #endif
  #include <WiFiUdp.h>
#endif
#include <EEPROM.h>

/* 
    Set Flash memory layout
    See Ch5 @ https://readthedocs.org/projects/arduino-esp8266/downloads/pdf/latest/
*/





//Profile profile;




//void serialPrintInfo();
//void trigger_delay();
//void pwm_loop();
//void pwm_write();

// remove this:
//extern void showFiles();

// Global store for device ID
//char * device_id; 

//enum ConnectedStates { UNCONNECTED = 0, CONNECTED = 1, MANAGED = 2 };
//ConnectedStates state;

//const char* mqttServer = "mqtt";

// GLOBALS
//bool global_mdns_enabled = false;

///* Identify the build */
//const char compile_date[] = __DATE__ " " __TIME__;

/*
void testCommands(){
    // Test the linked list
    
    String payload = "this is a string to test the linked list callback";

    // now get the matching command object
    Command * matched_command;
    matched_command = commands->getCommand("Test");

    void (*callbackfn)(String);

    if(matched_command != NULL){
    // A match was made - unpack and set the callback function
        callbackfn = matched_command->handler;
        Serial.println("About to test calling the callback function");
        Serial.print("  ");
        callbackfn(payload);
        Serial.println("Returned from the callback function");
    }else{
        Serial.println("Callback variable is NULL -- no callback function to call");
    }
}

// Help identify a Wemos D1 Mini by flashing the onboard LED a few times
void cbIdent(String params){

}

*/






// test dimming a single LED - assume this is an RGB board
/*
void shimmer(String s) {

    unsigned int red = 0, green = 0, blue = 0;
    
    controller->rgb(255,0,0);
    delay(1000);
    controller->rgb(0,0,0);

    // Turn off all LEDs

    // Pause for a few seconds
    delay(5000);

    controller->rgb(255,0,0);
    delay(1000);
    controller->rgb(0,255,0);
    delay(1000);
    controller->rgb(0,0,255);
    
    // Slowly ramp the red brightness
    
    Serial.println("shimmering start");
    for(red = 0; red < 10; red++){
        Serial.print("red = ");
        Serial.println(red);
        controller->rgb(red, green, blue);
        delay(500);
        yield();
    }

    for(red = 10; red >0 ; red--){
        Serial.print("red = ");
        Serial.println(red);
        controller->rgb(red, green, blue);
        delay(500);
        yield();
    }
    Serial.println("shimmering end");


}
*/







/**
 * 
 * Thisis the OLD Setup ... for reference 
 * 
 * 
    // Default connectivity state
    state = UNCONNECTED;

    Serial.println();
    Serial.print("Booting app version ");
    Serial.println(__BI__BUILD_NUMBER);

    // Generate device id based on MAC (e.g. A0B1C2)
    device_id = new char[7];
    #ifdef TARGET_esp8266
        sprintf(device_id,"%06X",ESP.getChipId());
    #endif
    #ifdef TARGET_esp32
        uint32_t id = 0;
        for(int i=0; i<17; i=i+8) {
            id |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
        }
        Serial.printf("%06X\n", id);
        sprintf(device_id,"%06X",id);
    #endif
    EEPROM.begin(512);

    // Start LittleFS
//#define LITTLEFS_FORMAT
//#ifdef LITTLEFS_FORMAT
    LittleFS.format(); // Run this once..!
    Serial.println("Formatted LittleFS");
//#endif

    if(!LittleFS.begin()){
        Serial.println("Unable to mount filesystem");
    }else{
        Serial.println("Mounted filesystem");
    }

    // Initialise the command interpretor/dispatcher so that other 
    //  modules will be able to add their own commands to during later
    //  setup functions. Once set-up, register any command from this
    // module.
    //
    setup_commands();
    commands->add("restart", restart);

    //commands->add("shimmer", shimmer);
    commands->add("onoff", cbOnOff);

    //
    //Try to start the WLAN
    // - use existing config if available
    //- otherwise boot to the WLAN setup utility
    
    if(setup_wlan() == true){
        state = CONNECTED;

        // Start MDNS
        if (global_mdns_enabled){
            mdns_setup();
        }

        mqtt_setup();
    
    }else{
       state = UNCONNECTED;
       settingMode = true;
    }

    setup_config();

    setup_ota();
    serialPrintInfo();

    //
    // Load application config
    //
    
    //Serial.print(config.get_appName());


    // 
    // Setup external hardware/IO
    //
    setup_hardware();

    //
    // Setup application modules
    //
    setup_fade_around();
    setup_effect_lightning();
    setup_pwm();

    setup_skulleyes();

    watchdog_start();
#endif 

#ifdef DEBUG
//    Serial.println("End of setup()");

#endif
}





// OLD LOOP

void loop() {


    watchdog_loop();

#ifdef TARGET_esp8266

    ota_loop();
    loop_config();

    // Process network-related requests
    loop_wlan();

    // Some loop functions should only run when in CONNECTED mode 
    if(state == CONNECTED){

        // Check for/process incoming MQTT requests - also maintain MQTT connectivity
        mqtt_loop();
        if (global_mdns_enabled){
            mdns_loop();
        }

        static bool once = false;
        if(!once){
            Serial.print("My IP Address is : ");
            Serial.println(ipAddress.toString());

            once=true;
        }
    }
    
    // Dispatch event-driven processes
    //dispatch_loop();
  
    // Feature loops
    loop_fade_around();           // Fade Around effect
    loop_skulleyes();           // Skull eyes
    fader_loop();               // RGB faders
    loop_effect_lightning();    // Lightning effect
    loop_effect_fogger();       // Fogger effect
    pwm_loop();                 // PWM effects

#endif

}

*/

