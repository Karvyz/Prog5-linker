#ifndef __ELF_UTILS_H__
#define __ELF_UTILS_H__

/**
 * @file     elf_utils.h
 * @author   groupe 14
 * @date     14 Decembre 2022
 * @brief    toutes les fonctions auxiliaires.
 */

#include <stdio.h>
#include <stdint.h>

#define MAX_STRTAB_LEN 300

/**
 * @brief   Lit un certain nombre d'octets à partir d'un flux.
 *          On lit en "big endian" (inverse les caractères)
 *
 * @param   buffer pointeur
 * @param   s taille (en octets) d'un bloc unitaire à lire.
 * @param   n nombre de blocs unitaires à lire
 * @param   f flux à partir duquel les octets doivent être lus
 * @return  size_t
 */
size_t bread(void * buffer, size_t s, size_t n, FILE *f);

void print_elf_type(FILE *fout, Elf32_Word e_type);

void print_elf_version(FILE *fout, Elf32_Word e_version);

void print_elf_machine(FILE *fout, Elf32_Half e_machine);

void print_OS_ABI(FILE *fout, unsigned char OSABI);

/**
 * @brief Lit tt les noms de sections depuis la section
 *        `shstrtab` et les renvoi dans le tableau "shrstrtab"
 *
 * @param f flux
 * @param STable section `shstrtab`
 * @return
 */
void read_section_names(FILE *f, Elf32_Shdr STable);

/**
 * @brief Retourne le nom de la section, dont l'index de table
 *        de chaîne de caractères est st_name, depuis la table des
 *        chaînes de caractères correspondante (ici, shstrtab)
 *
 * @param st_name index de la section désirée
 * @return châine de caractères (nom de la section)
 */
char * read_from_shstrtab(uint32_t st_name);

/**
 * @brief Recherche une section par son nom
 *
 * @param name nom de la section à rechercher
 * @param shnum nombre de sections
 * @param sections tableau d'en-têtes section
 * @param section  section retournée
 * @return int (1 si trouvée, 0 sinon)
 */
int get_section_by_name(char *name, int shnum, Elf32_Shdr *sections, Elf32_Shdr *section);

#endif