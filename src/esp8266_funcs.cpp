
#include <Arduino.h>
#include "esp8266_funcs.h"

// Enable debugging to serial
//#define DEBUG

#ifdef ESP8266
void neopixelWrite(uint8_t pin, uint8_t red_val, uint8_t green_val, uint8_t blue_val){
  static bool initialized = false;

  #ifdef DEBUG
    Serial.println("neopixel for 8266");
  #endif
  
  if(!initialized){
    analogWriteRange(255);
    analogWriteFreq(10000);
    pinMode(RGB_RED, OUTPUT); 
    pinMode(RGB_GREEN, OUTPUT); 
    pinMode(RGB_BLUE, OUTPUT); 
    initialized = true;
  }

  analogWrite(RGB_RED, red_val);
  analogWrite(RGB_BLUE, blue_val);
  analogWrite(RGB_GREEN, green_val);

  #ifdef DEBUG
    Serial.println("neopixel for 8266 - end");
  #endif
}
#endif