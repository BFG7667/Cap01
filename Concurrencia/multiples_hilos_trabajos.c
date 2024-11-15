/*********************************************************************************************
Autor: B. Fajardo
Fecha: 2024
Materia: Sistemas Operativos
Tópico: Creación de múltiples hilos para la ejecución de trabajos paralelos
Modo de compilación y ejecución:
    gcc -o multiples_hilos_trabajos multiples_hilos_trabajos.c -lpthread
    Esto compila el programa y genera el ejecutable `multiples_hilos_trabajos`.

    ./multiples_hilos_trabajos
    Esto ejecuta el programa principal, que crea múltiples hilos, cada uno ejecutando un trabajo
    específico definido por un índice.

Descripción del programa:
    Este programa crea un número fijo de hilos utilizando la biblioteca pthread en C. Cada hilo
    ejecuta una función que imprime el número de "job" (trabajo) asignado, demostrando cómo
    crear y manejar múltiples hilos de forma concurrente en un sistema. La función principal 
    espera a que todos los hilos finalicen antes de terminar el programa.

Archivos para el funcionamiento del programa:
    multiples_hilos_trabajos.c  Código fuente del programa que crea y gestiona múltiples hilos.
**********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define N 5  // Número de trabajos e hilos

static void *function(void *arg) {
    size_t job = *(size_t *)arg;
    printf("Job %zu\n", job);
    return NULL;
}

int main(int argc, char *argv[]) {
    size_t jobs[N];           // Vector de trabajos
    pthread_t hilos[N];       // Vector de hilos

    for (size_t i = 0; i < N; i++) {
        jobs[i] = i;
        pthread_create(&hilos[i], NULL, function, &jobs[i]);
    }

    for (size_t i = 0; i < N; i++) {
        pthread_join(hilos[i], NULL);  // Espera por cada hilo hasta que finalice
    }

    return 0;
}
