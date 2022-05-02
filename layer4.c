/**
 * @file layer4.c
 * @author Florian Dion - Nicolas Dejean - Nathan Fleury
 * @brief
 * @version 0.1
 * @date 2022-04-05
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "layer4.h"
#include "struct.h"

int existing_file(inode_table_t *inode_table, char *filename) {
	for (int i = 0; i < INODE_TABLE_SIZE; i++)
		if (strcmp(inode_table[i] -> filename, filename) == 0)
			return i; // Exists
	return -1; // Does not exist
}

int write_file(char *filename, file_t *file) {
	inode_table_t *inode_table;
	if (read_inodes_table(inode_table)) {
		int fileIndex = existing_file(inode_table, filename);
		if (fileIndex) {
			if (file -> size <= inode_table[fileIndex] -> size) {
				// Modification de l'inode (à mettre dans une fonction update_inode() ?)
				inode_table[fileIndex] -> size = file -> size;
				inode_table[fileIndex] -> nblock = compute_nblock(file -> size);

				time_t timing = time(NULL);
  				strcpy(inode_table[fileIndex] -> mtimestamp, ctime(&timing));

				//Ecriture de cette table mise à jour
				int backup = write_inodes_table();
				return backup;
			} else {
				delete_inode(fileIndex);
				init_inode(filename, file -> size, virtual_disk_sos.super_block.first_free_byte);
			}
		}
		else {
			init_inode(filename, file -> size, virtual_disk_sos.super_block.first_free_byte);
		}
	} else {
		fprintf(stderr, "Read inodes table problem\n");
		return 0;
	}
	return 1;
}

int read_file(char *filename, file_t *file) {
	inode_table_t *inode_table;
	if (read_inodes_table(inode_table)) {
		int fileIndex = existing_file(inode_table, filename);
		if (fileIndex) {
			// Lecture du fichier
			fseek(virtual_disk_sos.storage, inode_table[fileIndex] -> first_byte, SEEK_SET);

			if (fread(file -> data[MAX_FILE_SIZE], file -> size, 1, virtual_disk_sos.storage) != 1) {
				fprintf(stderr, "Data file reading problem\n");
				return READ_FAILURE;
			}

			file -> size = inode_table[fileIndex] -> size;
		} else {
			printf(stderr, "Attempt to read inexisting file\n");
			return 0;
		}
	}
	else {
		fprintf(stderr, "Read inodes table problem\n");
		return 0;
	}
	return 1;
}

int delete_file(char *filename) {
	inode_table_t *inode_table;
	if (read_inodes_table(inode_table)) {
		int fileIndex = existing_file(inode_table, filename);
		if (fileIndex) {
			delete_inode(fileIndex);
		} else {
			fprintf(stderr, "Attempt to delete inexisting file\n");
			return 0;
		}
	}
	else {
		fprintf(stderr, "Read inodes table problem\n");
		return 0;
	}
	return 1;
}

int load_file_from_host(char *filename) {
	return 1;
}

int store_file_to_host(char *filename) {
	return 1;
}
