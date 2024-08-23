/*********************************************************************************************
Autor: B. Fajardo
Fecha: 30/07/2024
Materia: Sistemas Operativos
Tema: Argumentos de entrada en C
**********************************************************************************************/

#include <stdio.h>

/**Al inicio del programa con el valor de la edad**/
/**Se presentan una serie de preguntas que el usuario debe responder**/

int main(int argc, char *argv[]){

  printf("\n Ha ingresado [%d]: \n\n", argc);
  printf("\n Ha ingesado la edad: %s\n\n;", argv[0]);
  printf("\n Ha ingesado el mes: %s\n\n;", argv[1]);
  
  int x = 102;
  int *p = x;
  printf("\n El valor X es de: %d \n", x);
  printf("\n La dirección de X es: %p \n\n", p);
  
  int vector [] = {5,10,20,30,40,50,80};
  int *p1 = vector;
  int *p2 = vector+20;
  int diferencia  = p2-p1;
  
  printf("\n El valor p2 es de: %p \n", p1);
  printf("\n El valor p2 es de: %p \n", p2);
  printf("\n El valor resta de: %d \n", diferencia);
  
  printf("\n");
    
return 0;
  
}