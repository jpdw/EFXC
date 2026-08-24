#include "onoff.h"
#include "Core.h"

extern Core core;

// Real per-platform pin aliases - previously this file defined its own
// #define D5 5 / D6 6 / D7 7, which would shadow (or silently mismatch)
// the Arduino-ESP8266 core's own D5/D6/D7 constants (which resolve to the
// real GPIO14/12/13, not the literal integers 5/6/7) - see issue #3.
// ESP32 has no D5/D6/D7 framework aliases at all, so this branches the
// same way hal.cpp's CHANNEL_1/2/3 already do.
#ifdef ESP8266
    #define ONOFF_PIN_1 D5
    #define ONOFF_PIN_2 D6
    #define ONOFF_PIN_3 D7
#endif
#ifdef ESP32
    #define ONOFF_PIN_1 5
    #define ONOFF_PIN_2 6
    #define ONOFF_PIN_3 7
#endif

// This is EFXC's only current "effect" (per issue #3's comment): all
// three pins on (white), then off again 5 seconds later. Whether this
// should go through HwLedController (lib/Hal) instead of raw
// digitalWrite() - matching cbTestSlide's approach - is a real open
// question, deferred until issue #7 settles how/whether that controller
// actually gets configured on the live path.
void cbOnOffTurnOff(){
    Serial.println("OnOff: off");
    digitalWrite(ONOFF_PIN_1, LOW);
    digitalWrite(ONOFF_PIN_2, LOW);
    digitalWrite(ONOFF_PIN_3, LOW);
}

void cbOnOff(String s){
    static boolean initialized = false;
    if (!initialized){
        pinMode(ONOFF_PIN_1, OUTPUT);
        pinMode(ONOFF_PIN_2, OUTPUT);
        pinMode(ONOFF_PIN_3, OUTPUT);
        initialized = true;
    }

    Serial.println("OnOff: on");
    digitalWrite(ONOFF_PIN_1, HIGH);
    digitalWrite(ONOFF_PIN_2, HIGH);
    digitalWrite(ONOFF_PIN_3, HIGH);

    // Non-blocking "on for 5s, then off" - was a raw delay(5000) here
    // before (see issue #3), which blocked Core's own WLAN/MQTT handling
    // for the full 5 seconds on every trigger. One-shot via Core's
    // Scheduler instead - this callback returns immediately.
    core.scheduler.schedule(5000, cbOnOffTurnOff, false);
}

void setup_switcher(){
    core.commands.add("onoff", cbOnOff);
}
