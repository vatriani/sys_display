/**
 *  \file      serialport.h
 *  \brief     Defines the SerialPort class as a communication handler
 *  \author    Niels Neumann
 *  \version   0.1
 *  \date      2024
 *  \copyright GNU Public License v3
 */
#ifndef SERIALPORT_HPP
#define SERIALPORT_HPP

#include <string>

#include "s_buffer.h"

class SerialPort {
private:
  buffer received;
  std::string err;

public:
  bool newData;

  SerialPort ( );
  ~SerialPort ( );

  void loop ( ) ;

  std::string recv ( );
  std::string getErr ( );
};

#endif
