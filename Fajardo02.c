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
    
    int a, b;
    
    printf("Introduce el primer entero: \n");
    scanf("%d", &a);
    printf("Introduce el segundo entero: \n");
    scanf("%d", &b);
    int suma = a + b;
    printf("El resultado es: %d\n",  suma);
  }
  
/**Función que calcule el área de una circunferencia**/

  void areacircunferencia(){
    int radio;
    float pi = 3.1416;
    
    printf("Introduce el radio: \n");
    scanf("%d", &radio);
    float area = radio*radio*pi;
    printf("El area es: %f\n", area);
  }



int main(int argc, char *argv[]){
  
  bienvenida();
  suma2enteros();
  areacircunferencia();
    
return 0;
  
}