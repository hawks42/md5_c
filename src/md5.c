#include "md5.h"
#include <stdint.h> 
#include <stdlib.h>
#include <string.h> 
#include <stdio.h>

/*
    RFC1321 : Initialize MD Buffer (32-bit register)
    word A: 01 23 45 67
    word B: 89 ab cd ef
    word C: fe dc ba 98
    word D: 76 54 32 10
*/

void    md5_init(uint32_t state[4]) {
        state[0] = 0x67452301;
        state[1] = 0xEFCDAB89;
        state[2] = 0x98BADCFE;
        state[3] = 0x10325476;
}  

/*
    Padding Bits : bloc 512bits (64octets)
    Add 1 bit
    Add bit 0 => 448 mod 512
*/

void    md5_padding(const uint8_t *input, uint8_t **output, uint64_t *padded_length) {
        size_t input_length = strlen((const char *)input);
        size_t total_length = ((input_length + 8) / 64 + 1) * 64;

        *padded_length = total_length;
        *output = (uint8_t *)calloc(total_length, sizeof(uint8_t));
        memcpy(*output, input, input_length);

        (*output)[input_length] = 0x80;
        uint64_t bit_length = input_length * 8;
        memcpy(*output + total_length - 8, &bit_length, sizeof(uint64_t));
}
