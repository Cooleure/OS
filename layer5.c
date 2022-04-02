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
#include "layer1.h"
#include "layer3.h"
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
    for(int i = 0; i<virtual_disk_sos.super_block.number_of_files; i++){
      inode_t inode = virtual_disk_sos.inodes[i];
      printf("%-*s | %7d\n", FILENAME_MAX_SIZE, inode.filename, inode.size);
    }
  }else{
    for(int i = 0; i<40; i++){
      printf("―");
    }
    printf("\n");
    for(int i = 0; i<virtual_disk_sos.super_block.number_of_files; i++){
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
    }
    for(int i = 0; i<40; i++){
      printf("―");
    }
    printf("\n");
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
  if(!strcmp(cmd->args[0], "quit")){
    return 1;
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
