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

/**
 * @brief Perform the OS Console
 *
 * @return 0 if no problems were encountered, 1 otherwise
 */
int console();

/**
 * @brief Ask for the password for the connection
 *
 * @note The comparison of passwords is done by a hashing algorithm
 * @return 1 if no problems were encountered, 0 otherwise
 */
int login();


#endif
