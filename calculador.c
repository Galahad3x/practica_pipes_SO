/* -----------------------------------------------------------------------
PRA1: Pipes
Codi font: calculador.c

Joel Aumedes Serrano
---------------------------------------------------------------------- */
#include "all.h"

void tancar();
char isPrime(int);
int numPrimes;

int main(int argc, char const *argv[]) {
    int number;
    t_infoNombre result;
    result.pid = getpid();
    int llegir;
    //Llegir cada nombre, fer els càlculs i escriure al pipe de respostes el resultat en format t_infoNombre
    while((llegir = read(GC_SORT,&number,sizeof(int))) > 0){
        result.nombre = number;
        if((result.primer = isPrime(number)) == 'S'){
            ++numPrimes;
        }
        if(write(CC_ENT,&result,sizeof(t_infoNombre)) == -1){
            perror("Error escrivint al pipe de respostes: ");
            exit(-2);
        }
    }
    if(llegir < 0){
        perror("Error al llegir del pipe de nombres: ");
        exit(-1);
    }
    close(GC_SORT);
    close(CC_ENT);
    signal(SIGTERM,tancar);
    pause();
}

char isPrime(int n){
    for(int i = 2; i < n;i++){
        if(n%i == 0){
            return 'N';
        }
    }
    return 'S';
}

void tancar(){
    exit(numPrimes);
}
