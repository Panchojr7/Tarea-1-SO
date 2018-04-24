int main()
{
   
int i, fin=0, pid, pipePaH[2], pipeHaP[2]; 
int count=3, numb[]={0,0,0,0,0,0}; 
char control[1]; 
pipe(pipePaH); 
pipe(pipeHaP);

if((pid = fork() ) == 0){ 
    close(pipePaH[1]); 
    close(pipeHaP[0]); 
    write(pipeHaP[1] ,"0", 1); 
} 
else{
    close(pipeHaP[1]); 
    close(pipePaH[0]);
}

if(pid>0){ 
    read(pipeHaP[0],control,1); 
    if(control[0]=='0'){ 
        //printf("turno %c\n",control[0]); 
    write(pipePaH[1],"1",1); 
    } 
} 
else{ 
    read(pipePaH[0],control,1);
    if(control[0]== '1'){
        //printf("turno %c\n",control[0]);
        write(pipeHaP[1],"0",1);
    } 
}




    return 0;
}