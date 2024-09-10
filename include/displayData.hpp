/**
 *  \file      serialbuffer.h
 *  \brief     Defines the buffer for data handling in program.
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
#ifndef DISPLAY_DATA_HPP
#define DISPLAY_DATA_HPP



#include <string>



/**
 * Data struct used to hold values for write to display.
 */
struct displayData {
  /// GPU temperature in degree celsius
  std::string gpu_t;
  /// GPU utilization in percent
  std::string gpu_u;
  /// GPU powerdraw in watts
  std::string gpu_p;
  /// CPU temperature in degree celsius
  std::string cpu_t;
  /// CPU clockspeed in MHz
  std::string cpu_c;
  /// CPU utilization in percent
  std::string cpu_u;
  /// Fan 1 fanspeed in RPM
  std::string fan1;
  /// Fan 2 fanspeed in RPM
  std::string fan2;
  /// Fan 3 fanspeed in RPM
  std::string fan3;
  /// Radiator fanspeed in RPM
  std::string liquid_f;
  /// Pump speed in RPM
  std::string liquid_p;
  /// Water temperature in degree celsius
  std::string liquid_t;
};

#endif
