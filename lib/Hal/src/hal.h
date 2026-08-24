#ifndef HAL_H_INCLUDED
#define HAL_H_INCLUDED

#include <arduino.h>

/*
 * Define colours
 *
 * Not really sure if this was being used. 
 *
 */
class Colour{
    public:
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int bright;
    Colour(int red, int green, int blue);
    void set(int red, int green, int blue);
    Colour();
};


/*
 * Hardware Abstraction Layer
 * - Abstract the hardware ports on the platforms so that they present
 *   a common interface and support simplified application of effects
 *   at the application level
 * 
 * - Two objects are provided:
 *   - HwPort
 * 
 *     Represents a single output - abstracting it away from the lower level
 *     detail including operating PWM on the device, or 'brightness' as a %
 * 
 *   - HwLedController
 * 
 *     Representing a group of HwPorts used together as a single device e.g.
 *     three outputs controlling the red/green/blue elements of a traditional
 *     rgb LED package - potentially allowing the same interface to be used
 *     to control a 'neopixel' (single clocked-data channel) style LED.
 * 
 * 
 * 
 */

class HwPort{
    public:
        void begin(uint8_t, bool, String);
        //HwType type,
        uint8_t pin;
        boolean inverted;     
        String name;
        void on();
        void off();
        void toggle();
        bool pwm(unsigned int);
        bool pwm(float);
        void stop();
        unsigned int brightness = 100;          // scaling factor for brightness
        void trigger();                         // set (or reset) pwm from _nextPwm
        bool triggerMode(bool);                 // set/unset (& return) trigger mode
        bool triggerMode();                     // return trigger mode
    private:
        void _writePin(uint8_t);
        void _pwm(unsigned int);
        unsigned int _pwmMaxDutyCycle = 1024;   // maximum value for PWM duty cycle (e.g. 1024)
        unsigned int _nextPwm;                  // next value for the PWM duty cycle
        bool _triggerMode = false;              // whether to store new value till triggered
};

class HwLedController{
    public:
        HwLedController();                      // Base Constructor
        HwPort * outputs[4] = {nullptr, nullptr, nullptr, nullptr}; // unconfigured slots must be null, not garbage - see issue #1
        void begin(unsigned int);
        void add(unsigned int, uint8_t, bool, String);
        void output(unsigned int, bool);
        void outputConfig(unsigned int, unsigned int, bool, String);

        // Simple on/off
        void on(unsigned int = 0);              // ON  - single channel or 0 for all
        void off(unsigned int = 0);             // OFF - single channel or 0 for all

        // Combine 3 output channels into a single multicolour output
        void rgbConfig(unsigned int, unsigned int, unsigned int, bool = false); // Set Ports for Red, Green & Blue
        void rgb(unsigned int, unsigned int, unsigned int); // Specify as Red, Green & Blue channels
        void rgb(unsigned int);                 // Specify integer (RGB as 8-bit each i.e. 0xFF00FF)
        void trigger();                         // set (or reset) pwm from _nextPwm
        bool triggerMode(bool);                 // set/unset (& return) trigger mode
        bool triggerMode();                     // return trigger mod
        void transitionDelay(unsigned int);  // set a transition delay
        unsigned int transitionDelay();         // get current transition delay value
    private:
        // 0 = "not configured yet" (same convention as on()/off()'s 0 =
        // "all") - rgb() checks this before use, see issue #1.
        unsigned int channelRed = 0;
        unsigned int channelGreen = 0;
        unsigned int channelBlue = 0;
        unsigned int channelCount = 0;          // number of defined ports/channels
        unsigned int _transitionDelay = 0;       // time to pause during a transition
        bool _triggerMode = false;              // whether to store new value till triggered
};

void setup_hardware();

extern HwLedController * controller;

#endif