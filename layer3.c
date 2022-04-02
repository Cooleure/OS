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
#include <stdio.h>
#include <stdlib.h>

int init_user_table(){
    strcpy(virtual_disk_sos.users_table[ROOT_UID].login, "root");

    //Initialisation du mot de passe
	 char hashRes[SHA256_BLOCK_SIZE*2 + 1];

    printf("Mot de passe utilisateur : ");
    char mdp[PASSWORD_SIZE];
    scanf("%s", mdp);
    sha256ofString((BYTE *)mdp,hashRes);
    strcpy(virtual_disk_sos.users_table[ROOT_UID].passwd, hashRes);

    //Ecriture de la table dans le disque (apres la table d'inodes)
    int retour = write_user_table();
    return retour;
}


int load_user_table(){
    //Positionnement dans le disque
    fseek(virtual_disk_sos.storage, INODES_START + INODE_TABLE_SIZE*INODE_SIZE*BLOCK_SIZE + 1, SEEK_SET);

    //Lecture
    if(fread(virtual_disk_sos.users_table, USER_MEM_SIZE * NB_USERS, 1, virtual_disk_sos.storage) != 1){
        fprintf(stderr, "Users table reading problem\n");
        return 0;
    }
    return 1;
}

int new_user(char login[FILENAME_MAX_SIZE], char passwd[SHA256_BLOCK_SIZE*2 + 1]){
    if(virtual_disk_sos.super_block.number_of_users == NB_USERS){
        return 0;
    }
    user_t* user = &virtual_disk_sos.users_table[virtual_disk_sos.super_block.number_of_users];
    strcpy(user->login, login);
    strcpy(user->passwd, passwd);
    virtual_disk_sos.super_block.number_of_users++;
}

int write_user_table(){
    fseek(virtual_disk_sos.storage, INODES_START + INODE_TABLE_SIZE*INODE_SIZE*BLOCK_SIZE + 1, SEEK_SET);

    if(fwrite(virtual_disk_sos.users_table, USER_MEM_SIZE * NB_USERS, 1,virtual_disk_sos.storage) != 1){
        fprintf(stderr, "users table writing problem\n");
        return 0;
    }
    return 1;
}

int delete_user(char login[FILENAME_MAX_SIZE]){
    int i=0;
    while(i<virtual_disk_sos.super_block.number_of_users){
        if(strcmp(login, virtual_disk_sos.users_table[i].login)){

        }
        i++;
    }
}
