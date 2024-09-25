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

  display->setDisplayData ( data );

  // testdata
  data->cpu_u = "2";
  data->cpu_c = "5120";
  data->cpu_t = "41";
  data->gpu_t = "61";
  data->gpu_u = "2";
  data->gpu_p = "14";
  data->fan1_s = "761";
  data->fan2_s = "1028";
  data->fan3_s = "759";
  data->liquid_f = "620";
  data->liquid_p = "1800";
  data->liquid_t = "31,2";
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
