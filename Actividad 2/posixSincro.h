/******************************************************************
*            Pontificia Universidad Javeriana                     *
*                         ---=---                                 *
*                                                                 *
* Autores: Danna Rojas, María Fernanda Velandia,                  * 
*          Cristian Becerra, Giovanny Duran                       *
* Fecha: 14 Noviembre 2025                                        *
* Docente: J. Corredor                                           *
* Archivo: posixSincro.h                                         * 
* Objetivo: Declarar estructuras y funciones para               *
*           sincronización de múltiples productores y un spooler *
*           usando POSIX threads.                                 *
* Descripción:                                                    *
*   - Buffers circulares para mensajes de productores.          *
*   - Mutex y condiciones para sincronización.                  *
*   - Prototipos de funciones productor y spooler.               *
******************************************************************/

#ifndef POSIX_SINCRO_H
#define POSIX_SINCRO_H

#include <pthread.h>

#define MAX_BUFFERS 8 // número máximo de buffers

/* Buffers y contadores globales */
extern char buf[MAX_BUFFERS][100];    // buffers de mensajes
extern int buffer_index;              // índice de escritura
extern int buffer_print_index;        // índice de lectura
extern int buffers_available;         // buffers libres
extern int lines_to_print;            // líneas disponibles para imprimir

/* Mutex y condiciones */
extern pthread_mutex_t buf_mutex;     // mutex de acceso al buffer
extern pthread_cond_t buf_cond;       // condición buffer libre
extern pthread_cond_t spool_cond;     // condición spooler

/* Funciones de sincronización */
void *producer(void *arg);            // función productor
void *spooler(void *arg);             // función spooler 

#endif

