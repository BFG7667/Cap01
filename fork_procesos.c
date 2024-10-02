/*********************************************************************************************
Autor: B. Fajardo
Fecha: 01/10/2024
Materia: Sistemas Operativos
Tema: Creación de Procesos
Tópico: Uso de la función fork()
Descripción:
Este programa implementa la creación de múltiples procesos hijos utilizando la
función `fork()`. El número de procesos a crear se especifica como argumento al
ejecutar el programa. Cada vez que se crea un nuevo proceso, se imprime un
mensaje indicando si es el proceso padre o hijo, incluyendo su identificador de
proceso (PID), el identificador del proceso padre (PPID) y su generación.
**********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void creacion_fork(int linea, int generacion) {
  pid_t proceso;
  proceso = fork(); // Crea un nuevo proceso

  if (proceso < 0) { // Error en la creación del proceso
    printf("Error creación proceso\n");
    exit(1);
  } else if (proceso == 0) { // Proceso hijo
    printf("%d: Proceso =HIJO= PID: %d, PPID (padre): %d, Generación: %d\n",
           linea, getpid(), getppid(), generacion + 1);
    exit(0); // El proceso hijo termina aquí para evitar que continúe creando
             // más forks
  } else {   // Proceso padre
    printf("%d: Proceso =PADRE= PID: %d, Hijo PID: %d, Generación: %d\n", linea,
           getpid(), proceso, generacion);
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Uso: %s <número de procesos>\n", argv[0]);
    return 1;
  }

  char *end;
  int p = strtol(argv[1], &end,
                 10); // Convierte el argumento en número entero de forma segura
  if (*end != '\0' || p <= 0) {
    printf("Error: El argumento debe ser un número entero positivo.\n");
    return 1;
  }

  int generacion = 1; // Definimos la primera generación como 1

  for (int i = 0; i < p; i++) {
    creacion_fork(i, generacion); // Pasa la generación actual
  }

  for (int i = 0; i < p; i++) {
    wait(NULL); // El proceso padre espera a que los hijos terminen
  }

  printf("\n---\n\n");
  return 0;
}
