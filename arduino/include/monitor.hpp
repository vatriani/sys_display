/**
 *  \file      monitor.h
 *  \brief     Defines the Monitor class as main programm
 *  \author    Niels Neumann
 *  \version   0.2
 *  \date      2025
 *  \copyright GNU Public License v3
 */
#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <string>
#include <list>
#include <vector>



#include "display.hpp"
#include "serialport.hpp"
#include "displaydata.hpp"



class Monitor {
private:
  Display* display;
  SerialPort* serial;
  displayData* data;

  bool checkErrors ( );
  std::vector<std::string> SplitString ( std::string, std::string );

public:
  Monitor ( );
  ~Monitor ( );

  void mainLoop ( );
  void parseSerial ( std::string );
};

#endif
