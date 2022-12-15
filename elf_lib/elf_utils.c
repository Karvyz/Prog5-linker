/**
 * @file     elf_utils.h
 * @author   groupe 14
 * @date     14 Decembre 2022
 */

#include <stdio.h>
#include "elf_utils.h"

size_t bread(void * buffer, size_t s, size_t n, FILE *f){
    char* cb = buffer;
    for (int k = 0; k < n ; k++) { // Pour tous les blocs
        for (int j = s - 1; j >= 0 ; j--) { // Pour tous les octets en big Endian
            char tmp = 0;
            for (int i = 0; i < 8; i++) { // On lit un octet
                tmp = tmp << 1;
                tmp += fgetc(f);
            }
            cb[k * s + j] = tmp;
        }
    }
    return s * n;
}