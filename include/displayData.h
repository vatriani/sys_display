/**
 * serialbuffer.h Defines the buffer for data handling in program
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

#ifndef DISPLAY_DATA_H
#define DISPLAY_DATA_H

#include <string>

struct displayData {
    std::string gpu_t;     // GPU Temp
    std::string gpu_u;     // GPU Utilization
    std::string gpu_p;     // GPU Powerdraw

    std::string cpu_t;      // CPU Temp
    std::string cpu_c;      // CPU Clock
    std::string cpu_u;      // CPU Utilization

    std::string fan1;       // Fan1 RPM
    std::string fan2;       // Fan2 RPM
    std::string fan3;       // Fan3 RPM

    std::string liquid_f;  // Fan RPM
    std::string liquid_p;  // Pump RPM
    std::string liquid_t;  // Liquid Temp
};

#endif
