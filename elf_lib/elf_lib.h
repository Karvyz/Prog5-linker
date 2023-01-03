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
void read_symbols(FILE *f, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH, Elf32_Sym *arr_elf_ST, int *nb_sym);

/**
 * @brief   Affichage d'un symbole
 *          du fichier ELF
 *
 * @param   f flux
 * @param   fout flux de sortie
 * @param   elf_h en-tête ELF
 * @param   arr_elf_SH table des sections
 * @param   arr_elf_SH table des symboles
 * @param   nb_sym nombre de symboles
 */
void print_symbol(FILE *fout, Elf32_Shdr *arr_elf_SH, Elf32_Sym elf_Sym);

/**
 * @brief   Affichage de la table des symboles
 *          du fichier ELF
 *
 * @param   fout flux de sortie
 * @param   elf_h en-tête ELF
 * @param   arr_elf_SH table des sections
 * @param   arr_elf_SH table des symboles
 * @param   nb_sym nombre de symboles
 */
void print_symbols(FILE *fout, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH, Elf32_Sym *arr_elf_ST, int nb_sym);

/* Etape 5 */

/**
 * @brief   Affichage de la table des relocations
 *          du fichier ELF
 *
 * @param   fout flux de sortie
 */
void print_relocations(FILE *fout);

/* Etape 6 */

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
void fusion_sections(FILE *f1, FILE *f2, Elf32_Ehdr header1, Elf32_Shdr sections1, Elf32_Ehdr header2, Elf32_Shdr sections2);

#endif