/**
 * @file layer4.h
 * @author Florian Dion - Nicolas Dejean - Nathan Fleury
 * @brief
 * @version 0.1
 * @date 2022-04-05
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef LAYER4_H
#define LAYER4_H

#include "layer1.h"
#include "layer2.h"
#include "layer3.h"
#include "struct.h"
#include "sha256_utils.h"
#include "sha256.h"
/**
 * @brief return index of existing file in the table of inode or -1
 * 
 * @param filename 
 * @return int 
 */
int existing_file(char *filename);

/**
 * @brief Write file on the system. If does not exist, then created, else updated.
 *
 * @param filename Name of the file
 * @param file The file to read the data from
 * @return 1 if no problems were encountered, 0 otherwise
 */
int write_file(char *filename, file_t *file);

/**
 * @brief Read the file from the system
 *
 * @param filename Name of the file
 * @param file The file to write the result to
 * @return 1 if no problems were encountered, 0 otherwise or if the file does not exist
 */
int read_file(char *filename, file_t *file);

/**
 * @brief Delete the file from system
 *
 * @param filename Name of the file
 * @return 1 if no problems were encountered, 0 otherwise of if the file does not exist
 */
int delete_file(char *filename);

/**
 * @brief Read the file from the host computer and write it into the system
 *
 * @param filename Name of the file
 * @return 1 if no problems were encountered, 0 otherwise
 */
int load_file_from_host(char *filename);

/**
 * @brief Read the file from the system and write it into the host computer
 *
 * @param filename Name of the file
 * @return 1 if no problems were encountered, 0 otherwise
 */
int store_file_to_host(char *filename);

#endif