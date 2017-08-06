#include <time.h>
#include <stdio.h>             /* for convenience */
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <math.h>

typedef struct _control{
    int init;
    int end;
    FILE *doc;
} control;

int aleatorio(int a, int b){
    return (rand()%(b-a+1))+a;
}
//nos devuelve el numero de lineas del archivo
int numero_lineas(char *rutas, int *tam_lineas){
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

control *numeroHilo(int num_hilos){
    int *lineas;
    int di,modulo,acum=0,num_lineas=numero_lineas(argc[1],lineas);
    control *arreglo=(control *)malloc (sizeof(control)*b);   
    modulo=num_hilos%numero_lineas;
    if(modulo == 0){
    //escogeremos el entero menor de la division
        di=num_hilos/num_lineas;
        
        for(int i=0;i<b;i++)
        {
            arreglo[i].init=acum;
            acum=acum+(di-1);
            arreglo[i].end=acum;
            acum=acum+1;
            arreglo[i].doc=fopen(argc[1],"r");
        }  
    }else{
        //cogemos el entero mayor
        //cuando la division no es exacta
        di=(num_hilos/num_lineas);
        int n=num_hilos-1;
        for(int i=0;i<b;i++)
       {
            if(n>=acum)
            {
                arreglo[i].init=acum;
                acum=acum+(di-1);
                arreglo[i].end=acum;
                acum=acum+1;
                arreglo[i].doc=fopen(argc[1],"r");
            }
            //coge el resto de los caracteres
            else{
                arreglo[i].init=acum;
                acum=a-1;
                arreglo[i].end=acum;
                arreglo[i].doc=fopen(argc[1],"r");
            }
            n=n-di;
       }  
    }
    return arreglo;
}

double obtenerTiempoActual(){
    struct timespec tsp;

    clock_gettime( CLOCK_REALTIME, &tsp);

    double secs = (double)tsp.tv_sec;
    double nano = (double)tsp.tv_nsec/1000000000.0;

    return secs+nano;
}



int main(int argc, char *argv[]){

    pthread_t thr[atoi(argv[2])];
    int i,rc;
    long sumatotal;
    long *arr;
    arr= (long*) malloc(sizeof(long)*atoi(argv[1]));
    if (argc==1) {
		printf("./Taller12 <ruta> <numero_hilos> <palabra1> <palabra2> <palabra3>");
		exit(1);
	}
    
    srand(time(0));
    double ti = obtenerTiempoActual();
    for(i=0;i<atoi(argv[1]);i++){
        arr[i]=aleatorio(50,200);
    }
    control *arreglo=numeroHilo(atoi(argv[1]),atoi(argv[2]));
    for (i = 0; i < atoi(argv[2]); i++) {
        arreglo[i].arr=arr;
        if ((rc = pthread_create(&thr[i], NULL, sumador, &arreglo[i]))) {
            fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
            return EXIT_FAILURE;
        }
    }
    void *sumaparcial=NULL;
    for (i = 0; i < atoi(argv[2]); ++i) {
        pthread_join(thr[i], &sumaparcial);
        printf("Valor Hilo %d: %lu\n",(i+1),(long)sumaparcial);
        sumatotal =sumatotal+ (long)sumaparcial;
    }
    double tf = obtenerTiempoActual()-ti;
    printf("Suma Total: %d\n",(int)sumatotal);

	printf("Tiempo de ejecucion: %9f segundos\n",tf);
    return 0;
}