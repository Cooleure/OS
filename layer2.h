/**
 * @file layer2.h
 * @author Florian Dion - Nicolas Dejean - Nathan Fleury
 * @brief
 * @version 0.1
 * @date 2022-02-20
 */
 #ifndef LAYER2_H
 #define LAYER2_H
 #define READ_FAILURE 404
 #include "struct.h"

 extern inode_table_t inodes_table;

 /**
  * @brief Creation the Superblock of disk OS
  *
  * @param OS disk
  *
  * @Return 0 if an error occurs
  */
int write_super_block();

/**
 * @brief Read the Superblock of disk OS
 *
 * @param *super_block
 *
 * @Return 0 if OK else READ_FAILURE
 */
int read_super_block(super_block_t * super_block);



#endif
