/******************************************************************
*            Pontificia Universidad Javeriana                     *
*                         ---=---                                 *
*                                                                 *
* Autores: Danna Rojas, María Fernanda Velandia,                  * 
*          Cristian Becerra, Giovanny Duran                       *
* Fecha: 14 Noviembre 2025                                        *
* Docente: J. Corredor                                           *
* Archivo: concurrenciaPosix.c                                     * 
* Objetivo: Implementar búsqueda de máximo usando hilos POSIX.   *
* Descripción:                                                    *
*   - Cada hilo calcula máximo parcial de un segmento.           *
*   - La función principal combina máximos parciales en global.  *
******************************************************************/

#include "concurrenciaPosix.h"
#include <stdio.h>
#include <stdlib.h>

/* Hilo: buscar máximo parcial */
void *buscarMax(void *parametro) {
    param_H *argumentos = (param_H *)parametro; // Obtener parámetros del hilo
    int inicio = argumentos->inicio;            // índice inicial
    int fin = argumentos->fin;                  // índice final
    int *v = argumentos->vector;               // vector a procesar

    if (inicio >= fin) {                        // segmento vacío
        argumentos->maxparcial = INT_MIN;      // asignar valor mínimo
        pthread_exit(NULL);
    }

    int local_max = v[inicio];                 // inicializar máximo local
    for (int i = inicio + 1; i < fin; i++) {
        if (v[i] > local_max)                  // comparar elemento
            local_max = v[i];                  // actualizar máximo
    }

    argumentos->maxparcial = local_max;        // guardar máximo parcial
    pthread_exit(NULL);
}

/* Función principal: máximo global con hilos */
int maximoValor(int *vec, int n, int nhilos) {
    if (n <= 0) return INT_MIN;                // vector vacío
    if (nhilos <= 0) nhilos = 1;               // al menos un hilo
    if (nhilos > n) nhilos = n;                // no más hilos que elementos

    pthread_t *threads = malloc(sizeof(pthread_t) * nhilos); // array de hilos
    param_H *args = malloc(sizeof(param_H) * nhilos);        // array de argumentos
    if (!threads || !args) {                   // verificar memoria
        free(threads);
        free(args);
        return INT_MIN;
    }

    int base = n / nhilos;                      // tamaño base por hilo
    int rem = n % nhilos;                       // resto para distribución
    int idx = 0;

    for (int t = 0; t < nhilos; ++t) {
        int chunk = base + (t < rem ? 1 : 0);  // ajustar tamaño de segmento
        args[t].inicio = idx;                   // asignar inicio
        args[t].fin = idx + chunk;              // asignar fin
        args[t].vector = vec;                   // asignar vector
        args[t].maxparcial = INT_MIN;           // inicializar máximo
        idx += chunk;

        pthread_create(&threads[t], NULL, buscarMax, &args[t]); // crear hilo
    }

    int global_max = INT_MIN;                   // inicializar máximo global
    for (int t = 0; t < nhilos; ++t) {
        pthread_join(threads[t], NULL);         // esperar hilo
        if (args[t].maxparcial > global_max)    // comparar máximo parcial
            global_max = args[t].maxparcial;   // actualizar máximo global
    }

    free(threads);                              // liberar memoria
    free(args);
    return global_max;                          // devolver máximo global
}

/* MAIN para prueba */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <fichero> <nhilos>\n", argv[0]);
        exit(-1);
    }

    FILE *fichero = fopen(argv[1], "r");       // abrir fichero
    if (!fichero) {
        perror("No se puede abrir fichero");
        exit(-2);
    }

    int n;
    fscanf(fichero, "%d", &n);                 // leer tamaño del vector
    int *vec = malloc(sizeof(int) * n);        // reservar vector
    for (int i = 0; i < n; i++)
        fscanf(fichero, "%d", &vec[i]);       // leer elementos
    fclose(fichero);

    int nhilos = atoi(argv[2]);                 // número de hilos
    int maximo = maximoValor(vec, n, nhilos);  // calcular máximo

    printf("Máximo encontrado: %d\n", maximo); // mostrar resultado
    free(vec);
    return 0;
}

