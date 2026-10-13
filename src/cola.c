#include "cola.h"
#include "lista.h"
#include <stdlib.h>

struct cola {
	lista_t *lista;
};

cola_t *cola_crear()
{
	cola_t *nueva_cola = malloc(sizeof(cola_t));

	if (nueva_cola == NULL) {
		return NULL;
	}

	nueva_cola->lista = lista_crear();

	if (nueva_cola->lista == NULL) {
		free(nueva_cola);
		return NULL;
	}

	return nueva_cola;
}

bool cola_encolar(cola_t *c, void *e)
{
	bool exito = false;

	if (c == NULL) {
		return exito;
	}

	size_t posicion_final = lista_cantidad(c->lista);

	if (lista_insertar(c->lista, e, posicion_final)) {
		exito = true;
	}

	return exito;
}

void *cola_desencolar(cola_t *c)
{
	if (c == NULL || cola_esta_vacia(c)) {
		return NULL;
	}

	void *data = lista_eliminar(c->lista, 0);

	return data;
}

void *cola_frente(cola_t *c)
{
	if (c == NULL || cola_esta_vacia(c)) {
		return NULL;
	}

	void *data = lista_obtener(c->lista, 0);

	return data;
}

bool cola_esta_vacia(cola_t *c)
{
	bool vacia = false;

	if (c == NULL) {
		return !vacia;
	}

	if (lista_esta_vacia(c->lista)) {
		vacia = true;
	}

	return vacia;
}

size_t cola_cantidad(cola_t *c)
{
	size_t cantidad = 0;

	if (c == NULL) {
		return cantidad;
	}

	cantidad = lista_cantidad(c->lista);

	return cantidad;
}

void cola_destruir(cola_t *cola)
{
	if (cola == NULL) {
		return;
	}

	lista_destruir(cola->lista);
	free(cola);
}