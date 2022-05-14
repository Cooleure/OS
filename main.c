#include <stdio.h>
#include <string.h>

#include "layer1.h"
#include "layer2.h"
#include "layer3.h"
#include "layer5.h"
#include "user_interface.h"

int main(void) {
	init_disk_sos("Tests");
	if (login()) console();
	switch_off();
	exit(0);
}
