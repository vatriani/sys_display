#include "serialport.hpp"



#include <Arduino.h>



SerialPort::SerialPort ( ) {
  newData = false;

  Serial.begin ( 115200 );

  while (!Serial) {
    break; //
  }
  //err="test fehler";
}



SerialPort::~SerialPort ( ) {
  Serial.end ( );
}



void SerialPort::loop ( ) {
  if ( Serial.available() > 0 ) newData = true;
  else newData = false;
}



std::string SerialPort::recv ( ) {
  std::string ret;
  char recvChar[s_buffer::protoMessLength];

  Serial.readBytesUntil ( s_buffer::protoLastByte, recvChar, s_buffer::protoMessLength );

  return std::string ( recvChar );
}



std::string SerialPort::getErr ( ) {
  return err;
}
