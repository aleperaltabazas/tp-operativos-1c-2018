/*
 * shared-library.h
 *
 *  Created on: 21 abr. 2018
 *      Author: utnso
 */

#ifndef SHARED_LIBRARY_H_
#define SHARED_LIBRARY_H_

#include <stdio.h>
#include <stdint.h>
#include <openssl/md5.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <unistd.h>
#include <readline/readline.h>
#include <pthread.h>
#include <parsi/parser.h>
#include "estructuras.h"
#include <fcntl.h>

//Variables super globales

#define PACKAGE_SIZE 1024
#define BACKLOG 20

t_log * logger;
char* mensajePlanificador =	"My name is Planificador.c and I'm the fastest planifier alive...";
char* mensajeESI = "A wild ESI has appeared!";
char* mensajeESILista = "Gotcha! Wild ESI was added to the list!";
char* mensajeInstancia = "It's ya boi, instancia!";
char* mensajeCoordinador = "Coordinador: taringuero profesional.";

package_int id_coordinador = {
		.packed = 0
};

package_int id_planificador = {
		.packed = 1
};

package_int id_ESI = {
		.packed = 2
};

package_int id_instancia = {
		.packed = 3
};

//Funciones servidor

int levantar_servidor(char* puerto, int tries);
	/*
	 * Descripción: crea un socket escuchante y llama a bind() para atarlo al puerto y crear
	 * 		un servidor en dicho puerto. En caso que falle el bindeo, se intenta de nuevo. Si falla 5
	 * 		veces, se cierra el proceso.
	 * Argumentos:
	 * 		char* puerto: puerto en el que se encuentra el servidor.
	 * 		int tries: número de intentos realizados.
	 */

//Funciones cliente

int conectar_a(char* ip, char* puerto, package_int id, int tries);
	/*
	 * Descripción: establece una conexión con un servidor y le envía un mensaje, y devuelve
	 * 		el socket servidor. En caso que falle la conexión, se intenta de nuevo. Si falla 5 veces,
	 * 		se cierra el proceso.
	 * Argumentos:
	 * 		char* ip: ip del servidor.
	 * 		char* puerto: puerto por el que escucha el servidor.
	 * 		package_int id: mensaje a enviar al servidor.
	 * 		int tries: número de intentos realizados.
	 */

void chequear_servidor(package_int id, int server_socket);
	/*
	 * Descripción: revisa el mensaje devuelto por el servidor para saber de qué servidor se trata.
	 * 		En caso de que sea un servidor desconocido, llama a salir_con_error().
	 * Argumentos:
	 * 		package_int id: identificación enviada por el servidor.
	 * 		int server_socket: socket del servidor.
	 */

void serializar_packed(package_int pedido, char** message);
	/*
	 * Descripción: serializa un mensaje del tipo package_int.
	 * Argumentos:
	 * 		package_int pedido: mensaje a serializar.
	 * 		char** message: el recipiente del mensaje serializado.
	 */

void deserializar_packed(package_int *pedido, char **package);
	/*
	 * Descripción: deserializa un mensaje del tipo package_int.
	 * Argumentos:
	 * 		package_int *pedido: el recipiente del mensaje a deserializar.
	 * 		char** message: buffer de memoria con el mensaje a deserializar.
	 */

void serializar_aviso(aviso_con_ID aviso, char** message);
	/*
	 * Descripción: serializa un mensaje del tipo aviso_con_ID.
	 * Argumentos:
	 * 		aviso_con_ID aviso: mensaje a serializar.
	 * 		char** message: el recipiente del mensaje serializado.
	 */

void deserializar_aviso(aviso_con_ID *aviso, char** package);
	/*
	 * Descripción: deserializa un mensaje del tipo aviso_con_ID.
	 * Argumentos:
	 * 		aviso_con_ID aviso: el recipiente del mensaje a deserializar.
	 * 		char** message: buffer de memoria con el mensaje a deserializar.
	 */

void avisar_cierre(int sockfd);
	/*
	 * Descripción: informa a un proceso que éste va a cerrar.
	 * Argumentos:
	 * 		int sockfd: socket al cual se ennviará el mensaje de cierre.
	 */

void salir_con_error(char* mensaje, int socket);
	/*
	 * Descripción: llama a exit_gracefully() para terminar el proceso con error (1), loggea
	 * 		un mensaje de error y cierra el descriptor de archivo del socket.
	 * Argumentos:
	 * 		char* mensaje: el mensaje a loggear.
	 * 		socket: socket a cerrar.
	 */

void exit_gracefully(int return_val);
	/*
	 * Descripción: llama a exit() para terminar el proceso con un valor determinado.
	 * Argumentos:
	 * 		int return_val: el valor con el que se llama a exit().
	 */

void iniciar_log(char* nombre, char* mensajeInicial);
	/*
	 * Descripción: crea un log con un nombre pasado por parámetro y se loggea
	 * 		un mensaje inicial pasado por parámetro.
	 * Argumentos:
	 * 		char* nombre: nombre del logger.
	 * 		char* mensajeInicial: mensaje a loggear al principio.
	 */

void loggear(char* mensaje);
	/*
	 * Descripción: loggea un mensaje en la variable global logger.
	 * Argumentos:
	 * 		char* mensaje: el mensaje a loggear.
	 */

void terminar_conexion(int sockfd);
	/*
	 * Descripción: envía un mensaje de terminación a un proceso a trav�s de un socket.
	 * Argumentos:
	 * 		int sockfd: el socket por el cual se envía el mensaje.
	 */

void enviar_aviso(int sockfd, aviso_con_ID aviso);
	/*
	 * Descripción: envía un mensaje a un servidor del tipo aviso_con_ID.
	 * Argumentos:
	 * 		int sockfd: socket del servidor.
	 * 		aviso_con_ID aviso: aviso a enviar.
	 */

aviso_con_ID recibir_aviso(int sockfd);
	/*
	 * Descripción: recibe un aviso del tipo aviso_con_ID de otro proceso.
	 * Argumentos:
	 * 		int sockfd: socket del proceso.
	 */

void enviar_packed(package_int packed, int sockfd);
	/*
	 * Descripción: envia un paquete package_int a otro proceso.
	 * Argumentos:
	 * 		package_int packed: el paquete a enviar.
	 * 		int sockfd: socket del proceso al cual enviar el paquete.
	 */

package_int recibir_packed(int sockfd);
	/*
	 * Descripción: recibe un paquete package_int de otro proceso.
	 * Argumentos:
	 * 		int sockfd: socket del proceso del cual recibir el paquete.
	 */

void enviar_cadena(char* cadena, int sockfd);
	/*
	 * Descripción: envía una cadena a otro proceso.
	 * Argumentos.
	 * 		char* cadena: la cadena a enviar.
	 * 		int sockfd: el socket del proceso al cual enviar la cadena.
	 */

char* recibir_cadena(int sockfd, uint32_t size);
	/*
	 * Descripción: recibe una cadena de otro proceso.
	 * Argumentos:
	 * 		int sockfd: socket del proceso del cual recibir la cadena.
	 * 		int size: tamaño de la cadena a recibir.
	 */

char* serializar_valores_set(int tamanio_a_enviar, parametros_set * valor_set);
	/*
	 * Descripción: serializa un mensaje del tipo parametros_set.
	 * Argumentos:
	 * 		tamanio_a_enviar: para hacer el malloc.
	 * 		valor_set: el recipiente del mensaje serializado.
	 */

void cerrar_clave(char* clave);
	/*
	 * Descripción: agrega el caracter \0 a una clave al final.
	 * Argumentos:
	 * 		char* clave: la clave a cerrar.
	 */

char* serializar_key_package(clave_package* package);
	/*
	 * Descripción: serializa de forma dinámica un clave_package usado para hacer GET o STORE.
	 * Argumentos:
	 * 		clave_package* package: puntero del paquete a serializar.
	 */

void fill_clave_package(clave_package* package, char** clave);
	/*
	 * Descripción: completa los campos del paquete a enviar con la clave y su tamaño, y el
	 * 		tamaño total.
	 * Argumentos:
	 * 		clave_package* package: puntero del paquete a rellenar.
	 * 		char** clave: doble puntero de la clave usada para rellenar.
	 */

void enviar_clave_package(int sockfd, char* clave);
	/*
	 * Descripción: envía una clave a través del sockfd, llamando a serializar_clave_package() y
	 * 		fill_clave_package().
	 * Argumentos:
	 * 		int sockfd: file descriptor del socket al cual enviar.
	 * 		char* clave: clave a enviar.
	 */

void recibir_clave_package(int sockfd, clave_package* package);
	/*
	 * Descripción: recibe un clave_package y lo guarda en el package pasado por argumento.
	 * Argumentos:
	 * 		int sockfd: file descriptor del socket del cual recibir.
	 * 		clave_package* package: puntero del paquete en donde guardar la información recibida.
	 */

#endif /* SHARED_LIBRARY_H_ */
