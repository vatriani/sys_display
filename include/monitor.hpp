/**
 *  \file      monitor.h
 *  \brief     Defines the Monitor class as main programm
 *  \author    Niels Neumann
 *  \version   0.1
 *  \date      2024
 *  \copyright GNU Public License v3
 */
#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <string>

#include "display.hpp"
#include "serialport.hpp"
#include "displaydata.hpp"



class Monitor {
private:
  Display* display;
  SerialPort* serial;
  displayData* data;
  bool checkErrors ( );

public:
  Monitor ( );
  ~Monitor ( );

  void mainLoop ( );
  void parseSerial ( std::string );
};

#endif
