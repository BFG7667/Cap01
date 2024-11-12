/*********************************************************************************************
Autor: B. Fajardo
Fecha: 2024
Materia: Sistemas Operativos
Tópico: Implementación del patrón Publicador/Suscriptor
Modo de compilación y ejecución:
    gcc -o suscriptor suscriptor.c
    Esto compila el programa y genera el ejecutable `suscriptor`.

    ./suscriptor -s pipeSSC
    Esto ejecuta el programa, que se suscribe a uno o más tópicos y recibe noticias del Sistema de
    Comunicación a través del pipe especificado.

Descripción del programa:
    Este programa implementa un proceso suscriptor como parte del sistema de noticias que sigue el patrón
    Publicador/Suscriptor. Los suscriptores se conectan al Sistema de Comunicación a través de un pipe nominal
    para suscribirse a diferentes tópicos de noticias. Una vez suscritos, se mantienen a la espera de recibir noticias
    correspondientes a esos tópicos y las muestran en pantalla.

Archivos para el funcionamiento del programa:
    - publicador.c, suscriptor.c y sistema.c Código fuente de los tres programas del sistema Publicador/Suscriptor
**********************************************************************************************/

// suscriptor.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <ctype.h>

#define MAX_BUFFER_SIZE 300
#define VALID_TOPICS "AECPS"

volatile sig_atomic_t terminar = 0;

void mostrar_uso() {
    fprintf(stderr, "Uso: suscriptor -s pipeSSC\n");
    exit(EXIT_FAILURE);
}

void manejar_senal(int signo) {
    if (signo == SIGINT) {
        terminar = 1;
    }
}

// Función para validar tópicos de suscripción
int validar_suscripcion(const char *suscripcion) {
    for (size_t i = 0; i < strlen(suscripcion); i++) {
        if (!strchr(VALID_TOPICS, toupper(suscripcion[i])) && !isspace(suscripcion[i]) && suscripcion[i] != ',') {
            fprintf(stderr, "Error: Tópico de suscripción no válido: %c\n", suscripcion[i]);
            return 0;
        }
    }
    return 1;
}

// Configura la suscripción y envía al sistema de comunicación
int enviar_suscripcion(const char *pipeSSC, const char *suscripcion, char *pipe_name) {
    // Crear un pipe único para el suscriptor
    snprintf(pipe_name, MAX_BUFFER_SIZE / 3, "pipeSSC_%d", getpid());
    if (mkfifo(pipe_name, 0666) == -1 && access(pipe_name, F_OK) == -1) {
        perror("Error al crear el pipe específico para suscriptor");
        return -1;
    }

    // Preparar mensaje de suscripción
    char mensaje[MAX_BUFFER_SIZE];
    snprintf(mensaje, sizeof(mensaje), "%s|%s", pipe_name, suscripcion);

    // Abrir el pipe para enviar la suscripción
    int fd_write = open(pipeSSC, O_WRONLY);
    if (fd_write == -1) {
        perror("Error al abrir el pipe para escritura");
        unlink(pipe_name);
        return -1;
    }

    // Enviar el mensaje de suscripción
    if (write(fd_write, mensaje, strlen(mensaje)) == -1) {
        perror("Error al escribir la suscripción en el pipe");
        close(fd_write);
        unlink(pipe_name);
        return -1;
    }
    close(fd_write);
    return 0;
}

// Recibe noticias en el pipe único del suscriptor
void recibir_noticias(const char *pipe_name) {
    int fd_read = open(pipe_name, O_RDONLY);
    if (fd_read == -1) {
        perror("Error al abrir el pipe para lectura");
        unlink(pipe_name);
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytes_leidos;
    while (!terminar && (bytes_leidos = read(fd_read, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_leidos] = '\0';
        printf("Noticia recibida: %s\n", buffer);
        fflush(stdout);
    }

    if (bytes_leidos == -1) {
        perror("Error al leer del pipe");
    }

    close(fd_read);
    unlink(pipe_name);
}

int main(int argc, char *argv[]) {
    char *pipeSSC = NULL;

    // Parseo de argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            pipeSSC = argv[++i];
        } else {
            mostrar_uso();
        }
    }

    if (!pipeSSC) {
        mostrar_uso();
    }

    // Manejo de señal SIGINT
    signal(SIGINT, manejar_senal);

    // Validar existencia del pipe de suscriptores
    if (access(pipeSSC, F_OK) == -1) {
        fprintf(stderr, "Error: El pipe '%s' no existe.\n", pipeSSC);
        exit(EXIT_FAILURE);
    }

    // Solicitar tópicos al usuario
    char suscripcion[MAX_BUFFER_SIZE / 2];
    printf("Ingrese los tópicos de suscripción (A, E, C, P, S), separados por comas o espacios: ");
    if (!fgets(suscripcion, sizeof(suscripcion), stdin)) {
        perror("Error al leer la suscripción");
        exit(EXIT_FAILURE);
    }
    suscripcion[strcspn(suscripcion, "\n")] = 0; // Eliminar salto de línea

    // Validar suscripción
    if (!validar_suscripcion(suscripcion)) {
        exit(EXIT_FAILURE);
    }

    // Enviar la suscripción al sistema de comunicación
    char pipe_name[MAX_BUFFER_SIZE / 3];
    if (enviar_suscripcion(pipeSSC, suscripcion, pipe_name) == -1) {
        exit(EXIT_FAILURE);
    }

    // Recibir y mostrar noticias
    recibir_noticias(pipe_name);

    printf("Suscriptor finalizado de manera segura.\n");
    return 0;
}
