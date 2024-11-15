/*********************************************************************************************
Autor: B. Fajardo
Fecha: 2024
Materia: Sistemas Operativos
Tópico: Creación y manejo de hilos con pthread
Modo de compilación y ejecución:
    gcc -o creacion_hilo_pthread creacion_hilo_pthread.c -lpthread
    Esto compila el programa y genera el ejecutable `creacion_hilo_pthread`.

    ./creacion_hilo_pthread
    Esto ejecuta el programa principal, que crea un hilo secundario y espera su finalización.

Descripción del programa:
    Este programa crea un hilo independiente utilizando la biblioteca pthread en C. El hilo
    ejecuta una función que imprime un mensaje de inicio, duerme por 2 segundos, y luego
    imprime un mensaje de finalización. El programa principal espera a que el hilo termine
    antes de imprimir su mensaje final, demostrando el uso básico de pthreads para gestionar
    la concurrencia en un programa.

Archivos para el funcionamiento del programa:
    creacion_hilo_pthread.c  Código fuente del programa que crea y gestiona un hilo.
**********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

void *function(void *arg) {
    printf("Función Hilo: INICIO \n");
    sleep(2);  // espera 2 segundos (dormir)
    printf("Función Hilo: FIN\n");
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t hilo;  // se identifica el hilo

    // Se crea el hilo, se envía la función
    int hiloCreacion = pthread_create(&hilo, NULL, &function, NULL);

    // Se verifica que el hilo se creó exitosamente
    if (hiloCreacion) {
        printf("Fallo de creación del hilo :: %d \n", hiloCreacion);
    } else {
        printf("Hilo creado con ID :: %d \n", hiloCreacion);
    }

    printf("Espera de salida del hilo...\n");

    // Se espera que el proceso del hilo termine
    hiloCreacion = pthread_join(hilo, NULL);
    if (hiloCreacion) {
        printf("Fallo de salida del hilo :: %d \n", hiloCreacion);
    }

    printf("Final de programa principal\n");
    return 0;
}
