#include "wlan.h"


// To be removed to config - do not hardcode real credentials here, this
// file is committed to a public repo. See JIOT-Core's Wlan for the
// EEPROM/LittleFS-backed profile storage this should eventually use
// instead (see the wider revival-project integration plan).
const char * default_ssid = "";
const char * default_pass = "";







// networkState enumerator
// might be too simplistic and need more states
enum class NetworkState{
    WLAN_OFF = 0, // WLAN off / uninitialised
    WLAN_STA_CONNECTING = 3,  // Connection attempt started by not completed
    WLAN_STA_CONNECTED = 4,   // Connected (assume with IP/ GW etc)
    WLAN_AP_MODE = 2,
    WLAN_STARTUP = 1
};

// Declare a variable of this and set to an initial value 
NetworkState networkState = NetworkState::WLAN_OFF;


// Abstract away the platform's wifi - at least, attempt to
//
// 
class WlanInterface{
    public:
        WlanInterface();
        void connect();
        void loop();
        NetworkState networkState;
        const char * getSsid() { return this->_ssid; };
        void setSsid(const char * ssid) { this->_ssid = ssid; };
        void setPass(const char * pass) { this->_pass = pass; };
        void setSsidPass(const char *, const char *);
    private:
        void checkWlanConnection();
        const char * _ssid;
        const char * _pass;

};


WlanInterface::WlanInterface(){
    // Initialise a few things
    Serial.println("WlanInterface::constructor()");

    this->networkState = NetworkState::WLAN_OFF;

    // read in WLAN related config from json file
 //====>   this->readConfig();

    // if have got wlan network to connect to, try to connect
    // otherwise start AP mode
}

// Try to connect to the remote access point
void WlanInterface::connect(){
    Serial.println("WlanInterface::connect()");
    WiFi.begin();
    this->networkState = NetworkState::WLAN_STA_CONNECTING;
    Serial.println("Starting WLAN");

}

// Set both essid & password in one call -- convenience version of setters
void WlanInterface::setSsidPass(const char * ssid, const char * pass) {
    this->_ssid = ssid;
    this->_pass = pass;
}

// Start AP mode based on config
// if no config then use defaults from top of this file
//void WlanInterface::startStaMode(){
//}

void WlanInterface::loop(){
    this->checkWlanConnection();

}

// Monitor for WiFi.status() to have changed
// *to* WL_CONNECTED -- and trigger a 
// single call to ::Connected() function 
// (which could become a cb at some point)
void WlanInterface::checkWlanConnection(){
    static boolean connected = false;

    if(WiFi.status() == WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }else{
        if (connected = false){
            Serial.printf("\nJust connected\n");
            connected = true;
        }
    }
}



WlanInterface * wlan;

void wlan_setup(){
    Serial.println("wlan_setup: start");
    wlan = new WlanInterface;

    // Call persistent config to get WLAN setup details
    // persistent_config()......

    // Set WLAN detals
    wlan->setSsidPass(default_ssid, default_pass);

    // Attempt to connect WLAN
    wlan->connect();
    Serial.println("wlan_setup: complete");
}

void wlan_loop(){
    wlan->loop();
}