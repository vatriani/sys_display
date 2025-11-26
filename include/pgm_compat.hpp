/*
 * pgm_compat.h
 *
 * Provide small compatibility fallbacks for PROGMEM/FPSTR so code that uses
 * Arduino/AVR flash-string helpers can also build on host/CI (non-AVR) targets.
 *
 * Usage:
 *   #include "pgm_compat.h"
 *   u8g2->drawStr(0, 15, FPSTR("Waiting:"));
 *
 * On AVR/Arduino platforms this preserves the intended behavior; on other
 * platforms FPSTR() becomes a no-op and PROGMEM is defined away.
 */

#ifndef PGM_COMPAT_H
#define PGM_COMPAT_H

/* Ensure PROGMEM exists as a no-op on non-AVR platforms */
#ifndef PROGMEM
#define PROGMEM
#endif

/* Provide FPSTR fallback:
 * - On AVR/Arduino try to use the AVR/Arduino definitions.
 * - Otherwise define FPSTR(x) to just return the literal pointer.
 */
#ifndef FPSTR
  /* Prefer the Arduino-provided macros if building for AVR/Arduino cores. */
  #if defined(__AVR__) || defined(ARDUINO_ARCH_AVR) || defined(ARDUINO)
    /* include avr/pgmspace if available; Arduino core may already define FPSTR */
    #include <avr/pgmspace.h>
    #ifndef FPSTR
      /* If the Arduino core didn't define FPSTR, provide a reasonable definition.
         This casts a PSTR() literal to the FlashStringHelper type expected by
         Arduino string helpers. */
      #ifdef PSTR
        #define FPSTR(s) (reinterpret_cast<const __FlashStringHelper *>(PSTR(s)))
      #else
        /* If PSTR is not available for some AVR toolchains, fall back to raw literal */
        #define FPSTR(s) (s)
      #endif
    #endif
  #else
    /* Non-AVR / host: FPSTR is a no-op */
    #define FPSTR(s) (s)
  #endif
#endif /* FPSTR */

#endif /* PGM_COMPAT_H */
