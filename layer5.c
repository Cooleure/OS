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

void dumpCommand(command *cmd){
  for(int i = 0; i < cmd->argc; i++){
    printf("%s\n", cmd->args[i]);
  }
}

command *createCommand(char *input){
    char splitChar = ' ';
    int inputLen = (int) strlen(input);

    int numberOfArguments = 1;

    for (int i = 0; i < inputLen; i++)
    {
        if (input[i] == splitChar)
            numberOfArguments++;
    }
    size_t totalSize = sizeof(numberOfArguments);
    command *cmd = malloc(sizeof(command));
    cmd->argc = 0;
    cmd->args = malloc(sizeof(char *) * numberOfArguments);

    int startOfArg = 0;
    int outputIndex = 0;

    for (int i = 0; i < inputLen; i++)
    {
        if (input[i] == splitChar)
        {
            int argLen = i - startOfArg;
            totalSize+=(argLen+1)*sizeof(char);
            cmd->args[outputIndex] = malloc(sizeof(char) * argLen + 1);
            strncpy(cmd->args[outputIndex], input + startOfArg, argLen);
            cmd->args[outputIndex][argLen] = 0;

            startOfArg = i + 1;
            outputIndex++;
        }
    }

    // append the last argument
    int argLen = inputLen - startOfArg;
    totalSize+=(argLen+1)*sizeof(char);
    cmd->args[outputIndex] = malloc(sizeof(char) * argLen + 1);
    strncpy(cmd->args[outputIndex], input + startOfArg, argLen);
    cmd->args[outputIndex][argLen] = 0;

    cmd->argc = numberOfArguments;
    return cmd;
}

void freeCommand(command *cmd){
  for(int i = 0; i < cmd->argc; i++){
    free(cmd->args[i]);
  }
  free(cmd->args);
}

void ls(int lOption){
  italic("Current files:\n");
  if(!lOption){
    printf("%-*s | Size (Bytes)\n", FILENAME_MAX_SIZE, "FileName");
    for(int i = 0; i<FILENAME_MAX_SIZE+15; i++){
      printf("―");
    }
    printf("\n");
    for(int i = 0; i<INODE_TABLE_SIZE; i++){
      if(strcmp(virtual_disk_sos.inodes[i].filename, "")){
        inode_t inode = virtual_disk_sos.inodes[i];
        printf("%-*s | %7d\n", FILENAME_MAX_SIZE, inode.filename, inode.size);
      }
    }
  }else{
    for(int i = 0; i<40; i++){
      printf("―");
    }
    printf("\n");
    for(int i = 0; i<INODE_TABLE_SIZE; i++){
      if(strcmp(virtual_disk_sos.inodes[i].filename, "")){
        inode_t inode = virtual_disk_sos.inodes[i];
        printf("| ◉ File name     ▶ %s\n", inode.filename);
        printf("| ◉ Size          ▶ %d (Bytes)\n", inode.size);
        printf("| ◉ Owner id      ▶ %d\n", inode.uid);
        printf("| ◉ Owner rights  ▶ ");
        printRights(inode.uright);
        printf("\n| ◉ User rights   ▶ ");
        printRights(inode.oright);
        printf("\n| ◉ Create date   ▶ %s", inode.ctimestamp);
        printf("| ◉ Last update   ▶ %s", inode.mtimestamp);
        printf("| ◉ Block count   ▶ %d\n", inode.nblock);
        printf("| ◉ First byte    ▶ %d\n", inode.first_byte);
        printf("| ◉ Inode id      ▶ %d\n", i);
        for(int i = 0; i<40; i++){
          printf("―");
        }
        printf("\n");
      }
    }
  }
}

void cr(char *fileName){
  init_inode(fileName, 0, virtual_disk_sos.super_block.first_free_byte);
}

void listusers(){
  printf("――――――――――――――――――――――――――――――――――――――――――\n");
  printf("| UID | Login                            |\n");
  printf("――――――――――――――――――――――――――――――――――――――――――\n");
  for(int i = 0; i < NB_USERS; i++){
    if(strcmp(virtual_disk_sos.users_table[i].login, ""))
      printf("| %-3d | %-32s |\n", i, virtual_disk_sos.users_table[i].login);
  }
  printf("――――――――――――――――――――――――――――――――――――――――――\n");
}

void edit(char* filename) {
  int index;
  file_t* file = malloc(sizeof(file_t));
  uint size;
  int i=0;
  int c;

  //Verification fichier existe
  if ((index = existing_file(filename)) != -1){

    //Verification droits
    if (virtual_disk_sos.inodes[index].oright == rw || virtual_disk_sos.inodes[index].uright == Rw){
      printf("Vous n'avez pas les droits pour editer ce fichier\n");
      return;
    }
    
    //Creation du fichier tmp
    FILE* desF = fopen("tmp.txt", "wt");
    if(desF == NULL){
      fprintf(stderr, "Erreur creation fichier tmp\n");
      return;
    }

    read_file(filename, file);
    
    //Ecriture du texte dans le fichier tmp
    if (file->size > 0){
      fprintf(desF, "%s", file->data);
      fclose(desF);
    }

    //modification du fichier
    system("nano tmp.txt");
    

    //Verification taille fichier
    FILE* f = fopen("tmp.txt", "rt");
    if (f == NULL){
      perror("tmp.txt");
    }

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    if(size > MAX_FILE_SIZE){
      fprintf(stderr, "Erreur, taille du fichier trop grande\n");
      remove("tmp.txt");
      return;
    }
    else{
      file->size = size;
      //fgets(file->data, file->size, f);
      fread(file->data, file->size, 1, f);
      /*do{
        c = fgetc(f);
        if (c!= EOF){
          printf("c = %c\n", c);
          file->data[i] = c;
        }
        else printf("OHHHHHHH\n");
        i++;
      }while(c != EOF);
      file->data[i] = '\0';*/
      file->data[file->size] = '\0';
      printf("affichage : %s\n", file->data);
      write_file(filename, file);
    }

    fclose(f);
    remove("tmp.txt");
  }
  else{
    printf("Fichier inexistant\n");
  }
}


void chmod1(char* filename){
  int mode;
  int index;
  if ((index = existing_file(filename)) != -1){
    printf("――――――――――――――――――――――――――――――――――――――――――\n");
    printf("Voulez vous rendre le fichier accesible en lecture (1) | en ecriture (2) |en lecture/ecriture (3)\n");
    scanf("%d", &mode);
    switch(mode){
      case 1:
        virtual_disk_sos.inodes[index].oright = Rw;
        break;
      case 2 :
        virtual_disk_sos.inodes[index].oright = rW;
        break;
      case 3 :
        virtual_disk_sos.inodes[index].oright = RW;
        break;
      default :
        break;
    }
    printf("Modification des droits termine\n");
    write_inodes_table();
  }
  else{
    printf("Fichier inexistant\n");
  }
}

int performCommand(command *cmd){
  if(cmd->argc == 0) return 1;
  if(!strcmp(cmd->args[0], "ls")){
    if(cmd->argc == 1){
      ls(0);
      return 0;
    }
    else if(cmd->argc == 2 && !strcmp(cmd->args[1], "-l")){
      ls(1);
      return 0;
    }
  }
  else if(!strcmp(cmd->args[0], "cr")){
    if(cmd->argc == 2){
      cr(cmd->args[1]);
      return 0;
    }
  }
  else if(!strcmp(cmd->args[0], "listusers")){
    listusers();
    return 0;
  }
  else if(!strcmp(cmd->args[0], "help")){
    dumpHelp();
    return 0;
  }
  else if(cmd->argc == 2 && !strcmp(cmd->args[0], "edit")){
    edit(cmd->args[1]);
    return 0;
  }
  else if (cmd->argc == 2 && !strcmp(cmd->args[0], "chmod")){
    chmod1(cmd->args[1]);
    return 0;
  }
  else if(!strcmp(cmd->args[0], "quit")){
    return 1;
  }else{
    if(strcmp(cmd->args[0], "")){
      printf("Unkown command, type 'help' to display help context.\n");
    }
  }
  commandUsage(cmd->args[0]);
  return 0;
}

int login(){
  int tries = 0;
  char password[PASSWORD_SIZE+1];
  char hashRes[SHA256_BLOCK_SIZE*2 + 1];
  clearConsole();
  dumpLogo();
  printf("Login to root: ");
  while(tries < 3){
    fgets(password, PASSWORD_SIZE, stdin);
    password[strcspn(password, "\n")] = 0;
    sha256ofString((BYTE *)password,hashRes);
    if(strcmp(hashRes, virtual_disk_sos.users_table[ROOT_UID].passwd)==0){
      printf("Login success !\n");
      return 1;
    }
    tries++;
    if(tries<3)printf("Wrong password, try again: ");
  }
  printf("Max tries reached... Aborting system launch\n");
  return 0;
}

int console(){
  int exit = 0;
  char *login = "root";
  command *cmd;
  char *strCmd = malloc(sizeof(char)*75);

  dumpHelp();
  while(!exit){
    printf("%s ➣ ",login);
    fgets(strCmd, 75, stdin);
    strCmd[strcspn(strCmd, "\n")] = 0; //remove \n at end

    //Construct command
    cmd = createCommand(strCmd);
    exit = performCommand(cmd);
    freeCommand(cmd);
    free(cmd);
  }
  free(strCmd);
  return 0;
}
