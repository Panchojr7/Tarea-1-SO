#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FILAS "ABCDE"


/*Estructuras*/
typedef struct grid{
	char zona[5][5]; 
	int barcos; //barcos vivos
} Grid;


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
			snprintf(j1, sizeof(j1), "./J1/%c%d", filas[f],c);
			snprintf(j2, sizeof(j2), "./J2/%c%d", filas[f],c);
		    mkdir(j1, 0777);
		    mkdir(j2, 0777);
		}
	}
}	

int existe(int j, char c){
	struct stat st = {0};
	char barco = 'barco';
	char ruta[25];
	if (j == 1){
		snprintf(ruta, sizeof(ruta), "./J1/%c/%c", c, barco);
		if (stat(ruta, &st) == -1){
    		return 0;
		}
		else{
			return 1;
		}
	}
	else if (j == 2){
		snprintf(ruta, sizeof(ruta), "./J2/%c/%c", c, barco);
		if (stat(ruta, &st) == -1){
    		return 0;
		}
		else{
			return 1;
		}
	}
	else{
		return -1; // Error jugador no existe
	}
	
}

void crearbarco(int j, char c){
	char ruta[25];
	if (j == 1){
		snprintf(ruta, sizeof(ruta), "./J1/%c/barco.txt", c);
		FILE *fp;
		fp=fopen(ruta,"w");
		fclose(fp);
	}
	else if (j == 2){
		snprintf(ruta, sizeof(ruta), "./J2/%c/barco.txt", c);
		FILE *fp;
		fp=fopen(ruta,"w");
		fclose(fp);
	}
}

int main()
{
	tablero();
	/*Inicializamos los grid de cada jugador con memoria dinamica*/
	Grid * grid1 = (Grid *)mmap(NULL, sizeof(Grid), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	Grid * grid2 = (Grid *)mmap(NULL, sizeof(Grid), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

	int pid;
	pid = fork();
	int pipe1[2], pipe2[2];
	pipe(pipe1);
	pipe(pipe2);
	int msg =1;
	int capitan;

	if(pid>0){
		capitan = 1;
		printf("Bienvenido Capitan 1\n");
		printf("Las coordenadas del juegp se componen de una Fila (letra mayuscula) seguida de una Columna (numero), ej B1\n");
		printf("Columnas: 1 - 2 - 3 - 4 - 5\n");
		printf("Filas: A - B - C - D - E\n");
		printf("TIP: No ingrese coordenadas fuera de rango o el programa presentara problemas de ejecucion.\n");
		int i = 5;
		while (i != 0){
			char coord[70];
			printf("Ingrese la coordenada donde desea agregar un barco\n");
			scanf("%s", coord);
			if (existe(capitan, coord)==0){
				crearbarco(capitan, coord);
				i -=1;
			}
			else{
				printf("Error: En esa coordenada ya existe un barco.\n");
			}
		}
		close(pipe1[1]);
		read(pipe1[0], msg, sizeof(msg));
	}
	else{
		capitan = 2;
		close(pipe1[0]);
		write(pipe1[1], msg, sizeof(msg));
		printf("Bienvenido Capitan 2\n");
		printf("Las coordenadas del juegp se componen de una Fila (letra mayuscula) seguida de una Columna (numero), ej B1\n");
		printf("Columnas: 1 - 2 - 3 - 4 - 5\n");
		printf("Filas: A - B - C - D - E\n");
		printf("TIP: No ingrese coordenadas fuera de rango o el programa presentara problemas de ejecucion.\n");
		int i = 5;
		while (i != 0){
			char coord[70];
			printf("Ingrese la coordenada donde desea agregar un barco\n");
			scanf("%s", coord);
			if (existe(capitan,coord)==0){
				crearbarco(capitan, coord);
				i -=1;
			}
			else{
				printf("Error: En esa coordenada ya existe un barco.\n");
			}
		}
	}

	












	return 0;
}
