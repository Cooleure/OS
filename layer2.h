/**
 * @file layer2.h
 * @author Florian Dion - Nicolas Dejean - Nathan Fleury
 * @brief
 * @version 0.1
 * @date 2022-02-20
 */
 #ifndef LAYER2_H
 #define LAYER2_H

 /**
  * @brief Creation the Superblock of disk OS
  *
  * @param OS disk
  *
  * @Return 0 if an error occurs
  */
int write_super_block(virtual_disk_t * virtual_disk_sos);

/**
 * @brief Read the Superblock of disk OS
 *
 * @param OS disk
 *
 * @Return super block else NULL
 */
super_block_t read_super_block(virtual_disk_t * virtual_disk_sos);





#endif
