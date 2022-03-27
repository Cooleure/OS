/**
 * @file layer5.h
 * @author Florian Dion - Nicolas Dejean - Nathan Fleury
 * @brief
 * @version 0.1
 * @date 2022-03-27
 */
#ifndef LAYER5_H
#define LAYER5_H

typedef struct s_command{
  char **args;
  int argc;
} command;

int console();


#endif
