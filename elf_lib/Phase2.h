#ifndef PROG5_LINKER_PHASE_2_H
#define PROG5_LINKER_PHASE_2_H

#include <elf.h>
#include <stdio.h>

/* Etape 6 */

typedef struct {
    int old_index; // ancien numero de section 
    int new_index; // nouveau numero de section
    Elf32_Off offset; //offset de concatenation (separation entre les deux sections)
} SectionChanges;

typedef struct {
    Elf32_Shdr *sections; // nouveau header section
    char **sections_names;
    int nb_sections;    // nombre de section
    char **data;        // donné (fichier) des sections
    SectionChanges *changes;
} SectionFusion;

/**
 * @brief   Ecris en big endian
 *          dans le fichier f
 *
 * @param   ptr   pointeur vers les données à écrire
 * @param   size  taille d'un bloc unitaire à écrire
 * @param   nmemb nombre de blocs unitaires à écrire
 * @param   f     flux dans lequel écrire
 * @return
 */
int bwrite(void *ptr, size_t size, size_t nmemb, FILE *f);

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
SectionFusion *fusion_sections(FILE *f1, FILE *f2, Elf32_Ehdr header1, Elf32_Shdr *sections1, Elf32_Ehdr header2, Elf32_Shdr *sections2);

/**
 * @brief   Fusion, renumérotation et corrrection des symboles
 *         du fichier ELF
 *
 *         Cette fonction est appelée après la fusion des sections
 *
 *
 *
 */
void fusion_symbols(FILE *f1, FILE *f2, Elf32_Ehdr header1, Elf32_Sym *symbole1, int nb_symbols, Elf32_Ehdr header2, Elf32_Sym *symbole2, int nb_symbols2, SectionFusion *sectionsFusion);

#endif //PROG5_LINKER_PHASE_2_H