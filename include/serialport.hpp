/**
 * serialport.h Defines the SerialPort class as a communication handler
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
