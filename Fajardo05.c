/*********************************************************************************************
Autor: B. Fajardo
Fecha: 30/07/2024
Materia: Sistemas Operativos
Tema: Argumentos de entrada en C
**********************************************************************************************/

#include <stdio.h>

/**Tabla con los diferentes tipos de datos en C**/

int main(int argc, char *argv[]){
  
     // Carácter
  char c ='A';
  printf("Carácter: %c\n", c);
  
    // Carácter sin signo
  unsigned char uc ='B';
  printf("Carácter: %c\n", uc);
  
    // Entero corto 
  short s = 12345;
  printf("Entero corto: %hd\n", s);
  
    // Entero corto sin signo
  unsigned short us = 54321;
  printf("Entero corto sin signo: %hu\n", us);
  
    // Entero
  int i = 123456;
  printf("Entero: %d\n", i);
  
    // Entero sin signo
  unsigned int ui = 654321;
  printf("Entero sin signo: %u\n", ui);
  
    // Entero largo
  long l = 123456789L;
  printf("Entero largo: %ld\n", l);
  
    // Entero largo sin signo
  unsigned long ul = 987654321UL;
  printf("Entero largo sin signo: %lu\n", ul);

    
return 0;
  
}