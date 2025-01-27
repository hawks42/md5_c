#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>

void    print_block(const uint8_t *block, size_t length);
void    print_decoded_block(const uint32_t *M, size_t length);
void    print_padded_message(const uint8_t *padded, uint64_t padded_length);
void    print_state(uint32_t A, uint32_t B, uint32_t C, uint32_t D, const char *label);
void    print_md5(const uint32_t state[4]);


#endif