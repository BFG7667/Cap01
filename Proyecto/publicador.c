/*********************************************************************************************
Autor: B. Fajardo
Fecha: 2024
Materia: Sistemas Operativos
Tópico: Implementación del patrón Publicador/Suscriptor
Modo de compilación y ejecución:
    gcc -o publicador publicador.c
    Esto compila el programa y genera el ejecutable `publicador`.

    ./publicador -p pipePSC -f file -t timeN
    Esto ejecuta el programa, que envía noticias al Sistema de Comunicación a través del pipe especificado.
    
Descripción del programa:
    Este programa implementa un proceso publicador como parte del sistema de noticias que sigue el patrón
    Publicador/Suscriptor. Los publicadores leen noticias desde un archivo y las envían al sistema de comunicación
    mediante un pipe nominal. El sistema de comunicación luego se encarga de distribuir estas noticias a los
    suscriptores que están interesados en los tópicos específicos.

    El archivo de noticias debe tener un formato específico: cada línea comienza con una letra mayúscula que
    representa la categoría de la noticia, seguida de un símbolo de dos puntos y el texto de la noticia. Las noticias
    se envían una por una, esperando un tiempo configurable entre cada envío.

Archivos para el funcionamiento del programa:
    publicador.c, suscriptor.c y sistema.c Código fuente de los tres programas del sistema Publicador/Suscriptor
**********************************************************************************************/

// publicador.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>

#define MAX_BUFFER_SIZE 200
#define MAX_NEWS_LENGTH 200

volatile sig_atomic_t terminar = 0;

void mostrar_uso() {
    fprintf(stderr, "Uso: publicador -p pipePSC -f file -t timeN\n");
    exit(EXIT_FAILURE);
}

void manejar_senal(int signo) {
    if (signo == SIGINT) {
        terminar = 1;
    }
}

int validar_noticia(const char *noticia) {
    // Verifica longitud y formato (categoria: contenido).
    if (strlen(noticia) > MAX_NEWS_LENGTH) {
        fprintf(stderr, "Error: La noticia excede el límite de %d caracteres: %s\n", MAX_NEWS_LENGTH, noticia);
        return 0;
    }
    if (noticia[0] < 'A' || noticia[0] > 'Z' || noticia[1] != ':' || noticia[2] != ' ') {
        fprintf(stderr, "Formato incorrecto en la noticia: %s\n", noticia);
        return 0;
    }
    return 1;
}

int enviar_noticia(const char *noticia, int fd) {
    ssize_t bytes_escritos = write(fd, noticia, strlen(noticia));
    if (bytes_escritos == -1) {
        perror("Error al escribir en el pipe");
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    char *pipePSC = NULL;
    char *file = NULL;
    int timeN = 0;

    // Parseo de argumentos
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            pipePSC = argv[++i];
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            file = argv[++i];
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            timeN = atoi(argv[++i]);
            if (timeN <= 0) {
                fprintf(stderr, "Error: El tiempo debe ser un valor positivo.\n");
                mostrar_uso();
            }
        } else {
            mostrar_uso();
        }
    }

    if (!pipePSC || !file) {
        mostrar_uso();
    }

    // Manejo de señal SIGINT
    signal(SIGINT, manejar_senal);

    // Crear pipe si no existe
    if (access(pipePSC, F_OK) == -1) {
        if (mkfifo(pipePSC, 0666) == -1) {
            perror("Error al crear el pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Abrir archivo de noticias
    FILE *fp = fopen(file, "r");
    if (!fp) {
        perror("Error al abrir el archivo de noticias");
        exit(EXIT_FAILURE);
    }

    // Abrir pipe
    int fd = open(pipePSC, O_WRONLY);
    if (fd == -1) {
        perror("Error al abrir el pipe");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    // Envío de noticias
    char buffer[MAX_BUFFER_SIZE];
    while (!terminar && fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\n")] = 0; // Eliminar salto de línea

        if (!validar_noticia(buffer)) {
            continue; // Omitir noticias no válidas
        }

        if (enviar_noticia(buffer, fd) == -1) {
            break; // Error al enviar la noticia
        }

        sleep(timeN); // Esperar antes de la siguiente noticia
    }

    // Cierre seguro
    fclose(fp);
    close(fd);

    printf("Publicador finalizado de manera segura.\n");
    return 0;
}
