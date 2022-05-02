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

/**
 * @brief Initialize the user table, and root user
 * @return 0 if something goes wrong
 */
int init_user_table();

/**
 * @brief Load user table into virtual disk
 * @return 0 if something goes wrong
 */
int load_user_table();

/**
 * @brief Create a new user
 * @param login of the user
 * @param hashed passord
 * @return 0 if something goes wrong
 */
int new_user(char login[FILENAME_MAX_SIZE], char passwd[SHA256_BLOCK_SIZE*2 + 1]);

/**
 * @brief Write the user table into disk
 * @return 0 if something goes wrong
 */
int write_user_table();

/**
 * @brief Remove a user
 */
int delete_user(char login[FILENAME_MAX_SIZE]);

#endif
