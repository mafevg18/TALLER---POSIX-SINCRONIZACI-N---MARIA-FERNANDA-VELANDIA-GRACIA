###################################################################
#            Pontificia Universidad Javeriana                     #
#                         ---=---                                 #
#                                                                 #
# Autores: Danna Rojas, María Fernanda Velandia,                  # 
#	Cristian Becerra, Giovanny Duran                          #
# Fecha: 14 Noviembre 2025                                        #
# Docente:  J. Corredor                                           #
# Archivo:  Makefile                                              # 
# Descripción: Automatización para la compilación de programas    #
###################################################################

GCC = gcc
FLAGS = -pthread -lrt

PROGRAMAS = concurrenciaPosix posixSincro

concurrenciaPosix:
	$(GCC) concurrenciaPosix.c -o $@ $(FLAGS)

posixSincro:
	$(GCC) posixSincro.c -o $@ $(FLAGS)

clean:
	$(RM) $(PROGRAMAS)

