/*********************************************************************************************
Autor: B. Fajardo
Fecha: 24/09/2024
Materia: Sistemas Operativos
Tema: Comunicación entre procesos
Tópico: Uso de FIFO en Linux (Servidor)
Descripción:
Este programa implementa un servidor FIFO que recibe cadenas de texto desde un
cliente FIFO y las imprime en la terminal. El servidor continuará leyendo
mensajes hasta que reciba la palabra "end", lo que provocará la finalización del
proceso.
**********************************************************************************************/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // Para mknod y S_IFIFO
#include <unistd.h>

#define FIFO_FILE "MYFIFO" // Nombre del archivo FIFO

int main() {
  int fd;
  char readbuf[80];
  char end[10];
  int to_end;
  int read_bytes;

  // Crear el FIFO si no existe
  if (access(FIFO_FILE, F_OK) == -1) { // Verifica si el FIFO ya existe
    if (mknod(FIFO_FILE, S_IFIFO | 0640, 0) == -1) {
      perror("Error al crear FIFO");
      exit(EXIT_FAILURE);
    }
  }

  strcpy(end, "end");

  // Bucle infinito para leer mensajes del cliente
  while (1) {
    fd = open(FIFO_FILE, O_RDONLY); // Abrimos la FIFO en modo solo lectura
    if (fd == -1) {
      perror("Error al abrir la FIFO");
      exit(EXIT_FAILURE);
    }

    // Leemos el mensaje del cliente
    read_bytes = read(fd, readbuf, sizeof(readbuf) - 1);
    if (read_bytes == -1) {
      perror("Error al leer de la FIFO");
      close(fd);
      exit(EXIT_FAILURE);
    }

    readbuf[read_bytes] =
        '\0'; // Aseguramos que el buffer sea una cadena válida
    printf("Mensaje recibido: \"%s\" con %d caracteres\n", readbuf,
           (int)strlen(readbuf));

    // Comparamos la cadena recibida con "end"
    to_end = strcmp(readbuf, end);

    // Si el mensaje es "end", cerramos el archivo y terminamos el bucle
    if (to_end == 0) {
      close(fd); // Cerramos la FIFO
      break;     // Salimos del bucle
    }

    close(fd); // Cerramos la FIFO después de leer cada mensaje
  }

  return 0; // Finalizamos el programa
}
