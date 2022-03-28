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
#include "layer5.h"
#include "user_interface.h"

#define _CRTDBG_MAP_ALLOC

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

int performCommand(command *cmd){
  if(cmd->argc == 0) return 1;
  if(cmd->argc == 1){
    if(strcmp(cmd->args[0],"quit") == 0) return 1;
    if(strcmp(cmd->args[0],"ls") == 0){
      //ls code
    }
  }
  if(strcmp(cmd->args[0],"user") == 0){
    if(cmd->argc == 1){
      clearConsole();
      dumpUserHelp();
    }else{
      if(cmd->argc == 2){
        if(strcmp(cmd->args[1],"list")){
          //List users
        }
      }else if(cmd->argc == 3) {
        if(strcmp(cmd->args[3],"create")){
          //create user
        }else if(strcmp(cmd->args[3],"remove")){
          //remove user
        }else if(strcmp(cmd->args[3],"login")){
          //login user
        }
      }

    }
  }
  return 0;
}

void freeCommand(command *cmd){
  for(int i = 0; i < cmd->argc; i++){
    free(cmd->args[i]);
  }
  free(cmd->args);
}

int console(){
  int exit = 0;
  char *login = "root";
  command *cmd;
  char *strCmd = malloc(sizeof(char)*75);

  clearConsole();
  dumpLogo();
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
