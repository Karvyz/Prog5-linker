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
    names1 = malloc(nb_sym1 * sizeof(char *));
    for(int i=0; i<nb_sym1; i++){
        names1[i] = malloc(sizeof(char) * 100);
        names1[i]  = "";
    }
    char *name1 = NULL;

    char **names2 = NULL;
    names2 = malloc(nb_sym2 * sizeof(char *));
    for(int i=0; i<nb_sym2; i++){
        names2[i] = malloc(sizeof(char) * 100);
        names2[i]  = "";
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
    Elf32_Shdr *sections = malloc(sizeof(Elf32_Shdr) * ((header1.e_shentsize * header1.e_shnum) + (header2.e_shentsize * header2.e_shnum)));
    if (!sections) {
        perror("Erreur lors de l'allocation de la table des sections");
        exit(EXIT_FAILURE);
    }
    memset(sections, 0, sizeof(Elf32_Shdr) * ((header1.e_shentsize * header1.e_shnum) + (header2.e_shentsize * header2.e_shnum)));

    // Structure fusion qui contiendra tous les éléments utiles une fois la fusion terminée
    SectionFusion *fusion = NULL;
    fusion = malloc(sizeof(SectionFusion));
    if (!fusion) {
        
        perror("Erreur lors de l'allocation de la structure SectionFusion");
        exit(EXIT_FAILURE);
    }
    // data contient les données des sections
    fusion->data=malloc(sizeof(char) * 400);
    if (!fusion->data) {
        perror("Erreur lors de l'allocation de la structure SectionFusion->data");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<nb_sym1+nb_sym2; i++){
        fusion->data[i] = NULL;
    }

// char **monPP;
// monPP=(char**)malloc(sizeof(char*)*longueurT1);    
// int i;
// for(i= 0 ; i < longueurT1 ; i++)
//     monPP[i]=(char*)malloc(sizeof(char)*longueur);

    fusion->sections_names=malloc(sizeof(char) * 400);
    if (!fusion->sections_names) {
        perror("Erreur lors de l'allocation de la structure SectionFusion->sections_names");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<nb_sym1+nb_sym2; i++){
        fusion->sections_names[i] = malloc(sizeof(char)*50);
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
        fprintf(stderr, "i=%d\n", i);

        if(sections1[i].sh_type == SHT_STRTAB && sections[i].sh_name == header1.e_shstrndx){
            //new_header.e_shstrndx = sections1[i].sh_offset;
        }

        name1 = malloc(sizeof(char) *30);
        if(!name1){
            perror("Erreur allocation name1\n");
            exit(1);
        }
        name1 = read_from_shstrtab(sections1[i].sh_name, read_section_names(f1, sections1[header1.e_shstrndx]));
        names1[i] = name1;

        if(sections1[i].sh_type == SHT_PROGBITS) {
            // test si chaque section du 2ème fichier est de type PROGBIT
            // Booleen pour vérifier si la section 1 peut être ajoutée si pas de fusion possible
            int bool = 0;
            for(int j = 1; j < nb_sym2; j++) {
                fprintf(stderr, "j=%d\n", j);
                // Nom de la section courant du fichier 2
                name2 = malloc(sizeof(char) * 30);
                if(!name2) {
                    perror("Erreur allcoation name2\n");
                    exit(1);
                }
                name2 = read_from_shstrtab(sections2[j].sh_name, read_section_names(f2, sections2[header2.e_shstrndx]));
                names2[j] = name2;
                // Debug
                if(strcmp(names1[i], "\0") == 0 || strcmp(names2[j], "\0") == 0 || names1[i] == NULL || names2[j] == NULL || strcmp(names1[i], "") == 0 || strcmp(names2[j], "") == 0){
                    continue;
                }
                fprintf(stderr, "name 1 = %s name2 = %s\n", names1[i], names2[j]);
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
                    sections[i].sh_link = sections1[i].sh_link;
                    sections[i].sh_info = sections1[i].sh_info;
                    sections[i].sh_addralign = sections1[i].sh_addralign;
                    sections[i].sh_entsize = sections1[i].sh_entsize;
                    // On met à jour le tableau changes
                    changes[j].old_index = j;
                    changes[j].new_index = i;
                    changes[j].offset = sections1[i].sh_size;
                    // On met à jour l'offset courant
                    offset += sections1[i].sh_size + sections2[j].sh_size;
                    // On met le data contenant la fusion des 2 sections dans la structure de fusion
                    fusion->data[i] = data;
                    free(data);
                    strcpy(fusion->sections_names[i], names1[i]);
                    break;
                }
                free(name2);
            }
            if(!bool) {
                // Allocation mémoire à data
                data = malloc(sections1[i].sh_size);
                if (!data) {
                    perror("Erreur lors de l'allocation de la mémoire");
                    exit(EXIT_FAILURE);
                }
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
                sections[i].sh_flags = sections1[i].sh_flags;
                sections[i].sh_link = sections1[i].sh_link;
                sections[i].sh_info = sections1[i].sh_info;
                sections[i].sh_addralign = sections1[i].sh_addralign;
                sections[i].sh_entsize = sections1[i].sh_entsize;
                // On met le data contenant la section du 1 dans la structure de fusion
                fusion->data[i] = data;
                // Libérer la mémoire
                free(data);
                strcpy(fusion->sections_names[i], names1[i]);
                // On met à jour l'offset courant
                offset += sections1[i].sh_size;
            }
            //else de if(progbit)
        }else{
            data = malloc(sections1[i].sh_size);
            if (!data) {
                perror("Erreur lors de l'allocation de la mémoire");
                exit(EXIT_FAILURE);
            }
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
            sections[i].sh_flags = sections1[i].sh_flags;
            sections[i].sh_link = sections1[i].sh_link;
            sections[i].sh_info = sections1[i].sh_info;
            sections[i].sh_addralign = sections1[i].sh_addralign;
            sections[i].sh_entsize = sections1[i].sh_entsize;
            // On met à jour fusion
            fusion->data[i] = data;
            // Libérer la mémoire
            free(data);
            strcpy(fusion->sections_names[i], names1[i]);
            // Mise à jour de l'offset courant
            offset += sections1[i].sh_size;
        }
        // Incrémentation du nombre de sections
        nb_sym++;
        // Libérer la mémoire
        free(name1);
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
            fprintf(stderr, "section 2 non concat : j = %d, new numero = %d\n", j, nb_sym1 + current);
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
            sections[new].sh_link = sections2[j].sh_link;
            sections[new].sh_info = sections2[j].sh_info;
            sections[new].sh_addralign = sections2[j].sh_addralign;
            sections[new].sh_entsize = sections2[j].sh_entsize;
            // On met les données dans fusion
            fusion->data[new] = data;
            // Mise à jour du nombre de sections
            nb_sym++;
            // Libérer la mémoire
            free(data);
            strcpy(fusion->sections_names[new], names2[j]);
            // On met à jour l'offset courant
            offset += sections2[j].sh_size;
        }
        // Copie de changes
        fusion->changes[j] = changes[j];
    }
    // Mise à jour de fusion avec les éléments finaux
    fusion->sections = sections;
    fusion->nb_sections = nb_sym;

    return fusion;
}

/* Etape 7 */
void fusion_symbols(FILE *f1, FILE *f2, Elf32_Ehdr header1, Elf32_Sym *symbole1, int nb_symbols1, Elf32_Ehdr header2, Elf32_Sym *symbole2, int nb_symbols2, SectionFusion *sectionsFusion){
    Elf32_Sym* fusion_symbole = NULL;
    Elf32_Shdr SymTab;
    int current = 0;
    int j,z,i,nouv_nb_symbols,plus_grand_nb_symbols;
    for(i = 0; i < sectionsFusion->nb_sections; i++){
        if (strcmp(sectionsFusion->sections_names[i],".symtab") == 0) { // Si le nom de la section correspond au nom recherché
            SymTab = sectionsFusion->sections[i]; // On modifie la section (vide) passée en paramètre par la section trouvée
            break;
        }
    }
//////////////// Nouveau nombre de symbol
    if(nb_symbols1 <= nb_symbols2){
        plus_grand_nb_symbols = 2; 
    }else{
        plus_grand_nb_symbols = 1;
    }
    
    if(plus_grand_nb_symbols == 1){
        nouv_nb_symbols = nb_symbols1 + (nb_symbols1-nb_symbols2); 
    }else{
        nouv_nb_symbols = nb_symbols2 + (nb_symbols2-nb_symbols1); 
    }
//////////////// Routine de décision en cas de symbole GLOBAL 
 if(ELF32_ST_BIND(symbole1[i].st_info) == STB_GLOBAL){
        for(j=i;j<nouv_nb_symbols;j++){
            if(symbole1[i].st_name == symbole2[j].st_name){
                if(ELF32_ST_TYPE(symbole1[i].st_shndx) == SHN_UNDEF && ELF32_ST_TYPE(symbole2[i].st_shndx) == SHN_UNDEF){
                    fusion_symbole[current].st_name = symbole1[i].st_name;
                    fusion_symbole[current].st_value = symbole1[i].st_value;
                    fusion_symbole[current].st_size = symbole1[i].st_size;
                    fusion_symbole[current].st_info = symbole1[i].st_info;
                    fusion_symbole[current].st_other = symbole1[i].st_other;
                }if else(ELF32_ST_TYPE(symbole1[i].st_shndx) != SHN_UNDEF && ELF32_ST_TYPE(symbole2[i].st_shndx) != SHN_UNDEF){
                    fprintf(stderr,)
                    return 0; //ERREUR FATALE POUR LA FUSION
                }if else(ELF32_ST_TYPE(symbole1[i].st_shndx) != SHN_UNDEF){
                    fusion_symbole[current].st_name = symbole1[i].st_name;
                    fusion_symbole[current].st_value = symbole1[i].st_value;
                    fusion_symbole[current].st_size = symbole1[i].st_size;
                    fusion_symbole[current].st_info = symbole1[i].st_info;
                    fusion_symbole[current].st_other = symbole1[i].st_other;
                }if else(ELF32_ST_TYPE(symbole2[i].st_shndx) != SHN_UNDEF){
                    fusion_symbole[current].st_name = symbole2[i].st_name;
                    fusion_symbole[current].st_value = symbole2[i].st_value;
                    fusion_symbole[current].st_size = symbole2[i].st_size;
                    fusion_symbole[current].st_info = symbole2[i].st_info;
                    fusion_symbole[current].st_other = symbole2[i].st_other;
                }
        }
    }

    while(i = 0; i < nouv_nb_symbols;i++){
        if (symbole1[i].st_name == symbole2[i].st_name){
            fusion_symbole[current].st_name = symbole1[i].st_name;
            fusion_symbole[current].st_value = symbole1[i].st_value;
            fusion_symbole[current].st_size = symbole1[i].st_size;
            fusion_symbole[current].st_info = symbole1[i].st_info;
            fusion_symbole[current].st_other = symbole1[i].st_other;
            current++;
        }else{
            j=i;
            while(j<nouv_nb_symbols){
                z=j;
                while(z<nouv_nb_symbols){
                    if(symbole1[j].st_name == symbole2[z].st_name){
                        break;
                    }
                    z++;
                }
                if(symbole1[j].st_name == symbole2[z].st_name){
                    break;
                }
                j++
            }
            y=i;
            while(y<j){
                fusion_symbole[current].st_name = symbole1[y].st_name;
                fusion_symbole[current].st_value = symbole1[y].st_value;
                fusion_symbole[current].st_size = symbole1[y].st_size;
                fusion_symbole[current].st_info = symbole1[y].st_info;
                fusion_symbole[current].st_other = symbole1[y].st_other;
                current++;
                y++;
            }
            y=i;
            while(y<z){
                fusion_symbole[current].st_name = symbole2[y].st_name;
                fusion_symbole[current].st_value = symbole2[y].st_value;
                fusion_symbole[current].st_size = symbole2[y].st_size;
                fusion_symbole[current].st_info = symbole2[y].st_info;
                fusion_symbole[current].st_other = symbole2[y].st_other;
                current++;
                j++;
            }
        }
    }

}

    // fusion_symbole->st_name;		/* Symbol name (string tbl index) */
    // fusion_symbole->st_value;		/* Symbol value */
    // fusion_symbole->st_size;		/* Symbol size */
    // fusion_symbole->st_info;		/* Symbol type and binding */
    // fusion_symbole->st_other;		/* Symbol visibility */
    // fusion_symbole->st_shndx;	




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
