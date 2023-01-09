#ifndef FUSION_RELOC_H
#define FUSION_RELOC_H

#include <elf.h>
#include <stdio.h>

/**
 * @brief   
 *
 * @param   
 * @param   
 */
void fusion_relocation(FILE *f1, FILE *f2, FILE *result, Elf32_Ehdr elf_h1, Elf32_Shdr* elf_SH1, Elf32_Sym *elf_Sym1, Elf32_Ehdr elf_h2, Elf32_Shdr* elf_SH2, Elf32_Sym *elf_Sym2);

#endif 