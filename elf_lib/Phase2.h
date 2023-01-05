#ifndef PROG5_LINKER_PHASE_2_H
#define PROG5_LINKER_PHASE_2_H

#include <elf.h>
#include <stdio.h>

/* Etape 6 */

typedef struct {
    int old_index;
    int new_index;
    Elf32_Off offset;
} SectionChanges;

// Fusion et numérotation des sections

/**
 * @brief   Fusion et numérotation des sections
 *          du fichier ELF
 *
 * @param   f1 flux 1 (fichier qui sera modifié)
 * @param   f2 flux 2 (fichier qui sera ajouté)
 * @param   header1 en-tête du fichier 1
 * @param   sections1 table des sections du fichier 1
 * @param   header2 en-tête du fichier 2
 * @param   sections2 table des sections du fichier 2
 */
void fusion_sections(FILE *f1, FILE *f2, Elf32_Ehdr header1, Elf32_Shdr *sections1, Elf32_Ehdr header2, Elf32_Shdr *sections2);

#endif //PROG5_LINKER_PHASE_2_H