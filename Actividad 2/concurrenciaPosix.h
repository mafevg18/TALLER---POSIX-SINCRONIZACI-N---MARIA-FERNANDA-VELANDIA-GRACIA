/******************************************************************
*            Pontificia Universidad Javeriana                     *
*                         ---=---                                 *
*                                                                 *
* Autores: Danna Rojas, María Fernanda Velandia,                  * 
*          Cristian Becerra, Giovanny Duran                       *
* Fecha: 14 Noviembre 2025                                        *
* Docente: J. Corredor                                           *
* Archivo: concurrenciaPosix.h                                     * 
* Objetivo: Declarar funciones y estructuras para               *
*           encontrar el máximo de un vector usando             *
*           múltiples hilos POSIX.                               *
* Descripción:                                                    *
*   - Estructura para pasar parámetros a cada hilo.             *
*   - Prototipos de funciones para máximo parcial y global.     *
******************************************************************/

#ifndef CONCURRENCIA_POSIX_H
#define CONCURRENCIA_POSIX_H

#include <pthread.h>
#include <limits.h>

/* Estructura de argumentos por hilo */
typedef struct argHilos{
    int inicio;        // índice inicio del segmento
    int fin;           // índice final del segmento
    int *vector;       // puntero al vector
    int maxparcial;    // máximo parcial calculado
} param_H;

/* Función que calcula máximo parcial de un hilo */
void *buscarMax(void *parametro);

/* Función que calcula máximo global usando múltiples hilos */
int maximoValor(int *vec, int n, int nhilos);

#endif

