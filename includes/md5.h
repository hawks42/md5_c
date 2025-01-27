#ifndef MD5_H
#define MD5_H

#include <stdint.h>

void    md5_init(uint32_t state[4]);
void    md5_padding(const uint8_t *input, uint8_t **output, uint64_t *padded_length);

#endif