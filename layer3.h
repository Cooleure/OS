/**
 * @file layer3.h
 * @author Florian Dion - Nicolas Dejean - Nathan Fleury
 * @brief
 * @version 0.1
 * @date 2022-03-27
 */
#ifndef LAYER3_H
#define LAYER3_H

#include "layer1.h"
#include "layer2.h"
#include "struct.h"
#include "sha256_utils.h"
#include "sha256.h"

#define ROOT_UID 0
#define PASSWORD_SIZE 15

int init_user_table();
int load_user_table();
int new_user(char login[FILENAME_MAX_SIZE], char passwd[SHA256_BLOCK_SIZE*2 + 1]);
int write_user_table();
int delete_user(char login[FILENAME_MAX_SIZE]);

#endif
