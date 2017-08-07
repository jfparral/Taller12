#include <stdio.h>             /* for convenience */
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#define MAX 1000000
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
char ** palabras;
int * contadas;
/*typedef struct _control{
    int init;
    int end;
    char *doc;
    char *buscadas;
} control;*/

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

void * buscador(void* ctrl)
{
    char * str = (char *) ctrl;


	pthread_mutex_lock(&mutex);

		
	char *alto;
	alto = strtok(str," \"!·$%&/()=º|@#~½¬<>-_'ç`+*[]{}ḉç¿,.!?:;");
    while( alto != NULL ){
    	if(!strcmp(*(palabras), alto)){
    		*(contadas) = *(contadas) + 1;
    	}
    	else if(!strcmp(*(palabras+1), alto)){
    		*(contadas+1) = *(contadas+1) + 1;
    	}
    	else if(!strcmp(*(palabras+2), alto)){
    		*(contadas+2) = *(contadas+2) + 1;
    	}
       	alto = strtok(NULL, " \"!·$%&/()=º|@#~½¬<>-_'ç`+*[]{}ḉç¿,.!?:;");
   	}
		

   	printf("Resultados encontrados: \nPalabra: %s  Numero: %d\nPalabra: %s  Numero: %d\nPalabra: %s  Numero: %d\n",
       *(palabras), *(contadas), *(palabras+1), *(contadas+1), *(palabras+2), *(contadas+2));

	
	pthread_mutex_unlock(&mutex);
	return (void *)0;
}

int main(int argc, char *argv[]){
    int *ncaracteres;
    char ** bloques;
    int cont=0;

    palabras = malloc(sizeof(char*)*3);
	contadas = malloc(sizeof(int)*3);
    
    if (argc==1) {
		printf("./Hilos <ruta> <numero_hilos> <palabra1> <palabra2> <palabra3>");
		exit(1);
	}

    for(int k=3;k<argc;k++)
    {
        palabras[k-3]=argv[k];
        contadas[k-3]=0;
    }

    int nlineas = numero_lineas(argv[1], 0);
    ncaracteres=malloc(sizeof(int)*nlineas);
    numero_lineas(argv[1], ncaracteres);
    for(int i = 1; i < nlineas ; i++){
        cont  += *(ncaracteres+i);
    }

    int divisor = cont/atoi(argv[2]);

	int seek = 0;
    bloques = malloc(sizeof(char*)*atoi(argv[2]));

    for(int i = 0 ; i < atoi(argv[2]) ; i++){
        
        FILE * file = fopen(argv[1],"r");
        fseek(file, seek,SEEK_SET); 
        *(bloques+i) = malloc(sizeof(char)*divisor);

        for( int j = 0 ; j < divisor ; j++){
            char c = getc(file);
            *(*(bloques+i)+j) = c;    
        }      
        seek = seek + divisor;
	}

    int p;
	pthread_t * threads = malloc(sizeof(pthread_t)*atoi(argv[2]));

	for(int i = 0; i < atoi(argv[2]); i++){

		p = pthread_create(threads+i, NULL, buscador, (void *)*(bloques+i));
		if(p < 0){
			fprintf(stderr, "Error al crear el hilo : %d\n", i);
			exit(-1);	
		}


	}
	for(int j = 0; j < atoi(argv[2]); j++){

		p = pthread_join(threads[j], NULL);
		if(p < 0){
			fprintf(stderr, "Error al esperar por el hilo\n");
			exit(-1);
		}

	}

    return 0;
}