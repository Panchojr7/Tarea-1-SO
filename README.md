#Antes de ejecutar:
Borrar las carpetas J1 y J2 del directorio en caso de existir. 

#Ejecución:
Ejecutar los comandos en el siguiente orden:
1º make
2º ./exe

#Consideraciones:
Se utilizó memoria compartida para crear una estructura tipo tablero, la que es utlizada para llevar la cuenta de los barcos vigentes de cada jugador, esto es usado para llevar un control a la hora de enviar mensajes de ataque. 
Además se utilizaron pipes para facilitar la programación de los turnos del juego.

#Post Data:
En caso de ejecutar el programa y que los paneles muestren dos o tres 1's en la columna E, favor de cerrar, borrar las carpetas y ejecutar de nuevo, esto es por un cache no controlado, aún así no afecta el desarrollo natural del juego.