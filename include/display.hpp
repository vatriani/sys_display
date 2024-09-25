/**
 *  \file       display.hpp
 *  \brief      Defines the Display class to handle the screen connected to an
                NodeMCU.
 *  \author     Niels Neumann
 *  \version    0.1
 *  \date       2024
 *  \copyright  GNU Public License v3
 *  \pre        First initialize the system.
 *  \bug
 *  \warning		Early development
 *  \deprecated
 *  \todo			  CPU value for utilization
 */



#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <Arduino.h>
#include <U8g2lib.h>

#include "defines.h"
#include "string.h"



/**
 * \brief Defines the Display with all functionalities
 */
class Display {
private:
  U8G2_SH1106_128X64_NONAME_1_SW_I2C* u8g2;
  unsigned char actualPage;

  const char* title[4] = {
    "CPU",
    "LIQUID",
    "GPU",
    "SYS"
  };

  void drawHeader ( );
  void drawContend ( );

public:
  Display ( );
  ~Display ( );

  void draw ( );
  void next ( );
};

#endif
