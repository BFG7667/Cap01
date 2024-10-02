/*********************************************************************************************
Autor: B. Fajardo
Fecha: 01/10/2024
Materia: Sistemas Operativos
Tema: Comunicación entre Procesos
Tópico: Uso de FIFO (named pipe) para la comunicación bidireccional
Descripción:
Este archivo es el código del **cliente** que se comunica con un servidor
mediante una FIFO (pipe con nombre). El cliente envía cadenas al servidor, que
las procesa (invirtiéndolas) y luego las devuelve al cliente. El cliente sigue
enviando mensajes hasta que el usuario introduce la palabra "end", momento en el
cual finaliza la comunicación y se cierra la FIFO.

Archivo Cliente: **clienteBidireccional.c**
**********************************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_FILE "/tmp/fifo_twoway" // Definición del archivo FIFO

int main() {
  int fd;           // Descriptor de archivo para el FIFO
  int end_process;  // Variable para controlar cuándo finalizar el proceso
  int stringlen;    // Longitud de la cadena ingresada por el usuario
  int read_bytes;   // Bytes leídos desde el FIFO
  char readbuf[80]; // Buffer para enviar y recibir datos
  char end_str[5];  // Cadena "end" para terminar la comunicación

  printf("FIFO_CLIENT: Enviar mensajes infinitamente, para terminar ingrese "
         "\"end\"\n");

  /* Abrir la FIFO */
  fd = open(FIFO_FILE, O_CREAT | O_RDWR, 0666); // Agregar los permisos
  if (fd == -1) {
    perror("Error al abrir la FIFO");
    return 1;
  }

  strcpy(end_str, "end"); // Almacena la cadena "end" para comparación

  /* Bucle infinito para enviar mensajes al servidor */
  while (1) {
    printf("Ingrese una cadena: ");
    if (fgets(readbuf, sizeof(readbuf), stdin) ==
        NULL) { // Verifica que fgets() funciona correctamente
      perror("Error al leer la entrada");
      return 1;
    }

    stringlen = strlen(readbuf); // Calcula la longitud de la cadena
    readbuf[stringlen - 1] =
        '\0'; // Reemplaza el salto de línea con un terminador nulo

    /* Comparar la cadena con "end" para decidir si continuar o terminar */
    end_process = strcmp(readbuf, end_str);

    if (end_process != 0) {
      /* Enviar cadena al servidor si no es "end" */
      ssize_t bytes_written = write(fd, readbuf, strlen(readbuf));
      if (bytes_written == -1) {
        perror("Error al escribir en la FIFO");
        return 1;
      }
      printf("FIFOCLIENT: Enviada cadena: \"%s\", longitud: %d\n", readbuf,
             (int)strlen(readbuf));

      /* Leer la respuesta del servidor */
      read_bytes =
          read(fd, readbuf, sizeof(readbuf)); // Lee la respuesta del servidor
      if (read_bytes == -1) {
        perror("Error al leer de la FIFO");
        return 1;
      }
      readbuf[read_bytes] = '\0'; // Añadir terminador nulo a la cadena recibida
      printf("FIFOCLIENT: Recibida cadena: \"%s\", longitud: %d\n", readbuf,
             (int)strlen(readbuf));
    } else {
      /* Si la cadena es "end", finalizar la comunicación */
      ssize_t bytes_written =
          write(fd, readbuf, strlen(readbuf)); // Envía "end" al servidor
      if (bytes_written == -1) {
        perror("Error al escribir en la FIFO");
        return 1;
      }
      printf("FIFOCLIENT: Enviada cadena: \"%s\", longitud: %d\n", readbuf,
             (int)strlen(readbuf));
      close(fd); // Cierra la FIFO
      break;     // Sale del bucle
    }
  }

  return 0;
}
