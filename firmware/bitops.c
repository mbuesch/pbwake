/*
 * Bit operations
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

/* Find most significant (last) bit set. */
uint8_t fls16(uint16_t value)
{
	uint8_t ret = 16u;

	if (value == 0u)
		return 0u;

	if ((value & 0xFF00u) == 0u) {
		value = (uint16_t)(value << 8);
		ret = (uint8_t)(ret - 8u);
	}
	if ((value & 0xF000u) == 0u) {
		value = (uint16_t)(value << 4);
		ret = (uint8_t)(ret - 4u);
	}
	if ((value & 0xC000u) == 0u) {
		value = (uint16_t)(value << 2);
		ret = (uint8_t)(ret - 2u);
	}
	if ((value & 0x8000u) == 0u) {
		value = (uint16_t)(value << 1);
		ret = (uint8_t)(ret - 1u);
	}

	return ret;
}

/* Find most significant (last) bit set. */
uint8_t fls8(uint8_t value)
{
	uint8_t ret = 8u;

	if (value == 0u)
		return 0u;

	if ((value & 0xF0u) == 0u) {
		value = (uint8_t)(value << 4);
		ret = (uint8_t)(ret - 4u);
	}
	if ((value & 0xC0u) == 0u) {
		value = (uint8_t)(value << 2);
		ret = (uint8_t)(ret - 2u);
	}
	if ((value & 0x80u) == 0u) {
		value = (uint8_t)(value << 1);
		ret = (uint8_t)(ret - 1u);
	}

	return ret;
}
