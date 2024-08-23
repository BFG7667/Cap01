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
  
   int c, d, e, f, h;
  
  printf("\n");
  
  printf("\n Nombre: %s\n\n", argv[1]);
  printf("\n Apellido: %s\n\n", argv[2]);
  printf("\n Ingrese Edad: \n\n");
  scanf("%d", &c);
  printf("\n Ingrese día Nacimiento: \n\n");
  scanf("%d", &d);
  printf("\n Ingrese mes Nacimiento: \n\n");
  scanf("%d", &e);
  printf("\n Ingrese año Nacimiento: \n\n");
  scanf("%d", &f);
  printf("\n Carrera: %s\n\n", argv[3]);
  printf("\n Ingrese Semestre: \n\n");
  scanf("%d", &h);
  
  printf("\n");
    
return 0;
  
}