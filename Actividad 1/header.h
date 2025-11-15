/******************************************************************
*            Pontificia Universidad Javeriana                     *
*                         ---=---                                 *
*                                                                 *
* Autores: Danna Rojas, María Fernanda Velandia,                  * 
*          Cristian Becerra, Giovanny Duran                       *
* Fecha: 14 Noviembre 2025                                        *
* Docente: J Corredor                                            *
* Archivo: header.h                                              * 
* Objetivo: Declarar constantes, estructuras y semáforos          *
*           para el problema productor-consumidor.               *
* Descripción:                                                    *
*   - Define el tamaño del buffer, nombres de semáforos y memoria.*
*   - Define la estructura de datos compartidos.                  *
******************************************************************/

#ifndef HEADER_H
#define HEADER_H

#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER 5  // Tamaño del buffer compartido

typedef struct {
    int bus[BUFFER]; // Buffer circular
    int entrada;     // Índice de producción
    int salida;      // Índice de consumo
} compartir_datos;

#define SHM_NAME "/memoria_compartida" // Nombre de memoria compartida
#define SEM_VACIO "/vacio"             // Semáforo espacios vacíos
#define SEM_LLENO "/lleno"             // Semáforo espacios llenos

#endif

