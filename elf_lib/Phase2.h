#ifndef PROG5_LINKER_PHASE_2_H
#define PROG5_LINKER_PHASE_2_H

/**
 * @file    Phase2.h
 * @author  groupe 14
 * @date    14 Decembre 2022
 * @brief   Fonctions de la phase 2
 *          du projet
 */

#include <elf.h>
#include <stdio.h>

/* Etape 6 */

/**
 * @struct SectionChanges
 * @brief  Changement de numéros de sections
 *
 * SectionChanges sert à stocker l'ancien et
 * le nouveau numéro de section du 2ème fichier
 * ainsi que l'offset de concaténation
 */
typedef struct {
    int old_index;
    int new_index;
    Elf32_Off offset;
} SectionChanges;

/**
 * @struct SectionFusion
 * @brief  Résultat de la fusion
 *
 * SectionFusion sert à stocker les
 * résultats de la fusion des fichiers
 */
typedef struct {
    Elf32_Shdr *sections;
    int nb_sections;
    char **data;
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
void fusion_symbols(FILE *f1, FILE *f2, Elf32_Ehdr header1, Elf32_Shdr *sections1, Elf32_Ehdr header2, Elf32_Shdr *sections2);

#endif //PROG5_LINKER_PHASE_2_H