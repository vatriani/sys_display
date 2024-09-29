/**
 *  \file       defines.h
 *  \brief      Defines some macros for pio
 *  \author     Niels Neumann
 *  \version    0.1
 *  \date       2024
 *  \copyright  GNU Public License v3
 */
#ifndef DEFINES_H
#define DEFINES_H

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#ifndef HIGH
#define HIGH 1
#define LOW 0
#endif

#define _(char) gettext(char)

#endif
