#ifndef __ELF_LIB_H__
#define __ELF_LIB_H__

/**
 * @file    elf_lib.h
 * @author  groupe 14
 * @date    14 Decembre 2022
 * @brief   librarie qui contient toutes les fonctions
 *          de lecture sur un fichier format ELF.
 */

#include <stdio.h>
#include <elf.h>

/* Etape 1 */

/**
 * @brief   Initialise et modifie l'en-tête du fichier ELF
 *          par effet de bord
 *
 * @param   f flux
 * @param   elf_h pointeur en-tête ELF
 */
void init_header(FILE *f, Elf32_Ehdr *elf_h);

/**
 * @brief   Affiche l'en-tête du fichier ELF
 *
 * @param   fout flux de sortie
 * @param   elf_h en-tête ELF
 */
void print_elf (FILE *fout, Elf32_Ehdr elf_h);

/* Etape 2 */

/**
 * @brief   Lecture de la table des sections
 *          du fichier ELF
 *
 * @param   f flux
 * @param   elf_h en-tête ELF
 * @param   arr_elf_SH table des sections
 */
void read_sections(FILE *f, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH);

/**
 * @brief   Affiche de la table des sections
 *          du fichier ELF
 *
 * @param   f flux
 * @param   fout flux de sortie
 * @param   elf_h en-tête ELF
 * @param   arr_elf_SH table des sections
 */
void print_sections_header(FILE *fout, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH);

/* Etapte 3 */

/**
 * @brief   Affichage d'un contenu de section
 *         du fichier ELF
 * @param   f flux
 * @param   fout flux de sortie
 * @param   elf_h en-tête ELF
 * @param   arr_elf_SH table des sections
 * @param   elf_h en-tête de la section
 */
void print_section_content(FILE *f, FILE *fout, Elf32_Shdr *elf_SH);

/* Etape 4 */

/**
 * @brief   Lecture de la table des symboles
 *          du fichier ELF
 *
 * @param   f flux
 * @param   elf_h en-tête ELF
 * @param   arr_elf_SH table des sections
 * @param   arr_elf_SH table des symboles
 * @param   nb_sym nombre de symboles
 */


#endif