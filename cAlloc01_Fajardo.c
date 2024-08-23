/*********************************************************************************************
Autor: B. Fajardo
Fecha: 13/08/2024
Materia: Sistemas Operativos
Tema: Gestión de Memoria
Tópico: Reserva de Memoria con la función cAlloc()
La función cAlloc() reserva un bloque de memoria de tamaño requerido. 
La asignación es dinamica, y util para complejas estructuras de datos.
Devuelve un puntero señalando el inicio del bloque reservado.
**********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main() {
  /* se declaran las variables y punteros necesarios*/
  int *p1 = calloc(4, sizeof(int)); // Asignación de vector 4 tamaño int
  int *puntero = calloc(1, sizeof(int[4])); // Bloque de memoria de 1 int
  int *p3 = calloc(1, sizeof *p3); //Bloque de memoria de 5 int

  /*Bucle para llenar los espacios Primer Bloque Reservado (puntero)*/

  printf("Construcción y calculo de la secuencia de los primeros 10 términos \n");
  for (int i = 0; i < 10; i++) {
    *(puntero+i) = i; // Se llena la posición 9na con el valor i
    printf("El valor de la %d posición de puntero es %d\n\n", i, *(puntero+i));
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