#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define GC_ENT 10
#define GC_SORT 11
#define CC_ENT 20
#define CC_SORT 21

#define NUMSIZE 20

typedef struct{
    int pid;
    int nombre;
    char primer;
} t_infoNombre;
