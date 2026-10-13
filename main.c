#include "src/diccionario.h"
#include "src/cola.h"
#include <stdio.h>
#include <ctype.h>

#define ARCHIVO 1

#define TEXTO 2

#define ERROR -1
#define EXITO 0

#define NULO "NULL"

const char CARACTER_NULO = '\0';
const char SEPARADOR_ESPACIO = ' ';

const char *MODO_LECTURA = "r";
const char *FORMATO_LECTURA = "%m[^,],%m[^,],\n";

void liberar_datos(char *palabra)
{
	free(palabra);
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

			//Avanzo aritméticamente
			texto++;
		}
	}

	if (err) {
		free(palabra);
		return false;
	}

	//Encolo
	if (!cola_encolar(cola, (void*)(char *)palabra)) {
		return false;
	}

	//Verifico si llegué al final
	if (*texto == CARACTER_NULO) {
		return true;
	}

	return parsear_texto(cola, palabra + 1);
}

/*
 * Se espera un archivo abierto correctamente en modo (LECTURA) y un diccionario
 *
 * Devuelve true si las claves con sus valores fueron agregados correctamente.
 * Devuelve false si ha ocurrido un error cargando el diccionario
 * 
 * 		- En caso de error se cierra el archivo
*/
bool cargar_diccionario(FILE* archivo, struct diccionario *d)
{

	if (!archivo || !d) {
		return false;
	}

	bool err = false;

	bool exito = true;

	char *llave = { NULL };
	char *valor = { NULL };

	while (fscanf(archivo, FORMATO_LECTURA, *llave, *valor) != EOF && !err) {

		if (!diccionario_insertar(d, llave, (char*)valor, NULL)) {
			err = true;
		}

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
 * 
*/
void mostrar_por_pantalla(struct cola *c, struct diccionario *d)
{

	char *palabra = { NULL };
	
	char *dato_cola = { NULL };

	char *valor = { NULL };

	if (cola_cantidad(c) == 0) {
		return;
	}

	while (cola_cantidad(c) != 0) {

		dato_cola = (char*)cola_desencolar(c);

		palabra = diccionario_buscar(d, dato_cola);

		if (diccionario_buscar(d, palabra)) { //Si se encuentra en el diccionario, muestro su valor
			
			valor = diccionario_eliminar(d, palabra);
			
			printf("%s ",valor);
			
		} else { //Si no se encuentra en el diccionario, muestro la palabra
			
			printf("%s ", palabra);
		
		}
	}
	if (palabra) { //Palabra podría seguir siendo NULL | NO SE PUEDE LIBERAR NULL
		free(palabra);
	}
	if (valor) { //Valor podría seguir siendo NULL | NO SE PUEDE LIBERAR NULL
		free(valor);
	}

	free(dato_cola);

}

int main(int argc, char *argv[])
{
    //if (argc < 3) {
    //    //Mensaje
    //    return ERROR;
    //}

	unsigned capacidad_diccionario = 20;

	diccionario_t *diccionario = diccionario_crear(capacidad_diccionario);

	if (!diccionario) {
		return ERROR;
	}

	FILE *archivo = fopen(argv[TEXTO], MODO_LECTURA);

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

	return EXITO;
}