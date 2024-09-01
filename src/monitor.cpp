#include "monitor.hpp"

#include <Arduino.h>



/**
 * Definition of setup() in Arduino
 */
Monitor::Monitor ( ) {
    pinMode ( LED_BUILTIN, OUTPUT );
    display = new Display ( );
    data = new displayData;
}



Monitor::~Monitor ( ) {

}



void Monitor::parseSerial ( char* buff ) {

}



/**
 * Arduino loop ()
 */
void Monitor::mLoop ( ) {
    while (1) {
        display->draw();
        display->next();
        digitalWrite ( LED_BUILTIN, HIGH );
        delay ( 1000 );
        digitalWrite ( LED_BUILTIN, LOW );
        delay ( 1000 );
    }
}
