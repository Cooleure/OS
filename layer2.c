/**
 * @file layer2.c
 * @author Florian Dion - Nicolas Dejean - Nathan Fleury
 * @brief
 * @version 0.1
 * @date 2022-02-20
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

#include "layer1.h"
#include "layer2.h"
#include "struct.h"


int init_super_block() {
	virtual_disk_sos.super_block.number_of_files = 0;
	virtual_disk_sos.super_block.number_of_users = 1;
	virtual_disk_sos.super_block.nb_blocks_used = 0;
	virtual_disk_sos.super_block.first_free_byte = (SUPER_BLOCK_SIZE * BLOCK_SIZE) + (INODE_TABLE_SIZE * INODE_SIZE * BLOCK_SIZE) + (NB_USERS * USER_MEM_SIZE);

	return write_super_block();
}

int write_super_block() {
	fseek(virtual_disk_sos.storage, 0, SEEK_SET);

	if (fwrite(&virtual_disk_sos.super_block, SUPER_BLOCK_SIZE * BLOCK_SIZE, 1, virtual_disk_sos.storage) != 1) {
		fprintf(stderr, "Super block write problem\n");
		return 1;
	}

	return 0;
}

int read_super_block(super_block_t *super_block) {
	fseek(virtual_disk_sos.storage, 0, SEEK_SET);

	if (fread(super_block, SUPER_BLOCK_SIZE * BLOCK_SIZE, 1, virtual_disk_sos.storage) != 1) {
		fprintf(stderr, "Block reading problem\n");
		return READ_FAILURE;
	}

	return 0;
}

void update_free_byte(int nbBlock, char sign, super_block_t *super_block) {
	switch (sign) {
		case '+':
			super_block->first_free_byte += nbBlock * BLOCK_SIZE;
			break;

		case '-':
			super_block->first_free_byte -= nbBlock * BLOCK_SIZE;

		default:
			break;
	}
}

int read_inodes_table() {
	fseek(virtual_disk_sos.storage, INODES_START, SEEK_SET);

	if (fread(virtual_disk_sos.inodes, INODE_TABLE_SIZE * INODE_SIZE * BLOCK_SIZE, 1, virtual_disk_sos.storage) != 1) {
		fprintf(stderr, "Inode table reading problem\n");
		return READ_FAILURE;
	}

	int nblocks = 0;
	for (int i = 0; i < INODE_TABLE_SIZE; i++) {
		if (strcmp(virtual_disk_sos.inodes[i].filename, "")) {
			nblocks+= virtual_disk_sos.inodes[i].nblock;
		}
	}

	update_free_byte(nblocks, '+', &(virtual_disk_sos.super_block));

	return 1;
}

int write_inodes_table() {
	fseek(virtual_disk_sos.storage, INODES_START, SEEK_SET);

	for (int i = 0; i < INODE_TABLE_SIZE; i++) {
		if (fwrite(&(virtual_disk_sos.inodes[i]), INODE_SIZE * BLOCK_SIZE, 1, virtual_disk_sos.storage) != 1) {
			fprintf(stderr, "Inode writing problem\n");
			return 0;
		}
	}

	return 1;
}

void delete_inode(int i) {
	// Dernière inode de la table
	inode_t inode = virtual_disk_sos.inodes[get_unused_inode() - 1];

	virtual_disk_sos.inodes[i] = inode;
	virtual_disk_sos.inodes[get_unused_inode() - 1].first_byte = 0;
	strcpy(virtual_disk_sos.inodes[get_unused_inode() - 1].filename, "");
	virtual_disk_sos.super_block.number_of_files--;

	// Actualisation de la table dans le disque
	write_inodes_table();
}

int init_inode(char *file, int size, uint pos) {
	if (virtual_disk_sos.super_block.number_of_files == 10) {
		fprintf(stderr, "Inode table full !\n");
		return 0;
	}

	// Récuperation du pointeur de la première inode disponible
	int index = get_unused_inode();
	inode_t *inode = &virtual_disk_sos.inodes[index];

	inode->size = size;
	inode->first_byte = pos;
	inode->nblock = compute_nblock(size);
	strcpy(inode->filename, file);

	time_t timing = time(NULL);
	strcpy(inode->ctimestamp, ctime(&timing));
	strcpy(inode->mtimestamp, ctime(&timing));
	//Remove \n char
	inode->ctimestamp[strcspn(inode->ctimestamp, "\n")] = 0;
	inode->mtimestamp[strcspn(inode->mtimestamp, "\n")] = 0;

	// Mise à jour du super block
	virtual_disk_sos.super_block.nb_blocks_used += inode->nblock;
	update_free_byte(inode->nblock, '+', &virtual_disk_sos.super_block);
	virtual_disk_sos.super_block.number_of_files++;

	// Ecriture de cette table mise à jour
	return write_inodes_table();
}

int get_unused_inode() {
	for (int i = 0; i < INODE_TABLE_SIZE; i++) {
		if (!strcmp(virtual_disk_sos.inodes[i].filename, "")) {
			return i;
		}
	}

	return virtual_disk_sos.super_block.number_of_files;
}
