/*********************************************************************************************
Autor: B. Fajardo
Fecha: 13/08/2024
Materia: Sistemas Operativos
Tema: Gestión de Memoria
Tópico: Reserva de Memoria con la función malloc()
La función malloc() reserva un bloque de memoria de tamaño bytes y devuelve un
puntero señalando el inicio del bloque reservado.
**********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main() {
  /* se declaran las variables y punteros necesarios*/

  int *bloque00 = malloc(4 * sizeof(int)); // Asignación de vector 4 tamaño int
  int *puntero = malloc(15 * sizeof(*puntero)); // Bloque de memoria de 15 int

  /*Bucle para llenar los espacios Primer Bloque Reservado (bloque00)*/
  for (int i = 0; i < 4; i++) {
    bloque00[i] = i * 2;
    printf("El valor de memoria reservada bloque00[%d] es: %d\n\n", i,
           bloque00[i]);
  }

  /*Bucle para llenar los espacios Primer Bloque Reservado (puntero)*/
  if (puntero != NULL) {
    *(puntero + 8) = 9889; // Se llena la posición 9na con el valor 9889
    printf("El valor de la 9na posición de puntero es %d\n\n", *(puntero + 8));
    printf("El valor de la 9na posición de puntero es %d\n\n", puntero[8]);
  }

  /*LIBERACIÓN O RETORNO DE MEMORIA*/
  free(bloque00); // Liberación del bloque de memoria reservado
  free(puntero);  // Liberación del bloque de memoria reservado

  return 0;
}