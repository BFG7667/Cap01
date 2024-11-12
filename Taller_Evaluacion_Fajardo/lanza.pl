#!/usr/bin/perl
#**************************************************************
#         		Pontificia Universidad Javeriana
#     Autor: Juan Alba
#     Fecha: 10/11/24
#     Materia: Sistemas Operativos
#     Tema: Taller de Evaluación de Rendimiento
#     Fichero: script automatización ejecución por lotes 
#	  Descripcion:El script automatiza la ejecución de dos programas con diferentes tamaños de matriz y números de hilos,
# 	              repitiendo cada combinación 30 veces y guardando los resultados en archivos correspondientes.
#****************************************************************/

# Obtiene el directorio actual de trabajo y lo almacena en la variable $Path
$Path = `pwd`; # `pwd` ejecuta el comando shell para obtener el directorio actual
chomp($Path);  # Elimina el salto de línea al final del resultado de `pwd`

# Arrays que contienen los nombres de los ejecutables, tamaños de matriz y números de hilos
@Nombre_Ejecutable = ("MM_transpuesto", "MM_clasico"); # Nombres de los programas a ejecutar
@Size_Matriz = (800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800); # Tamaños de matrices
@Num_Hilos = (1, 2, 4); # Números de hilos a usar en las ejecuciones
$Repeticiones = 30; # Número de veces que se repetirá cada ejecución

# Itera sobre cada ejecutable, tamaño de matriz y número de hilos
foreach $Nombre_Ejecutable (@Nombre_Ejecutable) {
    foreach $size (@Size_Matriz) {
        foreach $hilo (@Num_Hilos) {
            # Crea un nombre de archivo para almacenar los resultados
            $file = "$Path/$Nombre_Ejecutable-".$size."-Hilos-".$hilo.".dat";

            # Repite la ejecución del programa $Repeticiones veces
            for ($i = 0; $i < $Repeticiones; $i++) {
                # Ejecuta el programa con los parámetros actuales y redirige la salida al archivo
                system("$Path/$Nombre_Ejecutable $size $hilo  >> $file");
                
                # Imprime en la terminal el comando que se está ejecutando
                printf("$Path/$Nombre_Ejecutable $size $hilo \n");
            }

            # Cierra el archivo de salida (innecesario, ya que Perl gestiona esto automáticamente)
            close($file);
            
            # Incrementa un contador p.
            $p = $p + 1;
        }
    }
}



