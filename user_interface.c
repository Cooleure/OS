/**
 * @file user_interface.c
 * @author Florian Dion - Nicolas Dejean - Nathan Fleury
 * @brief
 * @version 0.1
 * @date 2022-03-27
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "user_interface.h"
#include "struct.h"

//https://askubuntu.com/questions/528928/how-to-do-underline-bold-italic-strikethrough-color-background-and-size-i
void italic(char* msg){
  char italicMsg[strlen(msg)+9];
  strcpy(italicMsg,"\e[3m");
  strcat(italicMsg, msg);
  strcat(italicMsg, "\e[0m");
  printf("%s", italicMsg);
}

void dumpLogo(){
  FILE *logo = fopen("res/logo.txt","r");
  if(logo == NULL) return;

  char c = fgetc(logo);
  while (c != EOF) {
    printf ("%c", c);
    c = fgetc(logo);
  }
  fclose(logo);
  return;
}

void clearConsole(){
  printf("\e[1;1H\e[2J");
}

void dumpHelp(){
  printf("╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍\n");
  printf("》Help Menu:《\n");
  printf("\n");
  italic("[] -> optional   <> -> required\n");
  printf(" ⌨ quit                             ➪ poweroff OS\n");
  printf(" ⌨ ls [-l]                          ➪ list files\n");
  printf(" ⌨ cat <file_name>                  ➪ dump file content\n");
  printf(" ⌨ rm <file_name>                   ➪ remove a file\n");
  printf(" ⌨ cr <file_name>                   ➪ create a file\n");
  printf(" ⌨ edit <file_name>                 ➪ edit a file\n");
  printf(" ⌨ load <file_name>                 ➪ load file from host\n");
  printf(" ⌨ store <file_name>                ➪ store file to host\n");
  printf(" ⌨ chown <file_name> <user_name>    ➪ change file owner\n");
  printf(" ⌨ chmod <file_name>                ➪ change file rights\n");
  printf(" ⌨ listusers                        ➪ list users\n");
  printf(" ⌨ adduser <user_name>              ➪ create a new user\n");
  printf(" ⌨ rmuser <user_name>               ➪ remove user\n");
  printf("╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍\n");
}

void commandUsage(char* cmd){
  if(!strcmp(cmd, "ls")) printf("Usage: ls [-l]\n");
}

void printRights(unsigned int r){
  switch (r) {
    case rw: printf("rw"); break;
    case rW: printf("rW"); break;
    case Rw: printf("Rw"); break;
    case RW: printf("RW"); break;
    default: break;
  }
}
