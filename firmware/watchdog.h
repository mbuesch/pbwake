#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include "util.h"

extern uint8_t saved_mcusr section_noinit;

uint16_t watchdog_interval_ms(void);

#endif /* WATCHDOG_H_ */
