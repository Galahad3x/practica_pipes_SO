/*-----------------------------------------------------------------------
PRA1: Pipes
Codi font: generador.c

Joel Aumedes Serrano
---------------------------------------------------------------------- */
#include "all.h"

int N;

int main(int argc, char *argv[]) {
    N = atoi(argv[1]);
    //Escriure un a un els números al pipe de nombres
    for(int i = 2;i <= N;i++){
        if(write(GC_ENT,&i,sizeof(int)) <= 0){
            perror("Error escrivint al pipe de nombres: ");
            exit(-2);
        }
    }
    close(GC_ENT);
    signal(SIGTERM, SIG_DFL);
    pause();
}
