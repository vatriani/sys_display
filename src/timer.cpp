/**
 *  \file      timer.cpp
 *  \brief     Implementations of Timer class
 *  \author    Niels Neumann
 *  \version   0.1
 *  \date      2024
 *  \copyright GNU Public License v3
 */
#include <Arduino.h>

#include "timer.hpp"



Timer::Timer ( ) {
  delay = 1000;
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
    unsigned long currentMillis = millis ( );

    if ( update ) update = false;

    if ( currentMillis - previousMillis >= delay ) {
      update = true;
      previousMillis = currentMillis;
    }
  }
}
