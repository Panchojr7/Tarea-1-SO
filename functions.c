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

void tablero(){
    /* Declaraciones*/
    int c,f;
    char filas[] = FILAS;
    mkdir("J1",0777);
    mkdir("J2",0777);

    /*Creacion de directorios */
    for (f = 0; f < 5; ++f){ //Filas
        for (c= 1; c < 6; ++c){ //Columnas
            char j1[25], j2[25];
            snprintf(j1, sizeof(j1), "./J1/%c_%d", filas[f],c);
            snprintf(j2, sizeof(j2), "./J2/%c_%d", filas[f],c);
            mkdir(j1, 0777);
            mkdir(j2, 0777);
        }
    }
}   

int letra(char a){
    if(a=='A'){
        return 0;
    }
    else if (a=='B'){
        return 1;
    }
    else if (a=='C'){
        return 2;
    }
    else if (a=='D'){
        return 3;
    }
    else if (a=='E'){
        return 4;
    }
}

int casteo(char c){
    int i= (int)c - '0';
    return i-1;
}

void instrucciones(int j){
    printf(RED  "Bienvenido Capitan %d\n", j);
    printf(YELLOW   "Las coordenadas del juego se componen de una Columna (letra mayuscula) seguida de una Fila (numero), ej B1\n");
    printf("Filas: 1 - 2 - 3 - 4 - 5\n");
    printf("Columnas: A - B - C - D - E\n" RESET);
}

int comprobar(char coord1, char coord2){
    char columnas[] = FILAS;
    int i;
    int l=0;
    int n=0;
    for (i = 0; i < 5; ++i){
        if (coord1 == columnas[i]){
            l=1;
        }
        if (casteo(coord2)==i){
            n=1;
        }
    }
    if (n==0 || l==0){
        return 0;
    }
    else{
        return 1;
    }
}

int existe(int j, char coord1, char coord2){
    struct stat st = {0};
    if (j == 1){
        char ruta1[19];
        snprintf(ruta1, sizeof(ruta1), "./J1/%c_%c/barco.txt", coord1, coord2);
        if (stat(ruta1, &st) == -1){
            return 0;
        }
        else{
            return 1;
        }
    }
    else if (j == 2){
        char ruta2[19];
        snprintf(ruta2, sizeof(ruta2), "./J2/%c_%c/barco.txt", coord1, coord2);
        if (stat(ruta2, &st) == -1){
            return 0;
        }
        else{
            return 1;
        }
    }
}

void crearbarco(int j, char coord1, char coord2){
    if (j == 1){
        char ruta1[19];
        snprintf(ruta1, sizeof(ruta1), "./J1/%c_%c/barco.txt", coord1, coord2);
        FILE *fp;
        fp=fopen(ruta1,"w");
        fclose(fp);
    }
    else if (j == 2){
        char ruta2[19];
        snprintf(ruta2, sizeof(ruta2), "./J2/%c_%c/barco.txt", coord1, coord2);
        FILE *fp;
        fp=fopen(ruta2,"w");
        fclose(fp);
    }
}

void panel(int w, int arr[5][5]){
    int i;
    if (w==1){
        printf(YELLOW "REPRESENTACIÓN DE VALORES\n0:Casilla no atacada\n1:Casilla atacada sin éxito\n3:Casilla atacada con éxito\n");
        printf(GREEN    "|				Panel de Combate Jugador 1					|\n");
        printf("|	N°	|	A	|	B	|	C	|	D	|	E	|\n");
        for (i = 0; i < 5; ++i){
            printf("|	%d	|	%d	|	%d	|	%d	|	%d	|	%d	|\n", i+1, arr[0][i], arr[1][i], arr[2][i], arr[3][i], arr[4][i]);
                
        }
        printf("\n" RESET);
    }
    else if (w==2){
        printf(YELLOW "REPRESENTACIÓN DE VALORES\n0:Casilla no atacada\n1:Casilla atacada sin éxito\n3:Casilla atacada con éxito\n");
        printf(CYAN "|				Panel de Combate Jugador 2					|\n");
        printf("|	N°	|	A	|	B	|	C	|	D	|	E	|\n");
        for (i = 0; i < 5; ++i){
            printf("|	%d	|	%d	|	%d	|	%d	|	%d	|	%d	|\n", i+1, arr[0][i], arr[1][i], arr[2][i], arr[3][i], arr[4][i]);
        }
        printf("\n" RESET);
    }
}