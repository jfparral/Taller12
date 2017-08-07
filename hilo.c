#include <stdio.h>             /* for convenience */
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#define MAX 1000000
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct _control{
    int init;
    int end;
    char *doc;
    char *buscadas;
} control;

//nos devuelve el numero de lineas del archivo
int numero_lineas(char *ruta, int *tam_lineas){
    if(ruta != NULL){
        FILE *ar = fopen(ruta,"r");
        int lineas =0;
        int tam_linea;
        while(!feof(ar)){
            tam_linea++;
            char c =getc(ar);
            if(c == '\n'){
                if(tam_lineas != NULL){
                    tam_lineas[lineas] = tam_linea;
                }
                lineas++;
                tam_linea = 0;
            }
        }
        fclose(ar);
        return lineas;
    }
    return -1;
}

control *numeroHilo(char *ruta,int num_hilos,char* buscadas){
    int *lineas;
    int di,modulo,acum=0,num_lineas=numero_lineas(ruta,lineas);
    control *arreglo=(control *)malloc (sizeof(control)*num_hilos);   
    modulo=num_lineas%num_hilos;
    if(modulo == 0){
    //escogeremos el entero menor de la division
        di=num_lineas/num_hilos;
        
        for(int i=0;i<num_hilos;i++)
        {
            arreglo[i].init=acum;
            acum=acum+(di-1);
            arreglo[i].end=acum;
            acum=acum+1;
            arreglo[i].doc=ruta;
            arreglo[i].buscadas=buscadas;
        }  
    }else{
        //cogemos el entero mayor
        //cuando la division no es exacta
        di=num_lineas/num_hilos;
        int n=num_hilos-1;
        for(int i=0;i<num_hilos;i++)
       {
            if(n>=acum)
            {
                arreglo[i].init=acum;
                acum=acum+(di-1);
                arreglo[i].end=acum;
                acum=acum+1;
                arreglo[i].doc=ruta;
                arreglo[i].buscadas=buscadas;
            }
            //coge el resto de los caracteres
            else{
                arreglo[i].init=acum;
                acum=num_lineas-1;
                arreglo[i].end=acum;
                arreglo[i].doc=ruta;
                arreglo[i].buscadas=buscadas;
            }
            n=n-di;
       }  
    }
    return arreglo;
}

void * buscador(void* ctrl)
{
    char line[MAX];
    control *data = (control *)ctrl;
    char *p;
    char *buscar=data.buscadas;
    FILE *fp=fopen(data.doc,"rt");;
    int i=0;
    char aux[MAX];
    pthread_mutex_lock(&mutex);
    fgets(line,MAX,fp);
    while(!feof(fp)) {
                   i++;
                   if(p=strstr(line,"addres")) { /* tomo como subpalabra "addres" 
                   si se encuentra, sscanf guarda en aux la palabra que contiene a la subpalabra "addres"*/
                     sscanf(p,"%s",aux);
                     }
                     if(!strcmp(aux,"addres")) /* comparo las palabras, y en caso de ser iguales sale del while */
                         break;
                   fgets(line,MAX,fp);
    }
    pthread_mutex_unlock(&mutex);
    fclose(fp);
    
    
    return (void *)line;
}

int main(int argc, char *argv[]){

    pthread_t thr[atoi(argv[2])];
    int i,rc;
    char *palabras;
    char *buscadas;
    palabras= (char *) malloc(sizeof(char)*MAX);
    if (argc==1) {
		printf("./Taller12 <ruta> <numero_hilos> <palabra1> <palabra2> <palabra3>");
		exit(1);
	}
    for(int k=3;k<argc;k++)
    {
        buscadas[k-3]=argv[k];
    }
    control *arreglo=numeroHilo(atoi(argv[1]),atoi(argv[2]),buscadas);
    for (i = 0; i < atoi(argv[2]); i++) {
        if ((rc = pthread_create(&thr[i], NULL, buscador, &arreglo[i]))) {
            fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
            return EXIT_FAILURE;
        }
    }
    for (i = 0; i < atoi(argv[2]); ++i) {
        pthread_join(thr[i], &palabras);
        //printf("Valor Hilo %d: %lu\n",(i+1),(char)palabras);
    }
    return 0;
}