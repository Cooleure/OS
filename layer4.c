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
#include <sys/types.h>
#include <sys/stat.h>
#include "layer4.h"
#include "struct.h"

int existing_file(char *filename) {
	for (int i = 0; i < INODE_TABLE_SIZE; i++)
		if (strcmp(virtual_disk_sos.inodes[i].filename, filename) == 0)
			return i; // Exists
	return -1; // Does not exist
}

int write_file(char *filename, file_t *file) {
	if (read_inodes_table()) {
		int fileIndex = existing_file(filename);
		if (fileIndex >= 0) {
			if (file -> size <= virtual_disk_sos.inodes[fileIndex].size) {
				// Modification de l'inode
				virtual_disk_sos.inodes[fileIndex].size = file -> size;
				virtual_disk_sos.inodes[fileIndex].nblock = compute_nblock(file -> size);

				time_t timing = time(NULL);
  				strcpy(virtual_disk_sos.inodes[fileIndex].mtimestamp, ctime(&timing));

				//Ecriture de cette table mise à jour
				if (!write_inodes_table()) {
					fprintf(stderr, "Update inodes table problem\n");
					return 0;
				}
			} else {
				delete_inode(fileIndex);
				if (!init_inode(filename, file -> size, virtual_disk_sos.super_block.first_free_byte)) {
					fprintf(stderr, "Create inodes table problem\n");
					return 0;
				}
			}
		}
		else {
			if (!init_inode(filename, file -> size, virtual_disk_sos.super_block.first_free_byte)) {
				fprintf(stderr, "Create inodes table problem\n");
				return 0;
			}
		}
	} else {
		fprintf(stderr, "Read inodes table problem\n");
		return 0;
	}

	return 1;
}

int read_file(char *filename, file_t *file) {
	if (read_inodes_table()) {
		int fileIndex = existing_file(filename);
		
		if (fileIndex >= 0) {
			// Lecture du fichier
			fseek(virtual_disk_sos.storage, virtual_disk_sos.inodes[fileIndex].first_byte, SEEK_SET);
			printf("test %d\n", virtual_disk_sos.inodes[fileIndex].size);
			file->size = 0;
			printf("OHHHHHHHHHHHHHHHHHH\n");
			
			
			if (fread(file -> data[MAX_FILE_SIZE], file->size, 1, virtual_disk_sos.storage) != 1) {
				fprintf(stderr, "Data file reading problem\n");
				return READ_FAILURE;
			}
		
		} else {
			fprintf(stderr, "Attempt to read inexisting file\n");
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
	if (read_inodes_table()) {
		int fileIndex = existing_file(filename);
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
	// Ouverture du fichier de host
	FILE *idFile;
	idFile = fopen(filename, "r");
	if (idFile == NULL)
	{
		perror(filename);
		return 0;
	}

	// Création du fichier de OsFromScratch
	file_t file;

	struct stat infos;
	if (stat(filename, &infos) == -1) {
		fprintf(stderr, "Read stats file problem\n");
		return 0;
	}

	file.size = infos.st_size;
	if (file.size > MAX_FILE_SIZE) file.size = MAX_FILE_SIZE;

	if (fread(file.data, file.size, 1, idFile) != 1) {
		fprintf(stderr, "Data file reading problem\n");
		return 0;
	}

	// Ecriture sur le disque
	if (!write_file(filename, &file)) {
		fprintf(stderr, "Data file writing problem\n");
		return 0;
	}

	return 1;
}

int store_file_to_host(char *filename) {
	FILE *idFile = fopen(filename, "w");

	file_t file;
	if (read_file(filename, &file) == 0) {
		fprintf(stderr, "File reading on system problem\n");
		return 0;
	}

	if (fwrite(file.data, 1, file.size, idFile) == 0) {
		fprintf(stderr, "Data file writing problem\n");
		return 0;
	}

	return 1;
}
