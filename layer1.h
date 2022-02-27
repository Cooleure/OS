/**
 * @file layer1.h
 * @author Florian Dion - Nicolas Dejean - Nathan Fleury
 * @brief
 * @version 0.1
 * @date 2022-02-19
 */
#ifndef LAYER1_H
#define LAYER1_H

#include "struct.h"
#define READ_FAILURE 404

extern virtual_disk_t vitual_disk_sos;

/**
 * @brief Initialization of virtual_disk_sos with the vdisk file
 *
 * @param dirname
 */
void init_disk_sos (char* dirname);

/**
 * @brief save the catalog on the virtual disk
 *
 * @return int
 */
int switch_off ();

/**
 * @brief return the number of necessary block to stock size byte
 *
 * @param size
 * @return int
 */
int compute_nblock (const int size);

/**
 * @brief write the block b at the position pos on the disk
 *
 * @param b
 * @param pos
 * @return int
 */
int write_block (const block_t b, int pos);

/**
 * @brief Read the block "block" at the position pos in the disk
 * return READ_FAILURE if there was a problem
 *
 * @param block
 * @param pos
 * @return int
 */
int read_block (block_t block, int pos);

/**
 * @brief Clear memory of a block
 *
 * @param pos
 * @return int
 */
int clear_block (int pos);

/**
 * @brief Print the block b
 *
 * @param b
 */
void print_block (const block_t b);

/**
 * @brief Closes the virtual disk
 *
 */
int switch_off ();

#endif
