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


/*Gros doute sur cette fonction vu qu'on veux passer le fichier vdisk à virtual_disk_sos.storage
alors que diname est le nom du repertoire contenant le vdisk*/
void init_disk_sos ( char* dirname){
  strcat(dirname, "/d0");
  vitual_disk_sos.storage = fopen(dirname, "w+b");
}

int compute_nblock(const int size){
  return ceil(size/BLOCK_SIZE); //Arrondi par excès
}

int write_block (const block_t b, int pos){
  FILE* f = fopen(virtual_disk_sos.storage,"w");
  if (f == NULL){
    fprintf(stderr, "Probleme d'ouverture du disque\n");
    return 1;
  }
  fseek(f, pos*BLOCK_SIZE, SEEK_SET);
  if(fwrite(&b, BLOCK_SIZE, 1, f) != BLOCK_SIZE){
    fprintf(stderr, "Probleme d'ecriture d'un bloc\n");
    return 1;
  }
  if(fclose(f) == EOF){
      fprintf(stderr, "Probleme de fermeture du disque");
      return 1;
  }
  return 0;
}

int read_block (block_t block, int pos){
  FILE* f = fopen(virtual_disk_sos.storage,"r");
  if (f == NULL){
    fprintf(stderr, "Probleme d'ouverture du disque\n");
    return 1;
  }

  fseek(f, pos*BLOCK_SIZE, SEEK_SET);
  if (fread(&block, BLOCK_SIZE, 1, f) != BLOCK_SIZE){
    fprintf(stderr, "Probleme de lecture du block\n");
    return READ_FAILURE;
  }
  if(fclose(f) == EOF){
      fprintf(stderr, "Probleme de fermeture du disque");
      return 1;
  }
  return 0;

}


void print_block (const block_t b){
  for(int i=0; i<BLOCK_SIZE; i++){
    printf("%x",b.data[i]);
  }
}
