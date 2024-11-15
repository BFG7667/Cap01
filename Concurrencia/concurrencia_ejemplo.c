/*********************************************************************************************
Autor: B. Fajardo
Fecha: 2024
Materia: Sistemas Operativos
Tópico: Ejemplo de Concurrencia y Sincronización de Hilos con Mutex
Modo de compilación y ejecución:
    gcc -o concurrencia_ejemplo concurrencia_ejemplo.c -lpthread
    Esto compila el programa y genera el ejecutable `concurrencia_ejemplo`.

    ./concurrencia_ejemplo
    Esto ejecuta el programa principal, que crea varios hilos que imprimen mensajes de inicio
    y finalización de manera secuencial usando un mutex para sincronización.

Descripción del programa:
    Este programa crea y ejecuta tres hilos en paralelo utilizando la biblioteca pthread en C.
    Cada hilo imprime un mensaje de inicio, realiza una operación simulada, y luego imprime un
    mensaje de finalización. Se utiliza un mutex para asegurar que los mensajes de cada hilo se
    impriman en orden, evitando el entrelazado de mensajes en la salida. Este ejemplo ilustra el
    concepto de concurrencia y cómo evitar condiciones de carrera usando sincronización.

**********************************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

// Definimos un mutex global para controlar el acceso a la sección crítica.
pthread_mutex_t mutex;

// Función que será ejecutada por cada hilo
void *funcion_hilo(void *arg) {
    // Bloquea el mutex para que solo un hilo a la vez entre en la sección crítica
    pthread_mutex_lock(&mutex);

    // Obtenemos el identificador del hilo (un valor de 0 a 2)
    size_t id = *(size_t *)arg;
    printf("Hilo %zu ha iniciado\n", id);

    // Simulación de una operación (con una espera) dentro de la sección crítica
    for (int i = 0; i < 5; i++) {
        printf("Hilo %zu está trabajando...\n", id);
        sleep(1);  // Simula trabajo con una espera de 1 segundo
    }

    printf("Hilo %zu ha finalizado\n", id);

    // Libera el mutex para que otros hilos puedan entrar en la sección crítica
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    // Creamos un array para almacenar los identificadores de los hilos
    pthread_t hilos[3];
    size_t id_hilos[3]; // Array para almacenar los IDs de cada hilo
    int error;

    // Inicializamos el mutex antes de crear los hilos
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Error al inicializar el mutex\n");
        return 1;
    }

    // Creamos tres hilos y les asignamos un ID único
    for (size_t i = 0; i < 3; i++) {
        id_hilos[i] = i;
        // Creamos un hilo y le pasamos el ID como argumento
        error = pthread_create(&hilos[i], NULL, funcion_hilo, &id_hilos[i]);
        if (error != 0) {
            printf("Error al crear el hilo %zu: %s\n", i, strerror(error));
        }
    }

    // Esperamos a que todos los hilos terminen
    for (int i = 0; i < 3; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Destruimos el mutex después de que todos los hilos han terminado
    pthread_mutex_destroy(&mutex);

    printf("Todos los hilos han terminado.\n");

    return 0;
}
