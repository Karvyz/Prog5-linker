/*
 * elf_read.c : Programme principal
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

void usage(char *name) {
    fprintf(stderr, "Usage:\n"
        "%s [ --help ] [ --debug file ] [ -h | -S | -s ] file\n\n"
        "Prints information about the given ELF file. The --debug flag enables the output produced by "
        "calls to the debug function in the named source file.\n"
        , name);
}