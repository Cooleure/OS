#include <stdio.h>
#include <string.h>
#include "layer1.h"
#include "layer2.h"
#include "user_interface.h"
#include "layer5.h"
#include "layer3.h"

int main(void) {
  init_disk_sos("Tests");
  //ecriture d'une inode dans la table d'inode et positionnement du premier byte à first_free_byte (pos)
  if(login()) console();
  switch_off();
  exit(0);
}
