#ifndef COMPAT_H_
#define COMPAT_H_

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/cpufunc.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>


#ifndef TIFR0
# define TIFR0	TIFR
#endif
#ifndef TIMSK0
# define TIMSK0	TIMSK
#endif
#ifndef WDIE
# define WDIE	WDTIE
#endif
#ifndef MUX2
# define MUX2	2
#endif
#ifndef MUX3
# define MUX3	3
#endif
#ifndef REFS1
# define REFS1	7
#endif
#ifndef REFS2
# define REFS2	4
#endif
#ifndef WDTCR
# define WDTCR	WDTCSR
#endif
#ifndef TIMER0_OVF_vect
# define TIMER0_OVF_vect TIM0_OVF_vect
#endif
#ifndef TIMER0_COMPA_vect
# define TIMER0_COMPA_vect TIM0_COMPA_vect
#endif


#endif /* COMPAT_H_ */
