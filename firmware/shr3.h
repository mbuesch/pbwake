#ifndef SHR3_H_
#define SHR3_H_

#include "util.h"

uint8_t shr3_get_random_bits(uint8_t nr_bits);
uint8_t shr3_get_random_value8(uint8_t min_value, uint8_t max_value);
void shr3_init(uint32_t seed);

#endif /* SHR3_H_ */
