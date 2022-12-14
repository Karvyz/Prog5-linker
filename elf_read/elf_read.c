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

int main(int argc, char *argv[]) {
    int opt;
    char *file_name;
    FILE *file;
    Elf32_Ehdr header;
    Elf32_Shdr *sections;
    Elf32_Shdr *section;
    Elf32_Shdr *section_names;
    char *section_names_data;
    int i;

    struct option longopts[] = {
            {"header",      no_argument,       NULL, 'h'},
            {"sections",    no_argument,       NULL, 'S'},
            {"section",     required_argument, NULL, 'x'},
            {"symbols",     no_argument,       NULL, 's'},
            {"relocations", no_argument,       NULL, 'r'},
            {"help",        no_argument,       NULL, 'H'},
            {NULL,          0,                 NULL, 0}
    };

    file_name = NULL;

    return 0;
}