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

#endif