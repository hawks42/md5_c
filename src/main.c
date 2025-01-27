#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "md5.h"
#include "debug.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage : %s <input>\n", argv[0]);
        return 1;
    }

    md5_process(argv[1]);
    return 0;
}