#ifndef FUSION_RELOC_H
#define FUSION_RELOC_H

#include <elf.h>
#include <stdio.h>



Elf32_Addr new_offset_reloc(Elf32_Addr old_offset);


/**
 * @brief   calcul le r_info en fonction de la nouvelle table de symbole
 *
 * @param   r_info contenue de le fichier d'entrer
 * 
 * @return le r_info calculer   
 */
Elf32_Word new_info_reloc(Elf32_Word old_info);


/**
 * @brief   
 *
 * @param   elf_SH sections
 * @param   f    fichier d'entrer (f1 ou f2)
 * @param   result fichier de sortie
 */
void lecture_ecriture_reloc(Elf32_Shdr elf_SH, FILE *f, FILE *result);


/**
 * @brief   
 *
 * @param   f1 fichier d'entre 1
 * @param   f2 fichier d'entre 2
 * @param   result ficheir de sortie
 * @param   elf_h1 header 1
 * @param   elf_SH1 sections 1
 * @param   elf_sym1 symbole 1
 * @param   elf_h2 header 2
 * @param   elf_SH2 sections 2
 * @param   elf_sym2 symbole 2
 */
void fusion_relocation(FILE *f1, FILE *f2, FILE *result, Elf32_Ehdr elf_h1, Elf32_Shdr* elf_SH1, Elf32_Sym *elf_Sym1, Elf32_Ehdr elf_h2, Elf32_Shdr* elf_SH2, Elf32_Sym *elf_Sym2);

#endif 