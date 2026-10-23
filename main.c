#include "src/diccionario.h"
#include "src/cola.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define ARCHIVO 1

#define TEXTO 2

#define ERROR -1
#define EXITO 0

#define NULO "NULL"

const char CARACTER_NULO = '\0';
const char SEPARADOR_ESPACIO = ' ';

const char *MODO_LECTURA = "r";
const char *FORMATO_LECTURA = "%m[^,],%m[^\n]\n";

void liberar_datos(void *palabra)
{
	free((void *)(char *)palabra);
}

void cola_destruir_todo(cola_t *c)
{
	if (!c)
		return;

	while (cola_cantidad(c) != 0) {
		char *dato = cola_desencolar(c);
		free(dato);
	}

	cola_destruir(c);
}

/*
 * La cola debe estar previamente creada.
 *
 * Devuelve true si la operación ha sido exitosa, caso contrario devuelve false por algún error en la operación.
*/
bool parsear_texto(cola_t *cola, char *texto)
{
	if (!cola || !texto) {
		return false;
	}

	//Caso base
	if (*texto == CARACTER_NULO) {
		return true;
	}

	char *palabra = malloc(sizeof(char) + 1);

	if (!palabra) {
		return false;
	}

	size_t tope_palabra = 0;

	bool err = false;

	while (*texto != SEPARADOR_ESPACIO && *texto != CARACTER_NULO && !err) {
		palabra[tope_palabra] = texto[0];
		tope_palabra++;

		char *aux = realloc(palabra, sizeof(char) * (tope_palabra + 1));

		if (aux == NULL) {
			err = true;

		} else {
			palabra = aux;

			texto++;
		}
	}

	if (err) {
		free(palabra);
		return false;
	}

	palabra[tope_palabra] = CARACTER_NULO;

	if (!cola_encolar(cola, palabra)) {
		free(palabra);
		return false;
	}

	if (*texto == CARACTER_NULO) {
		return true;
	}

	return parsear_texto(cola, texto + 1);
}

/*
 * Se espera un archivo abierto correctamente en modo (LECTURA) y un diccionario
 *
 * Devuelve true si las claves con sus valores fueron agregados correctamente.
 * Devuelve false si ha ocurrido un error cargando el diccionario
 * 
*/
bool cargar_diccionario(FILE *archivo, struct diccionario *d)
{
	if (!archivo || !d) {
		return false;
	}

	bool err = false;

	bool exito = true;

	char *llave = NULL;

	char *valor = NULL;

	while (fscanf(archivo, FORMATO_LECTURA, &llave, &valor) == 2 && !err) {
		if (!diccionario_insertar(d, llave, valor, NULL)) {
			free(llave);
			free(valor);
			err = true;
		}
		free(llave);
	}

	if (err) {
		fclose(archivo);

		exito = false;
	}

	return exito;
}

/*
 * Se espera una cola y un diccionario previamente verificados.
 *
 * Muestra por pantalla lo solicitado por el enunciado. 
*/
void mostrar_por_pantalla(struct cola *c, struct diccionario *d)
{
	if (cola_cantidad(c) == 0) {
		return;
	}

	char *palabra = NULL;

	char *valor = NULL;

	while (cola_cantidad(c) != 0) {
		palabra = (char *)cola_desencolar(c);

		if (diccionario_buscar(
			    d,
			    palabra)) { //Si se encuentra en el diccionario, muestro su valor.

			valor = diccionario_obtener(d, palabra);

			printf("%s", valor);
			free(palabra);
		} else { //Si no se encuentra en el diccionario, muestro la palabra.

			printf("%s", palabra);
			free(palabra);
		}

		if (cola_cantidad(c) != 0) {
			printf(" ");
		}
	}

	printf("\n");
}

int main(int argc, char *argv[])
{
	if (argc < 3) {
		printf("Mal formato pibe\n");
		return ERROR;
	}

	unsigned capacidad_diccionario = 20;

	diccionario_t *diccionario = diccionario_crear(capacidad_diccionario);

	if (!diccionario) {
		return ERROR;
	}

	FILE *archivo = fopen(argv[ARCHIVO], MODO_LECTURA);

	if (!archivo) {
		diccionario_destruir(diccionario);
		return ERROR;
	}

	if (!cargar_diccionario(archivo, diccionario)) {
		diccionario_destruir_todo(diccionario, liberar_datos);
		fclose(archivo);
		return ERROR;
	}

	//Cierro el archivo, no lo voy a usar más
	fclose(archivo);

	cola_t *cola = cola_crear();

	if (!cola) {
		diccionario_destruir(diccionario);
		return ERROR;
	}

	//Cargo la cola con las palabras
	if (!parsear_texto(cola, argv[TEXTO])) {
		return ERROR;
	}

	mostrar_por_pantalla(cola, diccionario);

	diccionario_destruir_todo(diccionario, liberar_datos);

	cola_destruir_todo(cola);

	return EXITO;
}