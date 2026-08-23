#include "hal.h"

#include <ArduinoJson.h>

// Define globally in this module
HwLedController * controller = new HwLedController;


#ifdef ESP8266
#define CHANNEL_1 D7
#define CHANNEL_2 D6
#define CHANNEL_3 D5
#endif
#ifdef ESP32
#define CHANNEL_1 7
#define CHANNEL_2 6
#define CHANNEL_3 5
#endif


//
// HwPort Class - Methods/functions
//
void HwPort::begin(uint8_t pin, bool inverted, String name){
    this->pin = pin;
    this->inverted = inverted;
    this->name = name;
    pinMode(this->pin,OUTPUT);
    Serial.print("setting output pin ");
    Serial.println(pin);
    this->off();
}

void HwPort::stop(){
}

void HwPort::_writePin(uint8_t new_state){
    if(this->inverted){
        //Serial.println("inverted");
        new_state = (new_state==HIGH)?LOW:HIGH;
    }else{
        //Serial.println("not inverted");

    }
    //Serial.print("pin = ");
    //Serial.println(this->pin);

    digitalWrite(this->pin, new_state);
}

void HwPort::_pwm(unsigned int dutyCycle){
    unsigned int scaledDutyCycle = dutyCycle * this->_pwmMaxDutyCycle / 256;
    // Scale for brightness
    scaledDutyCycle = scaledDutyCycle * this->brightness / 100;
    // Invert if required
    if(this->inverted){
        scaledDutyCycle = this->_pwmMaxDutyCycle - scaledDutyCycle;
    }
    this->_nextPwm = scaledDutyCycle;
    if(!this->_triggerMode){
        analogWrite(this->pin,scaledDutyCycle);
    }
}

void HwPort::trigger(){
    analogWrite(this->pin,_nextPwm);
}
bool HwPort::triggerMode(bool newState){
    this->_triggerMode = newState;
    return this->_triggerMode;
}

bool HwPort::triggerMode(){
    return this->_triggerMode;
}

void HwPort::on(){
    this->_writePin(HIGH);
}

void HwPort::off(){
    this->_writePin(LOW);
}

bool HwPort::pwm(unsigned int dutyCycle){
    if(dutyCycle > this->_pwmMaxDutyCycle){
        return false;
    }
    this->_pwm(dutyCycle);
    return true;
}

// Duty cycle expressed as a percentage ie 50.00 => 0.5 x 1024
bool HwPort::pwm(float dutyCycle){
    float d = dutyCycle * this->_pwmMaxDutyCycle / 100;
    unsigned int e = (unsigned int) d;
    this->_pwm(e);
    return true;
}

void HwLedController::begin(unsigned int port){
    this->outputs[1] = new HwPort;
}

void HwLedController::output(unsigned int port, bool state){
    HwPort * a = this->outputs[port];

    if(state == true){
        a->on();
    }else{
        a->off();
    }
}

void HwLedController::outputConfig(unsigned int port, unsigned int pin, bool inverted, String name){
    HwPort * hwPort = new HwPort;
    //Serial.print("port ");
    //Serial.print(port);
    //Serial.print(" setting inverted ");
    //Serial.println(inverted);
    hwPort->begin(pin, inverted, name);
    hwPort->brightness = 50;            // hard set brightness scaling
    hwPort->off();
    hwPort->triggerMode(false);

    this->outputs[port-1] = hwPort;
    
    // crude counter... should do this better:
    this->channelCount++;
}

// ON  - single channel or 0 for all
void HwLedController::on(unsigned int channel){
    
    //Serial.print("HwController::on");
    //Serial.println(channel);

    if(!channel){
        for(unsigned int i=0;i<this->channelCount;i++){
            this->outputs[i]->on();
        }
    }else{
        this->outputs[channel-1]->on();
    }
}

// OFF  - single channel or 0 for al
void HwLedController::off(unsigned int channel){
    if(!channel){
        for(unsigned int i=0;i<this->channelCount;i++){
            this->outputs[i]->off();
        }
    }else{
        this->outputs[channel-1]->off();
    }
}             

void HwLedController::rgbConfig(unsigned int red, unsigned int green, unsigned int blue, bool triggerMode){
    this->channelRed = red;
    this->channelGreen = green;
    this->channelBlue = blue;

    // set trigger mode
    this->triggerMode(triggerMode);
    for(unsigned int i=0;i<this->channelCount;i++){
        this->outputs[i]->triggerMode(triggerMode);
    }

    // set max scaling into each HwPort
    //this->outputs[this->channelRed]-> ??? *** CURRENTLY HARD SET IN HWPORT TO 1024
    //this->_maxPwmScaling = maxDC/256;
}

void HwLedController::rgb(unsigned int red, unsigned int green, unsigned int blue){

    // tranistion delay blank-before-change if active
    if(this->_transitionDelay){
        this->outputs[this->channelRed-1]->pwm((unsigned int) 0);
        this->outputs[this->channelGreen-1]->pwm((unsigned int)0);
        this->outputs[this->channelBlue-1]->pwm((unsigned int)0);

        if(this->triggerMode()){
            this->trigger();
        }

        delay(this->_transitionDelay);
    }

    this->outputs[this->channelRed-1]->pwm(red);
    this->outputs[this->channelGreen-1]->pwm(green);
    this->outputs[this->channelBlue-1]->pwm(blue);
    
    if(this->triggerMode()){
        this->trigger();
    }
}

void HwLedController::trigger(){
    for(unsigned int i=0;i<this->channelCount;i++){
        this->outputs[i]->trigger();
    }
}

bool HwLedController::triggerMode(bool newState){
    this->_triggerMode = newState;
    return this->_triggerMode;
}

bool HwLedController::triggerMode(){
    return this->_triggerMode;
}

void HwLedController::transitionDelay(unsigned int transitionDelay){
    this->_transitionDelay = transitionDelay;
}

unsigned int HwLedController::transitionDelay(){
    return this->_transitionDelay;
}




Colour::Colour(int red, int green, int blue){
    this->red=red*1023/255;
    this->green=green*1023/255;
    this->blue=blue*1023/255;
};
void Colour::set(int red, int green, int blue){
    this->red=red;
    this->green=green;
    this->blue=blue;
}

Colour *colour = new Colour(0,0,0);

// return value for analogWrite
unsigned int c(int x, int maximum, int pc){
    int r = x * pc / 100;   // scale it by %
    r = maximum - r;        // invert it
    return(r);
}


//
// Should be moved out of hal.cpp
//
void cbControllerOutputsOff(String S){
    controller->off();
}

void cbTestSlide(String S){

    Serial.println("test_slide");
    controller->transitionDelay(50);

    controller->rgb(255,0,0);
    delay(3000);
    //controller->rgb(0,0,0);
    //delay(50);
    controller->rgb(0,255,0);
    delay(3000);
    //controller->rgb(0,0,0);
    //delay(50);
    controller->rgb(0,0,255);
    delay(3000);
    //controller->rgb(0,0,0);
    //delay(50);
    controller->rgb(153, 50, 204);
    delay(3000);
    //controller->rgb(0,0,0);
    //delay(50);
    controller->rgb(255,255,0);
    delay(3000);
    //controller->rgb(0,0,0);
    //delay(50);
    controller->rgb(0,255,0);
    delay(3000);
    controller->rgb(0,0,0);



}



void setup_hardware(){
    /* 
    // CB007 - 4 CH ON/OFF
    hwPorts[n(1)].begin(D7, false, "Green spot");
    hwPorts[n(2)].begin(D6, false, "Unused");
    //hwPorts[n(3)].begin(D5, false, "Blue spot");
    hwPorts[n(4)].begin(D0, false, "White strip");
    */

    //
    // CB006 - 3 CH RGB
    //

    bool inverted = false;
    // Configure which output channels are red, blue & green
    controller->outputConfig(1, CHANNEL_3, inverted, "Red"); // channel #, pin, inverted, channel name
    controller->outputConfig(2, CHANNEL_2, inverted, "Green");
    controller->outputConfig(3, CHANNEL_1, inverted, "Blue");
    
    // Configure which output channels are red, blue & green
    controller->rgbConfig(1,2,3,true);
}


void test_deserialise(){
    //StaticJsonDocument<512> payload;

    DynamicJsonDocument doc(2048);

    const char * json = "\
    {\
        \"hardware\": {\
            \"name\": \"CB007-01\",\
            \"outputs\":[\
                {\"pin\": \"d7\", \"inverted\": true, \"name\":\"Channel 1\"},\
                {\"pin\": \"d6\", \"inverted\": true, \"name\":\"Channel 2\"},\
                {\"pin\": \"d5\", \"inverted\": true, \"name\":\"Channel 3\"},\
                {\"pin\": \"d0\", \"inverted\": true, \"name\":\"Channel 4\"}\
            ],\
            \"inputs\":[\
                {\"pin\": \"d3\", \"name\": \"DS18B20\"}\
            ]\
        }\
    }\
    ";

    deserializeJson(doc, json);
    JsonObject object = doc.as<JsonObject>();
    //const char * stuff = object["hardware"]["name"];

    if(object.containsKey("hardware")){
        Serial.println("has hardware");

        
        JsonVariant j = object["hardware"]["outputs"];
        if(!j.isNull()){
            JsonArray a = j.as<JsonArray>();
            Serial.println("Has hardware.outputs");
            Serial.println(j.size());

            for(unsigned int i=0;i<j.size();i++){

                Serial.println(a["name"]);
            }

        }
    }

    //Serial.println(stuff);
    //JsonObject row = object["hardware"]["outputs"][0];
    //char * portStr = object["hardware"]["outputs"][0]["pin"];

    //char * s = row["pin"];
    //Serial.println(s);

//    bool inverted = (stuff2["inverted"]=="false")?false:true;
//    char * name = stuff2["name"];
//    hwPorts[n(3)].begin(D5, inverted, name);


    //Serial.println(stuff2);

}

