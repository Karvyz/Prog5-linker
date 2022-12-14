#ifndef __ELF_LIB_H__
#define __ELF_LIB_H__

/* Etape 1 */

void init_header(FILE *f, Elf32 *elf_h);
void print_elf (FILE *f, Elf32 elf_h);

/* Etape 2 */

void read_sections(FILE *f, Elf32 elf_h, Elf32_SH *arr_elf_SH);
void print_sections_header(FILE *f, FILE *fout, Elf32 elf_h, Elf32_SH *arr_elf_SH);

/* Etape 3 */

void read_data_section(FILE *f, FILE *fout, Elf32 elf_h, Elf32_SH *arr_elf_SH, Elf32_SH *elf_SH);
void print_data_section(FILE *f, FILE *fout, Elf32 elf_h, Elf32_SH *arr_elf_SH, Elf32_SH *elf_SH);

/* Etape 4 */

void read_symbol_section(FILE *f, Elf32 elf_h, Elf32_SH *arr_elf_SH, Elf32_Sym *arr_elf_SYM, size_t *nbSymboles);
void print_symbol(FILE *fout, Elf32_SH *arr_elf_sh, Elf32_Sym elf_SYM);
void print_symbols(FILE *fout, Elf32 elf_h, Elf32_SH *arr_elf_SH, size_t nbSymboles, Elf32_Sym * arr_elf_SYM);

/* Etape 5 */

void read_reloc(FILE *f, Elf32_Rel *elf_REL);
void read_reloca(FILE *f, Elf32_Rela *elf_RELA);
void read_relocsa(FILE *f, Elf32 elf_h, Elf32_SH *arr_elf_SH, Elf32_RelArray *arr_elf_REL, Elf32_RelaArray *arr_elf_RELA, size_t *nbRel, size_t *nbRela);
void print_relocs(FILE *fout, Elf32 elf_h, Elf32_SH *arr_elf_SH, Elf32_Sym *arr_elf_SYM, Elf32_RelArray *arr_elf_REL, Elf32_RelaArray *arr_elf_RELA, size_t nbRel, size_t nbRela);
#endif