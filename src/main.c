#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "md5.h"

int main(void) {
    const uint8_t *input = (const uint8_t *)"BONJOUR";
    uint8_t *padded = NULL;
    uint64_t padded_length = 0;

    uint32_t state[4];

    md5_init(state);
    md5_padding(input, &padded, &padded_length);

    for (uint64_t i = 0; i < padded_length; i++) {
        printf("%02x ", padded[i]);
        if ((i + 1) % 16 == 0 ) printf("\n");
    }
    printf("\n");

    free(padded);

    return 0;
}