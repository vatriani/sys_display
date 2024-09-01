/**
 * s_buffer.h Defines Data Struct for communication
 * Copyright © 2024 - Niels Neumann  <vatriani.nn@googlemail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef S_BUFFER_H
#define S_BUFFER_H



#ifndef __CHAR8__TYPE__
    #define __CHAR8__TYPE__
    #define char8_t unsigned char
#endif



const char8_t protoVersion = 0x01;
const char8_t protoSeperator = ',';
const char8_t protoLastByte = 0x00;
const char8_t protoAcc = 0x01;
const char8_t protoError = 0x02;



const unsigned int strLength = 5;



/**
 * Data struct used for comunication over serial port
 */
struct _buffer {
    /**
     * cpuLines
     * 0 = cpu temp
     * 1 = cpu mhz
     * 2 = cpu utilization
     *
     * liquidLines
     * 0 = fan speed
     * 1 = punp speed
     * 3 = water temp
     *
     * nvidiaLines
     * 0 = gpu temp
     * 1 = gpu utilization
     * 2 = power draw
     *
     * systemLines
     * 0 = fan 1 speed
     * 1 = fan 2 speed
     * 2 = fan 3 speed
     */
    char8_t* cpuLines[3];
    char8_t* liquidLines[3];
    char8_t* nvidiaLines[3];
    char8_t* systemLines[3];
};

typedef struct _buffer buffer;

#endif
