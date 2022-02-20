#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "block.h"
#include "struct.h"

virtual_disk_t vitual_disk_sos;

void init_disk_sos (FILE* dirname){
  vitual_disk_sos.storage = dirname;
}

int compute_nblock(const int size){
  return ceil(size/BLOCK_SIZE); //Arrondi par excès
}
