/* -----------------------------------------------------------------------
PRA1: Pipes
Codi font: calculador.c

Joel Aumedes Serrano
---------------------------------------------------------------------- */
#include "all.h"

void tancar();
int numPrimes = 0;

int main(int argc, char const *argv[]) {
    int number;
    while(read(GC_SORT,&number,sizeof(number)) != 0){
        t_infoNombre result;
        result.pid = getpid();
        result.nombre = number;
        result.primer = 'S';
        numPrimes += 1;
        for(int i = 2; i < number;i++){
            if(number%i == 0){
                result.primer = 'N';
                numPrimes -= 1;
            }
        }
        if(write(CC_ENT,&result,sizeof(t_infoNombre)) == -1){
            perror("Error while writing");
        }
    }
    close(GC_SORT);
    close(CC_ENT);
    signal(SIGTERM,tancar);
    pause();
}

void tancar(){
    exit(numPrimes);
}
