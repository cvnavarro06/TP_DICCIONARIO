#include "diccionario.h"
#include <string.h>
#include <stdio.h>

#define CARGA_MINIMA 0.75
#define CAPACIDAD_MINIMA 3

#define FACTOR_CARGA_MAX 0.75

#define ERROR -1

struct entrada
{   
    char *clave;
    void *valor;
};

struct diccionario 
{
    struct entrada *entradas;
    size_t capacidad_hash;
    size_t cantidad;
};


char *duplicar_string(char *string)
{
	if (string == NULL) {
		return NULL;
	}

	size_t largo_string = strlen(string) + 1;

	char *nuevo_string = malloc(largo_string * sizeof(char));

	if (nuevo_string != NULL) {
		memcpy(nuevo_string, string, largo_string);
	}

	return nuevo_string;
}

// Función "algoritmo DJB2" utilizado para hash
size_t hash(char *clave) 
{
    size_t hash = 5381;

    unsigned char caracter;

    while (*clave != '\0'){

        caracter = (unsigned char)(*clave);

        hash = (hash * 33) + caracter; /* hash * 33 + c */

        clave++;
    }

    return hash;

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
size_t posicion_tabla(size_t h_clave, size_t capacidad)
{
    if (capacidad <= 0) {
        return 0;
    }

    size_t posicion = h_clave % capacidad;

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
    if (!d || !clave) {
        return false;
    }

    //Verificación de la tabla de hash (Ver si hace falta ReHashear)
    //if (factor_carga(d->cantidad, d->capacidad_hash) >= CARGA_MINIMA) {
    //    re_hashing(d);
    //}

    size_t capacidad_hash_actual = d->capacidad_hash;

    bool exito = false;

    size_t n_clave = hash(clave);
    size_t posicion = posicion_tabla(n_clave, capacidad_hash_actual);
    
    //Caso donde las claves son iguales
    if (d->entradas[posicion].clave != NULL) {
        if (strcmp(d->entradas[posicion].clave, clave) == 0) {
            if (anterior) {
                *anterior = d->entradas[posicion].valor;
            }
            d->entradas[posicion].valor = valor;
            
            exito = true;
        }
    } else {
        char *d_clave = duplicar_string(clave);
        
        if (!d_clave) {
            return false;
        }

        d->entradas[posicion].clave = d_clave;
        d->entradas[posicion].valor = valor;

        exito = true;
    }
    
    
    if (!exito) {
        size_t original = posicion;
        
        bool fin = false;

        posicion = (posicion + 1) % capacidad_hash_actual;

        while (d->entradas[posicion].clave && !fin && !exito) {
            posicion = (posicion + 1) % capacidad_hash_actual;

            //Si encuentro el lugar donde está la clave, inserto.
            if (strcmp(d->entradas[posicion].clave, clave) == 0) {
                d->entradas[posicion].valor = valor;

                if (anterior) {
                    *anterior = d->entradas[posicion].valor;
                }
                exito = true;
            }

            //Si la tabla esta llena salgo de loop (Caso improbable)
            if (posicion == original) {
                fin = true;
            }
        }

        //Si no existía la clave en la tabla, inserto nuevos valores en la misma.
        if (!fin && !exito) {
            d->entradas[posicion].valor = valor;
            d->entradas[posicion].clave = clave;
            
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

    size_t n_clave = hash(clave);

    size_t posicion = posicion_tabla(n_clave, d->capacidad_hash);

    if (d->entradas[posicion].clave != NULL) {
        if (strcmp(d->entradas[posicion].clave, clave) == 0) 
            encontrado = true;
    }

    if (!encontrado) {
        posicion = (posicion + 1) % d->capacidad_hash;

        size_t original = posicion;
        
        bool fin = false;
        
        while (d->entradas[posicion].clave != NULL || !encontrado || !fin) {

            if (strcmp(d->entradas[posicion].clave, clave) == 0) {
                encontrado = true;
            }

            if (original == posicion) {
                fin = true;
            }
        }
    }

    return encontrado;
}

void *diccionario_obtener(diccionario_t *d, char *clave)
{
    if (d == NULL || clave == NULL || diccionario_cantidad(d) == 0) {
        return NULL;
    }
    
    char *valor_obt = { NULL };

    size_t capacidad_hash_actual = d->capacidad_hash;

    size_t n_clave = hash(clave);

    size_t posicion = posicion_tabla(n_clave, capacidad_hash_actual);

    bool encontrado = false;

    if (strcmp(d->entradas[posicion].clave, clave) == 0) {
        valor_obt = d->entradas[posicion].valor;

        encontrado = true;
    }

    if (!encontrado) {
        size_t original = posicion;

        bool fin = false;
        posicion = (posicion + 1) % capacidad_hash_actual;
        
        //Voy buscando en proximos indices de la tabla
        while (d->entradas[posicion].clave || !fin || !encontrado) {
            
            //Si son iguales, las claves, devuelvo el valor
            if (strcmp(d->entradas[posicion].clave, clave) == 0) {
                valor_obt = d->entradas[posicion].valor;

                encontrado = true;
            }

            /* Si ya recorrí toda la tabla y no se encontro la clave, dejo de buscar.
             * Por defecto se devolverá NULL
            */
            if (original = posicion) {
                fin = true;
            }
        }
    }

    return valor_obt;
}

void *diccionario_eliminar(diccionario_t *d, char *clave)
{
    if (!d || !clave || diccionario_cantidad(d) == 0) {
        return NULL;
    }

    void *dato_eliminado = { NULL };

    bool exito = false;

    size_t capacidad_hash_actual = d->capacidad_hash;
    
    size_t n_clave = hash(clave);
    
    size_t posicion = posicion_tabla(n_clave, d->capacidad_hash);

    bool encontrado = false;

    bool fin = false;
    
    size_t original = posicion; 

    while (d->entradas[posicion].clave != NULL && !encontrado && !fin) {

        if (strcmp(d->entradas[posicion].clave, clave) == 0) {
            encontrado = true; 
        }
        
        posicion = (posicion + 1) % capacidad_hash_actual;

        if (posicion == original) {
            fin = true;
        }
    }

    //Caso: No se encontro la clave
    if (d->entradas[posicion].clave == NULL) {
        return NULL;
    }

    if (encontrado) {
        dato_eliminado = d->entradas[posicion].valor;

        free(d->entradas[posicion].clave);
        d->entradas[posicion].clave = NULL;
        d->entradas[posicion].valor = NULL;

        exito = true;
    }

    size_t vacio = posicion;
    size_t siguiente = (vacio + 1) % capacidad_hash_actual;

    //Si el espacio siguiente del eliminado está vacío, no se debe de acomodar nada.
    while (d->entradas[siguiente].clave != NULL) {
        
        size_t clave_m = hash(d->entradas[siguiente].clave);
        size_t posicion_m = posicion_tabla(clave_m, capacidad_hash_actual);

        bool se_mueve = false;

        if (posicion_m <= siguiente) {
            if (posicion_m <= vacio && vacio < siguiente) se_mueve = true;

        } else { // Manejo del solapamiento circular (wrap-around)

            if (vacio >= posicion_m || vacio < siguiente) se_mueve = true;
        }

        // Mover el elemento al hueco y convertir 'next' en el nuevo hueco
        if (se_mueve) {
            d->entradas[vacio] = d->entradas[siguiente];
            d->entradas[siguiente].clave = NULL;
            d->entradas[siguiente].valor = NULL;
            vacio = siguiente;
        }

        siguiente = (siguiente + 1) % capacidad_hash_actual;
    }

    if (exito) {
        d->cantidad--;
    }

    return dato_eliminado;
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

