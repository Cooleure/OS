#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "layer1.h"
#include "layer2.h"
#include "layer3.h"
#include "struct.h"

virtual_disk_t virtual_disk_sos;

int init_disk_sos(char* dirname) {
	char *tmp = malloc(sizeof(char) * (strlen(dirname) + strlen("/d0")) + 1);
	strcpy(tmp, dirname);
	strcat(tmp, "/d0");
	virtual_disk_sos.storage = fopen(tmp, "r+");
	free(tmp);

	for (int i = 0; i < INODE_TABLE_SIZE; i++) {
		strcpy(virtual_disk_sos.inodes[i].filename, "");
	}

	if ((read_super_block(&virtual_disk_sos.super_block)) == READ_FAILURE) {
		return 0;
	}

	if ((read_inodes_table()) == READ_FAILURE) {	
		return 0;
	}

	load_user_table();

	return 1;
}

int compute_nblock(const int size) {
	return ceil(size / BLOCK_SIZE) + 1; // Arrondi par excès
}

int write_block(const block_t b, int pos) {

	fseek(virtual_disk_sos.storage, pos * BLOCK_SIZE, SEEK_SET);

	if (fwrite(&b, BLOCK_SIZE, 1, virtual_disk_sos.storage) != 1) {
		fprintf(stderr, "Block write problem\n");
		return 1;
	}

	return 0;
}

int read_block(block_t * block, int pos) {
	fseek(virtual_disk_sos.storage, pos * BLOCK_SIZE, SEEK_SET);

	if (fread(block, BLOCK_SIZE, 1, virtual_disk_sos.storage) != 1) {
		fprintf(stderr, "Block reading problem\n");
		return READ_FAILURE;
	}

	return 0;
}

int clear_block(int pos) {
	block_t block;
	return write_block(block, pos);
}

void print_block(const block_t b) {
	for (int i = 0; i < BLOCK_SIZE; i++) {
		printf("%x", b.data[i]);
	}
}

int switch_off() {
	write_super_block();

	if (fclose(virtual_disk_sos.storage) == EOF){
		fprintf(stderr, "Vdisk closing problem\n");
		return 1;
	}

	return 0;
}
