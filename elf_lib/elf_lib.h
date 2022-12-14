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
 * @param   f flux
 * @param   elf_h en-tête ELF
 */
void print_elf (FILE *f, Elf32_Ehdr elf_h);

/* Etape 2 */

void read_sections(FILE *f, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH);
void print_sections_header(FILE *f, FILE *fout, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH);

#endif