/**
 * @file     elf_utils.h
 * @author   groupe 14
 * @date     14 Decembre 2022
 */

#include <stdio.h>
#include <elf.h>
#include <string.h>
#include <stdlib.h>
#include "elf_utils.h"

size_t bread(void * buffer, size_t s, size_t n, FILE *f){
    char tmp[n][s];
    char c;

    for(int i = 0; i<n; i++){ // pour chaque bloc
        for(int j = 0; j<s; j++){ // pour chaque octet
            c = fgetc(f);
            if(c == EOF){
                return 0;
            }
            tmp[i][j] = c; // on stocke l'octet dans le tableau
        }
    }
    for(int i = 0; i<n; i++){
        for(int j = 0; j<s; j++){
            ((char*)buffer)[i*s+j] = tmp[i][s-j-1]; // on inverse les octets
        }
    }

    return s * n;
}

void print_elf_type(FILE *fout, Elf32_Word e_type){
    fprintf(fout, "  Type:                              ");
    switch(e_type){
        case ET_NONE:
            fprintf(fout, "No file type\n");
            break;
        case ET_REL:
            fprintf(fout, "REL (Relocatable file)\n");
            break;
        case ET_EXEC:
            fprintf(fout, "Executable file\n");
            break;
        case ET_DYN:
            fprintf(fout, "Shared object file\n");
            break;
        case ET_CORE:
            fprintf(fout, "Core file\n");
            break;
        case ET_LOPROC:
            fprintf(fout, "Processor-specific\n");
            break;
        case ET_HIPROC:
            fprintf(fout, "Processor-specific\n");
            break;
        default:
            fprintf(fout, "%#x\n", e_type);
            break;
    }
}

void print_elf_version(FILE *fout, Elf32_Word e_version){
    fprintf(fout, "  Version:                           ");
    switch(e_version){
        case EV_NONE:
            fprintf(fout, "Invalid version\n");
            break;
        case EV_CURRENT:
            fprintf(fout, "1 (current)\n");
            break;
        default:
            fprintf(fout, "%x\n", e_version);
            break;
    }
}

void print_elf_machine(FILE *fout, Elf32_Half e_machine) {
    fprintf(fout, "  Machine:                           ");
    switch (e_machine) {
        case ET_NONE:
            fprintf(fout, "No machine\n");
            break;
        case EM_M32:
            fprintf(fout, "AT&T WE 32100\n");
            break;
        case EM_SPARC:
            fprintf(fout, "SPARC\n");
            break;
        case EM_386:
            fprintf(fout, "Intel Architecture\n");
            break;
        case EM_68K:
            fprintf(fout, "Motorola 68000\n");
            break;
        case EM_88K:
            fprintf(fout, "Motorola 88000\n");
            break;
        case EM_860:
            fprintf(fout, "Intel 80860\n");
            break;
        case EM_MIPS:
            fprintf(fout, "MIPS R3000 Big-Endian\n");
            break;
        case EM_MIPS_RS3_LE:
            fprintf(fout, "MIPS R3000 Little-Endian\n");
            break;
        case EM_ARM:
            fprintf(fout, "ARM\n");
            break;
        default:
            fprintf(fout, "%x\n", e_machine);
            break;
    }
}

void print_OS_ABI(FILE *fout, unsigned char OSABI){
    fprintf(fout, "  OS/ABI:                            ");
    switch (OSABI) {
        case ELFOSABI_SYSV: fprintf(fout, "UNIX - System V\n"); break;
        case ELFOSABI_HPUX: fprintf(fout, "HP-UX ABI\n"); break;
        case ELFOSABI_NETBSD: fprintf(fout, "NetBSD ABI\n"); break;
        case ELFOSABI_LINUX: fprintf(fout, "Linux ABI\n"); break;
        case ELFOSABI_SOLARIS: fprintf(fout, "Solaris ABI\n"); break;
        case ELFOSABI_IRIX: fprintf(fout, "IRIX ABI\n"); break;
        case ELFOSABI_FREEBSD: fprintf(fout, "FreeBSD ABI\n"); break;
        case ELFOSABI_TRU64: fprintf(fout, "TRU64 UNIX ABI\n"); break;
        case ELFOSABI_ARM: fprintf(fout, "ARM Architecture ABI\n"); break;
        case ELFOSABI_STANDALONE: fprintf(fout, "Stand-alone (embedded) ABI\n"); break;
        default: fprintf(fout, "Unknown OS/ABI\n"); break;
    }
}

/* Etape 3 */

//char shstrtab[MAX_STRTAB_LEN];
//char symstrtab[MAX_STRTAB_LEN];

char * read_section_names(FILE *f, Elf32_Shdr STable) {
    char *shstrtab = NULL;
    shstrtab = malloc(sizeof(char) * MAX_STRTAB_LEN);
    int s = 0;
    fseek(f, STable.sh_offset, SEEK_SET);
    while (s != STable.sh_size) {
        bread(&shstrtab[s], sizeof(char), 1, f);
        s++;
    }
    return shstrtab;
}

char * read_symbol_names(FILE *f, Elf32_Shdr STable) {
    char *symstrtab = NULL;
    symstrtab = malloc(sizeof(char) * MAX_STRTAB_LEN);
    int s = 0;
    fseek(f, STable.sh_offset, SEEK_SET);
    while (s != STable.sh_size) {
        bread(&symstrtab[s], sizeof(char), 1, f);
        s++;
    }
    return symstrtab;
}

char * read_from_shstrtab(uint32_t st_name, const char * shstrtab) {
    int i = (int) st_name;
    char *nSection = malloc(MAX_STRTAB_LEN); // Max 150 char
    int j = 0;

    while (shstrtab[i] != '\0') {
        nSection[i - st_name] = shstrtab[i]; // copy the name of the section in nSection
        i++;
        j++;
    }
    if(!j){
        nSection[0] = '\0';
    }
    return nSection;
}

int get_section_by_name(char *name, int shnum, Elf32_Shdr *sections, Elf32_Shdr *section, char *shstrtab) {
    int i = 0;
    for (i = 0; i < shnum; i++) {
        char *name2 = NULL;
        name2 = malloc(MAX_STRTAB_LEN);
        if(!name2)
            fprintf(stderr, "Error: malloc failed\n");
        strcpy(name2,read_from_shstrtab(sections[i].sh_name, shstrtab));
        // check name2
        if(strcmp(name2, "\0") == 0 || strcmp(name2, "") == 0 || name2==NULL || strcmp(name, "\0") == 0 || strcmp(name, "") == 0 || name==NULL){
            continue;
        } else if (strcmp(name2, name) == 0) { // Si le nom de la section correspond au nom recherché
            *section = sections[i]; // On modifie la section (vide) passée en paramètre par la section trouvée
            return 1;
        }
    }
    return 0;
}

/* Etape 4 */
void print_st_type(FILE *fout, Elf32_Word st_type){
    printf(" ");
    switch(ELF32_ST_TYPE(st_type)){
        case STT_NOTYPE:
            fprintf(fout, "NOTYPE ");
            break;
        case STT_OBJECT:
            fprintf(fout, "OBJECT ");
            break;
        case STT_FUNC:
            fprintf(fout, "FUNC   ");
            break;
        case STT_SECTION:
            fprintf(fout, "SECTION");
            break;
        case STT_FILE:
            fprintf(fout, "FILE   ");
            break;
        case STT_LOPROC:
            fprintf(fout, "LOPROC ");
            break;
        case STT_HIPROC:
            fprintf(fout, "HIPROC ");
            break;
        default:
            fprintf(fout, "UNKNOWN");
            break;
    }
}

void print_st_bind(FILE *fout, Elf32_Word st_bind){
    printf(" ");
    switch(ELF32_ST_BIND(st_bind)){
        case STB_LOCAL:
            fprintf(fout, "LOCAL ");
            break;
        case STB_GLOBAL:
            fprintf(fout, "GLOBAL");
            break;
        case STB_WEAK:
            fprintf(fout, "WEAK  ");
            break;
        case STB_LOPROC:
            fprintf(fout, "LOPROC");
            break;
        case STB_HIPROC:
            fprintf(fout, "HIPROC");
            break;
        default:
            fprintf(fout, "UNKNOWN");
            break;
    }
}

void print_st_visibility(FILE *fout, Elf32_Word st_visibility){
    printf(" ");
    switch(ELF32_ST_VISIBILITY(st_visibility)){
        case STV_DEFAULT:
            fprintf(fout, "DEFAULT ");
            break;
        case STV_INTERNAL:
            fprintf(fout, "INTERNAL");
            break;
        case STV_HIDDEN:
            fprintf(fout, "HIDDEN  ");
            break;
        case STV_PROTECTED:
            fprintf(fout, "PROTECTED");
            break;
        default:
            fprintf(fout, "UNKNOWN ");
            break;
    } 
}

void print_st_shndx(FILE *fout, Elf32_Word st_shndx){
    printf(" ");
    switch(st_shndx){
        case SHN_UNDEF:
            fprintf(fout, "UND");
            break;
        case SHN_ABS:
            fprintf(fout, "ABS");
            break;
        case SHN_COMMON:
            fprintf(fout, "COM");
            break;
        default:
            if (st_shndx < 100) {printf(" ");}
            if (st_shndx < 10) {printf(" ");}
            fprintf(fout, "%d", st_shndx);
            break;
    }
}

char * read_from_strtab(Elf32_Word st_name, const char * symstrtab) {
    int i = (int) st_name;
    char *nSection = malloc(MAX_STRTAB_LEN); // Max 150 char

    while (symstrtab[i] != '\0') {
        nSection[i - st_name] = symstrtab[i]; // copy the name of the section in nSection
        i++;
    }
    return nSection;
}

/* Etape 6 */

// TODO : bwrite
/*
int bwrite(void *ptr, size_t size, size_t nmemb, FILE *f) {
    int i = 0;
    // Ecrire en big endian
    for (i = 0; i < nmemb; i++) {
        fwrite(ptr, size, 1, f);
        ptr += size;
    }
    return 0;
}*/

char * revert_define_type_relocation(int val){
    char * type = malloc(sizeof("R_ARM_JUMP24"));
    sprintf(type, "%d", val);
    switch (val)
    {
    case 2:
        strcpy(type, "R_ARM_ABS32");
        break;
    case 29:
        strcpy(type, "R_ARM_JUMP24");
        break;
    case 28:
        strcpy(type, "R_ARM_CALL");
        break;
    case 1:
        strcpy(type, "R_ARM_PC24");
        break;
    case 40:
        strcpy(type, "R_ARM_V4BX");
        break;
    default:
        break;
    }
    return type;
}