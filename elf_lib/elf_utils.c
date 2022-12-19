/**
 * @file     elf_utils.h
 * @author   groupe 14
 * @date     14 Decembre 2022
 */

#include <stdio.h>
#include <elf.h>
#include "elf_utils.h"

size_t bread(void * buffer, size_t s, size_t n, FILE *f){
    char tmp[n][s];
    char c;

    for(int i = 0; i<n; i++){ // pour chaque bloc
        for(int j = 0; j<s; j++){ // pour chaque octet
            c = fgetc(f);
            if(c == EOF)
                return 0;
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
    fprintf(fout, "  Type:\t\t\t\t     ");
    switch(e_type){
        case ET_NONE:
            fprintf(fout, "No file type\n");
            break;
        case ET_REL:
            fprintf(fout, "Relocatable file\n");
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
    fprintf(fout, "  Version:\t\t\t     ");
    switch(e_version){
        case EV_NONE:
            fprintf(fout, "Invalid version\n");
            break;
        case EV_CURRENT:
            fprintf(fout, "Current version\n");
            break;
        default:
            fprintf(fout, "%x\n", e_version);
            break;
    }
}

void print_elf_machine(FILE *fout, Elf32_Half e_machine) {
    fprintf(fout, "  Machine:\t\t\t     ");
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
    fprintf(fout, "  OS/ABI:\t\t\t     ");
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

char * get_section_by_name(Elf32_Word index){
    // TODO
}