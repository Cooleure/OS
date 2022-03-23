#include <stdio.h>

#include "layer1.h"
#include "layer2.h"
#include "struct.h"

// create a file named di (i = diskid) in directory dirname of size bytes
// the file is initialized with 0
// dirname must be a valid directory
// 0 <= diskid < 10
void format(char *dirname, int size, int diskid){
  char filename[strlen(dirname)+4];
  snprintf(filename, strlen(dirname)+4, "%s/d%d", dirname, diskid);
  printf("%s\n", filename);
  FILE *fp = fopen(filename, "w+");
  assert(fp!=NULL);
  unsigned char zero=0;
  for(int i=0; i<size; i++) {
    int nb_write = fwrite(&zero, 1, 1, fp);
    assert(nb_write == 1);
  }
  fclose(fp);
}

int main(){
  printf("Indiquez le répertoire dans lequel installer le système: ");
  char * dirName = malloc(sizeof(char));
  scanf("%s", dirName);
  char *tmp = malloc(sizeof(char));
  strcpy(tmp, dirName);
  strcat(tmp, "/d0");
  if((virtual_disk_sos.storage = fopen(tmp, "r+"))){
    char answer;
    printf("Cette installation va remettre à zéro le disque virtuel, en êtes vous sûr ? (y/n) ");
    scanf("%c", &answer);
    if(answer != 'y') {
      printf("'%c'", answer);
      return 0;
    }
  }else{
    if((virtual_disk_sos.storage = fopen(tmp, "w+")) == NULL) exit(0);
  }
  int diskSize;
  printf("Quelle taille souhaitez vous allouer à votre OS ?: ");
  scanf("%d", &diskSize);
  int diskId;
  printf("Quel id souhaitez vous donner au disque ?: ");
  scanf("%d", &diskId);
  format(dirName, diskSize, diskId);
  init_super_block();
  write_inodes_table();
  return 0;
}
