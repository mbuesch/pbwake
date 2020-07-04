#ifndef MAIN_H_
#define MAIN_H_

#include "util.h"

#if (defined(__AVR_ATmega88__) ||\
     defined(__AVR_ATmega88P__) ||\
     defined(__AVR_ATmega328P__))
# define IS_ATMEGAx8		1
#elif (defined(__AVR_ATtiny13__) ||\
       defined(__AVR_ATtiny25__) ||\
       defined(__AVR_ATtiny45__) ||\
       defined(__AVR_ATtiny85__))
# define IS_ATMEGAx8		0
#else
# error "Unknown microcontroller."
#endif

void debugpin(bool newstate);

#endif /* MAIN_H_ */
