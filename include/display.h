/**
 * display.h Defines the Display class to handle the screen
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

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <U8g2lib.h>

#include "defines.h"
#include "string.h"



/**
 * Defines the Display with all functionalities
 */
class Display {
private:
    U8G2_SH1106_128X64_NONAME_1_SW_I2C* u8g2;
    unsigned char actualPage;

    const char* title[4] = {
        "CPU",
        "LIQUID",
        "GPU",
        "SYS"
    };

    void drawHeader ( );

public:
    Display ( );
    ~Display ( );

    void draw ( );
    void next ( );
};

#endif
