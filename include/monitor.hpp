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
#include <vector>

#define INTERVAL_PAGEFLIP 2000

#include "display.hpp"
#include "serialport.hpp"
#include "timer.hpp"
#include "displaydata.hpp"



class Monitor {
private:
  Display* display;
  SerialPort* serial;
  displayData* data;
  Timer* updatePage;
  
  /// for handling all time relevant things
  long previousMillis;
  bool statusLED;
  bool update;

  bool checkErrors ( );
  void toggleStatus ( );
  std::vector<std::string> SplitString ( std::string, std::string );

public:
  Monitor ( );
  ~Monitor ( );

  void mainLoop ( );
  void parseSerial ( std::string );
};

#endif
