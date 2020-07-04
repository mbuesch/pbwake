#ifndef BITOPS_H_
#define BITOPS_H_

#include "util.h"

/* Find most significant (last) bit set. */
static inline uint8_t fls16(uint16_t value)
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
static inline uint8_t fls8(uint8_t value)
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

#endif /* BITOPS_H_ */
