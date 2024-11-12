/*********************************************************************************************
Autor: B. Fajardo
Fecha: 24/09/2024
Materia: Sistemas Operativos
Tema: Comunicación entre procesos
Tópico: Uso de FIFO en Linux (Cliente)
Descripción:
Este programa implementa un cliente FIFO que permite enviar cadenas de texto a
un servidor a través de una tubería con nombre (FIFO). El programa continuará
enviando mensajes hasta que el usuario ingrese la palabra "end", lo que
provocará la finalización del proceso.
**********************************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FIFO_FILE "MYFIFO" // Nombre del archivo FIFO

int main() {
  int fd;
  int end_process;
  int stringlen;
  char readbuf[80];
  char end_str[5];

  printf("FIFO_CLIENT: Envíe mensajes, infinitamente, para finalizar ingrese "
         "\"end\"\n");

  // Abre el FIFO para escritura
  fd = open(FIFO_FILE, O_CREAT | O_WRONLY, 0640);
  if (fd == -1) {
    perror("Error al abrir el FIFO");
    exit(EXIT_FAILURE);
  }

  strcpy(end_str, "end");

  while (1) {
    printf("Ingrese cadena: ");

    // Verificamos si fgets devuelve NULL (error o fin de entrada)
    if (fgets(readbuf, sizeof(readbuf), stdin) == NULL) {
      perror("Error al leer la entrada");
      continue; // Si hay un error, simplemente continúa el bucle
    }

    stringlen = strlen(readbuf);
    readbuf[stringlen - 1] = '\0'; // Elimina el salto de línea

    end_process = strcmp(readbuf, end_str);

    // Si no es "end", envía el mensaje al FIFO
    if (end_process != 0) {
      if (write(fd, readbuf, strlen(readbuf)) == -1) {
        perror("Error al escribir en el FIFO");
      }
      printf("Cadena enviada: \"%s\" y longitud de cadena es %d\n", readbuf,
             (int)strlen(readbuf));
    } else {
      // Si es "end", envía el mensaje y termina
      if (write(fd, readbuf, strlen(readbuf)) == -1) {
        perror("Error al escribir en el FIFO");
      }
      printf("Cadena enviada: \"%s\" y longitud de cadena es %d\n", readbuf,
             (int)strlen(readbuf));
      close(fd); // Cerramos el FIFO
      break;
    }
  }
  return 0;
}
