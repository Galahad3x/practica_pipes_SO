/* -----------------------------------------------------------------------
PRA1: Pipes
Codi font: controlador.c

Joel Aumedes Serrano
---------------------------------------------------------------------- */
#include "all.h"

int N;
int PROCESSES;

int main(int argc, char *argv[]) {
    char s[100] = "Calcular primers de 2 a : ";
    char n[NUMSIZE + 1];
    while(0 < 1){ //LLegir fins a on calcular
        if(write(1,s,sizeof(s)) <= 0){
            perror("Error al escriure a sortida estàndard: ");
            exit(-3);
        }
        read(0,n,NUMSIZE);
        N = atoi(n);
        if(N > 2){
            break;
        }else{
            sprintf(s,"Número no vàlid, torna a escriure: \n");
        }
    }
    char s2[100];
    PROCESSES = atoi(argv[1]);
    sprintf(s2, "Processos a crear: %i Primers a calcular: de 2 a %d\n", PROCESSES, N);
    if(write(1,s2,strlen(s2)) <= 0){
        perror("Error al escriure a sortida estàndard: ");
        exit(-3);
    }
    int gcfd[2];
    int ccfd[2];

    //Creació pipe generador -> calculadors
    if(pipe(gcfd) == -1){
        perror("Error al crear pipe generador -> calculador: ");
        exit(-1);
    }

    //Creació generador
    int pid;
    switch (pid = fork()) {
        case -1:
            perror("Error al crear generador: ");
            exit(-1);
        case 0:
            dup2(gcfd[1],GC_ENT);
            close(gcfd[0]);
            close(gcfd[1]);
            if(execlp("./generador","generador",n, NULL) == -1){
                perror("Error durant el recobriment del generador: ");
                exit(-2);
            }
    }

    //Creació pipe calculadors -> controlador
    if(pipe(ccfd) == -1){
        perror("Error al crear pipe calculadors -> controlador: ");
        exit(-1);
    }

    //Creació calculadors
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
                if(execlp("./calculador", "calculador", NULL) == -1){
                    perror("Error durant el recobriment d'un calculador: ");
                    exit(-2);
                }
                exit(0);
        }
    }

    dup2(ccfd[0],CC_SORT);
    close(ccfd[1]);
    close(ccfd[0]);
    close(gcfd[1]);
    close(gcfd[0]);

    int primesPipe = 0;

    //Llegir resultats del pipe calculadors -> controlador i mostrar-los per pantalla
    int llegir;
    t_infoNombre result;
    while((llegir = read(CC_SORT,&result,sizeof(t_infoNombre)) > 0)){
        char s[100] = "\0";
        sprintf(s,"Controlador: Rebut del procés %d: Número %d Primer: %c \n",result.pid,result.nombre,result.primer);
        if(result.primer == 'S') primesPipe++;
        if(write(1,s,sizeof(s)) <= 0){
            perror("Error al escriure a sortida estàndard: ");
            exit(-3);
        }
    }

    if(llegir < 0){
        perror("Error al llegir resultats: ");
        exit(-3);
    }

    //Tancar el generador
    if(kill(pid, SIGTERM) == -1){
        perror("Error tancant el generador: ");
        exit(-4);
    }
    wait(NULL);

    int primesExit = 0;
    int status = 0;

    sleep(1);

    //Tancar els calculadors
    for(int i = 0; i < PROCESSES;i++){
        if(kill(pids[i], SIGTERM) == -1){
            perror("Error tancant un calculador: ");
            exit(-4);
        }
        wait(&status);
        primesExit += WEXITSTATUS(status);
    }

    //Escriure resultats finals
    char s3[100];
    sprintf(s3,"Primers del Pipe: %i \nPrimers a Exit: %i\n", primesPipe, primesExit);
    if(write(1,s3,strlen(s3)) <= 0){
        perror("Error al escriure a sortida estàndard: ");
        exit(-3);
    }
    exit(0);
}
