#include <stdio.h>
#include <string.h>
#include "layer1.h"
#include "layer2.h"
#include "user_interface.h"
#include "layer5.h"
#include "layer3.h"

int main(){
  init_disk_sos("Tests");
  //ecriture d'une inode dans la table d'inode et positionnement du premier byte à first_free_byte (pos)
  printf("%s\n", virtual_disk_sos.inodes[0].filename);
  init_inode("fichier1", 10, virtual_disk_sos.super_block.first_free_byte);
  if(login())console();
  switch_off();
  exit(0);
}
