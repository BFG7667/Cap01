/*********************************************************************************************
Autor: B. Fajardo
Fecha: 2024
Materia: Sistemas Operativos
Tópico: Implementación del patrón Publicador/Suscriptor
Modo de compilación y ejecución:
    gcc -o sistema sistema.c -lpthread
    Esto compila el programa y genera el ejecutable `sistema`.

    ./sistema -p pipePSC -s pipeSSC -t timeF
    Esto ejecuta el programa, que coordina la recepción de noticias desde los publicadores y las envía a los
    suscriptores correspondientes a través de los pipes especificados.

Descripción del programa:
    Este programa implementa el Sistema de Comunicación (SC) como parte del sistema de noticias que sigue
    el patrón Publicador/Suscriptor. El Sistema de Comunicación recibe noticias de los publicadores a través de
    un pipe nominal y las distribuye a los suscriptores que se hayan suscrito a los tópicos de esas noticias. El
    sistema también gestiona las suscripciones de los suscriptores.

Archivos para el funcionamiento del programa:
    - publicador.c, suscriptor.c y sistema.c Código fuente de los tres programas del sistema Publicador/Suscriptor
**********************************************************************************************/

// sistema.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <signal.h>

#define MAX_BUFFER_SIZE 200
#define MAX_SUBSCRIBERS 10

typedef struct {
    int fd;
    char topics[MAX_BUFFER_SIZE];
} Suscriptor;

Suscriptor suscriptores[MAX_SUBSCRIBERS];
int num_suscriptores = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
volatile sig_atomic_t terminar = 0;

void *gestionar_publicaciones(void *arg);
void *gestionar_suscripciones(void *arg);
void manejar_senal(int signo);
int agregar_suscriptor(const char *pipe_name, const char *topics);
void enviar_noticia_a_suscriptores(const char *noticia);
void notificar_fin_a_suscriptores(); // Nueva función para notificar fin de emisión

int main(int argc, char *argv[]) {
    char *pipePSC = NULL, *pipeSSC = NULL;
    int timeF = 0;

    // Parseo de argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            pipePSC = argv[++i];
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            pipeSSC = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            timeF = atoi(argv[++i]);
            if (timeF <= 0) {
                fprintf(stderr, "Error: El tiempo debe ser un valor positivo.\n");
                exit(EXIT_FAILURE);
            }
        } else {
            fprintf(stderr, "Uso: sistema -p pipePSC -s pipeSSC -t timeF\n");
            exit(EXIT_FAILURE);
        }
    }

    if (!pipePSC || !pipeSSC) {
        fprintf(stderr, "Uso: sistema -p pipePSC -s pipeSSC -t timeF\n");
        exit(EXIT_FAILURE);
    }

    // Manejo de señal SIGINT para terminar el proceso de manera segura
    signal(SIGINT, manejar_senal);

    // Crear los pipes si no existen
    if (mkfifo(pipePSC, 0666) == -1 && access(pipePSC, F_OK) == -1) {
        perror("Error al crear el pipe de publicaciones");
        exit(EXIT_FAILURE);
    }

    if (mkfifo(pipeSSC, 0666) == -1 && access(pipeSSC, F_OK) == -1) {
        perror("Error al crear el pipe de suscripciones");
        exit(EXIT_FAILURE);
    }

    // Crear hilos para gestionar publicaciones y suscripciones
    pthread_t hilo_publicaciones, hilo_suscripciones;
    pthread_create(&hilo_publicaciones, NULL, gestionar_publicaciones, (void *)pipePSC);
    pthread_create(&hilo_suscripciones, NULL, gestionar_suscripciones, (void *)pipeSSC);

    // Esperar a que terminen los hilos
    pthread_join(hilo_publicaciones, NULL);
    sleep(timeF); // Esperar timeF segundos por si llegan más publicaciones o suscriptores

    // Notificar a los suscriptores que la emisión ha terminado
    notificar_fin_a_suscriptores();

    pthread_cancel(hilo_suscripciones);

    printf("Sistema de Comunicación finalizado.\n");
    return 0;
}

void manejar_senal(int signo) {
    if (signo == SIGINT) {
        terminar = 1;
    }
}

int agregar_suscriptor(const char *pipe_name, const char *topics) {
    pthread_mutex_lock(&mutex);

    if (num_suscriptores >= MAX_SUBSCRIBERS) {
        fprintf(stderr, "Error: Número máximo de suscriptores alcanzado.\n");
        pthread_mutex_unlock(&mutex);
        return -1;
    }

    suscriptores[num_suscriptores].fd = open(pipe_name, O_WRONLY);
    if (suscriptores[num_suscriptores].fd == -1) {
        perror("Error al abrir el pipe para el suscriptor");
        pthread_mutex_unlock(&mutex);
        return -1;
    }
    strcpy(suscriptores[num_suscriptores].topics, topics);
    num_suscriptores++;
    printf("Suscriptor agregado: %s\n", topics);

    pthread_mutex_unlock(&mutex);
    return 0;
}

void enviar_noticia_a_suscriptores(const char *noticia) {
    pthread_mutex_lock(&mutex);

    for (int i = 0; i < num_suscriptores; i++) {
        if (strchr(suscriptores[i].topics, noticia[0])) {  // Compara la categoría
            if (write(suscriptores[i].fd, noticia, strlen(noticia)) == -1) {
                perror("Error al enviar la noticia al suscriptor");
            } else {
                // Imprimir confirmación de envío al suscriptor
                printf("Noticia enviada a suscriptor %d con temas: %s\n", i + 1, suscriptores[i].topics);
            }
        }
    }

    pthread_mutex_unlock(&mutex);
}

void *gestionar_publicaciones(void *arg) {
    char *pipePSC = (char *)arg;
    int fd = open(pipePSC, O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir el pipe de publicaciones");
        pthread_exit(NULL);
    }

    char buffer[MAX_BUFFER_SIZE];
    while (!terminar) {
        ssize_t bytes_leidos = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_leidos > 0) {
            buffer[bytes_leidos] = '\0';
            printf("Noticia recibida: %s\n", buffer);
            enviar_noticia_a_suscriptores(buffer);
        } else if (bytes_leidos == -1) {
            perror("Error al leer del pipe de publicaciones");
        }
    }

    close(fd);
    pthread_exit(NULL);
}

void *gestionar_suscripciones(void *arg) {
    char *pipeSSC = (char *)arg;
    int fd = open(pipeSSC, O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir el pipe de suscripciones");
        pthread_exit(NULL);
    }

    char buffer[MAX_BUFFER_SIZE];
    while (!terminar) {
        ssize_t bytes_leidos = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_leidos > 0) {
            buffer[bytes_leidos] = '\0';

            // Extrae el nombre del pipe y los tópicos
            char *pipe_name = strtok(buffer, "|");
            char *topics = strtok(NULL, "|");

            if (pipe_name && topics) {
                agregar_suscriptor(pipe_name, topics);
            }
        } else if (bytes_leidos == -1) {
            perror("Error al leer del pipe de suscripciones");
        }
    }

    close(fd);
    pthread_exit(NULL);
}

void notificar_fin_a_suscriptores() {
    pthread_mutex_lock(&mutex);
    const char *mensaje_fin = "Fin de emisión de noticias.";
    for (int i = 0; i < num_suscriptores; i++) {
        if (write(suscriptores[i].fd, mensaje_fin, strlen(mensaje_fin)) == -1) {
            perror("Error al enviar mensaje de fin al suscriptor");
        }
    }
    pthread_mutex_unlock(&mutex);
}
