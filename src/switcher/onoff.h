#ifndef ONOFF_H_INCLUDED
#define ONOFF_H_INCLUDED

#include <Arduino.h>


void cbOnOff(String s);

// Registers "onoff" as an MQTT command via core.commands.add(...). Call
// once from main.cpp's setup(), after core.start().
void setup_switcher();

#endif
