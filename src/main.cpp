/**
 *  \file      main.cpp
 *  \brief     Elevator for C++ class.
 *  \author    Niels Neumann
 *  \version   0.1
 *  \date      2024
 *  \copyright GNU Public License v3
 */


#include <Arduino.h>
#include "monitor.hpp"



Monitor* mon;


/// Arduino setup function
void setup ( ) {
  mon = new Monitor();
}


/// Arduino loop function
void loop ( ) {
  mon->mainLoop();
}
