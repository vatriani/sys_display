#include "serialport.hpp"



#include <Arduino.h>



SerialPort::SerialPort ( ) {
  newData = false;
  received = ( s_buffer::buffer* ) malloc ( sizeof ( char8_t ) * s_buffer::protoMessLength );

  Serial.begin ( 115200 );

  while (!Serial) {
    //if ( Serial )
      //err = "Waiting for connection";
    break;
  }
  isConnected = true;
}



SerialPort::~SerialPort ( ) {
  Serial.end ( );
  free ( received ) ;
}



void SerialPort::loop ( ) {
  if ( Serial.available() > 0 ) newData = true;
  else newData = false;
}



std::string SerialPort::recv ( ) {
  char recvChar[s_buffer::protoMessLength];

  int len = Serial.readBytesUntil ( s_buffer::protoLastByte, recvChar, s_buffer::protoMessLength );
  if (len <= 0) return std::string();
  return std::string(recvChar, static_cast<size_t>(len));
}



bool SerialPort::send ( std::string str ) {
  Serial.println( *str.c_str() );
  return true;
}



std::string SerialPort::getErr ( ) {
  return err;
}
