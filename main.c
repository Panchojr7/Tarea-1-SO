#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

#define FILAS "ABCDE"


/*Estructuras*/
typedef struct grid{
	int zona[5][5]; //ataques que me hacen
	int barcos; //barcos vivos mios
	int naves; //barcos vivos enemigo
	int mapa[5][5]; //ataque que yo hago 
} Grid;

/* Funciones */
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

void instrucciones(int j){
	printf(RED	"Bienvenido Capitan %d\n", j);
	printf(YELLOW	"Las coordenadas del juegp se componen de una Fila (letra mayuscula) seguida de una Columna (numero), ej B1\n");
	printf("Columnas: 1 - 2 - 3 - 4 - 5\n");
	printf("Filas: A - B - C - D - E\n");
	printf("TIP: No ingrese coordenadas fuera de rango o el programa presentara problemas de ejecucion.\n"	RESET);
}

int existe(int j, char coord1, char coord2){
	struct stat st = {0};
	if (j == 1){
		char ruta1[19];
		snprintf(ruta1, sizeof(ruta1), "./J1/%c%c/barco.txt", coord1, coord2);
		if (stat(ruta1, &st) == -1){
    		return 0;
		}
		else{
			return 1;
		}
	}
	else if (j == 2){
		char ruta2[19];
		snprintf(ruta2, sizeof(ruta2), "./J2/%c%c/barco.txt", coord1, coord2);
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
		snprintf(ruta1, sizeof(ruta1), "./J1/%c%c/barco.txt", coord1, coord2);
		FILE *fp;
		fp=fopen(ruta1,"w");
		fclose(fp);
	}
	else if (j == 2){
		char ruta2[19];
		snprintf(ruta2, sizeof(ruta2), "./J2/%c%c/barco.txt", coord1, coord2);
		FILE *fp;
		fp=fopen(ruta2,"w");
		fclose(fp);
	}
}



int main()
{
	tablero();

	/*Inicializamos los grid de cada jugador con memoria dinamica*/
	Grid * grid1 = (Grid *)mmap(NULL, sizeof(Grid), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	Grid * grid2 = (Grid *)mmap(NULL, sizeof(Grid), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

	int pid, pipePaH[2], pipeHaP[2]; 
	pipe(pipePaH); 
	pipe(pipeHaP);
	char turno[1];

	if((pid = fork() ) == 0){ 
    close(pipePaH[1]);
    close(pipeHaP[0]);
    write(pipeHaP[1] ,"0", 1);
	}
	else{
    close(pipeHaP[1]); 
    close(pipePaH[0]);
	}
	char coord[4];
	/* Creacion de barcos de cada jugador */

	/* Jugador 1 */
	if(pid>0){ 
	    read(pipeHaP[0],turno,1); 
	    if(turno[0]=='0'){ 
	        instrucciones(1);
	        int i = 5;
	        while (i > 0){
	            printf("Ingrese la coordenada donde desea agregar un barco\n");
	            scanf("%s", &coord);
	            if (existe(1, coord[0], coord[1]) == 0){
	                crearbarco(1, coord[0], coord[1]);
	                i = i - 1;
	            }
	            else{
	                printf("Error: En esa coordenada ya existe un barco.\n");
	            }
	        } 
	        write(pipePaH[1],"1",1);
	        read(pipeHaP[0],turno,1); 
	    } 
	} 

	/* Jugador 2 */
	else{ 
	    read(pipePaH[0],turno,1);
	    if(turno[0]== '1'){
	        instrucciones(2);
	        int t = 5;
	        while (t > 0){
	            printf("Ingrese la coordenada donde desea agregar un barco\n");
	            scanf("%s", &coord);
	            if (existe(2,coord[0],coord[1])==0){
	                crearbarco(2, coord[0], coord[1]);
	                t = t-1;
	            }
	            else{
	                printf("Error: En esa coordenada ya existe un barco.\n");
	            }
	        }
	        write(pipeHaP[1],"0",1);
	    } 
	}

	int fin=3;

	if((pid = fork() ) == 0){ 
    	close(pipePaH[1]);
    	close(pipeHaP[0]);
    	write(pipeHaP[1] ,"0", 1);
	}
	else{
    	close(pipeHaP[1]); 
    	close(pipePaH[0]);
	}

	/***********************************************************************************************************************************************/
																/* Comienza el juego */
	/**********************************************************************************************************************************************/

	while(fin > 0){

		/* Jugador 1 */
		if(pid>0){ 
		    read(pipeHaP[0],turno,1); 
		    if(turno[0]=='0'){
		    	grid1->zona[1][1]=7;
		        printf("turno %d\n", grid1->zona[1][1]);
		        scanf("%s",coord);
		    write(pipePaH[1],"1",1); 
		    } 
		} 

		/* Jugador 2 */
		else{ 
		    read(pipePaH[0],turno,1);
		    if(turno[0]== '1'){
		        printf("turno %c\n",turno[0]);
		        write(pipeHaP[1],"0",1);
		    } 
		}
	fin -= 1;
	}

	return 0;
}
