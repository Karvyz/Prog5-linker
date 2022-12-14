/**
 * @file     elf_read.c
 * @author   groupe 14
 * @date     14 Decembre 2022
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>
#include "../elf_lib/elf_lib.h"
#include "../elf_lib/elf_utils.h"
#include "../elf_lib/Phase2.h"

void usage(char *name) {
    fprintf(stderr, "Usage: \n"
        "\t%s [ -H | --help ] [ -h | -S | -s | -x <num|text> | -r | -F file2 ] file\n\n"
        "\tDisplay information about the given ELF file\n"
        "\t-H --help\tDisplay this information\n"
        "\t-h\t\tDisplay the ELF header\n"
        "\t-S\t\tDisplay the sections' headers\n"
        "\t-s\t\tDisplay the symbol table\n"
        "\t-x <num|text>\tDisplay the content of the section <num|text>\n"
        "\t-r\t\tDisplay the relocation table\n"
        "\t-F file2\tFusion of file2's sections in the file\n", name);
}

int sectionsAAfficher_nb = 0;
char *sectionsAAfficher[100];

int main(int argc, char *argv[]) {

    int opt;
    char *file_name;
    FILE *file;
    FILE *file2;
    char *file2_name;
    Elf32_Ehdr header;
    Elf32_Shdr *sections;
    Elf32_Sym *symbols;
    int i;
    int nb_symbols;

    struct option longopts[] = {
            {"header",      no_argument,       NULL, 'h'},
            {"sections",    no_argument,       NULL, 'S'},
            {"section",     required_argument, NULL, 'x'},
            {"symbols",     no_argument,       NULL, 's'},
            {"relocations", no_argument,       NULL, 'r'},
            {"help",        no_argument,       NULL, 'H'},
            {"fusion",      required_argument, NULL, 'F'},
            {NULL,          0,                 NULL, 0}
    };

    file_name = NULL;

    int show_header = 0, show_sections = 0, show_symbols = 0, show_relocations = 0, show_section = 0;
    int fusion = 0;

    file2_name = NULL;
    file2 = NULL;

    // gestion des options
    while ( (opt = getopt_long(argc, argv, "hSsrx:F:H", longopts, NULL)) != -1 ) {
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
                show_section = 1;
                if (sectionsAAfficher_nb < 100) {
                    sectionsAAfficher[sectionsAAfficher_nb] = optarg; // stocke le(s) nom(s) de section(s) ?? afficher
                    sectionsAAfficher_nb++;
                }
                break;
            case 'F':
                fusion = 1;
                file2_name = optarg;
                file2 = fopen(file2_name, "r");
                if(file2 == NULL){
                    fprintf(stderr, "Error: cannot open file %s", file2_name);
                    exit(1);
                }
                break;
            case 'H':
                usage(argv[0]);
                exit(0);
            default:
                fprintf(stderr, "Unrecognized option %c", opt);
                usage(argv[0]);
                exit(1);
        }
    }

    // Gestion des cas d'erreurs
    if (optind >= argc) {
        if(show_section){
            fprintf(stderr, "Error: no section number or name given\n");
        } else {
            fprintf(stderr, "No file given !\n");
        }
        usage(argv[0]);
        exit(1);
    }

    file_name = argv[optind];

    // Flux de lecture
    file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file %s\n", file_name);
        exit(1);
    }

    // Stockage des sections lues
    sections = malloc(sizeof(Elf32_Shdr) * MAX);
    if(!sections){
        perror("Erreur d'allocation m??moire (sections, elf_read.c:127)\n");
        exit(EXIT_FAILURE);
    }
    memset(sections, 0, sizeof(Elf32_Shdr) * MAX);

    // Stockage des symboles lus
    symbols = malloc(sizeof(Elf32_Sym) * 400);
    if(!symbols){
        perror("Erreur d'allocation m??moire (symbols, elf_read.c:135\n");
        exit(EXIT_FAILURE);
    }
    memset(symbols, 0, sizeof(Elf32_Sym) * MAX);

    nb_symbols = 0;
        
    // - Lecture de l'en-t??te
    init_header(file, &header);
    // - Lecture des en-t??tes de sections
    read_sections(file, header, sections);
    // - Lecture des noms de sections
    char* shstrtab = read_section_names(file, sections[header.e_shstrndx]);
    // - Lecture des en-t??tes de symboles
    read_symbols(file, header, sections, symbols, &nb_symbols);

    // Affichage du header
    if(show_header) {
        print_elf(stdout, header);
    }
    // Affichage du header des sections
    if(show_sections) {
        print_sections_header(file, stdout, header, sections, shstrtab);
    }
    // Affichage des symboles
    if(show_symbols) {
        Elf32_Shdr * strtab = NULL;
        strtab = malloc(sizeof(Elf32_Shdr));
        if(!strtab){
            perror("Erreur d'allocation m??moire (strtab, elf_read.c:164)\n");
            exit(EXIT_FAILURE);
        }
        if (get_section_by_name(".strtab", header.e_shnum, sections, strtab, shstrtab)){
            // -- lecture des noms de symboles avant affichage
            char* tmp = read_symbol_names(file, *strtab);
            print_symbols(stdout, header, sections, symbols, nb_symbols, shstrtab, tmp);
            free(tmp);
        }
        free(strtab);
    }
    // Affichage des tables de r??implantation
    if(show_relocations) {
        Elf32_Shdr * strtab = NULL;
        strtab = malloc(sizeof(Elf32_Shdr));
        if(!strtab){
            perror("Erreur d'allocation m??moire (strtab, elf_read.c:180)\n");
            exit(EXIT_FAILURE);
        }
        if (get_section_by_name(".strtab", header.e_shnum, sections, strtab, shstrtab)){
            // -- lecture des noms de symboles avant affichage
            char* tmp = read_symbol_names(file, *strtab);
            print_relocation(header, sections, symbols, file, shstrtab, tmp);
            free(tmp);
        }
        free(strtab);
    }
    // Affichage du contenu d'une ou plusieurs section(s)
    if(sectionsAAfficher_nb > 0) {
        for(i = 0; i < sectionsAAfficher_nb; i++) {
            char * name = sectionsAAfficher[i];
            if(name==NULL){
                fprintf(stderr, "Error: no section number or name given\n");
                exit(1);
            }
            int num = 0;
            // Check s'il s'agit d'un num??ro ou d'un nom de section
            int res = sscanf(name, "%d", &num);
            if(res == 1) {
                if (num >= 0 && num < header.e_shnum)
                    print_section_content(file, stdout, &sections[num], shstrtab);
            } else {
                Elf32_Shdr section;
                if(get_section_by_name(name, header.e_shnum, sections, &section, shstrtab))
                    print_section_content(file, stdout, &section, shstrtab);
            }
            fprintf(stdout, "\n");
        }
    }
    // Fusion de deux fichiers
    if(fusion){
        Elf32_Ehdr header2;
        // Lecture de l'en-t??te du 2??? fichier
        init_header(file2, &header2);

        Elf32_Shdr *sections2 = NULL;
        sections2 = malloc(sizeof(Elf32_Shdr) * 400);
        if(!sections2){
            perror("Erreur d'allocation m??moire (sections2, elf_read.c:222)\n");
            exit(EXIT_FAILURE);
        }
        // Lecture des en-t??tes de sections du 2??? fichier
        read_sections(file2, header2, sections2);

        // Stockage des r??sultats de la fusion des 2 fichiers
        SectionFusion *sectionsFusion = NULL;
        sectionsFusion = fusion_sections(file, file2, header, sections, header2, sections2);
        printf("Fusion effectu??e\n");
        for(i = 1; i < header2.e_shnum; i++){
            printf("Section %d du 2??me fichier ?? pour nouveau num??ro %d, offset de concat = %d\n", i, sectionsFusion->changes[i].new_index, sectionsFusion->changes[i].offset);
        }

        // Lib??rer la m??moire
        for(i=0; i<sectionsFusion->nb_sections; i++){
            free(sectionsFusion-> data[i]);
        }
        free(sectionsFusion -> data);
        free(sectionsFusion -> changes);
        free(sectionsFusion);
        free(sections2);
        // Close file
        fclose(file2);
    }

    // Lib??rer la m??moire
    free(shstrtab);
    free(sections);
    free(symbols);
    // Close file
    fclose(file);
    return 0;
}