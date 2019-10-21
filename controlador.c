/* -----------------------------------------------------------------------
PRA1: Pipes
Codi font: controlador.c

Joel Aumedes Serrano
---------------------------------------------------------------------- */
#include "all.h"

int N;
int PROCESSES;
void readN();

int main(int argc, char *argv[]) {
    readN();
    char s[100] = "\0";
    PROCESSES = atoi(argv[1]);
    sprintf(s, "Processos a crear: %i Primers a calcular: de 2 a %d\n", PROCESSES, N);
    write(1,s,strlen(s));
    //Creació pipe i procés generador
    int gcfd[2];
    int ccfd[2];

    if(pipe(gcfd) == -1){
        perror("Error al crear pipe generador -> calculador: ");
        exit(-1);
    }

    int pid;
    if ((pid = fork()) == -1) {
        perror("Error al crear generador: ");
        exit(-1);
    }else if(pid == 0){//Procés generador
        dup2(gcfd[1],GC_ENT);
        close(gcfd[0]);
        close(gcfd[1]);
        execlp("./generador","generador",&N, NULL);
        exit(-2);
    }

    if(pipe(ccfd) == -1){
        perror("Error al crear pipe calculadors -> controlador: ");
        exit(-1);
    }

    int pids[PROCESSES];
    for(int i = 0; i < PROCESSES;i++){
        pids[i] = fork();
        switch (pids[i]) {
            case -1:
                perror("Error al crear calculador: ");
                exit(-1);
            case 0:
                //Tancament de descriptors innecessaris
                dup2(ccfd[1],CC_ENT);
                dup2(gcfd[0],GC_SORT);
                close(ccfd[0]);
                close(ccfd[1]);
                close(gcfd[0]);
                close(gcfd[1]);
                execlp("./calculador", "calculador", NULL);
                exit(-2);
        }
    }

    dup2(ccfd[0],CC_SORT);
    close(ccfd[1]);
    close(ccfd[0]);

    int primesPipe = 0;

    for(int i = 2; i <= N;i++){
        char s[1000] = "\0";
        t_infoNombre result;
        read(CC_SORT,&result,sizeof(t_infoNombre));
        sprintf(s,"Controlador: Rebut del procés %i: Número %i Primer: %c\n",result.pid,result.nombre,result.primer);
        if(result.primer == 'S') primesPipe++;
        write(1,s,sizeof(s));
    }

    if(kill(pid, SIGTERM) == -1){
        perror("Error closing generator: ");
        exit(-1);
    }
    wait(NULL);

    int primesExit = 0;

    for(int i = 0; i < PROCESSES;i++){
        if(kill(pids[i], SIGTERM) == -1){
            perror("Error closing calculator: ");
            exit(-1);
        }
        int status;
        wait(&status);
        primesExit += WEXITSTATUS(status);
    }
    printf("exit %i\n", primesExit);
}

void readN(){
    char s[] = "Calcular primers de 2 a : ";
    write(1,s,sizeof(s));
    read(0,s,sizeof(s));
    N = atoi(s);
}
