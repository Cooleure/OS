#include <stdio.h>
#include <string.h>
#include "layer1.h"
#include "layer2.h"
#include "user_interface.h"

typedef struct s_command{
  char **args;
  int argc;
} command;

void dumpCommand(command *cmd){
  for(int i = 0; i < cmd->argc; i++){
    printf("%s\n", cmd->args[i]);
  }
}

command *createCommand(char *strCmd){
  command *cmd = malloc(strlen(strCmd)+1);

  if(strlen(strCmd) == 0){
    cmd->argc = 0;
    return cmd;
  }
  int argc = 0;
  int iStr = 0;
  char c;
  while((c = strCmd[iStr++]) != '\0'){
    //New argument
    int i = 0;
    char arg[50];
    //Construct the argument
    while(c != ' ' && c != '\0'){
      arg[i++] = c;
      c = strCmd[iStr++];
    }
    if(i>0){
      arg[i] = '\0';
      strcpy(cmd->args[argc], arg);
      printf("added word %s at %d\n", cmd->args[argc], argc);
      argc++;
    }
  }
  cmd->argc = argc;
  return cmd;
}

int performCommand(command *cmd){
  if(cmd->argc == 0) return 1;
  if(cmd->argc == 1){
    if(strcmp(cmd->args[0],"stop") == 0) return 1;
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

int main(){
  int exit = 0;
  char *login = "root";
  init_disk_sos("Tests");
  //ecriture d'une inode dans la table d'inode et positionnement du premier byte à first_free_byte (pos)
  init_inode("fichier1", 1000, virtual_disk_sos.super_block.first_free_byte);
  clearConsole();
  dumpLogo();
  dumpHelp();
  while(!exit){
    char *strCmd = malloc(sizeof(char)*75);
    printf("%s ➣ ",login);
    fgets(strCmd, 75, stdin);
    strCmd[strcspn(strCmd, "\n")] = 0; //remove \n at end

    //Construct command
    command *cmd = createCommand(strCmd);
    dumpCommand(cmd);
    exit = performCommand(cmd);
    free(cmd);
    free(strCmd);
  }
}
