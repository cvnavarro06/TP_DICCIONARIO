#include "diccionario.h"
#include <string.h>

#define HASH_CAPACITY 0.75
#define CAPACIDAD_MINIMA 3


#define ERROR -1

struct entrada
{   
    const char *clave; //string de lectura
    void *valor;
    bool vacio;
};

struct diccionario 
{
    struct entrada *entradas;
    size_t capacidad_hash;
    size_t cantidad;
};


/*
 * Debo de duplicar el tamaño del diccionario
 *
 * Devuelve el índice a 
 * 
*/
size_t hash(size_t capacidad, char *clave);

diccionario_t *diccionario_crear(unsigned capacidad_inicial)
{
    size_t capacidad;
    
    if (capacidad_inicial <= CAPACIDAD_MINIMA) {
        capacidad = capacidad_inicial;

    } else capacidad = capacidad_inicial;

    struct diccionario *nuevo_diccionario = malloc(sizeof(diccionario_t));

    if (!nuevo_diccionario) {
        return NULL;
    }

    nuevo_diccionario->cantidad = 0;
    nuevo_diccionario->capacidad_hash = capacidad;

    nuevo_diccionario->entradas = calloc(nuevo_diccionario->capacidad_hash, sizeof(struct entrada));

    if (!nuevo_diccionario->entradas) {
        free(nuevo_diccionario);
        return NULL;
    }

    nuevo_diccionario->entradas->vacio = true;

    return nuevo_diccionario;
}

bool diccionario_insertar(diccionario_t *d, char *clave, void *valor,
			  void **anterior)
{
    if (!d || !clave || !anterior) {
        return false;
    }

    bool exito = false;

    size_t indice = hash(d->capacidad_hash, clave);

    if(exito) {
        d->cantidad++;
    }
    
    return exito;
}

bool diccionario_buscar(diccionario_t *d, char *clave)
{
    if (!d || !clave) {
        return false;
    }

    bool encontrado = false;

    for (int i = 0; i<d->capacidad_hash && !encontrado; i++) {
        if (strcmp(d->entradas[i].clave, clave) == 0) {
            encontrado = true;
        }
    }

    return encontrado;
}

void *diccionario_obtener(diccionario_t *d, char *clave)
{
    if (d == NULL || clave == NULL) {
        return NULL;
    }
    
    char *valor_obt = { NULL };

    size_t indice = hash(d->capacidad_hash, clave);

    bool encontrado = false;

    //Esto no va a funcionar
    if (strcmp(d->entradas[indice].clave, clave) == 0) {
        valor_obt = d->entradas->valor;
    } else {
        for (int i = indice + 1; i<d->capacidad_hash && !encontrado; i++) {
            if (strcmp(d->entradas[i].clave, clave) == 0) {
                valor_obt = d->entradas[i].valor;

                encontrado = true;
            }
        }
    }

    return valor_obt;
}

void *diccionario_eliminar(diccionario_t *d, char *clave);

size_t diccionario_cantidad(diccionario_t *d)
{
    if (d == NULL) {
        return ERROR;
    }

    return d->cantidad;
}

size_t diccionario_iterar(diccionario_t *d, bool (*f)(char *, void *, void *),
			  void *extra);

void diccionario_destruir(diccionario_t *d)
{
    if (!d) {
        return;
    }

    for (int i = 0; i<d->capacidad_hash; i++) {

        if (&d->entradas[i] != NULL) {
            free(d->entradas[i].clave);
        } 
    }

    free(d->entradas);
    free(d);
}

void diccionario_destruir_todo(diccionario_t *d, void (*destructor)(void *));

