#include "monitor.hpp"

#include <Arduino.h>
#include <string>

#include "s_buffer.h"



/**
 * Definition of setup() in Arduino
 */
Monitor::Monitor ( ) {
  //pinMode ( LED_BUILTIN, OUTPUT );

  previousMillis = 0;
  statusLED = false;
  update = true;

  display = new Display ( );
  data = new displayData;
  serial = new SerialPort ( );
  updatePage = new Timer ( 5000 );

  display->setDisplayData ( data );
}



Monitor::~Monitor ( ) {

}



std::vector<std::string> Monitor::SplitString ( std::string str, std::string delimeter) {
  std::vector<std::string> splittedStrings = {};
  size_t pos = 0;

  while ( ( pos = str.find ( delimeter ) ) != std::string::npos ) {
    std::string token = str.substr ( 0, pos );
    if ( token.length ( ) > 0 )
      splittedStrings.push_back ( token );
    str.erase ( 0, pos + delimeter.length ( ) );
  }

  if ( str.length ( ) > 0 )
    splittedStrings.push_back ( str );
  return splittedStrings;
}



void Monitor::toggleStatus ( ) {
  if (statusLED == true ) {
    digitalWrite ( LED_BUILTIN, LOW );
    statusLED = false;
  }
  else {
    digitalWrite ( LED_BUILTIN, HIGH );
    statusLED = true;
  }
}



void Monitor::parseSerial ( std::string recv ) {
  std::vector<std::string> liste = SplitString ( recv, ";" );

  data->cpu_t = liste.at(1);
  data->cpu_c = liste.at(2);
  /*data->cpu_u = liste.at(3);
  data->liquid_f = liste.at(4);
  data->liquid_p = liste.at(5);
  data->liquid_t = liste.at(6);
  data->gpu_t = liste.at(7);
  data->gpu_u = liste.at(8);
  data->gpu_p = liste.at(9);
  data->fan1_s = liste.at(10);
  data->fan2_s = liste.at(11);
  data->fan3_s = liste.at(12);*/
}



/**
 * Arduino loop ()
 */
void Monitor::mainLoop ( ) {
  while ( 1 ) {

    if ( checkErrors ( ) == true ) {
      display->drawErr ( serial->getErr ( ) );
    }
    else {
      serial->loop ( );
      updatePage->loop ( );

      if ( serial->newData )
        parseSerial ( serial->recv ( ) );

      if ( !display->isWelcomeScreen ( ) ) display->draw ( );

      if ( updatePage->isUpdate ( ) ) {
        if ( display->isWelcomeScreen ( ) ) {
          display->welcomeScreenOff ( );
          updatePage->setDelay ( 2000 );
        }
        display->next ( );
      }
    }
  }
}



bool Monitor::checkErrors ( ) {
  if ( serial->getErr ( ).empty ( ) == true ) return false;
  else return true;
}
