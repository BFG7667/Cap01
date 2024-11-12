#include <stdio.h>
#include <stdlib.h>

// Función para leer un archivo y cargar los enteros en un arreglo dinámico
void leer_archivo(const char* archivo, int* arreglo, int n) {
    FILE* f = fopen(archivo, "r"); // Abrir el archivo en modo lectura
    if (f == NULL) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        if (fscanf(f, "%d", &arreglo[i]) != 1) {
            printf("Error al leer el archivo %s\n", archivo);
            fclose(f);
            exit(EXIT_FAILURE);
        }
    }

    fclose(f); // Cerrar el archivo después de leer
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Uso: %s N1 archivo00 N2 archivo01\n", argv[0]);
        return 1;
    }

    // Convertir los argumentos N1 y N2 a enteros
    int N1 = atoi(argv[1]);
    int N2 = atoi(argv[3]);

    // Asignar memoria dinámica para los arreglos
    int* arreglo1 = (int*)malloc(N1 * sizeof(int));
    int* arreglo2 = (int*)malloc(N2 * sizeof(int));

    if (arreglo1 == NULL || arreglo2 == NULL) {
        printf("Error al asignar memoria\n");
        return 1;
    }

    // Leer los archivos y cargar los datos en los arreglos
    leer_archivo(argv[2], arreglo1, N1);
    leer_archivo(argv[4], arreglo2, N2);

    // Imprimir los arreglos leídos
    printf("Arreglo 1:\n");
    for (int i = 0; i < N1; i++) {
        printf("%d ", arreglo1[i]);
    }
    printf("\n");

    printf("Arreglo 2:\n");
    for (int i = 0; i < N2; i++) {
        printf("%d ", arreglo2[i]);
    }
    printf("\n");

    // Liberar la memoria asignada
    free(arreglo1);
    free(arreglo2);

    return 0;
}
