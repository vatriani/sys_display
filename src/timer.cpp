#include <Arduino.h>

#include "timer.hpp"



Timer::Timer ( ) {
  activate = true;
}



Timer::Timer ( unsigned int var ) {
  delay = var;
  activate = true;
}



Timer::~Timer ( ) {

}



bool Timer::isUpdate ( ) { return update; }

void Timer::setDelay ( unsigned int val ) { delay = val; }

void Timer::enable ( ) { activate = true; }

void Timer::disable ( ) { activate = false; }

bool Timer::isEnabled ( ) { return activate; }



void Timer::loop ( ) {
  if ( activate ) {
    unsigned long currentMillis = currentMillis = millis ( );

    if ( update ) update = false;

    if ( currentMillis - previousMillis >= delay ) {
      update = true;
      previousMillis = currentMillis;
    }
  }
}
