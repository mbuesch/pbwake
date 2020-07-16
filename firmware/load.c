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
#include "watchdog.h"

#if IS_ATMEGAx8
# define LOADCTL_PORT	PORTD
# define LOADCTL_PIN	PIND
# define LOADCTL_BIT	PD5
#else
# define LOADCTL_PORT	PORTB
# define LOADCTL_PIN	PINB
# define LOADCTL_BIT	PB1
#endif

#define OCR_PHASE_HIGH		0x01u
#define OCR_PHASE_LOW		0xFFu
#define TIMER0PS		256
#define IRQPERIODMS(top)	(1000.0f / ((float)F_CPU / TIMER0PS / (top)))
#define MS2COUNT_HIGH(ms)	((uint32_t)((float)(ms) / IRQPERIODMS(OCR_PHASE_HIGH)))
#define MS2COUNT_LOW(ms)	((uint32_t)((float)(ms) / IRQPERIODMS(OCR_PHASE_LOW)))
#define MS2COUNT(ms, phase)	((uint8_t)((phase) ? MS2COUNT_HIGH(ms) : MS2COUNT_LOW(ms)))

#define PHASE_LOW_SHORT_DIV	10

enum load_phase {
	PHASE_LOW,
	PHASE_HIGH,
	PHASE_LOW_SHORT,
};

static struct {
	uint8_t count;
} load;

static void config_phase(enum load_phase phase)
{
	uint8_t ocr, count, min_count, max_count;

	/* Configure the hardware timer period. */
	switch (phase) {
	default:
	case PHASE_LOW:
	case PHASE_LOW_SHORT:
		ocr = OCR_PHASE_LOW;
		break;
	case PHASE_HIGH:
		ocr = OCR_PHASE_HIGH;
		break;
	}
	OCR0A = ocr;
	TCNT0 = ocr ^ 0x80u;
	TIFR0 = 1u << OCF0A;
	TCNT0 = 0u;

	/* Configure the software counter. */
	switch (phase) {
	default:
	case PHASE_LOW:
		max_count = MS2COUNT(CONF_LOW_MAX_MS, false);
		min_count = MS2COUNT(CONF_LOW_MIN_MS, false);
		break;
	case PHASE_LOW_SHORT:
		max_count = MS2COUNT((CONF_LOW_MAX_MS) / PHASE_LOW_SHORT_DIV, false);
		min_count = max_count;
		break;
	case PHASE_HIGH:
		max_count = MS2COUNT(CONF_HIGH_MAX_MS, true);
		min_count = MS2COUNT(CONF_HIGH_MIN_MS, true);
		break;
	}
	count = shr3_get_random_value8(min_count, max_count);
	load.count = max(count, 1u);
}

/* Hardware timer interrupt service routine. */
ISR(TIMER0_COMPA_vect)
{
	bool phase;

	debugpin(true);
	memory_barrier();

	load.count--;
	if (load.count == 0u) {
		LOADCTL_PIN |= 1u << LOADCTL_BIT;
		phase = !!(LOADCTL_PORT & (1u << LOADCTL_BIT));

		config_phase(phase ? PHASE_HIGH : PHASE_LOW);
	}

	wdt_reset();

	memory_barrier();
	debugpin(false);
}

void load_init(void)
{
	/* Configure the timer hardware. */
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

	/* If we had a watchdog-reset or brown-out-reset
	 * but not a power-on-reset and not an external-reset,
	 * then start with a low-phase to avoid continuous high
	 * in case the reset triggers continuously. */
	if (((saved_mcusr & ((1u << WDRF) | (1u << BORF))) != 0u) &&
	    ((saved_mcusr & ((1u << PORF) | (1u << EXTRF))) == 0u))
	{
		LOADCTL_PORT &= (uint8_t)~(1u << LOADCTL_BIT);
		config_phase(PHASE_LOW_SHORT);
	}
	else /* Start with a high phase. */
	{
		LOADCTL_PORT |= 1u << LOADCTL_BIT;
		config_phase(PHASE_HIGH);
	}
}
