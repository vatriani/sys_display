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
  /// only for calculation of the next timerevent.
  long previousMillis;
  /// Timer interval in miliseconds.
  unsigned int delay;
  /// holds timer event.
  bool update;
  /// defines if timer is enabled.
  bool activate;

public:
  /**
   * \brief Default constructor, activates the timer for 1000 milisecounds.
   */
  Timer ( );


  /**
   * \brief Constructor sets the timer to given var.
   *
   * \param var Time in miliseconds
   */
  Timer ( unsigned int var );


  /**
   * \brief Default destructor.
   */
  ~Timer ( );


  /**
   * \brief Simple get update.
   */
  bool isUpdate ( );


  /**
   * \brief Sets timer interval.
   *
   * \param var in miliseconds.
   */
  void setDelay ( unsigned int var);


  /**
   * \brief Loop function must be called in main loop.
   */
  void loop ( );


  /**
   * \brief Sets Timer on.
   */
  void enable ( );


  /**
   * \brief Sets Timer off.
   */
  void disable ( );

  
  /**
   * \brief simple check function if timer is activated.
   *
   * \return Returns if the timer is activated.
   */
  bool isEnabled ( );
};

#endif
