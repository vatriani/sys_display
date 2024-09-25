#include "monitor.hpp"

#include <Arduino.h>
#include <string>



/**
 * Definition of setup() in Arduino
 */
Monitor::Monitor ( ) {
  pinMode ( LED_BUILTIN, OUTPUT );

  serial = new SerialPort ( );
  display = new Display ( );
  data = new displayData;
}



Monitor::~Monitor ( ) {

}



void Monitor::parseSerial ( std::string ) {

}



/**
 * Arduino loop ()
 */
void Monitor::mainLoop ( ) {
  while ( 1 ) {
    if ( checkErrors ( ) == true ) {
      display->drawErr ( serial->getErr ( ).c_str ( ) );
    }
    else {
      serial->loop ( );

      display->draw ( );
      display->next ( );
    }
    digitalWrite ( LED_BUILTIN, HIGH );
    delay ( 1000 );
    digitalWrite ( LED_BUILTIN, LOW );
    delay ( 1000 );
  }
}


bool Monitor::checkErrors ( ) {
  if ( serial->getErr ( ).empty ( ) == true ) return false;
  else return true;
}
