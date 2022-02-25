#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "layer1.h"
#include "struct.h"

virtual_disk_t vitual_disk_sos;

void init_disk_sos (FILE* dirname){
  vitual_disk_sos.storage = dirname;
}

int compute_nblock(const int size){
  return ceil(size/BLOCK_SIZE); //Arrondi par excès
}

int write_block (const block_t b, int pos){
  int fd = open(virtual_disk_sos.storage, O_WRONLY);
  if (fd == -1){
    fprintf(stderr, "Probleme d'ouverture du disque\n");
    return 1;
  }
  lseek(fd, pos*BLOCK_SIZE, SEEK_SET);
  if(write(fd, &b, BLOCK_SIZE) != BLOCK_SIZE){
    fprintf(stderr, "Probleme d'ecriture d'un bloc\n");
    return 1;
  }
  if(close(fd) == -1){
      fprintf(stderr, "Probleme de fermeture du disque");
      return 1;
  }
  return 0;
}

int read_block (block_t block, int pos){
  int fd = open(virtual_disk_sos.storage, O_RDONLY);
  if (fd == -1){
    fprintf(stderr, "Probleme d'ouverture du disque\n");
    return 1;
  }

  lseek(fd, pos*BLOCK_SIZE, SEEK_SET);
  if (read(fd, &b, BLOCK_SIZE) != BLOCK_SIZE){
    fprintf(stderr, "Probleme de lecture du block\n");
    return READ_FAILURE;
  }
  if(close(fd) == -1){
      fprintf(stderr, "Probleme de fermeture du disque");
      return 1;
  }
  return 0;

}


void print_block (const block_t b, char* f){
  for(int i=0; i<BLOCK_SIZE; i++){
    printf("%x",b.data[i]);
  }
}
