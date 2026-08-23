#include "onoff.h"


    #define D5 5
    #define D6 6
    #define D7 7
    #define D0 0

void cbOnOff(String s){
    //pinMode(D0,OUTPUT);

    Serial.println("OnOff");
    //digitalWrite(D0,true);
    digitalWrite(D5,true);
    digitalWrite(D6,true);
    digitalWrite(D7,true);

    delay(5000);

    //digitalWrite(D0,false);
    digitalWrite(D5,false);
    digitalWrite(D6,false);
    digitalWrite(D7,false);
}