/*
 * Power bank wake
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
#include "load.h"
#include "main.h"
#include "shr3.h"
#include "util.h"

#define DEBUG_PORT	PORTB
#define DEBUG_BIT	PB0

static uint32_t EEMEM ee_seed = 1u;

void debugpin(bool newstate)
{
#if DEBUG
	if (newstate)
		DEBUG_PORT |= (1u << DEBUG_BIT);
	else
		DEBUG_PORT &= (uint8_t)~(1u << DEBUG_BIT);
#endif /* DEBUG */
}

/* Initialize I/O ports. */
static void ports_init(void)
{
	const uint8_t D = ((DEBUG) ? 1 : 0);
	const uint8_t ND = !D;

#if IS_ATMEGAx8
	/* PB0 = output / low (debug pin)
	 * PB1 = input / pullup
	 * PB2 = input / pullup
	 * PB3 = input / pullup
	 * PB4 = input / pullup
	 * PB5 = input / pullup
	 * PB6 = input / pullup
	 * PB7 = input / pullup
	 */
	DDRB =  (0 << DDB7) | (0 << DDB6) | (0 << DDB5) | (0 << DDB4) |
	        (0 << DDB3) | (0 << DDB2) | (0 << DDB1) | (D << DDB0);
	PORTB = (1 << PB7)  | (1 << PB6)  | (1 << PB5)  | (1 << PB4) |
	        (1 << PB3)  | (1 << PB2)  | (1 << PB1)  | (ND << PB0);
	/* PC0 = input / pullup
	 * PC1 = input / pullup
	 * PC2 = input / pullup
	 * PC3 = input / pullup
	 * PC4 = input / pullup
	 * PC5 = input / pullup
	 * PC6 = input / pullup
	 * PC7 = n/c
	 */
	DDRC =  (0        ) | (0 << DDC6) | (0 << DDC5) | (0 << DDC4) |
	        (0 << DDC3) | (0 << DDC2) | (0 << DDC1) | (0 << DDC0);
	PORTC = (0       )  | (1 << PC6)  | (1 << PC5)  | (1 << PC4) |
	        (1 << PC3)  | (1 << PC2)  | (1 << PC1)  | (1 << PC0);
	/* PD0 = input / pullup
	 * PD1 = input / pullup
	 * PD2 = input / pullup
	 * PD3 = input / pullup
	 * PD4 = input / pullup
	 * PD5 = output / low
	 * PD6 = input / pullup
	 * PD7 = input / pullup
	 */
	DDRD =  (0 << DDD7) | (0 << DDD6) | (1 << DDD5) | (0 << DDD4) |
	        (0 << DDD3) | (0 << DDD2) | (0 << DDD1) | (0 << DDD0);
	PORTD = (1 << PD7)  | (1 << PD6)  | (0 << PD5)  | (1 << PD4) |
	        (1 << PD3)  | (1 << PD2)  | (1 << PD1)  | (1 << PD0);
#else
	/* PB0 = output / low (debug pin)
	 * PB1 = output / low
	 * PB2 = input / pullup
	 * PB3 = input / pullup
	 * PB4 = input / pullup
	 * PB5 = input / pullup
	 */
	DDRB =  (0 << DDB5) | (0 << DDB4) | (0 << DDB3) |
	        (0 << DDB2) | (1 << DDB1) | (D << DDB0);
	PORTB = (1 << PB5)  | (1 << PB4)  | (1 << PB3) |
	        (1 << PB2)  | (0 << PB1)  | (ND << PB0);
#endif
}

/* Write the Power Reduction Register */
static void set_PRR(void)
{
#ifdef PRR
	PRR = (uint8_t)(0xFFu
		^ (1u << PRTIM0) /* enable timer 0 */
	);
#endif /* PRR */
}

static void rng_init(void)
{
	uint32_t seed;

	seed = eeprom_read_dword(&ee_seed);
	eeprom_update_dword(&ee_seed, seed + 1u);
	shr3_init(seed);
}

int _mainfunc main(void)
{
	ports_init();
	set_PRR();
	rng_init();
	load_init();

	irq_enable();
	while (1) {
		#pragma GCC diagnostic ignored "-Wconversion"
		set_sleep_mode(SLEEP_MODE_IDLE);
		#pragma GCC diagnostic pop
		sleep_mode();
	}
}
