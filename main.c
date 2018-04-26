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


/*Estructura*/
typedef struct grid{
	int zona[5][5]; //ataques que me hacen -> Mis Barcos
	int barcos; //barcos vivos mios
	int mapa[5][5]; //ataques que yo hago -> Mi panel
} Grid;


int main()
{
	tablero();

	/*Inicializamos los grid de cada jugador con memoria dinamica*/
	Grid * grid1 = (Grid *)mmap(NULL, sizeof(Grid), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	Grid * grid2 = (Grid *)mmap(NULL, sizeof(Grid), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	int * fin = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
	*fin=1;

	int i,j;
	for (i = 0; i < 5; i++){
		for (j = 0; j < 5; j++){
			grid1->mapa[i][j]= 0;
			grid2->mapa[i][j]= 0;
			grid1->zona[i][j]= 0;
			grid2->zona[i][j]= 0;
		}	
	}
	grid1->barcos=5;
	grid2->barcos=5;

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
	            if (comprobar(coord[0],coord[1])){
		            if (existe(1, coord[0], coord[1]) == 0){
		                crearbarco(1, coord[0], coord[1]);
		                grid1->zona[letra(coord[0])][coord[1]-1]=1;
		                i = i - 1;
		            }
		            else{
		                printf(CYAN "Error: En esa coordenada ya existe un barco.\n" RESET);
		            }
		        }
		        else{
		        	printf(CYAN"Error: Coordenada incorrecta.\n" RESET);
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
	            if (comprobar(coord[0],coord[1])){
		            if (existe(2,coord[0],coord[1])==0){
		                crearbarco(2, coord[0], coord[1]);
		                grid2->zona[letra(coord[0])][coord[1]-1]=1;
		                t = t-1;
		            }
	            	else{
	                	printf(CYAN "Error: En esa coordenada ya existe un barco.\n" RESET);
	            	}
	            }
	            else{
	            	printf(CYAN "Error: Coordenada incorrecta.\n" RESET);
	            }	
	        }
	        write(pipeHaP[1],"0",1);
	    } 
	}

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

	char ata[2];
	char que[2];
	int c=1;
	int d=1;
	while(*fin > 0){
		/* Jugador 1 */
		if(pid>0){ 
		    read(pipeHaP[0],turno,1);
		    if(turno[0]=='0' && *fin > 0){
		    	panel(1,grid1->mapa);
		        /* Primer Ataque */
		        c=1;
		        while(c>0){
		        	printf("Jugador 1, escriba la coordenada del primer ataque a realizar: ");
		        	scanf("%s",ata);
			        if(comprobar(ata[0],ata[1])==1){
			        	c=0;
				        if (existe(2,ata[0], ata[1])==1){
				        	char ruta[27];
				        	if (grid2->barcos == 1){
				        		grid2->barcos -=1;
				          		snprintf(ruta, sizeof(ruta), "./J2/%c_%c/barco.txt", ata[0], ata[1]);
				        		remove(ruta);
				        		printf(MAGENTA "[Jugador 1]   ");
				        		printf(RED "¡¡¡HAS HUNDIDO EL ÙLTIMO BARCO!!!\n" RESET);
				        		*fin = 0;
				        		break;
				        	}
				        	else{
				        		grid2->barcos -=1;
				        		grid1->mapa[letra(ata[0])][casteo(ata[1])] = 3;
				        		snprintf(ruta, sizeof(ruta), "./J2/%c_%c/barco.txt", ata[0], ata[1]);
				        		remove(ruta);
				        		printf(MAGENTA "[Jugador 1]   ");
				        		printf(RED "¡¡HAS HUNDIDO UN BARCO!!!\n" RESET);
				        	}
				        }
				        else{
				        	grid1->mapa[letra(ata[0])][casteo(ata[1])] = 1;
				        	printf(MAGENTA "[Jugador 1]   ");
				        	printf(BLUE "¡¡HAS FALLADO!!!\n" RESET);
				        }
				    }
				    else{
				    	printf(CYAN"Error: Coordenada incorrecta.\n" RESET);
				    }
				}
		        /* Segundo Ataque */
		        if (*fin != 0){c=1;}
		        else{d=0;}
		        while(c>0){
		        	printf("Jugador 1, escriba la coordenada del segundo ataque a realizar: ");
		        	scanf("%s",que);
			        if(comprobar(que[0],que[1])==1){
			        	c=0;
				        if (existe(2,que[0], que[1])==1){
				        	char ruta[27];
				        	if (grid2->barcos == 1){
				        		grid2->barcos -=1;
				        		snprintf(ruta, sizeof(ruta), "./J2/%c_%c/barco.txt", que[0], que[1]);
				        		remove(ruta);
				        		printf(MAGENTA "[Jugador 1]   ");
				        		printf(RED "¡¡¡HAS HUNDIDO EL ÙLTIMO BARCO!!!\n" RESET);
				        		*fin = 0;
				        		break;
				        	}
				        	else{
				        		grid2->barcos -=1;
				        		grid1->mapa[letra(que[0])][casteo(que[1])] = 3;
				        		snprintf(ruta, sizeof(ruta), "./J2/%c_%c/barco.txt", que[0], que[1]);
				        		remove(ruta);
				        		printf(MAGENTA "[Jugador 1]   ");
				        		printf(RED "¡¡¡HAS HUNDIDO UN BARCO!!!\n" RESET);
				        	}
				        }
				        else{
				        	grid1->mapa[letra(que[0])][casteo(que[1])] = 1;
				        	printf(MAGENTA "[Jugador 1]   ");
				        	printf(BLUE "¡¡¡HAS FALLADO!!!\n" RESET);

				        }
				    }
				    else{
				    	printf(CYAN"Error: Coordenada incorrecta.\n" RESET);
				    }
				}
		    write(pipePaH[1],"1",1);
		    } 
		} 

		/* Jugador 2 */
		else{ 
		    read(pipePaH[0],turno,1);
		    if(turno[0]== '1' && *fin > 0){
		        panel(2,grid2->mapa);

		        /* Primer Ataque*/
		        d=1;
		        while(d>0){
		        	printf("Jugador 2, escriba la coordenada del primer ataque a realizar: ");
		        	scanf("%s",ata);
			        if(comprobar(ata[0],ata[1])==1){
			        	d=0;
				        if (existe(1,ata[0], ata[1])==1){
				        	char ruta[27];
				        	if (grid1->barcos == 1){
				        		grid1->barcos -=1;
				        		snprintf(ruta, sizeof(ruta), "./J1/%c_%c/barco.txt", ata[0], ata[1]);
				        		remove(ruta);
				        		printf(MAGENTA "[Jugador 2]   ");
				        		printf(RED "¡¡¡HAS HUNDIDO EL ÙLTIMO BARCO!!!\n" RESET);
				        		*fin = 0;
				        		break;
				        	}
				        	else{
				        		grid1->barcos -=1;
				        		grid2->mapa[letra(ata[0])][casteo(ata[1])] = 3;
				        		snprintf(ruta, sizeof(ruta), "./J1/%c_%c/barco.txt", ata[0], ata[1]);
				        		remove(ruta);
				        		printf(MAGENTA "[Jugador 2]   ");
				        		printf(RED "¡¡¡HAS HUNDIDO UN BARCO!!!\n" RESET);
				        	}
				        }
				        else{
				        	grid2->mapa[letra(ata[0])][casteo(ata[1])] = 1;
				        	printf(MAGENTA "[Jugador 2]   ");
				        	printf(BLUE "¡¡¡HAS FALLADO!!!\n" RESET);
				        }
				    }
				    else{
				    	printf(CYAN"Error: Coordenada incorrecta.\n" RESET);
				    }
				}
		        /* Segundo Ataque */
		        if (*fin != 0){d=1;}
		        else{d=0;}
		        while(d>0){
			        printf("Jugador 2, escriba la coordenada del segundo ataque a realizar: ");
			        scanf("%s",que);
			        if(comprobar(que[0],que[1])==1){
			        	d=0;
				        if (existe(1,que[0], que[1])==1){
				        	char ruta[27];
				        	if (grid1->barcos == 1){
				        		grid1->barcos -=1;
				        		snprintf(ruta, sizeof(ruta), "./J1/%c_%c/barco.txt", que[0], que[1]);
				        		remove(ruta);
				        		printf(MAGENTA "[Jugador 2]   ");
				        		printf(RED "¡¡¡HAS HUNDIDO EL ÙLTIMO BARCO!!!\n" RESET);
				        		*fin = 0;
				        		break;
				        	}
				        	else{
				        		grid1->barcos -=1;
				        		grid2->mapa[letra(que[0])][casteo(que[1])] = 3;
				        		snprintf(ruta, sizeof(ruta), "./J1/%c_%c/barco.txt", que[0], que[1]);
				        		remove(ruta);
				        		printf(MAGENTA "[Jugador 2]   ");
				        		printf(RED "¡¡¡HAS HUNDIDO UN BARCO!!!\n" RESET);
				        	}
				        }
				        else{
				        	grid2->mapa[letra(que[0])][casteo(que[1])] = 1;
				        	printf(MAGENTA "[Jugador 2]   ");
				        	printf(BLUE "¡¡¡HAS FALLADO!!!\n" RESET);
				        }
				    }
				    else{
				    	printf(CYAN"Error: Coordenada incorrecta.\n" RESET);
				    }
				}
		    write(pipeHaP[1],"0",1);
		    }
		}
	}

	if(grid2->barcos == 0){
		printf("EL GANADOR ES EL JUGADOR 1\n");
		return 0;
	}
	else if(grid1->barcos == 0){
		printf("EL GANADOR ES EL JUGADOR 2\n");
		return 0;
	}

	return 0;
}
