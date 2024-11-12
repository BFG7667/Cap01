/*********************************************************************************************
Autor: B. Fajardo
Fecha: 23/07/2024
Materia: Sistemas Operativos
Tema: Programación en C: Primera parte
Requerimientos:
        - Función que sume 2 enteros
        - Función que calcule el área de una circunferencia
        - Función que de la bienvenida
**********************************************************************************************/

#include <stdio.h>

/**Función que da la bienvenida**/
  
  void bienvenida(){
    printf("Hola Mundo...\n");
    printf("\t\t Bienvenidos a mi clase de SISTEMAS OPERATIVOS \n");
  }
  
/**Función que sume 2 enteros**/
  
  void suma2enteros(){
    printf("Primer entero es: 7   \n");
    printf("Segundo entero es: 6   \n");
    int suma = 7 + 6;
    printf("El resultado es: %d\n",  suma);
  }
  
/**Función que calcule el área de una circunferencia**/

  void areacircunferencia(){
    int radio = 7;
    float pi = 3.1416;
    
    printf("El radio es: %d\n" , radio);
    float area = radio*radio*pi;
    printf("El area es: %f\n", area);
  }



int main(int argc, char *argv[]){
  
  bienvenida();
  suma2enteros();
  areacircunferencia();
    
return 0;
  
}