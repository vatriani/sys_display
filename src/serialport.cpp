#include "serialport.hpp"

#include <Arduino.h>

SerialPort::SerialPort ( ) {
  newData = false;

  Serial.begin(9600);

  while (!Serial) {
    break; //
  }
  //err="test fehler";
}

SerialPort::~SerialPort ( ) {

}

void SerialPort::loop ( ) {
  if ( Serial.available() > 0 ) newData = true;
  else newData = false;
}

std::string SerialPort::recv ( ) {
  std::string ret;
  char recvChar[protoMessLength];

  Serial.readBytesUntil ( protoLastByte, recvChar, protoMessLength );

  return std::string ( recvChar );
}


std::string SerialPort::getErr ( ) {
  return err;
}
