#ifndef MD5_H
#define MD5_H

#include <stdint.h>

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

void        md5_init(uint32_t state[4]);
void        md5_padding(const uint8_t *input, uint8_t **output, uint64_t *padded_length);
void        md5_transform(uint32_t state[4], const uint8_t block[64]);
void        md5_process(const char *input);


#endif