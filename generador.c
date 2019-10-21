/*-----------------------------------------------------------------------
PRA1: Pipes
Codi font: generador.c

Joel Aumedes Serrano
---------------------------------------------------------------------- */
#include "all.h"

int N;
void tancar();

int main(int argc, char *argv[]) {
    signal(SIGTERM, tancar);
    char s[100];
    N = *argv[1];
    for(int i = 2;i <= N;i++){
        write(GC_ENT,&i,sizeof(i));
    }
    close(GC_ENT);
}

void tancar(){
    exit(0);
}
