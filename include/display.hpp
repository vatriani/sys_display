/**
 *  \file       display.hpp
 *  \brief      Defines the Display class to handle the screen connected to an
 *              NodeMCU.
 *  \author     Niels Neumann
 *  \version    0.2
 *  \date       2025
 *  \copyright  GNU Public License v3
 */
#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <Arduino.h>
#include <U8g2lib.h>

#include <string>

#include "defines.h"
#include "timer.hpp"
#include "displaydata.hpp"



enum DisplayState {
  DRAW,
  WELCOME,
  ERROR,
  WAITING
};



enum Page {
  CPU,
  LIQUID,
  GPU,
  SYS
};



/**
 * \brief Defines the Display with all functionalities
 */
class Display {
private:
  U8G2_SH1106_128X64_NONAME_1_SW_I2C* u8g2;
  Timer updatePage;
  Page actualPage;
  displayStrings displayString;
  displayData* data;
  const char* title[4] = {
    "CPU",
    "LIQUID",
    "GPU",
    "SYS"
  };
  const unsigned int line[3] = { 31, 46, 61 };

  void drawHeader ( );
  void drawContend ( );
  void drawWelcome ( );
  void sReplace ( std::string* input, std::string replace );

public:
  Display ( );
  ~Display ( );

  DisplayState state;

  void draw ( );
  void next ( );

  void drawWaiting ( std::string message = "" );
  void drawErr ( std::string err );

  void setDisplayData ( displayData* dataN );
};

#endif
