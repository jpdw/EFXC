#include "examples.h"
#include "hal.h"
#include "Core.h"

extern Core core;

void cbControllerOutputsOff(String s){
    controller->off();
}

void cbTestSlide(String s){

    Serial.println("test_slide");
    controller->transitionDelay(50);

    controller->rgb(255,0,0);
    delay(3000);
    controller->rgb(0,255,0);
    delay(3000);
    controller->rgb(0,0,255);
    delay(3000);
    controller->rgb(153, 50, 204);
    delay(3000);
    controller->rgb(255,255,0);
    delay(3000);
    controller->rgb(0,255,0);
    delay(3000);
    controller->rgb(0,0,0);
}

void setup_examples(){
    core.commands.add("outoff", cbControllerOutputsOff);
    core.commands.add("test_slide", cbTestSlide);
}
