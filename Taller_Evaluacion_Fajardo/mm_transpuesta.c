/**************************************************************
		Pontificia Universidad Javeriana
	Autor: J. Corredor
	Fecha: Febrero 2024
	Materia: Sistemas Operativos
	Tema: Taller de Evaluación de Rendimiento
	Fichero: fuente de multiplicación de matrices NxN por hilos.
	Objetivo: Evaluar el tiempo de ejecución del 
		      algoritmo transpuesto de multiplicación de matrices.
			  Se implementa con la Biblioteca POSIX Pthreads
****************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

// Tamaño del bloque de memoria para almacenar las matrices.
#define DATA_SIZE (1024*1024*64*3) 

// Mutex para sincronización, aunque no se usa en cálculos críticos en este caso.
pthread_mutex_t MM_mutex;
static double MEM_CHUNK[DATA_SIZE]; // Bloque de memoria estática para las matrices.
double *mA, *mB, *mC; // Punteros para matrices A, B y C.

// Estructura para pasar parámetros a los hilos.
struct parametros{
	int nH;  // Número total de hilos.
	int idH; // Identificador del hilo actual.
	int N;   // Dimensión de las matrices (N x N).
};

// Variables para medir el tiempo de ejecución.
struct timeval start, stop;

/**
 * Nombre: llenar_matriz.
 * descripcion: Rellena las matrices mA y mB con valores predefinidos y limpia la matriz c.
 * Argumentos SZ que es el tamaño de la matriz .
 */
void llenar_matriz(int SZ){ 
	srand48(time(NULL)); // Inicializa el generador de números aleatorios.
	for(int i = 0; i < SZ*SZ; i++){
		mA[i] = 1.1 * i; // Valores secuenciales para facilitar la verificación.
		mB[i] = 2.2 * i; 
		mC[i] = 0;  // Inicializa la matriz de resultados con ceros.
	}	
}

/**
 * Nombre: print_matrix
 * Descripcion: Imprime una matriz en la consola si el tamaño es pequeño.
 * Argumentos: sz que es el tamaño de la matriz y matriz que es un puntero a la matriz a imprimir.
*/
void print_matrix(int sz, double *matriz){
	if(sz < 12){
		for(int i = 0; i < sz*sz; i++){
			if(i % sz == 0) printf("\n"); // Salto de línea al final de cada fila.
			printf(" %.3f ", matriz[i]);  // Imprime valores con 3 decimales.
		}	
		printf("\n>-------------------->\n");
	}
}

/*
 * Nombre: inicial_tiempo.
 * Descripcion: Inicia la medición del tiempo.
*/
void inicial_tiempo(){
	gettimeofday(&start, NULL);
}

/*
 * Nombre: final_tiempo.
 * Descripcion: Finaliza la medición del tiempo e imprime el resultado.
*/
void final_tiempo(){
	gettimeofday(&stop, NULL);
	stop.tv_sec -= start.tv_sec;
	printf("\n:-> %9.0f µs\n", (double) (stop.tv_sec * 1000000 + stop.tv_usec));
}

/*
 * Nombre: mult_thread.
 * Descripcion: Multiplicación de matrices por hilos utilizando metodo transpuesta.
 * Argumentos: variables Puntero a la estructura de parámetros.
*/
void *mult_thread(void *variables){
	struct parametros *data = (struct parametros *)variables; // Cast de los datos recibidos.
	
	int idH = data->idH;
	int nH  = data->nH;
	int N   = data->N;
	int ini = (N / nH) * idH;    // Índice inicial de las filas para este hilo.
	int fin = (N / nH) * (idH+1);// Índice final de las filas para este hilo.

    for (int i = ini; i < fin; i++){  // Itera sobre las filas asignadas.
        for (int j = 0; j < N; j++){  // Itera sobre las columnas de B/C.
			double *pA, *pB, sumaTemp = 0.0;
			pA = mA + (i * N); // Puntero a la fila i de A.
			pB = mB + (j * N); // Puntero a la columna j de B.
            for (int k = 0; k < N; k++, pA++, pB++){ 
				sumaTemp += (*pA * *pB); // Producto punto para C[i][j].
			}
			mC[i * N + j] = sumaTemp; // Almacena el resultado.
		}
	}

	// Bloquea el mutex para evitar conflictos.
	pthread_mutex_lock(&MM_mutex);
	pthread_mutex_unlock(&MM_mutex);
	pthread_exit(NULL); // Finaliza el hilo.
}

int main(int argc, char *argv[]){
	if (argc < 3){
		printf("Uso: ./ejecutable tamMatriz numHilos\n");
		return -1;	
	}
    int SZ = atoi(argv[1]);       // Dimensión de la matriz.
    int n_threads = atoi(argv[2]); // Número de hilos.

    pthread_t p[n_threads]; // Arreglo de identificadores de hilos.
    pthread_attr_t atrMM;   // Atributos para los hilos.

	mA = MEM_CHUNK;            // Matriz A en el bloque de memoria.
	mB = mA + SZ * SZ;         // Matriz B después de A.
	mC = mB + SZ * SZ;         // Matriz C después de B.

	llenar_matriz(SZ);         // Inicializa las matrices.
	print_matrix(SZ, mA);      // Imprime la matriz A si es pequeña.
	print_matrix(SZ, mB);      // Imprime la matriz B si es pequeña.

	inicial_tiempo();          // Inicia la medición de tiempo.
	pthread_mutex_init(&MM_mutex, NULL); // Inicializa el mutex.
	pthread_attr_init(&atrMM);           // Inicializa atributos.
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE); // Hilos unibles.

    for (int j = 0; j < n_threads; j++){
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); 
		datos->idH = j;        // ID del hilo.
		datos->nH  = n_threads;// Total de hilos.
		datos->N   = SZ;       // Dimensión de la matriz.
        pthread_create(&p[j], &atrMM, mult_thread, (void *)datos); // Crea hilos.
	}

    for (int j = 0; j < n_threads; j++)
        pthread_join(p[j], NULL); // Espera a que todos los hilos terminen.
	final_tiempo();               // Detiene el temporizador.
	
	print_matrix(SZ, mC);         // Imprime la matriz C si es pequeña.

	pthread_attr_destroy(&atrMM); // Destruye atributos.
	pthread_mutex_destroy(&MM_mutex); // Destruye el mutex.
	pthread_exit(NULL);           // Finaliza el hilo principal.
}
