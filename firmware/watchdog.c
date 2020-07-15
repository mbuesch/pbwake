/*
 * Watchdog
 *
 * Copyright (c) 2020 Michael Buesch <m@bues.ch>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "compat.h"
#include "watchdog.h"
#include "util.h"

uint8_t saved_mcusr section_noinit;

/* Write to the WDT hardware register. */
static alwaysinline void wdt_setup(uint8_t wdto, bool wde, bool wdie)
{
	const uint8_t first = (uint8_t)((1u << WDCE) | (1u << WDE));
	const uint8_t secnd = (uint8_t)((wde ? (1u << WDE) : 0u) |
					(wdie ? (1u << WDIE) : 0u) |
					(wdto & 0x07u));

	if (_SFR_IO_ADDR(WDTCR) > 0x3F) {
		__asm__ __volatile__(
			"wdr \n"
			"sts %[WDTCR_], %[FIRST_] \n"
			"sts %[WDTCR_], %[SECND_] \n"
			: /* no out */
			: [WDTCR_] "M" (_SFR_MEM_ADDR(WDTCR)),
			  [FIRST_] "r" (first),
			  [SECND_] "r" (secnd)
		);
	} else {
		__asm__ __volatile__(
			"wdr \n"
			"out %[WDTCR_], %[FIRST_] \n"
			"out %[WDTCR_], %[SECND_] \n"
			: /* no out */
			: [WDTCR_] "I" (_SFR_IO_ADDR(WDTCR)),
			  [FIRST_] "r" (first),
			  [SECND_] "r" (secnd)
		);
	}
}

/* Early watchdog timer initialization. */
static void section_init3 wdt_early_init(void)
{
	irq_disable();

	/* Clear WDRF (and all other reset info bits). */
	saved_mcusr = MCUSR;
	MCUSR = 0;

	/* Enable the watchdog. */
	wdt_setup(WDTO_2S, true, false);
}
