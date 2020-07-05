/*
 * Power bank wake
 * Load control
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

#if IS_ATMEGAx8
# define LOADCTL_PORT	PORTD
# define LOADCTL_PIN	PIND
# define LOADCTL_BIT	PD5
#else
# define LOADCTL_PORT	PORTB
# define LOADCTL_PIN	PINB
# define LOADCTL_BIT	PB1
#endif

#define PHASE_HIGH_MIN_MS	2
#define PHASE_HIGH_MAX_MS	6

#define PHASE_LOW_MIN_MS	40000
#define PHASE_LOW_MAX_MS	55000

#define OCR_PHASE_HIGH		0x01u
#define OCR_PHASE_LOW		0xFFu
#define TIMER0PS		256
#define IRQPERIODMS(top)	(1000.0f / ((float)F_CPU / TIMER0PS / (top)))
#define MS2COUNT_HIGH(ms)	((uint32_t)((float)(ms) / IRQPERIODMS(OCR_PHASE_HIGH)))
#define MS2COUNT_LOW(ms)	((uint32_t)((float)(ms) / IRQPERIODMS(OCR_PHASE_LOW)))
#define MS2COUNT(ms, phase)	((uint8_t)((phase) ? MS2COUNT_HIGH(ms) : MS2COUNT_LOW(ms)))

static struct {
	uint8_t count;
} load;

static void config_phase(bool phase)
{
	uint8_t count, min_count, max_count;

	if (phase) {
		OCR0A = OCR_PHASE_HIGH;
		TCNT0 = OCR_PHASE_HIGH ^ 0x80u;
	} else {
		OCR0A = OCR_PHASE_LOW;
		TCNT0 = OCR_PHASE_LOW ^ 0x80u;
	}
	TIFR0 = 1u << OCF0A;
	TCNT0 = 0u;
	if (phase) {
		min_count = MS2COUNT(PHASE_HIGH_MIN_MS, true);
		max_count = MS2COUNT(PHASE_HIGH_MAX_MS, true);
	} else {
		min_count = MS2COUNT(PHASE_LOW_MIN_MS, false);
		max_count = MS2COUNT(PHASE_LOW_MAX_MS, false);
	}
	count = shr3_get_random_value8(min_count, max_count);
	load.count = max(count, 1u);
}

ISR(TIMER0_COMPA_vect)
{
	bool phase;

	debugpin(true);
	memory_barrier();

	load.count--;
	if (load.count == 0u) {
		LOADCTL_PIN |= 1u << LOADCTL_BIT;
		phase = !!(LOADCTL_PORT & (1u << LOADCTL_BIT));

		config_phase(phase);
	}

	wdt_reset();

	memory_barrier();
	debugpin(false);
}

void load_init(void)
{
	TCCR0B = 0u;
	OCR0A = 0u;
	OCR0B = 0u;
	TIMSK0 = 1u << OCIE0A;
	TCCR0A = (0u << COM0A1) | (0u << COM0A0) |
		 (0u << COM0B1) | (0u << COM0B0) |
		 (1u << WGM01) | (0u << WGM00);
	TCCR0B = (0u << FOC0A) | (0u << FOC0B) |
		 (0u << WGM02) |
		 (1u << CS02) | (0u << CS01) | (0u << CS00);

	LOADCTL_PORT |= 1u << LOADCTL_BIT;
	config_phase(true);
}
