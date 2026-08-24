#include "esp_funcs.h"

#include "build_info.h" // required for serialPrintInfo()

/* Output consistant version/ID info to Serial/debug */
void serialPrintInfo() {
    Serial.println();
    Serial.print("App version: ");
    Serial.println(__BI__BUILD_NUMBER);
//    Serial.print("Device ID: ");
//    Serial.println(device_id);
//    Serial.print("State: ");
    //Serial.println(((state==CONNECTED)?"Connected":"Unconnected"));

}

// Callback function to reset the prcoessor
void restart(String s){
    Serial.println("Restart initiated....");
    //mqtt_log("Restart initiated....");
    delay(1000);
    ESP.restart();
}



