/******************************************************************
*            Pontificia Universidad Javeriana                     *
*                         ---=---                                 *
*                                                                 *
* Autores: Danna Rojas, María Fernanda Velandia,                  * 
*          Cristian Becerra, Giovanny Duran                       *
* Fecha: 14 Noviembre 2025                                        *
* Docente: J. Corredor                                           *
* Archivo: consumidor.c                                           * 
* Objetivo: Implementar el consumidor del problema               *
*           productor-consumidor usando POSIX.                   *
* Descripción:                                                    *
*   - Consume elementos de un buffer compartido.                 *
*   - Sincroniza con el productor usando semáforos.              *
*   - Maneja memoria compartida y semáforos con nombre.          *
******************************************************************/

#include "header.h"

int main() {
    // Abrir semáforos existentes
    sem_t *vacio = sem_open(SEM_VACIO, 0); 
    sem_t *lleno = sem_open(SEM_LLENO, 0);
    if (vacio == SEM_FAILED || lleno == SEM_FAILED) {
        perror("Error al abrir semáforos");
        exit(EXIT_FAILURE);
    }

    // Abrir memoria compartida existente
    int fd_compartido = shm_open(SHM_NAME, O_RDWR, 0644);
    if (fd_compartido < 0) {
        perror("Error al abrir memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Mapear memoria compartida
    compartir_datos *compartir = mmap(NULL, sizeof(compartir_datos),
                                      PROT_READ | PROT_WRITE, MAP_SHARED, fd_compartido, 0);
    if (compartir == MAP_FAILED) {
        perror("Error en mmap consumidor");
        exit(EXIT_FAILURE);
    }

    // Bucle de consumo
    for (int i = 1; i <= 10; i++) {
        sem_wait(lleno); // Esperar elemento disponible
        int item = compartir->bus[compartir->salida]; // Leer elemento
        printf("Consumidor: Consume %d\n", item);
        compartir->salida = (compartir->salida + 1) % BUFFER; // Circular
        sem_post(vacio); // Liberar espacio
        sleep(2); // Simular tiempo de consumo
    }

    // Liberar recursos locales
    munmap(compartir, sizeof(compartir_datos));
    close(fd_compartido);
    sem_close(vacio);
    sem_close(lleno);

    printf("Consumidor finalizado.\n");
    return 0;
}

