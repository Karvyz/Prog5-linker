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
 * @param   shstrtab table des chaines de caractères
 */
void print_sections_header(FILE *fin, FILE *fout, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH, char *shstrtab);

/* Etapte 3 */

/**
 * @brief   Affichage d'un contenu de section
 *         du fichier ELF
 * @param   f flux
 * @param   fout flux de sortie
 * @param   elf_h en-tête ELF
 * @param   arr_elf_SH table des sections
 * @param   elf_h en-tête de la section
 * @param   shstrtab table des chaines de caractères
 */
void print_section_content(FILE *f, FILE *fout, Elf32_Shdr *elf_SH, char *shstrtab);

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
 * @param   fout flux de sortie
 * @param   arr_elf_Sym table des symboles
 * @param   elf_Sym symbole
 * @param   shstrtab table des chaines de caractères
 */
void print_symbol(FILE *fout, Elf32_Shdr *arr_elf_Sym, Elf32_Sym elf_Sym, const char *shstrtab, const char *symstrtab);

/**
 * @brief   Affichage de la table des symboles
 *          du fichier ELF
 *
 * @param   fout flux de sortie
 * @param   elf_h en-tête ELF
 * @param   arr_elf_SH table des sections
 * @param   arr_elf_SH table des symboles
 * @param   nb_sym nombre de symboles
 * @param   shstrtab table des chaines de caractères
 */
void print_symbols(FILE *fout, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH, Elf32_Sym *arr_elf_ST, int nb_sym, const char *shstrtab, const char *symstrtab);

/* Etape 5 */

/**
 * @brief   Affichage de la table des relocations
 *          du fichier ELF
 *
 * @param   elf_h en-tête ELF
 * @param   elf_SH table des sections
 * @param   elf_Sym table des symboles
 * @param   file flux
 */

void print_relocation(Elf32_Ehdr elf_h, Elf32_Shdr* elf_SH, Elf32_Sym *elf_Sym, FILE *file);

/**
 * @brief Gestion des contraintes lier à la fusion de  
 *        symbole gobaux.
 *        Effet de bord :   
 *        Insertion des symboles lors de l'execution
 *
 * @param fusion_symbole est la table de symbole modifier
 * @param symbole1 est la table de symbole du symbole global
 * @param symbole2 est la table de symbole à fusionner
 * @param nouv_nb_symbols est le nb de symbole de la nouvelle table
 * @param i est l'emplacement du symbole global
 * @param current est l'emplacement dans la nouvelle table 
 * @return 0 pour ok, 1 pour erreur
 */
int gestion_symbol_global(Elf32_Sym *fusion_symbole, Elf32_Sym *symbole1, Elf32_Sym *symbole2, int nouv_nb_symbols, int i, int *current);


#endif