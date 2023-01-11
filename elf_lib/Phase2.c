/**
 * @file     Phase2.c
 * @author   groupe 14
 * @date     14 Decembre 2022
 */

#include "Phase2.h"
#include "elf_utils.h"
#include <elf.h>
#include <string.h>
#include <stdlib.h>

/* Etape 6 */

int bwrite(void *ptr, size_t size, size_t nmemb, FILE *f) {
    int i = 0;
    char tmp[nmemb][size];
    int total = 0;

    for(i = 0; i<nmemb; i++){
        for(int j = 0; j<size; j++){
            // on inverse les octets
            tmp[i][j] = ((char *)ptr)[i*size+j];
        }
    }

    for(i = 0; i<nmemb; i++){ // pour chaque bloc
        for(int j = 0; j<size; j++){ // pour chaque octet
            if(fputc(tmp[nmemb-i-1][size-j-1], f) == EOF)
                if(total == 0)
                    return 0;
                else
                    return total;
            else
                total++;
        }
    }
    return total;
}

SectionFusion *fusion_sections(FILE *f1, FILE *f2, Elf32_Ehdr header1, Elf32_Shdr *sections1, Elf32_Ehdr header2, Elf32_Shdr *sections2){
    // TODO : Fusionner les deux fichiers elf en un seul

    // Nombre de symboles dans le fichier 1, 2
    // Nombre de symboles final après fusion
    int nb_sym1 = header1.e_shnum;
    int nb_sym2 = header2.e_shnum;
    int nb_sym = 0;

    // Noms des sections pour comparaison
    char **names1 = NULL;
    names1 = calloc(nb_sym1, sizeof(char *));
    for(int i=0; i<nb_sym1; i++){
        names1[i] = malloc(sizeof(char) * 100);
        if(!names1[i]){
            perror("Erreur d'allocation mémoire (names1[i], Phase2.c:54");
            exit(EXIT_FAILURE);
        }
        memset(names1[i], '\0', 100);
    }
    char *name1 = NULL;

    char **names2 = NULL;
    names2 = calloc(nb_sym2, sizeof(char *));
    for(int i=0; i<nb_sym2; i++){
        names2[i] = malloc(sizeof(char) * 100);
        if(!names2[i]){
            perror("Erreur d'allocation mémoire (names2[i], Phase2.c:66");
            exit(EXIT_FAILURE);
        }
        memset(names2[i], '\0', 100);
    }
    char *name2 = NULL;

    // data contiendra les données de la section ou des sections concaténées
    char *data = NULL;

    // Offset de concaténation
    Elf32_Off offset = 0;

    // Tableau pour mémoriser les changements de numéro de section et l'offset de la concaténation
    SectionChanges changes[header2.e_shnum];
    memset(changes, -1, sizeof(SectionChanges) * header2.e_shnum);

    // Section qui contiendra tous les éléments des sections fusionnées
    Elf32_Shdr *sections = NULL;
    sections = calloc(((header1.e_shentsize * header1.e_shnum) + (header2.e_shentsize * header2.e_shnum)), sizeof(Elf32_Shdr));
    if (!sections) {
        perror("Erreur lors de l'allocation de la table des sections");
        exit(EXIT_FAILURE);
    }
    //memset(sections, 0, sizeof(Elf32_Shdr) * ((header1.e_shentsize * header1.e_shnum) + (header2.e_shentsize * header2.e_shnum)));

    // Structure fusion qui contiendra tous les éléments utiles une fois la fusion terminée
    SectionFusion *fusion = NULL;
    fusion = calloc(1, sizeof(SectionFusion));
    if (!fusion) {
        perror("Erreur lors de l'allocation de la structure SectionFusion");
        exit(EXIT_FAILURE);
    }
    // data contient les données des sections
    fusion->data=calloc(MAX * nb_sym1+nb_sym2, sizeof(char *));
    if (!fusion->data) {
        perror("Erreur lors de l'allocation de la structure SectionFusion->data");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<nb_sym1+nb_sym2; i++){
        fusion->data[i] = calloc(MAX, sizeof(char));
        if(!fusion->data[i]){
            perror("Erreur d'allocation mémoire\n");
            exit(EXIT_FAILURE);
        }
        memset(fusion->data[i], '\0', MAX);
    }
    fusion->nb_sections = 0;
    fusion->sections = NULL;
    fusion->changes=malloc(sizeof(SectionChanges) * nb_sym2);
    if(!fusion->changes){
        perror("Erreur lors de l'allocation de la structure SectionFusion->changes");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<nb_sym2; i++){
        fusion->changes[i].old_index = -1;
        fusion->changes[i].new_index = -1;
        fusion->changes[i].offset = -1;
    }

    for (int i = 0; i < nb_sym1; i++){
        char * tmp2 = NULL;
        tmp2 = read_section_names(f1, sections1[header1.e_shstrndx]);
        name1 = read_from_shstrtab(sections1[i].sh_name, tmp2);
        unsigned long taille1 = 0;
        taille1 = strlen(name1);
        //fprintf(stderr, "taille = %lu, taille = %lu\n", strlen(names1[i]), taille1);
        if( (strcmp(name1, "") != 0 || strcmp(name1, "\0") != 0) && strlen(names1[i]) >= taille1){
            strcpy(names1[i], name1);
        }
        else if( (strcmp(name1, "") != 0 || strcmp(name1, "\0") != 0) && strlen(names1[i]) < taille1){
            char * tmp = NULL;
            tmp = realloc(names1[i], sizeof(char) * taille1);
            if(tmp){
                names1[i] = tmp;
            }
            strcpy(names1[i], name1);
            //free(tmp);
        }
        free(name1);

        if(sections1[i].sh_type == SHT_PROGBITS) {
            // test si chaque section du 2ème fichier est de type PROGBIT
            // Booleen pour vérifier si la section 1 peut être ajoutée si pas de fusion possible
            int bool = 0;
            for(int j = 1; j < nb_sym2; j++) {
                //fprintf(stderr, "j=%d\n", j);
                // Nom de la section courant du fichier 2
                char * tmp = NULL;
                tmp = read_section_names(f2, sections2[header2.e_shstrndx]);
                name2 = read_from_shstrtab(sections2[j].sh_name, tmp);
                if( (strcmp(name2, "") != 0 || strcmp(name2, "\0") != 0) && strlen(names2[j]) >= strlen(name2)){
                    strcpy(names2[j], name2);
                }
                else if( (strcmp(name2, "") != 0 || strcmp(name2, "\0") != 0) && strlen(names2[j]) < strlen(name2)){
                    char * tmp3 = NULL;
                    tmp3 = realloc(names2[j], sizeof(char) * strlen(name2));
                    if(tmp3){
                        names2[j] = tmp3;
                    } else{
                        free(tmp3);
                    }
                    strcpy(names2[j], name2);
                }
                free(tmp);
                free(name2);
                // Debug
                if(strcmp(names1[i], "\0") == 0 || strcmp(names2[j], "\0") == 0 || names1[i] == NULL || names2[j] == NULL || strcmp(names1[i], "") == 0 || strcmp(names2[j], "") == 0){
                    continue;
                }
                // test si type = PROGBIT et même nom que la section du 1
                if(sections2[j].sh_type == SHT_PROGBITS && (strcmp(names1[i], names2[j]) == 0)) {
                    // Mis à jour du booleen
                    bool = 1;
                    // allocation mémoire à data
                    data = malloc(sections1[i].sh_size + sections2[j].sh_size);
                    if (!data) {
                        perror("Erreur lors de l'allocation de la mémoire de data");
                        exit(EXIT_FAILURE);
                    }
                    // Initialisation de data
                    memset(data, 0, sections1[i].sh_size + sections2[j].sh_size);
                    // On se déplace à l'offset de la section du 1
                    fseek(f1, sections1[i].sh_offset, SEEK_SET);
                    // On copie dans data
                    memcpy(data, f1, sections1[i].sh_size);
                    // On se déplace à l'offset de la section du 2
                    fseek(f2, sections2[j].sh_offset, SEEK_SET);
                    // On copie dans data àl a suite de la section du 1
                    memcpy(data + sections1[i].sh_size, f2, sections2[j].sh_size);

                    // On met à jour la section finale
                    sections[i].sh_offset = offset;
                    sections[i].sh_size = sections1[i].sh_size + sections2[j].sh_size;
                    sections[i].sh_type = SHT_PROGBITS;
                    sections[i].sh_flags = sections1[i].sh_flags;
                    sections[i].sh_addr = sections1[i].sh_addr;
                    sections[i].sh_name = sections1[i].sh_name;
                    // On met à jour le tableau changes
                    changes[j].old_index = j;
                    changes[j].new_index = i;
                    changes[j].offset = sections1[i].sh_size;
                    // On met à jour l'offset courant
                    offset += sections1[i].sh_size + sections2[j].sh_size;
                    // On met le data contenant la fusion des 2 sections dans la structure de fusion
                    if(strcmp(data, "") != 0){
                        strcpy(fusion->data[i], data);
                    }
                    free(data);
                    break;
                }
                //free(name2);
            }
            if(!bool) {
                // Allocation mémoire à data
                data = malloc(sections1[i].sh_size);
                if (!data) {
                    perror("Erreur lors de l'allocation de la mémoire");
                    exit(EXIT_FAILURE);
                }
                //data = "";
                // On se déplace à l'offset de la section du 1
                fseek(f1, sections1[i].sh_offset, SEEK_SET);
                // On copie dans data la section du 1
                memcpy(data, f1, sections1[i].sh_size);
                // On met à jour la section finale
                sections[i].sh_offset = offset;
                sections[i].sh_size = sections1[i].sh_size;
                sections[i].sh_type = SHT_PROGBITS;
                sections[i].sh_flags = sections1[i].sh_flags;
                sections[i].sh_addr = sections1[i].sh_addr;
                sections[i].sh_name = sections1[i].sh_name;
                sections[i].sh_link = sections1[i].sh_link;
                sections[i].sh_info = sections1[i].sh_info;
                sections[i].sh_addralign = sections1[i].sh_addralign;
                sections[i].sh_entsize = sections1[i].sh_entsize;
                // On met le data contenant la section du 1 dans la structure de fusion
                if(strcmp(data, "") != 0){
                    strcpy(fusion->data[i], data);
                }
                // Libérer la mémoire
                free(data);
                // On met à jour l'offset courant
                offset += sections1[i].sh_size;
            }
            //free(tmp2);
        } else {
            //fprintf(stderr, "i = %d, size = %u\n", i, sections1[i].sh_size);
            if(sections1[i].sh_size == 0) {
                continue;
            }
            data = malloc(sections1[i].sh_size);
            if (!data) {
                perror("Erreur lors de l'allocation de la mémoire");
                exit(EXIT_FAILURE);
            }
            //data = "";
            // On se déplace à l'offset de la section du 1
            fseek(f1, sections1[i].sh_offset, SEEK_SET);
            // On copie dans data la section du 1
            memcpy(data, f1, sections1[i].sh_size);
            // On met à jour la section finale
            sections[i].sh_offset = offset;
            sections[i].sh_size = sections1[i].sh_size;
            sections[i].sh_type = sections1[i].sh_type;
            sections[i].sh_flags = sections1[i].sh_flags;
            sections[i].sh_addr = sections1[i].sh_addr;
            sections[i].sh_name = sections1[i].sh_name;
            sections[i].sh_link = sections1[i].sh_link;
            sections[i].sh_info = sections1[i].sh_info;
            sections[i].sh_addralign = sections1[i].sh_addralign;
            sections[i].sh_entsize = sections1[i].sh_entsize;
            // On met à jour fusion
            //fprintf(stderr, "fusion->data[%d] = %s\n", i, fusion->data[i]);
            //fprintf(stderr, "data = %x\n", data);
            if(strcmp(data, "") != 0 || strcmp(data, "\0") != 0 || data != NULL) {
                strcpy(fusion->data[i], data);
            }
            // Libérer la mémoire
            free(data);
            // Mise à jour de l'offset courant
            offset += sections1[i].sh_size;
        }
        // Incrémentation du nombre de sections
        nb_sym++;
        free(tmp2);
    }
    // Pour chaque section non concaténéé du fichier 2
    int current = 0;
    int new = 0;
    for (int j = 1; j < nb_sym2; j++) {
        if (changes[j].old_index == -1) {
            data = malloc(sections2[j].sh_size);
            if (!data) {
                perror("Erreur lors de l'allocation de la mémoire");
                exit(EXIT_FAILURE);
            }
            // On se déplace à l'offset de la section du 2
            fseek(f2, sections2[j].sh_offset, SEEK_SET);
            // On copie dans data la section du 2
            memcpy(data, f2, sections2[j].sh_size);
            // On met à jour le tableau changes
            //fprintf(stderr, "section 2 non concat : j = %d, new numero = %d\n", j, nb_sym1 + current);
            new = nb_sym1 + current;
            changes[j].old_index = j;
            changes[j].new_index = new;
            current++;
            // On met à jour la section finale
            sections[new].sh_offset = offset;
            sections[new].sh_size = sections2[j].sh_size;
            sections[new].sh_type = sections2[j].sh_type;
            sections[new].sh_flags = sections2[j].sh_flags;
            sections[new].sh_addr = sections2[j].sh_addr;
            sections[new].sh_name = sections2[j].sh_name;
            sections[new].sh_flags = sections2[j].sh_flags;
            sections[new].sh_link = sections2[j].sh_link;
            sections[new].sh_info = sections2[j].sh_info;
            sections[new].sh_addralign = sections2[j].sh_addralign;
            sections[new].sh_entsize = sections2[j].sh_entsize;
            // On met les données dans fusion
            if(strcmp(data, "") != 0) {
                strcpy(fusion->data[new], data);
            }
            // Mise à jour du nombre de sections
            nb_sym++;
            // Libérer la mémoire
            free(data);
            // On met à jour l'offset courant
            offset += sections2[j].sh_size;
        }
        // Copie de changes
        fusion->changes[j] = changes[j];
    }
    // Mise à jour de fusion avec les éléments finaux
    fusion->sections = sections;
    fusion->nb_sections = nb_sym;

    // Libérer la mémoire
    for(int i = 1; i < nb_sym1; i++) {
        free(names1[i]);
    }
    free(names1);
    for(int i=0; i<nb_sym2; i++) {
        free(names2[i]);
    }
    free(names2);
    free(sections);

    return fusion;
}

/* Etape 7 */
void fusion_symbols(FILE *f1, FILE *f2, Elf32_Ehdr header1, Elf32_Shdr *sections1, Elf32_Ehdr header2, Elf32_Shdr *sections2){
}

/*
// New header
Elf32_Ehdr new_header;
memset(new_header.e_ident, 0, EI_NIDENT);
new_header.e_ident[EI_MAG0] = ELFMAG0;
new_header.e_ident[EI_MAG1] = ELFMAG1;
new_header.e_ident[EI_MAG2] = ELFMAG2;
new_header.e_ident[EI_MAG3] = ELFMAG3;
new_header.e_ident[EI_CLASS] = ELFCLASS32;
new_header.e_ident[EI_DATA] = ELFDATA2MSB;
new_header.e_ident[EI_VERSION] = EV_CURRENT;
new_header.e_ident[EI_PAD] = 0;
new_header.e_type = header1.e_type;
new_header.e_machine = header1.e_machine;
new_header.e_version = header1.e_version;
new_header.e_entry = header1.e_entry;
new_header.e_phoff = header1.e_phoff;
new_header.e_shoff = sizeof(Elf32_Ehdr);
new_header.e_flags = header1.e_flags;
new_header.e_ehsize = header1.e_ehsize;
new_header.e_phentsize = header1.e_phentsize;
new_header.e_phnum = header1.e_phnum;
new_header.e_shentsize = header1.e_shentsize;
new_header.e_shnum = 0;
new_header.e_shstrndx = SHN_UNDEF;

// Ecriture du header dans le fichier resultat
bwrite(&new_header, sizeof(Elf32_Ehdr), 1, result);
 */