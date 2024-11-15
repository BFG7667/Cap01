/*********************************************************************************************
Autor: B. Fajardo
Fecha: 2024
Materia: Sistemas Operativos
Tópico: Implementación del patrón Productor-Consumidor con pthreads
Modo de compilación y ejecución:
    gcc -o productor_consumidor productor_consumidor.c -lpthread
    Esto compila el programa y genera el ejecutable `productor_consumidor`.

    ./productor_consumidor
    Esto ejecuta el programa principal, que crea varios hilos productores y consumidores
    que interactúan para incrementar y decrementar un contador compartido.

Descripción del programa:
    Este programa implementa el patrón de sincronización Productor-Consumidor utilizando
    la biblioteca pthread en C. Los productores incrementan un contador compartido hasta un 
    máximo y notifican a los consumidores cuando hay datos disponibles. Los consumidores
    decrementan el contador y notifican a los productores cuando el contador está por debajo
    del límite. Se utilizan mutexes y variables de condición para sincronizar el acceso al
    contador compartido y evitar condiciones de carrera.

Archivos para el funcionamiento del programa:
    productor_consumidor.c  Código fuente del programa que implementa el patrón productor-consumidor.
**********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_PROD_CONS 5
#define COUNTER_LIMIT 10
#define ITERATIONS 20

int counter = 0;
int total_operations = 0;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condVarProd = PTHREAD_COND_INITIALIZER;
pthread_cond_t condVarCons = PTHREAD_COND_INITIALIZER;

void *productor(void *arg) {
    while (1) {
        pthread_mutex_lock(&counter_mutex);
        
        // Esperar si el contador ha alcanzado el límite o si se alcanzó el límite de operaciones
        while (counter >= COUNTER_LIMIT && total_operations < ITERATIONS) {
            pthread_cond_wait(&condVarProd, &counter_mutex);
        }
        
        if (total_operations >= ITERATIONS) {
            pthread_mutex_unlock(&counter_mutex);
            break;
        }

        counter++;
        total_operations++;
        printf("Soy productor %ld, valor contador = %d\n", pthread_self(), counter);
        
        // Notificar a los consumidores
        pthread_cond_broadcast(&condVarCons);
        
        pthread_mutex_unlock(&counter_mutex);
        sleep(1);
    }
    return NULL;
}

void *consumidor(void *arg) {
    while (1) {
        pthread_mutex_lock(&counter_mutex);
        
        // Esperar si el contador está en 0 y se necesita consumir
        while (counter <= 0 && total_operations < ITERATIONS) {
            pthread_cond_wait(&condVarCons, &counter_mutex);
        }
        
        if (total_operations >= ITERATIONS && counter <= 0) {
            pthread_mutex_unlock(&counter_mutex);
            break;
        }

        printf("Soy consumidor %ld, valor contador = %d\n", pthread_self(), counter);
        counter--;
        
        // Notificar a los productores
        pthread_cond_broadcast(&condVarProd);
        
        pthread_mutex_unlock(&counter_mutex);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t proHilo[MAX_PROD_CONS], conHilo[MAX_PROD_CONS];
    int i;

    for (i = 0; i < MAX_PROD_CONS; i++) {
        pthread_create(&proHilo[i], NULL, productor, NULL);
        pthread_create(&conHilo[i], NULL, consumidor, NULL);
    }

    for (i = 0; i < MAX_PROD_CONS; i++) {
        pthread_join(proHilo[i], NULL);
        pthread_join(conHilo[i], NULL);
    }

    printf("Finalizando programa después de %d operaciones.\n", ITERATIONS);
    return 0;
}
