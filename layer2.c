#include <stdio.h>
#include <stdlib.h>

#include "struct.h"
#include "layer2.h"
#include "layer1.h"


//inode_table_t inodes_table;


int write_super_block(){

  super_block_t super_block;
  super_block.number_of_files = 0;
  super_block.number_of_users = 1;
  super_block.nb_blocks_used = 0;
  super_block.first_free_byte = SUPER_BLOCK_SIZE+1;

  fseek(virtual_disk_sos.storage, 0, SEEK_SET);
  if(fwrite(&super_block, SUPER_BLOCK_SIZE*BLOCK_SIZE, 1, virtual_disk_sos.storage) != SUPER_BLOCK_SIZE*BLOCK_SIZE){
    fprintf(stderr, "super block write problem\n");
    return 1;
  }
  return 0;
}

int read_super_block(super_block_t * super_block){

  fseek(virtual_disk_sos.storage, 0, SEEK_SET);

  if (fread(super_block, SUPER_BLOCK_SIZE*BLOCK_SIZE, 1, virtual_disk_sos.storage) != SUPER_BLOCK_SIZE*BLOCK_SIZE){
    fprintf(stderr, "Block reading problem\n");
    return READ_FAILURE;
  }
  return 0;
}

void update_free_byte (int nbBlock, char sign, super_block_t * super_block){
  switch (sign)
  {
  case '+':
    super_block->first_free_byte += nbBlock * BLOCK_SIZE;
    break;
  case '-':
    super_block->first_free_byte -= nbBlock * BLOCK_SIZE;
  
  default:
    break;
  }
}


int read_inodes_table(inode_table_t * table){
  fseek(virtual_disk_sos.storage, INODES_START, SEEK_SET);

  if (fread(table, INODE_TABLE_SIZE * INODE_SIZE * BLOCK_SIZE, 1, virtual_disk_sos.storage) != INODE_TABLE_SIZE*INODE_SIZE*BLOCK_SIZE){
    fprintf(stderr, "Inode table reading problem\n");
    return READ_FAILURE;
  }
  return 0;
}

int write_inodes_table(){
  fseek(virtual_disk_sos.storage, INODES_START, SEEK_SET);

  if (fwrite(virtual_disk_sos.inodes, INODE_TABLE_SIZE * INODE_SIZE * BLOCK_SIZE, 1, virtual_disk_sos.storage) != INODE_TABLE_SIZE * INODE_SIZE * BLOCK_SIZE){
    fprintf(stderr, "Inode writing problem\n");
    return 1;
  }
  return 0;
}