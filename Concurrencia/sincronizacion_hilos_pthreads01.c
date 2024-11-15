/*********************************************************************************************
Autor: B. Fajardo
Fecha: 2024
Materia: Sistemas Operativos
Tópico: Sincronización de hilos con pthreads y mutex
Modo de compilación y ejecución:
    gcc -o sincronizacion_hilos_pthreads sincronizacion_hilos_pthreads.c -lpthread
    Esto compila el programa y genera el ejecutable `sincronizacion_hilos_pthreads`.

    ./sincronizacion_hilos_pthreads
    Esto ejecuta el programa principal, que crea varios hilos y asegura que cada uno
    imprima mensajes de inicio y finalización de forma secuencial utilizando un mutex.

Descripción del programa:
    Este programa crea y ejecuta tres hilos en paralelo utilizando la biblioteca pthread en C.
    Cada hilo imprime un mensaje de inicio, realiza una operación simulada, y luego imprime un
    mensaje de finalización. Se utiliza un mutex para garantizar que solo un hilo acceda a la
    sección crítica a la vez, lo que permite un orden controlado en la impresión de mensajes
    y evita el entrelazado de mensajes de diferentes hilos.

Archivos para el funcionamiento del programa:
    sincronizacion_hilos_pthreads.c  Código fuente del programa que implementa la sincronización
                                     de hilos utilizando pthreads y mutex.
**********************************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t BLOQUEO;

void *function(void *job) {
    pthread_mutex_lock(&BLOQUEO);  // Inicia la sección crítica
    
    size_t IDh = *(size_t *)job;
    int cont = 5 * IDh;
    printf("\nHilo %zu ha iniciado\n", IDh);

    for (int i = 0; i < 230; i++) {
        cont *= (cont + 1);
    }

    printf("\nHilo %zu ha finalizado\n", IDh);

    pthread_mutex_unlock(&BLOQUEO);  // Finaliza la sección crítica
    return NULL;
}

int main() {
    size_t jobs[3];
    pthread_t hilos[3];
    int i = 0;
    int error;

    if (pthread_mutex_init(&BLOQUEO, NULL) != 0) {
        printf("\nFallo de inicialización de MUTEX\n");
        return 1;
    }

    while (i < 3) {
        jobs[i] = i;
        error = pthread_create(&hilos[i], NULL, &function, jobs + i);
        if (error != 0) {
            printf("\nError en la creación de hilos: [%s]", strerror(error));
        }
        i++;
    }

    pthread_join(hilos[0], NULL);
    pthread_join(hilos[1], NULL);
    pthread_join(hilos[2], NULL);

    pthread_mutex_destroy(&BLOQUEO);

    return 0;
}
