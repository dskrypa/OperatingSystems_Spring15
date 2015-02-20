/**
 * Douglas Skrypa
 * Assignment 1: Shell
 * 2015.02.19
 */

#ifndef SHELL_H
#define SHELL_H

void processInput(string input);
void runCommand(string* args);
char* getInput();
List tokenize(char* input);

#endif
