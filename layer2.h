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

 //extern inode_table_t inodes_table;

 /**
  * @brief Creation the Superblock of disk OS
  *
  * @param OS disk
  *
  * @Return 0 if an error occurs
  */
int init_super_block();

 /**
  * @brief Writing the actual super block on disk OS
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

/**
 * @brief Update the first free byte in the super block
 *
 * @param nbBlock
 * @param sign (+ or -)
 */
void update_first_free_byte (int nbBlock, char sign, super_block_t * super_block);


/**
 * @brief Read the inode table on the system, return 0 if OK or READ_FAILURE if KO
 *
 * @param table
 * @return int
 */
int read_inodes_table(inode_table_t * table);

/**
 * @brief Write the inodes table on the system with virtual disk virtual_disk_sos.inodes
 *
 * @return int : 0 if OK else 1
 */
int write_inodes_table();

/**
 * @brief delete inode i, replace others ant modify the super_block
 *
 * @param i
 */
void delete_inode(int i);

/**
 * @brief inits an inode
 *
 * @param file, size, pos
 */
inode_t init_inode(char* file, int size, uint pos);

#endif
