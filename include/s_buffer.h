 /**
  *  \file      s_buffer.h
  *  \brief     Defines the data struct for communication.
  *  \author    Niels Neumann
  *  \version   0.1
  *  \date      2024
  *  \copyright GNU Public License v3
  *  \pre
  *  \bug
  *  \warning
  *  \deprecated
  *  \todo
  */
#ifndef S_BUFFER_H
#define S_BUFFER_H



#ifndef __CHAR8__TYPE__
    #define __CHAR8__TYPE__
    #define char8_t unsigned char
#endif


/// Protocol version
const char8_t protoVersion = 0x01;
/// seperator between values
const char8_t protoSeperator = ';';
/// definition of last byte in communication
const char8_t protoLastByte = 0x00;
/// Acknolege sign
const char8_t protoAcc = 0x01;
/// Error sign
const char8_t protoError = 0x02;

/// setting string lengh to 4 + ZERO by string definition
const unsigned int protoStrLength = 5;
/// max message length
const int protoMessLength = 12 * protoStrLength + 2;


/**
 * Data struct used for comunication over serial port
 */
struct _buffer {
    /**
     * \brief cpu vitals
     * - 0 = cpu temp
     * - 1 = cpu mhz
     * - 2 = cpu utilization
     */
    char8_t* cpuLines[3];
    /**
     * liquid vitals
     * - 0 = fan speed
     * - 1 = punp speed
     * - 3 = water temp
     */
    char8_t* liquidLines[3];
    /**
     * nvidia vitals
     * - 0 = gpu temp
     * - 1 = gpu utilization
     * - 2 = power draw
     */
    char8_t* nvidiaLines[3];
    /**
     * system vitals
     * - 0 = fan 1 speed
     * - 1 = fan 2 speed
     * - 2 = fan 3 speed
     */
    char8_t* systemLines[3];
};



/// typedef for better readability
typedef struct _buffer buffer;



#endif
