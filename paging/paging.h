/**
 * Douglas Skrypa
 * Version: 2015.04.14
 * Homework 4: Page Replacement Algorithms Simulation
 */

#ifndef PAGING_H
#define PAGING_H

void printState(int page, int* frame, int framec, bool fault, int framesFilled);
int use_fifo(int* refStream, int refc, int framec);
int use_lru(int* refStream, int refc, int framec);
int use_opt(int* refStream, int refc, int framec);
List tokenize(String input);
String getInput();

#endif
