/**
 * monitor.h Defines the Monitor class as main programm
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

#ifndef MONITOR_H
#define MONITOR_H

#include "display.h"
#include "serialport.h"
#include "displayData.h"



class Monitor {
private:
    Display* display;
    SerialPort* serial;
    displayData* data;


public:
    Monitor ( );
    ~Monitor ( );

    void mLoop ();
    void parseSerial ( char* buff  );
};

#endif
