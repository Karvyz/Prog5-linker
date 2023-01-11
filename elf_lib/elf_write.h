#ifndef __ELF_WRITE_H__
#define __ELF_WRITE_H__

/**
 * @file    elf_write.h
 * @author  groupe 14
 * @date    14 Decembre 2022
 * @brief   librarie qui contient toutes les fonctions
 *          d'ecriture d'un fichier elf à partir de structure'.
 */

#include <stdio.h>
#include <elf.h>


/**
 * @brief   ecriture du header dans le fichier de sortie
 *
 * @param   f fichier de sortie
 * @param   elf_h header
 */
void write_header(FILE *f, Elf32_Ehdr elf_h);

/**
 * @brief   ecriture de la table des section dans le fichier de sortie
 *
 * @param   f fichier de sortie
 * @param   elf_h header
 * @param   arr_elf_SH table des sections
 */
void write_sections(FILE *f, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH);


/**
 * @brief   gestion de l'ecriture des structures de fusion dans un fichier de sortie
 *          (actuellement prend un fichier .o, le passe en structure puis reécrit un fichier .o)
 *
 * @param   elf_h header
 * @param   arr_elf_SH table des sections
 */
void write_main(Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH);

#endif