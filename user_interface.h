/**
 * @file user_interface.h
 * @author Florian Dion - Nicolas Dejean - Nathan Fleury
 * @brief
 * @version 0.1
 * @date 2022-03-27
 */
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

/**
 * @brief Make strings italic using Regex
 */
void italic(char* msg);

/**
 * @brief Clears the console using Regex
 */
void clearConsole();

/**
 * @brief Print the OS Logo (storeed at res/logo.txt)
 */
void dumpLogo();

/**
 * @brief Print help menu
 */
void dumpHelp();

/**
 * @brief Print command usage
 */
void commandUsage(char* cmd);

#endif
