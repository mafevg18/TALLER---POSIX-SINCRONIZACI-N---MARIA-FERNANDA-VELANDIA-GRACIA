/******************************************************************
*            Pontificia Universidad Javeriana                     *
*                         ---=---                                 *
*                                                                 *
* Autores: Danna Rojas, María Fernanda Velandia,                  * 
*          Cristian Becerra, Giovanny Duran                       *
* Fecha: 14 Noviembre 2025                                        *
* Docente: J. Corredor                                           *
* Archivo: posixSincro.c                                         * 
* Objetivo: Implementar múltiples productores y spooler         *
*           usando POSIX threads y sincronización con mutex y   *
*           condiciones.                                         *
* Descripción:                                                    *
*   - Productores generan mensajes y los colocan en buffers.    *
*   - Spooler imprime los mensajes de manera sincronizada.       *
******************************************************************/

#include "posixSincro.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Buffers y contadores globales */
char buf[MAX_BUFFERS][100];           // buffers circulares
int buffer_index = 0;                 // índice de escritura
int buffer_print_index = 0;           // índice de lectura
int buffers_available = MAX_BUFFERS;  // buffers libres
int lines_to_print = 0;               // líneas disponibles para imprimir

/* Mutex y condiciones */
pthread_mutex_t buf_mutex = PTHREAD_MUTEX_INITIALIZER; // mutex de acceso
pthread_cond_t buf_cond = PTHREAD_COND_INITIALIZER;   // condición buffer libre
pthread_cond_t spool_cond = PTHREAD_COND_INITIALIZER; // condición spooler


/* Escribe mensajes en el buffer */
void *producer(void *arg) {
    int r;
    int my_id = *((int *)arg); // id del hilo
    int count = 0;             // contador de mensajes

    for (int i = 0; i < 10; i++) {
        if ((r = pthread_mutex_lock(&buf_mutex)) != 0) { // bloquear mutex
            fprintf(stderr, "Error = %d (%s)\n", r, strerror(r));
            exit(1);
        }

        while (!buffers_available)                   // esperar buffer libre
            pthread_cond_wait(&buf_cond, &buf_mutex);

        int j = buffer_index;                        // seleccionar buffer
        buffer_index = (buffer_index + 1) % MAX_BUFFERS; // circular
        buffers_available--;                          // actualizar libre

        snprintf(buf[j], sizeof(buf[j]), "Thread %d: %d\n", my_id, ++count); // generar mensaje
        lines_to_print++;                             // incrementar líneas disponibles

        pthread_cond_signal(&spool_cond);            // avisar al spooler

        if ((r = pthread_mutex_unlock(&buf_mutex)) != 0) { // desbloquear mutex
            fprintf(stderr, "Error = %d (%s)\n", r, strerror(r));
            exit(1);
        }

        sleep(1); // simular tiempo de producción
    }

    return NULL;
}


/* Imprime mensajes de los buffers */
void *spooler(void *arg) {
    int r;
    (void)arg;

    while (1) {
        if ((r = pthread_mutex_lock(&buf_mutex)) != 0) { // bloquear mutex
            fprintf(stderr, "Error = %d (%s)\n", r, strerror(r));
            exit(1);
        }

        while (!lines_to_print)                          // esperar mensaje disponible
            pthread_cond_wait(&spool_cond, &buf_mutex);

        printf("%s", buf[buffer_print_index]);           // imprimir mensaje
        lines_to_print--;                                // actualizar contador
        buffer_print_index = (buffer_print_index + 1) % MAX_BUFFERS; // circular
        buffers_available++;                             // actualizar buffer libre

        pthread_cond_signal(&buf_cond);                  // avisar productor

        if ((r = pthread_mutex_unlock(&buf_mutex)) != 0) { // desbloquear mutex
            fprintf(stderr, "Error = %d (%s)\n", r, strerror(r));
            exit(1);
        }
    }

    return NULL;
}


/* Crea hilos productores y spooler */
int main() {
    pthread_t prod[3];       // hilos productores
    pthread_t sp;            // hilo spooler
    int ids[3] = {1, 2, 3}; // ids de hilos

    pthread_create(&sp, NULL, spooler, NULL); // crear spooler

    for (int i = 0; i < 3; i++)
        pthread_create(&prod[i], NULL, producer, &ids[i]); // crear productores

    for (int i = 0; i < 3; i++)
        pthread_join(prod[i], NULL);                       // esperar productores

    pthread_mutex_destroy(&buf_mutex);        // destruir mutex
    pthread_cond_destroy(&buf_cond);          // destruir condición
    pthread_cond_destroy(&spool_cond);        // destruir condición

    return 0;
}

