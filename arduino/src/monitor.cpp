#include "monitor.hpp"

#include <Arduino.h>
#include <string>
#include <vector>

#include "../../lib/serial_protocol.h"



/**
 * Definition of setup() in Arduino
 */
Monitor::Monitor ( ) {
  display = new Display ( );
  data = new displayData;
  serial = new SerialPort ( );

  display->setDisplayData ( data );

// some testdata
  data->cpu_t = "51.1";
  data->cpu_c = "5347";
  data->cpu_u = "2.0";
  data->liquid_f = "48";
  data->liquid_p = "40";
  data->liquid_t = "30.8";
  data->gpu_t = "56";
  data->gpu_u = "0";
  data->gpu_p = "79";
  data->fan1_s = "1675";
  data->fan2_s = "966";
  data->fan3_s = "1041";
}



Monitor::~Monitor ( ) {

}



std::vector<std::string> Monitor::SplitString ( std::string str, std::string delimeter) {
  std::vector<std::string> splittedStrings;
  std::string::size_type pos = str.find ( delimeter );

  while ( pos != std::string::npos ) {
    std::string token = str.substr ( 0, pos );
    splittedStrings.push_back ( token );
    str.erase ( 0, pos + delimeter.length ( ) );
    pos = str.find ( delimeter );
  }

  splittedStrings.push_back ( str );

  return splittedStrings;
}



void Monitor::parseSerial ( std::string recv ) {
  if ( !recv.empty ( ) ) {
    if ( recv.front ( ) == serial_protocol::protoVersion ) recv.erase ( 0, 2 ); else return;
    if ( !recv.empty ( ) && recv.back ( ) == serial_protocol::protoLastByte ) recv.pop_back ( );
  }

  std::vector<std::string> list = SplitString ( recv, ";" );

  if ( list.size ( ) < 12 ) {
    return;
  }

  data->cpu_t = list.at ( 0 );
  data->cpu_c = list.at ( 1 );
  data->cpu_u = list.at ( 2 );
  data->liquid_f = list.at ( 3 );
  data->liquid_p = list.at ( 4 );
  data->liquid_t = list.at ( 5 );
  data->gpu_t = list.at ( 6 );
  data->gpu_u = list.at ( 7 );
  data->gpu_p = list.at ( 8 );
  data->fan1_s = list.at ( 9 );
  data->fan2_s = list.at ( 10 );
  data->fan3_s = list.at ( 11 );
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

      if ( serial->newData ) {
        std::string raw = serial->recv ( );
        if ( !raw.empty ( ) ) {
          parseSerial ( raw );
        }
      }
    }
  }
}



bool Monitor::checkErrors ( ) {
  if ( serial->getErr ( ).empty ( ) == true ) return false;
  else return true;
}
