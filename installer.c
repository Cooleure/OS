#include <stdio.h>

#include "layer1.h"
#include "layer2.h"
#include "struct.h"


int main(){
  printf("Indiquez le répertoire dans lequel installer le système: \n");
  char * dirName = malloc(sizeof(char));
  scanf("%s", dirName);
  char *tmp = malloc(sizeof(char));
  strcpy(tmp, dirName);
  strcat(tmp, "/d0");
  virtual_disk_sos.storage = fopen(tmp, "r+");
  write_super_block();
  write_inodes_table();
  return 0;
}
