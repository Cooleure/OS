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

//https://askubuntu.com/questions/528928/how-to-do-underline-bold-italic-strikethrough-color-background-and-size-i
void italic(char* msg){
  char italicMsg[strlen(msg)+8];
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
  printf("╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍\n");
  printf("》Menu d'aide:《\n");
  printf("\n");
  printf(" ⌨ stop     ➪ poweroff OS\n");
  printf(" ⌨ ls       ➪ list files\n");
  printf(" ⌨ user     ➪ user help menu\n");
  printf("╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍\n");
}

void dumpUserHelp(){
  printf("╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍\n");
  printf("》Menu d'aide d'utilisateurs:《\n");
  printf("\n");
  italic("Some password might be asked with thoses commands\n");
  printf(" ⌨ user list             ➪ list all users\n");
  printf(" ⌨ user create <login>   ➪ create user\n");
  printf(" ⌨ user remove <login>   ➪ remove a user\n");
  printf(" ⌨ user login <login>    ➪ login to a user\n");
  printf("╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍╍\n");
}
