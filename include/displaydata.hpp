/**
 *  \file      serialbuffer.h
 *  \brief     Defines the buffer for data handling in program.
 *  \author    Niels Neumann
 *  \version   0.1
 *  \date      2024
 *  \copyright GNU Public License v3
 */
#ifndef DISPLAY_DATA_HPP
#define DISPLAY_DATA_HPP



#include <string>



/**
 * \brief Data struct used to hold values for write to display.
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
  std::string fan1_s;
  /// Fan 2 fanspeed in RPM
  std::string fan2_s;
  /// Fan 3 fanspeed in RPM
  std::string fan3_s;
  /// Radiator fanspeed in RPM
  std::string liquid_f;
  /// Pump speed in RPM
  std::string liquid_p;
  /// Water temperature in degree celsius
  std::string liquid_t;
};



/**
 * \brief Language definition for localisation
 */
struct displayStrings {
  /// Temperature $(placeholder for value) in celsius
  const std::string t = "temp  $ °C";
  /// Utilisation $(placeholder for value) in percent
  const std::string u = "utili $ %";
  /// powerconsumption $(placeholder for value) in watts
  const std::string p = "power $ watt";
  /// clockspeed $(placeholder for value) in MHz
  const std::string c = "clock $ MHz";
  /// Speed $(placeholder for value) in RPM
  const std::string s = "speed $ RPM";
};

#endif
