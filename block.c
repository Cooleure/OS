#include "block.h"

virtual_disk_t vitual_disk_sos;

void init_disk_sos (FILE* dirname){
  vitual_disk_sos.storage = dirname;
}
