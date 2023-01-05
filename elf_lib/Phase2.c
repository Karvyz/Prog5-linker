#include "Phase2.h"
#include "elf_utils.h"
#include <elf.h>
#include <string.h>
#include <stdlib.h>


/* Etape 6 */

void fusion_sections(FILE *f1, FILE *f2, Elf32_Ehdr header1, Elf32_Shdr *sections1, Elf32_Ehdr header2, Elf32_Shdr *sections2) {
    // TODO : Fusionner les deux fichiers elf en un seul
    int nb_sym1 = header1.e_shnum;
    int nb_sym2 = header2.e_shnum;

    // New header
    Elf32_Ehdr new_header;
    memset(new_header.e_ident, 0, EI_NIDENT);
    new_header.e_ident[EI_MAG0] = ELFMAG0;
    new_header.e_ident[EI_MAG1] = ELFMAG1;
    new_header.e_ident[EI_MAG2] = ELFMAG2;
    new_header.e_ident[EI_MAG3] = ELFMAG3;
    new_header.e_ident[EI_CLASS] = ELFCLASS32;
    new_header.e_ident[EI_DATA] = ELFDATA2MSB;
    new_header.e_ident[EI_VERSION] = EV_CURRENT;
    new_header.e_ident[EI_PAD] = 0;
    new_header.e_type = header1.e_type;
    new_header.e_machine = header1.e_machine;
    new_header.e_version = header1.e_version;
    new_header.e_entry = header1.e_entry;
    new_header.e_phoff = header1.e_phoff;
    new_header.e_shoff = sizeof(Elf32_Ehdr);
    new_header.e_flags = header1.e_flags;
    new_header.e_ehsize = header1.e_ehsize;
    new_header.e_phentsize = header1.e_phentsize;
    new_header.e_phnum = header1.e_phnum;
    new_header.e_shentsize = header1.e_shentsize;
    new_header.e_shnum = 0;
    new_header.e_shstrndx = SHN_UNDEF;

    // Création d'un fichier vide pour y stocker le résultat de la concaténation
    FILE *result = NULL;
    result = fopen("file_result.o", "w+b");
    if (!result) {
        perror("Erreur lors de la création du fichier résultat");
        fclose(result);
        exit(EXIT_FAILURE);
    }

    // Tableau pour mémoriser les changements de numéro de section et l'offset de la concaténation
    SectionChanges changes[header2.e_shnum];
    memset(changes, 0, sizeof(SectionChanges) * header2.e_shnum);

    //Elf32_Shdr *sections = malloc(sizeof(Elf32_Shdr) * ((header1.e_shentsize * header1.e_shnum) + (header2.e_shentsize * header2.e_shnum)));

    for (int i = 0; i < nb_sym1; i++) {
        char *data = NULL;
        if (sections1[i].sh_type == SHT_STRTAB && sections1[i].sh_name == header1.e_shstrndx) {
            new_header.e_shstrndx = sections1[i].sh_offset;
        }
        if (sections1[i].sh_type == SHT_PROGBITS) {
            for (int j = 0; j < nb_sym2; j++) {
                char *name1 = read_from_shstrtab(sections1[i].sh_name);
                char *name2 = read_from_shstrtab(sections2[j].sh_name);
                if (sections2[j].sh_type == SHT_PROGBITS && (strcmp(name1, name2) == 0)) {
                    data = malloc(sections1[i].sh_size + sections2[j].sh_size);
                    if (!data) {
                        perror("Erreur lors de l'allocation de la mémoire");
                        exit(EXIT_FAILURE);
                    }
                    fseek(f1, sections1[i].sh_offset, SEEK_SET);
                    memcpy(data, f1, sections1[i].sh_size);
                    fseek(f2, sections2[j].sh_offset, SEEK_SET);
                    memcpy(data + sections1[i].sh_size, f2, sections2[j].sh_size);
                    // On écrit les données dans le fichier résultat
                    fwrite(data, sections1[i].sh_size + sections2[j].sh_size, 1, result);
                    changes[j].old_index = j;
                    changes[j].new_index = i;
                    changes[j].offset = new_header.e_shoff;
                    new_header.e_shoff += sections1[i].sh_size + sections2[j].sh_size;
                } else {
                    data = malloc(sections1[i].sh_size);
                    if (!data) {
                        perror("Erreur lors de l'allocation de la mémoire");
                        exit(EXIT_FAILURE);
                    }
                    fseek(f1, sections1[i].sh_offset, SEEK_SET);
                    memcpy(data, f1, sections1[i].sh_size);
                    // On écrit les données dans le fichier résultat
                    fwrite(data, sizeof(Elf32_Shdr), 1, result);
                    new_header.e_shoff += sections1[i].sh_size;
                }
            }
        } else {
            data = malloc(sections1[i].sh_size);
            if (!data) {
                perror("Erreur lors de l'allocation de la mémoire");
                exit(EXIT_FAILURE);
            }
            fseek(f1, sections1[i].sh_offset, SEEK_SET);
            memcpy(data, f1, sections1[i].sh_size);
            // On écrit les données dans le fichier résultat
            fwrite(data, sizeof(Elf32_Shdr), 1, result);
        }
        new_header.e_shnum++;
        // Libérer la mémoire
        free(data);
    }
    // Chaque section non-concaténée du fichier 2
    for (int j = 0; j < nb_sym2; j++) {
        char *data = NULL;
        if (changes[j].old_index == 0) {
            data = malloc(sections2[j].sh_size);
            if (!data) {
                perror("Erreur lors de l'allocation de la mémoire");
                exit(EXIT_FAILURE);
            }
            fseek(result, new_header.e_shoff, SEEK_SET);
            // On écrit les données dans le fichier résultat
            fwrite(data, sections2[j].sh_size, 1, result);
            changes[j].old_index = j;
            changes[j].new_index = nb_sym2 + j;
            changes[j].offset = new_header.e_shoff;
            new_header.e_shoff += sections2[j].sh_size;
        }
        new_header.e_shnum++;
        // Libérer la mémoire
        free(data);
    }

    // Ajout du header dans le fichier resultat
    // TODO : écrire la section de chaîne de noms

}