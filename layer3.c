/**
 * @file layer3.c
 * @author Florian Dion - Nicolas Dejean - Nathan Fleury
 * @brief
 * @version 0.1
 * @date 2022-03-28
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "layer3.h"
#include "sha256_utils.h"

int init_user_table(){
    strcpy(virtual_disk_sos.users_table[ROOT_UID].login, "root");

    //Initialisation du mot de passe
	 char hashRes[SHA256_BLOCK_SIZE*2 + 1];

    printf("Définir un mot de passe utilisateur: ");
    char mdp[PASSWORD_SIZE+1];
	if (fgets(mdp, PASSWORD_SIZE, stdin) == NULL) {
		fprintf(stderr, "Error login init user table\n");
		return 0;
	}
    mdp[strcspn(mdp, "\n")] = 0;
    sha256ofString((BYTE *)mdp,hashRes);
    strcpy(virtual_disk_sos.users_table[ROOT_UID].passwd, hashRes);

    //Ecriture de la table dans le disque (apres la table d'inodes)
    return write_user_table();
}


int load_user_table(){
    //Positionnement dans le disque
    fseek(virtual_disk_sos.storage, USER_START, SEEK_SET);

    //Lecture
    if(fread(virtual_disk_sos.users_table, USER_MEM_SIZE * NB_USERS, 1, virtual_disk_sos.storage) != 1){
        fprintf(stderr, "Users table reading problem\n");
        return 0;
    }
    return 1;
}

int get_unused_user(){
  for(int i = 0; i < NB_USERS; i++){
    if(!strcmp(virtual_disk_sos.users_table[i].login,"")){
      return i;
    }
  }
  return virtual_disk_sos.super_block.number_of_files;
}

int new_user(char login[FILENAME_MAX_SIZE], char passwd[SHA256_BLOCK_SIZE*2 + 1]){
    if(virtual_disk_sos.super_block.number_of_users == NB_USERS){
        return 0;
    }
    int i = get_unused_user();
    user_t* user = &virtual_disk_sos.users_table[i];
    strcpy(user->login, login);
    strcpy(user->passwd, passwd);
    virtual_disk_sos.super_block.number_of_users++;
    return write_user_table();
}

int write_user_table(){
    fseek(virtual_disk_sos.storage, USER_START, SEEK_SET);

    if(fwrite(virtual_disk_sos.users_table, USER_MEM_SIZE * NB_USERS, 1,virtual_disk_sos.storage) != 1){
        fprintf(stderr, "users table writing problem\n");
        return 0;
    }
    return 1;
}

int delete_user(char login[FILENAME_MAX_SIZE]){
    int i=0;
    //Dernier utilisateur dans le tableau
    while(i<virtual_disk_sos.super_block.number_of_users){
        if(!strcmp(login, virtual_disk_sos.users_table[i].login)){
            //remplacement de l'utilisateur
            strcpy(virtual_disk_sos.users_table[i].login, "");
            virtual_disk_sos.super_block.number_of_users--;
            write_user_table();
            return 0;
        }
        i++;
    }
    return 1;
}
