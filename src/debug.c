#include <stdio.h>
#include <stdint.h>

void    print_block(const uint8_t *block, size_t length) {
        printf("Block content (%zu bytes):\n", length);

        for (size_t i = 0; i < length; i++) {
            printf("%02x ", block[i]);
            if ((i + 1) % 16 == 0) {
                printf("\n");
            }
        }

        printf("\n");
}

void    print_decoded_block(const uint32_t *M, size_t length) {
        printf("Decoded block (M[16]):\n");

        for (size_t i = 0; i < length; i++) {
            printf("M[%zu] = %08x\n", i, M[i]);
        }

        printf("\n");
}

void    print_padded_message(const uint8_t *padded, uint64_t padded_length) {
        printf("Message paddé (%llu octets) :\n", padded_length);

        for (uint64_t i = 0; i < padded_length; i++) {
            printf("%02x ", padded[i]);
            if ((i + 1) % 16 == 0) { 
                printf("\n");
            }
        }

        printf("\n");
}

void    print_state(uint32_t A, uint32_t B, uint32_t C, uint32_t D, const char *label) {
        printf("%s:\n", label);
        printf("A = %08x\n", A);
        printf("B = %08x\n", B);
        printf("C = %08x\n", C);
        printf("D = %08x\n", D);

        printf("\n");
}

void    print_md5(const uint32_t state[4]) {
        printf("MD5 hash : ");
        for (int i = 0; i < 4; i++) {
            uint32_t x = state[i];
            uint8_t b0 = x & 0xff;
            uint8_t b1 = (x >> 8) & 0xff;
            uint8_t b2 = (x >> 16) & 0xff;
            uint8_t b3 = (x >> 24) & 0xff;

            printf("%02x%02x%02x%02x", b0, b1, b2, b3);
        }
        printf("\n");
}
