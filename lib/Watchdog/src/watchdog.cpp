#include "watchdog.h"
#include "Arduino.h"

//static unsigned long wd_millis_next_minute;
//static unsigned long wd_minutes;
//void watchdog_start(){
//    wd_millis_next_minute = millis() + 1000;
//    wd_minutes = 0;
//
//}

void watchdog_loop(){
    static unsigned long wd_minutes = 0;
    static unsigned long wd_millis_next_minute = millis() + 1000;

    if (millis() > wd_millis_next_minute){
        Serial.print("Watchdog: Minute counter = ");
        Serial.print(wd_minutes);
        Serial.print(";");
        //free_heap();
        Serial.println(" ");
        wd_millis_next_minute = wd_millis_next_minute + (60*1000);
        wd_minutes = wd_minutes + 1;
    }
}