#include "monitor.hpp"

#include <Arduino.h>
#include <string>

#include <vector>

#include "s_buffer.h"



/**
 * Definition of setup() in Arduino
 */
Monitor::Monitor ( ) {
  display = new Display ( );
  data = new displayData;
  serial = new SerialPort ( );

  display->setDisplayData ( data );
}



Monitor::~Monitor ( ) {

}



std::vector<std::string> Monitor::SplitString ( std::string str, std::string delimeter) {
  std::vector<std::string> splittedStrings;
  std::string::size_type pos = str.find_first_of( delimeter );

  while ( pos != std::string::npos ) {
    std::string token = str.substr ( 0, pos );
    if ( token.length ( ) > 0 )
      splittedStrings.push_back ( token );
    str.erase ( 0, pos + delimeter.length ( ) );
    pos = str.find_first_of( delimeter );
  }

  if ( str.length ( ) > 0 )
    splittedStrings.push_back ( str );

  return splittedStrings;
}



void Monitor::parseSerial ( std::string recv ) {
  std::vector<std::string> list = SplitString ( recv, ";" );

  data->cpu_t = list.at(1);
  data->cpu_c = list.at(2);
  data->cpu_u = list.at(3);
  data->liquid_f = list.at(4);
  data->liquid_p = list.at(5);
  data->liquid_t = list.at(6);
  data->gpu_t = list.at(7);
  data->gpu_u = list.at(8);
  data->gpu_p = list.at(9);
  data->fan1_s = list.at(10);
  data->fan2_s = list.at(11);
  data->fan3_s = list.at(12);
}



/**
 * Arduino loop ()
 */
void Monitor::mainLoop ( ) {
  while ( 1 ) {
    if ( checkErrors ( ) == true ) {
      display->drawErr ( serial->getErr ( ) );
    } else {
      display->draw ( );

      serial->loop ( );

      if ( serial->newData )
        parseSerial ( serial->recv ( ) );
    }
  }
}



bool Monitor::checkErrors ( ) {
  if ( serial->getErr ( ).empty ( ) == true ) return false;
  else return true;
}
