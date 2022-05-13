#include <stdio.h>

#include "layer1.h"
#include "layer2.h"
#include "layer3.h"
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
    fwrite(&zero, 1, 1, fp);
  }
  fclose(fp);
}

int main(void) {
  printf("Indiquez le répertoire dans lequel installer le système: ");
  char * dirName = malloc(sizeof(char));
  if (scanf("%s", dirName) == EOF) {
	  fprintf(stderr, "Directory name reading error\n");
	  return 1;
  }
  char *tmp = malloc(sizeof(char)*strlen(dirName) + 4);
  strcpy(tmp, dirName);
  strcat(tmp, "/d0");
  if((virtual_disk_sos.storage = fopen(tmp, "r+"))){
    char answer;
    printf("Cette installation va remettre à zéro le disque virtuel, en êtes vous sûr ? (y/n) ");
    if (scanf(" %c", &answer) == EOF) {
		fprintf(stderr, "Reset reading error\n");
		return 2;
	}
    if(answer != 'y') {
      printf("'%c'", answer);
      return 0;
    }
  }else{
    if((virtual_disk_sos.storage = fopen(tmp, "w+")) == NULL) exit(0);
  }
  free(tmp);
  int diskSize;
  printf("Quelle taille souhaitez vous allouer à votre OS ?: ");
  if (scanf("%d", &diskSize) == EOF) {
	  fprintf(stderr, "OS size reading error\n");
	  return 3;
  }
  int diskId;
  printf("Quel id souhaitez vous donner au disque ?: ");
  if (scanf("%d", &diskId) == EOF) {
	  fprintf(stderr, "Disk name reading error\n");
	  return 4;
  }
  format(dirName, diskSize, diskId);
  init_super_block();
  printf("Initialisation de l'utilisateur root...\n");
  while (getchar() != '\n');
  init_user_table();
  return 0;
}
