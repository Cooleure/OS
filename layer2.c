#include <stdio.h>
#include <stdlib.h>

#include "struct.h"

inode_table_t inodes_table;


int write_super_block(virtual_disk_t * virtual_disk_sos){

  super_block_t super_block;
  super_block.number_of_files = 0;
  super_block.number_of_users = 1;
  super_block.nb_blocks_used = 0;
  super_block.first_free_byte = SUPER_BLOCK_SIZE+1;

  fseek(virtual_disk_sos->storage, 0, SEEK_SET);
  if(fwrite(&super_block, SUPER_BLOCK_SIZE*BLOCK_SIZE, 1, virtual_disk_sos->storage) != SUPER_BLOCK_SIZE*BLOCK_SIZE){
    fprintf(stderr, "super block write problem\n");
    return 1;
  }
  return 0;
}

super_block_t read_super_block(virtual_disk_t * virtual_disk_sos){
  super_block_t super_block;
  fseek(virtual_disk_sos->storage, 0, SEEK_SET);

  if (fread(&super_block, SUPER_BLOCK_SIZE*BLOCK_SIZE, 1, virtual_disk_sos.storage) != SUPER_BLOCK_SIZE*BLOCK_SIZE){
    fprintf(stderr, "Block reading problem\n");
    return READ_FAILURE;
  }
  return 0;
}
