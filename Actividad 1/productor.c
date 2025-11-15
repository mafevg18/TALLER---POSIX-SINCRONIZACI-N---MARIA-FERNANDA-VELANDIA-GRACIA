/******************************************************************
*            Pontificia Universidad Javeriana                     *
*                         ---=---                                 *
*                                                                 *
* Autores: Danna Rojas, María Fernanda Velandia,                  * 
*          Cristian Becerra, Giovanny Duran                       *
* Fecha: 14 Noviembre 2025                                        *
* Docente: J. Corredor                                           *
* Archivo: productor.c                                           * 
* Objetivo: Implementar el productor del problema                *
*           productor-consumidor usando POSIX.                   *
* Descripción:                                                    *
*   - Produce elementos en un buffer compartido.                 *
*   - Sincroniza con el consumidor usando semáforos.             *
*   - Maneja memoria compartida y semáforos con nombre.          *
******************************************************************/

#include "header.h"

int main() {
    // Crear semáforos con nombre
    sem_t *vacio = sem_open(SEM_VACIO, O_CREAT, 0644, BUFFER); // Semáforo de espacios vacíos
    sem_t *lleno = sem_open(SEM_LLENO, O_CREAT, 0644, 0);       // Semáforo de elementos llenos
    if (vacio == SEM_FAILED || lleno == SEM_FAILED) {
        perror("Error al crear semáforos");
        exit(EXIT_FAILURE);
    }

    // Crear memoria compartida
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0644);
    if (shm_fd < 0) {
        perror("Error al crear memoria compartida");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shm_fd, sizeof(compartir_datos)) == -1) {
        perror("Error al ajustar tamaño de memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Mapear memoria compartida
    compartir_datos *compartir = mmap(NULL, sizeof(compartir_datos),
                                      PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (compartir == MAP_FAILED) {
        perror("Error en mmap");
        exit(EXIT_FAILURE);
    }

    // Inicializar índices
    compartir->entrada = 0;
    compartir->salida = 0;

    // Bucle de producción
    for (int i = 1; i <= 10; i++) {
        sem_wait(vacio); // Esperar espacio libre
        compartir->bus[compartir->entrada] = i; // Insertar elemento
        printf("Productor: Produce %d\n", i);
        compartir->entrada = (compartir->entrada + 1) % BUFFER; // Circular
        sem_post(lleno); // Señalar elemento disponible
        sleep(1); // Simular tiempo de producción
    }

    // Liberar recursos locales
    munmap(compartir, sizeof(compartir_datos));
    close(shm_fd);
    sem_close(vacio);
    sem_close(lleno);

    printf("Productor finalizado.\n");
    return 0;
}

