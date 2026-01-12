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

#include "../../lib/serial_protocol.h"

class SerialPort {
private:
  serial_protocol::buffer *received;
  std::string err;

protected:
  bool isConnected;

public:
  bool newData;

  SerialPort ( );
  ~SerialPort ( );

  void loop ( ) ;

  std::string recv ( );
  bool send ( std::string str);
  std::string getErr ( );
};

#endif
