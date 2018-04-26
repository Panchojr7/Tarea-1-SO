#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "functions.h"

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define FILAS "ABCDE"


void tablero();
int letra(char a);
int casteo(char c);
void instrucciones(int j);
int comprobar(char coord1, char coord2);
int existe(int j, char coord1, char coord2);
void crearbarco(int j, char coord1, char coord2);
void panel(int w, int arr[5][5]);


#endif // FUNCTIONS_H