#ifndef __DICCIONARIO_H__
#define __DICCIONARIO_H__

#include <stdbool.h>
#include <stdlib.h>

typedef struct diccionario diccionario_t;

/*
 * Crea un diccionario con una capacidad inicial. Si la capacidad inicial es <= 3, se utiliza 3.
 */
diccionario_t *diccionario_crear(unsigned capacidad_inicial);

/*
 * Inserta clave/valor, devuelve true si pudo o false si no pudo. Si ya existía
 * la clave apunta anterior al valor asociado anteriormente.
 *
 * Si se necesita agrandar la tabla no está permitido hacerla crecer a mas del doble del tamaño.
 *
 *  No es válido insertar claves NULL.
 */
bool diccionario_insertar(diccionario_t *d, char *clave, void *valor,
			  void **anterior);

/*
 * Busca una clave en el diccionario y devuelve true si existe.
 */
bool diccionario_buscar(diccionario_t *d, char *clave);

/*
 * Busca una clave en el diccionario y devuelve el valor asociado.
 */
void *diccionario_obtener(diccionario_t *d, char *clave);

/*
 * Elimina una clave de la tabla y devuelve el valor asociado a esa clave o NULL si no existía.
 */
void *diccionario_eliminar(diccionario_t *d, char *clave);

/*
 * Devuelve la cantidad de claves almacenadas en el diccionario.
 */
size_t diccionario_cantidad(diccionario_t *d);

/*
 * Recorre el diccionario y aplica la función f a cada valor del diccionario.
   Devuelve la cantidad de veces que se invoca la función f.
 */
size_t diccionario_iterar(diccionario_t *d, bool (*f)(char *, void *, void *),
			  void *extra);

/*
 * Destruye el diccionario y la memoria asociada.
 */
void diccionario_destruir(diccionario_t *d);
void diccionario_destruir_todo(diccionario_t *d, void (*destructor)(void *));

#endif /* __DICCIONARIO_H__ */