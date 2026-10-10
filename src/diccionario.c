#include "diccionario.h"
#include <string.h>

#define CARGA_MINIMA 0.75
#define CAPACIDAD_MINIMA 3

#define FACTOR_CARGA_MAX 0.75

#define ERROR -1

struct entrada
{   
    const char *clave; //string de lectura
    void *valor;
};

struct diccionario 
{
    struct entrada *entradas;
    size_t capacidad_hash;
    size_t cantidad;
};


// Función "algoritmo DJB2" utilizado para hash
size_t hash(char *clave, size_t capacidad) 
{
    size_t hash = 5381;

    unsigned char caracter;

    while (*clave != '\0'){

        caracter = (unsigned char)(*clave);

        hash = (hash * 33) + caracter; /* hash * 33 + c */

        clave++;
    }

    return hash % capacidad;

}

/**
 * Devuelve el factor de carga de la tabla hash.
 */
float factor_carga(size_t cantidad, size_t capacidad)
{
    if (capacidad < 0) {
        return 0;
    }

    float f_carga = (float)cantidad / (float)capacidad;  

	return f_carga;
}

/*
 * Se espera que la capacidad sea mayor a 0.
 *
 * Devuelve la posición a introducir en el diciconario
*/
size_t posicion_tabla(size_t clave, size_t capacidad)
{
    if (capacidad <= 0) {
        return 0;
    }

    size_t posicion = clave % capacidad;

    return posicion;
}


/*
 * Debo de duplicar el tamaño del diccionario
 *
 * Devuelve el índice a 
 * 
*/
// A RE-WORKEAR
bool re_hashing(diccionario_t *d)
{
    if (!d) {
        return false;
    }

    size_t tamanio = d->capacidad_hash * 2;

    bool exito = false;

    struct diccionario *aux = realloc(d, tamanio);

    if (!aux) {
        return exito;
    } else {
        d = aux;
        exito = true;
    }

    return exito;
}


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

    return nuevo_diccionario;
}

bool diccionario_insertar(diccionario_t *d, char *clave, void *valor,
			  void **anterior)
{
    if (!d || !clave || !anterior) {
        return false;
    }

    //Verificación de la tabla de hash (Ver si hace falta ReHashear)
    if (factor_carga(d->cantidad, d->capacidad_hash) >= )

    bool exito = false;

    size_t n_clave = hash(d->capacidad_hash, clave);

    size_t indice = posicion_tabla(n_clave, d->capacidad_hash);

    
    //Caso donde las claves son iguales
    if (strcmp(d->entradas[indice].clave, clave) == 0) {
        if (anterior) {
            *anterior = d->entradas[indice].valor;
        }
        d->entradas[indice].valor = valor;
        
        exito = true;
    }
    
    size_t inicio = indice;

    if (!exito) {
        bool fin = false;

        indice = (indice + 1) %d->capacidad_hash;
        while (d->entradas[indice].clave || !fin) {
            indice = (indice + 1) %d->capacidad_hash;

            if (indice == inicio) {
                fin = true;
            }
        }

        if (!fin) {
            d->entradas[indice].valor = valor;
            d->entradas[indice].clave = clave;
            
            exito = true;
        }

    }

    if (anterior) {
        *anterior = NULL;
    }
    
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
    //if (strcmp(d->entradas[indice].clave, clave) == 0) {
    //    valor_obt = d->entradas->valor;
    //} else {
    //    for (int i = indice + 1; i<d->capacidad_hash && !encontrado; i++) {
    //        if (strcmp(d->entradas[i].clave, clave) == 0) {
    //            valor_obt = d->entradas[i].valor;

    //            encontrado = true;
    //        }
    //    }
    //}

    return valor_obt;
}

void *diccionario_eliminar(diccionario_t *d, char *clave)
{
    if (!d || !clave) {
        return NULL;
    }

    void *data = { NULL };

    bool exito = false;

    //Logica

    if (exito) {
        d->cantidad--;
    }

    return data;
}

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

