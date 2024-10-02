/**
 *  \file      timer.h
 *  \brief     Defines the Timer class for handling time specific events
 *  \author    Niels Neumann
 *  \version   0.1
 *  \date      2024
 *  \copyright GNU Public License v3
 */
#ifndef TIMER_HPP
#define TIMER_HPP

class Timer {
private:
  long previousMillis;
  unsigned int delay;
  bool update;
  bool activate;

public:
  Timer ( );
  Timer ( unsigned int );
  ~Timer ( );

  bool isUpdate ( );
  void setDelay ( unsigned int );
  void loop ( );
  void enable ( );
  void disable ( );
  bool isEnabled ( );
};

#endif
