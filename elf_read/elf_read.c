/*
 * elf_read.c : Programme principal
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "elf_lib/elf_lib.h"

void usage(char *name) {
    fprintf(stderr, "Usage: \n"
        "\t%s [ -H | --help ] [ -h | -S | -s | -x <num|text> | -r ] [ --debug] file\n\n"
        "\tDisplay information about the given ELF file\n"
        "\t-H --help\tDisplay this information\n"
        "\t-h\t\tDisplay the ELF header\n"
        "\t-S\t\tDisplay the sections' headers\n"
        "\t-s\t\tDisplay the symbol table\n"
        "\t-x <num|text>\tDisplay the content of the section <num|text>\n"
        "\t-r\t\tDisplay the relocation table\n"
        "\t--debug\t\tDisplay debug information\n"
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
    Elf32_Sym *symbols;
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
                // TODO
                fprintf(stderr, "Debug mode not implemented yet\n");
                break;
            default:
                fprintf(stderr, "Unrecognized option %c", opt);
                usage(argv[0]);
                exit(1);
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "No file given !\n");
        usage(argv[0]);
        exit(1);
    } else {
        file_name = argv[optind];

        file = fopen(file_name, "r");
        if (file == NULL) {
            fprintf(stderr, "Could not open file %s\n", file_name);
            exit(1);
        }

        // TODO
        // Define a macro SH_TABLE_MAX to 400
        //
        sections = malloc(sizeof(Elf32_Shdr) * 400);
        symbols = malloc(sizeof(Elf32_Sym) * 400);
        size_t nb_symbols = 0;

        // Lecture de l'en-tête

        init_header(file, &header);

    }

    if(show_header) {
        print_elf(stdout, header);
    }


    return 0;
}