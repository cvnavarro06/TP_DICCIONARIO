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
    if (factor_carga(d->cantidad, d->capacidad_hash) >= CARGA_MINIMA) {
        re_hashing(d);
    }

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
    
    size_t og = indice;

    if (!exito) {
        bool fin = false;

        indice = (indice + 1) %d->capacidad_hash;

        while (d->entradas[indice].clave || !fin) {
            indice = (indice + 1) %d->capacidad_hash;

            //Si encuentro el lugar donde está la clave, inserto.
            if (strcmp(d->entradas[indice].clave, clave) == 0) {
                d->entradas[indice].valor = valor;

                if (anterior) {
                    *anterior = d->entradas[indice].valor;
                }
                exito = true;
            }

            //Si la tabla esta llena salgo de loop (Caso improbable)
            if (indice == og) {
                fin = true;
            }
        }

        //Si no existía la clave en la tabla, inserto nuevos valores en la misma.
        if (!fin && !exito) {
            d->entradas[indice].valor = valor;
            d->entradas[indice].clave = clave;
            
            if (anterior) {
                *anterior = NULL;
            }

            exito = true;
        }

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

    size_t capacidad_hash_actual = d->capacidad_hash;

    size_t n_clave = hash(capacidad_hash_actual, clave);

    size_t indice = posicion_tabla(clave, capacidad_hash_actual);

    bool encontrado = false;

    if (strcmp(d->entradas[indice].clave, clave) == 0) {
        valor_obt = d->entradas[indice].valor;

        encontrado = true;
    }

    if (!encontrado) {
        size_t og = indice;

        bool fin = false;
        indice = (indice + 1) % capacidad_hash_actual;
        
        //Voy buscando en proximos indices de la tabla
        while (d->entradas[indice].clave || !fin || !encontrado) {
            
            //Si son iguales, las claves, devuelvo el valor
            if (strcmp(d->entradas[indice].clave, clave) == 0) {
                valor_obt = d->entradas[indice].valor;

                encontrado = true;
            }

            /* Si ya recorrí toda la tabla y no se encontro la clave, dejo de buscar.
             * Por defecto se devolverá NULL
            */
            if (og = indice) {
                fin = true;
            }
        }
    }

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

void diccionario_destruir_todo(diccionario_t *d, void (*destructor)(void *))
{
    if (!d || !destructor) {
        return;
    }

    for (int i = 0; i<d->capacidad_hash; i++) {

        if (&d->entradas[i] != NULL) {
            free(d->entradas[i].clave);
            destructor(d->entradas[i].valor);
        }
    }

    free(d->entradas);
    free(d);
}

