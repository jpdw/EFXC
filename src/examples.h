#ifndef EXAMPLES_H_INCLUDED
#define EXAMPLES_H_INCLUDED

#include <Arduino.h>

// Demo/showcase MQTT command callbacks exercising HwLedController (see
// lib/Hal) - moved out of hal.cpp since a library shouldn't ship demo
// code (issue #4). "cb" = callback, matching the naming/intent of the
// Command mechanism these are meant to be registered against.
void cbControllerOutputsOff(String s);
void cbTestSlide(String s);

// Registers the above as MQTT commands via core.commands.add(...). Call
// once from main.cpp's setup(), after core.start() and after the HAL
// controller has been configured (see setup_hardware() in hal.h).
void setup_examples();

#endif
