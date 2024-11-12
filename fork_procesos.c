/*********************************************************************************************
Autor: B. Fajardo
Fecha: 01/10/2024
Materia: Sistemas Operativos
Tema: Creación de Procesos
Tópico: Uso de la función fork()
Descripción:
Este programa demuestra la creación de múltiples procesos hijos utilizando la
función `fork()`. El número de procesos a crear se especifica como argumento al
ejecutar el programa. Cada vez que se crea un nuevo proceso, se imprime un
mensaje indicando si es el proceso padre o hijo, junto con su PID.
**********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void creacion_fork(int linea) {
  pid_t proceso;
  proceso = fork(); // Crea un nuevo proceso

  if (proceso < 0) { // Si `fork()` devuelve un valor negativo, hubo un error al
                     // crear el proceso
    printf("Error creación proceso\n");
    exit(1);                 // Finaliza el programa en caso de error
  } else if (proceso == 0) { // Si `fork()` devuelve 0, significa que estamos en
                             // el proceso hijo
    printf("%d: Proceso =HIJO= PID: %d\n", linea, getpid());
  } else { // Si `fork()` devuelve un valor positivo, estamos en el proceso
           // padre
    printf("%d: Proceso =PADRE= PID: %d\n", linea, getpid());
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) { // Verifica que el número de argumentos sea correcto
    printf("Uso: %s <número de procesos>\n", argv[0]);
    return 1;
  }

  int p = atoi(
      argv[1]); // Convierte el argumento a entero (número de procesos a crear)
  for (int i = 0; i < p; i++) {
    creacion_fork(i); // Crea el número especificado de procesos
  }

  printf("\n---\n\n"); // Separador visual
  return 0;
}
