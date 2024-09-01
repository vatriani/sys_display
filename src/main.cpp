/**
 * Elevator for C++ classes
 */
#include <Arduino.h>
#include "monitor.h"



Monitor* mon;



void setup ( ) {
  mon = new Monitor();
}



void loop ( ) {
  mon->mLoop();
}
