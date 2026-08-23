#include "persistent_config.h"

#include <Arduino.h>
#include <ArduinoJson.h>


#include <FS.h>
#include <LittleFS.h>

//#include "commands.h"
//#include "application.h"
//#include "mdns_resolve.h"
//#include "mqtt.h"

#define DEBUG_CONFIG

/*******************************************************************************
 * Little FS helper functions
 * 
 * Although this src file is for the persistant_config class, because that class
 * depends very highly on the LittleFS library that interacts with the actual
 * it makes sense to put the handful of LittleFS helper functions in here.
 * 
 ******************************************************************************/

#define FORMAT_LITTLEFS_IF_FAILED true

void littleFsSetup(){

    // Mount the LittleFS filesystem - or create from scratch if 
    // unable to mount it
    if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
        Serial.println("LittleFS Mount Failed");
    }else{
        Serial.println("LittleFS Mount Successful");
    }

    // Do anything else....

}



const char * applicationConfigFile = "app.cfg";

// Create "PersistendConfig" as variable appConfig;
PersistentConfig appConfig;

//bool configCheckForUpdateOnBoot = CHECK_FOR_UPDATE_ON_BOOT;
//const char* configAppName =  "Undefined";

#define FORMAT_LITTLEFS_IF_FAILED true

PersistentConfig::PersistentConfig(){
    Serial.println("constructor");

}



/**
 * loadFile - Read in json config file
 * de-serialise to internal key/value storage
 *  
 * @param none
 * @return true if file found, false otherwise
 */
bool PersistentConfig::loadFile(const char* filename){
    Serial.println("PersistentConfig::loadFile: start of loadFile");
    Serial.print("Filename is ");
    Serial.println(filename);

    File f = LittleFS.open(filename);
    Serial.print("outcome of file open = ");

    if(!f || f.isDirectory()){
        Serial.println("- failed to open file for reading");
    }else{
        Serial.println("0 sucess opening file for reading");
    }

    return true;    // Indicate success or failure
}



// Write config, as json, to LittleFS file
bool PersistentConfig::saveFile(){
/*
    StaticJsonDocument<200> jsonDoc;
    JsonObject json = jsonDoc.createNestedObject();

    File configFile = LittleFS.open(applicationConfigFile, "w");
    if (!configFile) {
        Serial.println("Failed to open config file for writing");
        return false;
    }
    serializeJsonPretty(json, configFile);
    configFile.close();
    return true;
*/
}



void PersistentConfig::catFile(){
/*
    File configFile = LittleFS.open(applicationConfigFile, "r");

    if (!configFile) {
        Serial.println("Failed to open config file");
    }

    while(configFile.available()) {
        Serial.write(configFile.read());
    }
    configFile.close();
    Serial.println("");
*/
}


// Create a sample file -- chicken vs egg
void littleFsCreateTempFile(){
    //PersistentConfig * newConfig = new PersistentConfig();
    // Set the filename
    //newConfig->setFilename(applicationConfigFile); 
    // Creat some data in the 
}


PersistentConfig* wifiConfig;

void persistent_config_setup(){

    //PersistentConfig appConfig = new PersistentConfig;

    Serial.println("persistent_config.cpp: Start of persistent_config_setup");
    // Make sure littleFS is mounted
    littleFsSetup();

    // Now load core app config (maybe this shouldn't happen here?)
    //Serial.println("Before constructor");
    //appConfig = new PersistentConfig();
    //Serial.println("After constructor");
    Serial.println("persistent_config.cpp: persistent_config_setup calling loadFile");

    appConfig.loadFile("/def.xyz");

    delay(5000);

    wifiConfig = new PersistentConfig();
    wifiConfig->loadFile("/wifi.conf");
    
    Serial.println("persistent_config.cpp: persistent_config_setup finished");

}
