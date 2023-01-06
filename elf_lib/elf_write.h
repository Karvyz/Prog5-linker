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

void write_header(FILE *f, Elf32_Ehdr elf_h);

void write_sections(FILE *f, Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH);

void write_main(Elf32_Ehdr elf_h, Elf32_Shdr *arr_elf_SH);

#endif