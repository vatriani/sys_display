/**
 * Elevator for C++ classes
 */
#include <Arduino.h>
#include "monitor.hpp"



Monitor* mon;



void setup ( ) {
  mon = new Monitor();
}



void loop ( ) {
  mon->mLoop();
}
