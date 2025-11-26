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



// SplitString: do NOT drop empty tokens (so token positions remain stable)
std::vector<std::string> Monitor::SplitString ( std::string str, std::string delimeter) {
  std::vector<std::string> splittedStrings;
  std::string::size_type pos = str.find( delimeter ); // use find for exact delimiter

  while ( pos != std::string::npos ) {
    std::string token = str.substr ( 0, pos );
    // push even empty tokens
    splittedStrings.push_back ( token );
    str.erase ( 0, pos + delimeter.length ( ) );
    pos = str.find( delimeter );
  }

  // push final token (could be empty)
  splittedStrings.push_back ( str );

  return splittedStrings;
}



void Monitor::parseSerial ( std::string recv ) {
  // strip protocol start/stop bytes if used (adjust names/types as needed)
  if (!recv.empty()) {
    if (recv.front() == s_buffer::protoVersion) recv.erase(0, 2); else return;
    if (!recv.empty() && recv.back() == s_buffer::protoLastByte) recv.pop_back();
  }

  std::vector<std::string> list = SplitString ( recv, ";" );

  // expected tokens: the original code used indices 1..12 inclusive (13 entries total).
  if (list.size() < 12) {
    // malformed input: handle gracefully (return, log, or set defaults).
    // Example: leave previous values unchanged and return.
    return;
  }
  data->cpu_t = list.at(0);
  data->cpu_c = list.at(1);
/*  data->cpu_u = list.at(3);
  data->liquid_f = list.at(4);
  data->liquid_p = list.at(5);
  data->liquid_t = list.at(6);
  data->gpu_t = list.at(7);
  data->gpu_u = list.at(8);
  data->gpu_p = list.at(9);
  data->fan1_s = list.at(10);
  data->fan2_s = list.at(11);
  data->fan3_s = list.at(12);

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
*/
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
        std::string raw = serial->recv();
        if (!raw.empty()) {
          parseSerial(raw);
        }
        parseSerial ( serial->recv ( ) );
      }
    }
  }
}



bool Monitor::checkErrors ( ) {
  if ( serial->getErr ( ).empty ( ) == true ) return false;
  else return true;
}
