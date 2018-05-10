/*
 * coordinador.h
 *
 *  Created on: 21 abr. 2018
 *      Author: utnso
 */

#ifndef COORDINADOR_H_
#define COORDINADOR_H_

#include <shared-library.h>
//#include "shared-library.h"

//Variables globales

//Hilos
pthread_t hilo_ESI;
pthread_t hilo_instancia;
pthread_t hilo_planificador;

//Funciones de hilos
void *atender_ESI(void* un_socket);
void *atender_Instancia(void* un_socket);
void *atender_Planificador(void* un_socket);

//Funciones



#endif /* COORDINADOR_H_ */
