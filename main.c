#include <stdio.h>
#include "layer1.h"
#include "layer2.h"



int main(){
  printf("%d", init_disk_sos("Tests"));
  //ecriture d'une inode dans la table d'inode et positionnement du premier byte à first_free_byte (pos)
  init_inode("fichier1", 1000, virtual_disk_sos.super_block.first_free_byte);
}
