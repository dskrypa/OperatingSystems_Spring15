/**
 * Douglas Skrypa
 * Version: 2015.03.03
 * Homework 2: Pizza Parlor
 */

#ifndef PIZZA_H
#define PIZZA_H

void* student(void* threadid);
void* pizzaParlor();
int get_slice();
void placeOrder();
bool hasOrder();
bool stillActive();
void make_pizza();
void errexit(char* msg);

#endif
