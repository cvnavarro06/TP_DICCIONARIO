#ifndef COLA_H_
#define COLA_H_
#include <stdlib.h>
#include <stdbool.h>

typedef struct cola cola_t;

cola_t *cola_crear();

bool cola_encolar(cola_t *c, void *e);

void *cola_desencolar(cola_t *c);

void *cola_frente(cola_t *c);

bool cola_esta_vacia(cola_t *c);

size_t cola_cantidad(cola_t *c);

void cola_destruir(cola_t *cola);

#endif // COLA_H_
