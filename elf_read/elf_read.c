/*
 * elf_read.c : Programme principal
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

void usage(char *name) {
    fprintf(stderr, "Usage:\n"
        "%s [ -H | --help ] [ -h | -S | -s | -x <num|text> | -r ] [ --debug] file\n\n"
        "Prints information about the given ELF file. The --debug flag enables the output produced by "
        "calls to the debug function in the named source file.\n"
        , name);
}

int sectionsAAfficher_s = 0;
char *sectionsAAfficher[100];

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
            {"debug",       no_argument,       NULL, 'd'},
            {NULL,          0,                 NULL, 0}
    };

    file_name = NULL;

    int show_header = 0, show_sections = 0, show_symbols = 0, show_relocations = 0;

    while ( (opt = getopt_long(argc, argv, "hSsrx:Hd", longopts, NULL)) != -1 ) {
        switch (opt) {
            case 'h':
                show_header = 1;
                break;
            case 'S':
                show_sections = 1;
                break;
            case 's':
                show_symbols = 1;
                break;
            case 'r':
                show_relocations = 1;
                break;
            case 'x':
                if (sectionsAAfficher_s < 100) {
                    sectionsAAfficher[sectionsAAfficher_s] = optarg;
                    sectionsAAfficher_s++;
                }
                break;
            case 'H':
                usage(argv[0]);
                exit(0);
            case 'd':
                add_debug_to(optarg);
                break;
            default:
                fprintf(stderr, "Unrecognized option %c", opt);
                usage(argv[0]);
                exit(1);
        }
    }

    return 0;
}