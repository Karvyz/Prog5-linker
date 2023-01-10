#ifndef __ELF_UTILS_H__
#define __ELF_UTILS_H__

/**
 * @file     elf_utils.h
 * @author   groupe 14
 * @date     14 Decembre 2022
 * @brief    Fonctions auxiliaires.
 */

#include <stdio.h>
#include <stdint.h>

#define MAX_STRTAB_LEN 300
#define MAX 400

/**
 * @brief   Lit un certain nombre d'octets à partir d'un flux.
 *          On lit en "big endian" (inverse les caractères)
 *
 * @param   buffer pointeur pour stocker ce qui est lu
 * @param   s taille (en octets) d'un bloc unitaire à lire
 * @param   n nombre de blocs unitaires à lire
 * @param   f flux à partir duquel les octets doivent être lus
 * @return  size_t lue
 */
size_t bread(void * buffer, size_t s, size_t n, FILE *f);

size_t bread_abs(void * buffer, size_t s, size_t n, FILE *f);

/**
 * @brief   Affiche le type du fichier ELF
 *
 * @param   fout flux de sortie
 * @param   e_type type du fichier ELF
 */
void print_elf_type(FILE *fout, Elf32_Word e_type);

/**
 * @brief   Affiche la version du fichier ELF
 *
 * @param   fout flux de sortie
 * @param   e_version version du fichier ELF
 */
void print_elf_version(FILE *fout, Elf32_Word e_version);

/**
 * @brief   Affiche la machine du fichier ELF
 *
 * @param   fout flux de sortie
 * @param   e_machine machine du fichier ELF
 */
void print_elf_machine(FILE *fout, Elf32_Half e_machine);

/**
 * @brief   Affiche la version de l'OS ABI du fichier ELF
 *
 * @param   fout flux de sortie
 * @param   OSABI version de l'OS ABI du fichier ELF
 */
void print_OS_ABI(FILE *fout, unsigned char OSABI);

/**
 * @brief   Lit tous les noms de sections depuis la section
 *          'shstrtab' et retourne les noms de sections
 *
 * @param   f flux
 * @param   STable section `shstrtab`
 * @return  char* tableau contenant tous les noms de sections
 */
char * read_section_names(FILE *f, Elf32_Shdr STable);

/**
 * @brief   Lit le nom de tous les symboles depuis la section
 *          'strtab' et retourne les noms de symboles
 *
 * @param   f flux
 * @param   STable section strtab
 * @return  char* tableau contenant tous les noms de symboles
 */
char * read_symbol_names(FILE *f, Elf32_Shdr STable);

/**
 * @brief  Retourne le nom de la section, dont l'index de table
 *         de chaîne de caractères est st_name, depuis la table des
 *         chaînes de caractères correspondante (ici, shstrtab)
 *
 * @param  st_name index de la section désirée
 * @param  shstrtab table des chaînes de caractères
 * @return châine de caractères (nom de la section)
 */
char * read_from_shstrtab(uint32_t st_name, const char * shstrtab);

/**
 * @brief Recherche une section par son nom
 *
 * @param  name nom de la section à rechercher
 * @param  shnum nombre de sections
 * @param  sections tableau d'en-tête de sections
 * @param  section  section remplie par effet de bord
 * @param  shstrtab table des chaînes de caractères
 * @return int (1 si trouvée, 0 sinon)
 */
int get_section_by_name(char *name, int shnum, Elf32_Shdr *sections, Elf32_Shdr *section, char *shstrtab);

/* Etape 4 */
/**
 * @brief Affiche le type du symbole
 *
 * @param fout flux de sortie
 * @param st_type type du symbole
 */
void print_st_type(FILE *fout, Elf32_Word st_type);

/**
 * @brief Affiche la liaison du symbole
 *
 * @param fout flux de sortie
 * @param st_bind type du symbole
 */
void print_st_bind(FILE *fout, Elf32_Word st_bind);

/**
 * @brief Affiche la visibilité du symbole
 *
 * @param fout flux de sortie
 * @param st_visibility visibilité du symbole
 */
void print_st_visibility(FILE *fout, Elf32_Word st_visibility);

/**
 * @brief Affiche la section du symbole
 *
 * @param fout flux de sortie
 * @param st_shndx section du symbole
 */
void print_st_shndx(FILE *fout, Elf32_Word st_shndx);

/**
 * @brief Retourne le nom de symbole, dont l'index de table
 *        de chaîne de caractères est st_name
 *
 * @param st_name index du nom de symbole
 * @param symstrtab table des noms des symboles
 * @return châine de caractères
 */
char * read_from_strtab(Elf32_Word st_name, char * symstrtab);

/*Etape 5 */
/**
 * * @brief renvoie le nom du type correspondant au numero dans la table des macros
 * @param val le numero de type present dans les macros
 * @return le nom du type
 */
char * revert_define_type_relocation(int val);

/* Etape 6 */


#endif