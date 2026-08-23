#ifndef PERSISTENT_CONFIG_H_INCLUDED
#define PERSISTENT_CONFIG_H_INCLUDED

extern void persistent_config_setup();

// Class to load a single config file in to
class PersistentConfig {
    public:
        PersistentConfig();
        bool loadFile(const char *);                      /* load config from LittleFS */
        bool saveFile();                                  /* save config to LittleFS */
        //bool getConfig();                               /* get config (for module) from update server */
        //const char* get(const char* module, const char* key); /* get value for a key within a module */
        //void set(const char* module, const char* key, const char* value); /* set value for a key within a module */
        //bool exists(char* module);                      /* does a module exist in config? */
        //bool exists(char* module, char* key);           /* does a key exists for a module? */
        //void clear();
        void catFile();
        //void run();
        // For expedience... 
        //char* get_appName();
        //void set_appName(char* value);
        //void test1();
        //void test2();
        void setFilename(const char *);
    private:
        //char* _appName;
        const char * fileName;          // Name of config file
        //bool needToGetConfig = false;        
        //void set_string(char **, String);
};


#endif