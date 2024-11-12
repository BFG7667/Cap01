/*********************************************************************************************
Autor: B. Fajardo
Fecha: 01/10/2024
Materia: Sistemas Operativos
Tema: Comunicación entre Procesos
Tópico: Uso de FIFO (named pipe) para la comunicación bidireccional
Descripción:
Este archivo es el código del **servidor** que utiliza una FIFO (pipe con
nombre) para implementar la comunicación bidireccional entre procesos. El
servidor lee datos enviados por el cliente a través de la FIFO, invierte la
cadena recibida y la envía de vuelta al cliente. El servidor sigue ejecutándose
hasta que recibe la cadena "end", momento en el cual se cierra la FIFO y el
servidor finaliza.

Archivo Servidor: **serverBidireccional.c**
**********************************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_FILE "/tmp/fifo_twoway" // Definición del archivo FIFO

void reverse_string(char *str) {
  int len = strlen(str);
  for (int i = 0; i < len / 2; i++) {
    char temp = str[i];
    str[i] = str[len - i - 1];
    str[len - i - 1] = temp;
  }
}

int main() {
  int fd;           // Descriptor de archivo para el FIFO
  char readbuf[80]; // Buffer para leer los datos del FIFO
  char end[10];     // Almacena la palabra "end" para comparación
  int to_end;       // Variable para comparar la entrada con "end"
  int read_bytes;   // Almacena el número de bytes leídos del FIFO

  /* Crear la FIFO si no existe */
  mkfifo(FIFO_FILE, S_IFIFO | 0640); // Crea el archivo FIFO con permisos 0640
  strcpy(end, "end"); // Almacena la cadena "end" para finalizar el bucle

  /* Abrir la FIFO para lectura y escritura */
  fd = open(FIFO_FILE, O_RDWR); // Abre la FIFO en modo de lectura y escritura

  while (1) {
    /* Leer datos desde la FIFO */
    read_bytes = read(fd, readbuf, sizeof(readbuf)); // Lee desde el FIFO
    readbuf[read_bytes] =
        '\0'; // Añade terminador nulo para tratarlo como string
    printf("FIFOSERVER: Recibido string: \"%s\", longitud: %d\n", readbuf,
           (int)strlen(readbuf));

    /* Comparar la cadena recibida con "end" para terminar el servidor */
    to_end = strcmp(readbuf, end);

    if (to_end == 0) {
      close(fd); // Cierra el descriptor de archivo
      break;     // Finaliza el bucle si se recibe la cadena "end"
    }

    /* Invertir la cadena recibida */
    reverse_string(readbuf); // Invierte la cadena
    printf("FIFOSERVER: Enviando cadena invertida: \"%s\", longitud: %d\n",
           readbuf, (int)strlen(readbuf));

    /* Verificar la escritura en el FIFO */
    ssize_t bytes_written = write(
        fd, readbuf, strlen(readbuf)); // Escribe la cadena invertida en el FIFO
    if (bytes_written == -1) {
      perror("Error al escribir en el FIFO");
      close(fd);
      return 1;
    }

    /*
    sleep - Esto es para asegurarse de que el otro proceso lea el mensaje,
    de lo contrario, este proceso podría recuperar el mensaje antes de que el
    cliente lo lea.
    */
    sleep(2); // Espera 2 segundos antes de la siguiente iteración
  }

  return 0;
}
