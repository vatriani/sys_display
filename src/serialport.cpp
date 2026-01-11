#include "serialport.hpp"



#include <Arduino.h>



SerialPort::SerialPort ( ) {
  newData = false;
  received = ( s_buffer::buffer* ) malloc ( sizeof ( char8_t ) * s_buffer::protoMessLength );

  Serial.begin ( 115200 );

  while ( !Serial ) {
    if ( Serial )
      err = "Waiting for connection";
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

  size_t len = Serial.readBytesUntil ( s_buffer::protoLastByte, recvChar, s_buffer::protoMessLength );
  if ( len <= 0 ) return std::string ( );
  return std::string ( recvChar, static_cast<size_t> ( len ) );
}



bool SerialPort::send ( std::string str ) {
  if ( !isConnected ) {
    err = "Not connected";
    return false;
  }

  // Send the payload bytes
  if ( !str.empty ( ) ) {
    Serial.write ( ( const uint8_t* ) str.c_str ( ), str.size ( ) );
  }

  // Ensure protocol terminator is sent if the caller didn't include it.
  if ( str.empty ( ) ||
        str.back ( ) != static_cast<char> ( s_buffer::protoLastByte ) ) {
    uint8_t term = static_cast<uint8_t> ( s_buffer::protoLastByte );
    Serial.write ( &term, 1 );
  }

  Serial.flush ( );
  return true;
}



std::string SerialPort::getErr ( ) {
  return err;
}
