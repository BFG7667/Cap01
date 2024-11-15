/*********************************************************************************************
Autor: B. Fajardo
Fecha: 2024
Materia: Sistemas Operativos
Tópico: Sincronización de hilos con pthreads
Modo de compilación y ejecución:
    gcc -o sincronizacion_hilos_pthreads sincronizacion_hilos_pthreads.c -lpthread
    Esto compila el programa y genera el ejecutable `sincronizacion_hilos_pthreads`.

    ./sincronizacion_hilos_pthreads
    Esto ejecuta el programa principal, que crea varios hilos que imprimen mensajes de inicio
    y finalización de manera secuencial usando un mutex para sincronización.

Descripción del programa:
    Este programa crea y ejecuta tres hilos en paralelo utilizando la biblioteca pthread en C.
    Cada hilo imprime un mensaje de inicio, realiza una operación simulada, y luego imprime un
    mensaje de finalización. Se utiliza un mutex para asegurar que los mensajes de cada hilo se
    impriman en orden, evitando el entrelazado de mensajes en la salida. Esto ejemplifica el uso
    de un mutex para controlar el acceso a una sección crítica en un entorno concurrente.

Archivos para el funcionamiento del programa:
    sincronizacion_hilos_pthreads.c  Código fuente del programa que implementa la sincronización
                                     de hilos utilizando pthreads y mutexes.
**********************************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;

void *function(void *job) {
    size_t IDh = *(size_t *)job;
    
    pthread_mutex_lock(&mutex);  // Inicia la sección crítica
    printf("\nHilo %zu ha iniciado\n", IDh);

    for (int i = 0; i < 230; i++) {
        // Simulación de trabajo
    }

    printf("\nHilo %zu ha finalizado\n", IDh);
    pthread_mutex_unlock(&mutex);  // Finaliza la sección crítica

    return NULL;
}

int main() {
    size_t jobs[3];
    pthread_t hilos[3];
    int error;

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < 3; i++) {
        jobs[i] = i;
        error = pthread_create(&hilos[i], NULL, &function, &jobs[i]);
        if (error != 0)
            printf("\nError en la creación de hilos: [%s]", strerror(error));
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(hilos[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}
