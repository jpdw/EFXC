
// Pin assignment for LEDs
// (which is quite different ESP32 Zero vs ESP8266)
#ifdef RGB_BUILTIN
#undef RGB_BUILTIN
#endif
#define RGB_BUILTIN 21
