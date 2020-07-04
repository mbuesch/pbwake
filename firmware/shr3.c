/*
 * SHR3 - Pseudo random number generator
 *
 * http://groups.google.com/group/sci.math/msg/9959175f66dd138f
 * http://groups.google.com/group/sci.math/msg/7e499231fb1e58d3
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

#include "bitops.h"
#include "compat.h"
#include "shr3.h"
#include "util.h"

static uint32_t shr3_state;

uint8_t shr3_get_random_bits(uint8_t nr_of_bits)
{
	uint32_t y = shr3_state;
	uint8_t ret = 0u;

	while (nr_of_bits > 0u) {
		y ^= y << 13;
		y ^= y >> 17;
		y ^= y << 5;

		ret = (uint8_t)(ret << 1u);
		ret = (uint8_t)(ret | (uint8_t)(y & 1u));
		nr_of_bits--;
	}
	shr3_state = y;

	return ret;
}

uint8_t shr3_get_random_value8(uint8_t min_value, uint8_t max_value)
{
	uint8_t range, nr_of_bits, value;

	range = (uint8_t)(max_value - min_value);
	if (range == 0u || min_value > max_value)
		return min_value;

	nr_of_bits = fls8(range);
	do {
		value = shr3_get_random_bits(nr_of_bits);
	} while (value > range);
	value = (uint8_t)(value + min_value);

	return value;
}

void shr3_init(uint32_t seed)
{
	shr3_state = seed ? seed : 0xFFFFFFFFu;
}
