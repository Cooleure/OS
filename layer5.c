/**
 * @file layer5.c
 * @author Florian Dion - Nicolas Dejean - Nathan Fleury
 * @brief
 * @version 0.1
 * @date 2022-03-27
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "layer1.h"
#include "layer3.h"
#include "layer4.h"
#include "layer5.h"
#include "user_interface.h"
#include "struct.h"
#include "sha256_utils.h"

session_t user;

void dumpCommand(command *cmd) {
	for (int i = 0; i < cmd->argc; i++) {
		printf("%s\n", cmd->args[i]);
	}
}

command *createCommand(char *input) {
	char splitChar = ' ';
	int inputLen = (int) strlen(input);

	int numberOfArguments = 1;

	for (int i = 0; i < inputLen; i++) {
		if (input[i] == splitChar)
			numberOfArguments++;
	}

	size_t totalSize = sizeof(numberOfArguments);
	command *cmd = malloc(sizeof(command));
	cmd->argc = 0;
	cmd->args = malloc(sizeof(char *) * numberOfArguments);

	int startOfArg = 0;
	int outputIndex = 0;

	for (int i = 0; i < inputLen; i++) {
		if (input[i] == splitChar) {
			int argLen = i - startOfArg;
			totalSize += (argLen + 1) * sizeof(char);
			cmd->args[outputIndex] = malloc(sizeof(char) * argLen + 1);
			strncpy(cmd->args[outputIndex], input + startOfArg, argLen);
			cmd->args[outputIndex][argLen] = 0;

			startOfArg = i + 1;
			outputIndex++;
		}
	}

	// Append the last argument
	int argLen = inputLen - startOfArg;
	totalSize += (argLen + 1) * sizeof(char);
	cmd->args[outputIndex] = malloc(sizeof(char) * argLen + 1);
	strncpy(cmd->args[outputIndex], input + startOfArg, argLen);
	cmd->args[outputIndex][argLen] = 0;

	cmd->argc = numberOfArguments;
	return cmd;
}

void freeCommand(command *cmd) {
	for (int i = 0; i < cmd->argc; i++) {
		free(cmd->args[i]);
	}
	free(cmd->args);
}

void ls(int lOption) {
	italic("Current files :\n");
	if (!lOption) {
		printf("%-*s | Size (Bytes)\n", FILENAME_MAX_SIZE, "FileName");
		for (int i = 0; i < FILENAME_MAX_SIZE + 15; i++) {
			printf("―");
		}

		printf("\n");
		for (int i = 0; i < INODE_TABLE_SIZE; i++) {
			if (strcmp(virtual_disk_sos.inodes[i].filename, "")) {
				inode_t inode = virtual_disk_sos.inodes[i];
				printf("%-*s | %7d\n", FILENAME_MAX_SIZE, inode.filename, inode.size);
			}
		}
	} else {
		for (int i = 0; i < 40; i++) {
			printf("―");
		}

		printf("\n");
		for (int i = 0; i < INODE_TABLE_SIZE; i++) {
			if (strcmp(virtual_disk_sos.inodes[i].filename, "")) {
				inode_t inode = virtual_disk_sos.inodes[i];
				printf("| ◉ File name       ▶ %s\n", inode.filename);
				printf("| ◉ Size            ▶ %d (Bytes)\n", inode.size);
				printf("| ◉ Owner id        ▶ %d\n", inode.uid);
				printf("| ◉ Owner rights    ▶ ");
				printRights(inode.uright);
				printf("\n| ◉ User rights     ▶ ");
				printRights(inode.oright);
				printf("\n| ◉ Create date     ▶ %s\n", inode.ctimestamp);
				printf("| ◉ Last update     ▶ %s\n", inode.mtimestamp);
				printf("| ◉ Block count     ▶ %d\n", inode.nblock);
				printf("| ◉ First byte      ▶ %d\n", inode.first_byte);
				printf("| ◉ Inode id        ▶ %d\n", i);

				for(int i = 0; i<40; i++) {
					printf("―");
				}
				printf("\n");
			}
		}
	}
}

void cr(char *fileName) {
	if (existing_file(fileName) != -1) {
		printf("Le fichier existe déjà !\n");
		return;
	}

	init_inode(fileName, 0, virtual_disk_sos.super_block.first_free_byte);
	int i = existing_file(fileName);
	virtual_disk_sos.inodes[i].uright = RW;
	virtual_disk_sos.inodes[i].uid = user.userid;
	write_inodes_table();
}

void listusers() {
	printf("――――――――――――――――――――――――――――――――――――――――――\n");
	printf("| UID | Login                            |\n");
	printf("――――――――――――――――――――――――――――――――――――――――――\n");
	for (int i = 0; i < NB_USERS; i++) {
		if (strcmp(virtual_disk_sos.users_table[i].login, "")) {
			printf("| %-3d | %-32s |\n", i, virtual_disk_sos.users_table[i].login);
		}
	}
	printf("――――――――――――――――――――――――――――――――――――――――――\n");
}

void edit(char *filename) {
	int index;
	file_t *file = malloc(sizeof(file_t));
	uint size;

	if ((index = existing_file(filename)) != -1) { // Vérification existence fichier
		// Vérification droits
		if (virtual_disk_sos.inodes[index].uid == user.userid) {
			if (user.userid != 0 && (virtual_disk_sos.inodes[index].uright == Rw || virtual_disk_sos.inodes[index].uright == rw)) {
				// Propriétaire
				printf("Vous ne possédez pas les droits nécessaires pour éditer ce fichier\n");
				return;
			}
		} else if (user.userid != 0 && (virtual_disk_sos.inodes[index].oright == rw || virtual_disk_sos.inodes[index].oright == Rw)) {
			// Autres
			printf("Vous ne possédez pas les droits nécessaires pour éditer ce fichier\n");
			return;
		}

		// Création du fichier tmp
		FILE* desF = fopen("tmp.txt", "wt");
		if (desF == NULL) {
			fprintf(stderr, "Erreur création fichier tmp\n");
			return;
		}

		read_file(filename, file);

		// Ecriture du texte dans le fichier tmp
		if (file->size > 0) {
			fprintf(desF, "%s", file->data);
			fclose(desF);
		}

		// Modification du fichier
		if (system("nano tmp.txt") != 0) {
			fprintf(stderr, "Nano editing error\n");
			return;
		}

		// Ouverture fichier tmp
		FILE* f = fopen("tmp.txt", "rt");
		if (f == NULL) {
			perror("tmp.txt");
			return;
		}

		// Vérification taille nouveau fichier
		fseek(f, 0, SEEK_END);
		size = ftell(f);
		if (size > MAX_FILE_SIZE) {
			fprintf(stderr, "Erreur, taille du fichier trop grande\n");
			remove("tmp.txt");
			return;
		}

		// Lecture nouveau fichier
		fseek(f, 0, SEEK_SET);
		int i = 0;
		char ch;
		do {
			ch = fgetc(f);
			file -> data[i] = (uchar) ch;
			i++;
		} while (ch != EOF);

		file->data[size] = '\0';
		file->size = size;
		write_file(filename, file);

		fclose(f);
		remove("tmp.txt");
	} else {
		printf("Fichier inexistant\n");
	}

	free(file);
}

void cat(char *filename) {
	file_t *file = malloc(sizeof(file_t));
	int index;
	if ((index = existing_file(filename)) != -1) {
		// Vérification droits
		if (virtual_disk_sos.inodes[index].uid == user.userid) {
			if (user.userid != 0 && (virtual_disk_sos.inodes[index].uright == rW || virtual_disk_sos.inodes[index].uright == rw)) {
				// Propriétaire
				printf("Vous ne possédez pas les droits nécessaires pour lire ce fichier\n");
				return;
			}
		} else if ((user.userid != 0) && (virtual_disk_sos.inodes[index].oright == rw || virtual_disk_sos.inodes[index].oright == rW)){
			// Autres
			printf("Vous ne possédez pas les droits nécessaires pour lire ce fichier\n");
			return;
		}

		read_file(filename, file);
		if (file->size > 0){
			printf("――――――――――――――――――――――――――――――――――――――――――\n");
			printf("――――――――――――――――――%s―――――――――――――――――――\n", filename);
			printf("%s\n", file->data);
			printf("――――――――――――――――――――――――――――――――――――――――――\n");
		}
	} else {
		printf("Fichier inexistant");
	}

	free(file);
}

void chmod1(char* filename) {
	int mode;
	int index;
	if ((index = existing_file(filename)) != -1) {
		// Vérification droits
		if (virtual_disk_sos.inodes[index].uid != user.userid) {
			// Propriétaire
			printf("Vous n'avez pas la permission de modifier les droits de ce fichier\n");
			return;
		}

		printf("――――――――――――――――――――――――――――――――――――――――――\n");
		printf("Voulez vous rendre le fichier accesible en : Lecture (1) | Ecriture (2) | Lecture/Ecriture (3)\n");
		if (scanf("%d", &mode) == EOF) {
			fprintf(stderr, "chmod1 input reading error\n");
			return;
		}

		switch(mode) {
			case 1:
				virtual_disk_sos.inodes[index].oright = Rw;
				break;

			case 2:
				virtual_disk_sos.inodes[index].oright = rW;
				break;

			case 3:
				virtual_disk_sos.inodes[index].oright = RW;
				break;

			default:
				break;
		}
		printf("Modification des droits terminé\n");
		write_inodes_table();
	}
	else{
		printf("Fichier inexistant\n");
	}
}

void rm(char *filename) {
	int index;
	char verif;
	if ((index = existing_file(filename)) != -1) {
		// Vérification droits
		if (user.userid != 0 && virtual_disk_sos.inodes[index].uid != user.userid){
			// Propriétaire
			printf("Vous ne possédez pas les droits nécessaires pour supprimer ce fichier\n");
			return;
		}

		printf("Voulez vous supprimer le fichier : %s ?\n", filename);
		printf("y : yes | n : no\n");
		if (scanf("%c", &verif) == EOF) {
			fprintf(stderr, "Chmod1 input reading error\n");
			return;
		}

		if (verif == 'y') {
			delete_file(filename);
			printf("Fichier supprimé\n");
		}
	}
}

void load(char *filename) {
	int verif = load_file_from_host(filename);
	if (verif == 1){
		printf("Importation terminée avec succès\n");
	}
}

void store(char *filename) {
	int verif = store_file_to_host(filename);
	if (verif == 1){
		printf("Exportation terminée avec succès\n");
	}
}

void chown1(char *filename, char *login) {
	int index;
	int uid = 0;
	while ((strcmp(login, virtual_disk_sos.users_table[uid].login)) && uid < virtual_disk_sos.super_block.number_of_users) {
		uid++;
	}

	if ((index = existing_file(filename)) != -1) {
		// Vérification droits
		if (virtual_disk_sos.inodes[index].uid != user.userid){
			// Propriétaire
			printf("Vous n'avez pas la permission de modifier les droits de ce fichier\n");
			return;
		}

		virtual_disk_sos.inodes[index].uid = uid;
		printf("Changement de propriétaire terminé\n");
	}
}

void adduser() {
	if (user.userid != 0) {
		printf("Vous ne possédez pas les droits nécessaires pour ajouter un utilisateur\n");
		return;
	}

	char login[FILENAME_MAX_SIZE];

	if (virtual_disk_sos.super_block.number_of_users < NB_USERS) {
		printf("Login : ");
		if (scanf("%s", login) == EOF) {
			fprintf(stderr, "Adduser login reading error\n");
			return;
		}
		printf("\n");

		// Initialisation du mot de passe
		char hashRes[SHA256_BLOCK_SIZE * 2 + 1];
		printf("Définir un mot de passe utilisateur : ");

		char mdp[PASSWORD_SIZE + 1];
		if (scanf("%s", mdp) == EOF) {
			fprintf(stderr, "Chmod1 input reading error\n");
			return;
		}

		sha256ofString((BYTE *) mdp, hashRes);
		new_user(login, hashRes);
	}
}

int user_id(char *login) {
	// Retourne l'id de l'utilisateur ou -1 s'il n'existe pas
	int i = 0;
	while (i < virtual_disk_sos.super_block.number_of_users) {
		if (!strcmp(login, virtual_disk_sos.users_table[i].login)) {
			return i;
		}
		i++;
	}

	return -1;
}

void modify_owner_file(int id) {
	// Id est l'uid de l'utilisateur supprimé
	for (int i = 0; i < virtual_disk_sos.super_block.number_of_files; i++) {
		if (virtual_disk_sos.inodes[i].uid == id) {
			// Si fichier appartient à l'utilisateur supprimé on passe les droits à root
			virtual_disk_sos.inodes[i].uid = 0;
		}
	}

	write_inodes_table();
}

void rmuser(char *login) {
	if (user.userid != 0) {
		printf("Vous ne possédez pas les droits nécessaires pour supprimer un utilisateur\n");
		return;
	}

	int id = user_id(login);
	int verif = delete_user(login);
	if (verif == 0) {
		modify_owner_file(id);
		printf("Utilisateur supprimé\n");
	} else {
		printf("Utilisateur inconnu\n");
	}
}

int changeUser(char *login) {
	int i = 0;
	char passwd[PASSWORD_SIZE + 1];
	char hashRes[SHA256_BLOCK_SIZE * 2 + 1];
	while (i < virtual_disk_sos.super_block.number_of_users) {
		if (!strcmp(login, virtual_disk_sos.users_table[i].login)) {
			printf("Mot de passe : ");
			if (scanf("%s", passwd) == EOF) {
				fprintf(stderr, "changeUser input reading error\n");
				return 0;
			}

			sha256ofString((BYTE *)passwd,hashRes);

			// Vérification du mot de passe
			if (!strcmp(hashRes, virtual_disk_sos.users_table[i].passwd)) {
				user.userid = i;
				printf("Changement d'utilisateur terminé\n");
				return 1;
			} else {
				printf("Mot de passe incorrect\n");
			}
			break;
		}
		i++;
	}

	return 0;
}

int performCommand(command *cmd) {
	if (cmd->argc == 0) return 1;

	if (!strcmp(cmd->args[0], "ls")) {
		if (cmd->argc == 1) {
			ls(0);
			return 0;
		} else if (cmd->argc == 2 && !strcmp(cmd->args[1], "-l")) {
			ls(1);
			return 0;
		}
	}else if (!strcmp(cmd->args[0], "cr")) {
		if (cmd->argc == 2) {
			cr(cmd->args[1]);
			return 0;
		}
	} else if (!strcmp(cmd->args[0], "listusers")) {
		listusers();
		return 0;
	} else if (!strcmp(cmd->args[0], "help")) {
		dumpHelp();
		return 0;
	} else if (!strcmp(cmd->args[0], "edit")) {
		if (cmd->argc == 2) {
			edit(cmd->args[1]);
			return 0;
		}
	} else if (!strcmp(cmd->args[0], "cat")) {
		if (cmd->argc == 2) {
			cat(cmd->args[1]);
			return 0;
		}
	} else if (!strcmp(cmd->args[0], "rm")) {
		if (cmd->argc == 2) {
			rm(cmd->args[1]);
			return 0;
		}
	} else if (!strcmp(cmd->args[0], "adduser")) {
		if (cmd->argc == 1) {
			adduser();
			return 0;
		}
	} else if (!strcmp(cmd->args[0], "rmuser")) {
		if (cmd->argc == 2) {
			rmuser(cmd->args[1]);
			return 0;
		}
	} else if (!strcmp(cmd->args[0], "load")) {
		if (cmd->argc == 2) {
			load(cmd->args[1]);
			return 0;
		}
	} else if (!strcmp(cmd->args[0], "store")) {
		if (cmd->argc == 2) {
			store(cmd->args[1]);
			return 0;
		}
	} else if (!strcmp(cmd->args[0], "chown")) {
		if (cmd->argc == 3) {
			chown1(cmd->args[1], cmd->args[2]);
			return 0;
		}
	} else if (!strcmp(cmd->args[0], "chmod")) {
		if (cmd->argc == 2) {
			chmod1(cmd->args[1]);
			return 0;
		}
	} else if (!strcmp(cmd->args[0], "quit")) {
		return 1;
	} else {
		if (strcmp(cmd->args[0], "")) {
			printf("Unkown command, type 'help' to display help context.\n");
		}
	}

	commandUsage(cmd->args[0]);

	return 0;
}

int login() {
	int tries = 0;
	char password[PASSWORD_SIZE + 1];
	char hashRes[SHA256_BLOCK_SIZE * 2 + 1];
	clearConsole();
	dumpLogo();
	printf("Login to root: ");
	while(tries < 3) {
		if (fgets(password, PASSWORD_SIZE, stdin) == NULL) {
			fprintf(stderr, "Reading error loging\n");
			return 0;
		}

		password[strcspn(password, "\n")] = 0;
		sha256ofString((BYTE *) password, hashRes);
		if (strcmp(hashRes, virtual_disk_sos.users_table[ROOT_UID].passwd)==0) {
			printf("Login success !\n");
			return 1;
		}

		tries++;
		if (tries < 3) printf("Wrong password, try again: ");
	}

	printf("Max tries reached... Aborting system launch\n");

	return 0;
}

int console() {
	int exit = 0;
	char login[FILENAME_MAX_SIZE] = "root";
	user.userid = 0;
	int i = 0;
	command *cmd;
	char *strCmd = malloc(sizeof(char) * 75);

	dumpHelp();
	while(!exit) {
		printf("%s ➣ ", login);
		if (fgets(strCmd, 75, stdin) == NULL) {
			fprintf(stderr, "Reading error console\n");
			return 1;
		}

		strCmd[strcspn(strCmd, "\n")] = 0; // Remove \n at end

		// Construct command
		cmd = createCommand(strCmd);
		if (cmd->argc == 2 && !strcmp(cmd->args[0], "chuser")) {
			i = changeUser(cmd->args[1]);
			if (i) strcpy(login, cmd->args[1]);
		} else {
			exit = performCommand(cmd);
		}

		freeCommand(cmd);
		free(cmd);
	}

	free(strCmd);

	return 0;
}
