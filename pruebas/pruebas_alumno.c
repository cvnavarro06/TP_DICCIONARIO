#include "pa2m.h"
#include "diccionario.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define CAPACIDAD_MIN 10

#define MAX_VECTOR 10

bool sumar_elementos_diccionario(char *clave, void *valor, void *extra)
{
	int *suma = (int *)extra;
	*suma += (int)(intptr_t)valor;
	return true;
}

static int elementos_destruidos = 0;

void destructor_contador(void *elemento)
{
	elementos_destruidos++;
}

// Pruebas en un diccionario vacío
void diccionario_pruebas_crear_vacio()
{
	pa2m_nuevo_grupo("PRUEBAS EN DICCIONARIO VACÍO");

	diccionario_t *d = diccionario_crear(CAPACIDAD_MIN);

	pa2m_afirmar(d != NULL, "Se crea correctamente el diccionario");

	pa2m_afirmar(diccionario_cantidad(d) == 0,
		     "Un diccionario recien creado tiene 0 elementos");

	char *clave1 = "no_existe";
	pa2m_afirmar(
		diccionario_buscar(d, clave1) == false,
		"Se busca una clave en un diccionario vacío, devuelve false");

	pa2m_afirmar(diccionario_obtener(d, clave1) == NULL,
		     "Se obtiene en un diccionario vacío, devuelve NULL");

	pa2m_afirmar(diccionario_eliminar(d, clave1) == NULL,
		     "Se elimina en un diccionario vacío, devuelve NULL");

	int extra = 2;

	pa2m_afirmar(diccionario_iterar(d, sumar_elementos_diccionario,
					(void *)(intptr_t)extra) == 0,
		     "Se itera un diccionario vacío, devuelve 0");

	diccionario_destruir(d);
}

// Insercion en diccionario NULL
void diccionario_pruebas_null()
{
	int n1 = 10;
	void *anterior = NULL;
	char *clave = "clave";

	pa2m_afirmar(diccionario_insertar(NULL, clave, (void *)(intptr_t)n1,
					  &anterior) == false,
		     "Insertar en undiccionario NULL devuelve false");

	pa2m_afirmar(diccionario_buscar(NULL, clave) == false,
		     "Buscar en un diccionario NULL devuelve false");

	pa2m_afirmar(diccionario_obtener(NULL, clave) == NULL,
		     "Obtener en un diccionario NULL devuelve NULL");

	pa2m_afirmar(diccionario_eliminar(NULL, clave) == NULL,
		     "Eliminar en un diccionario NULL devuelve NULL");

	pa2m_afirmar(diccionario_cantidad(NULL) == 0,
		     "Cantidad en un diccionario NULL devuelve 0");

	int extra = 4;

	pa2m_afirmar(diccionario_iterar(NULL, sumar_elementos_diccionario,
					(void *)(intptr_t)extra) == 0,
		     "Se itera un diccionario vacío, devuelve 0");
}

// Pruebas con claves NULL
void diccionario_pruebas_clave_null()
{
	pa2m_nuevo_grupo("PRUEBAS CON CLAVES NULL EN UN DICCIONARIO");

	diccionario_t *d = diccionario_crear(CAPACIDAD_MIN);
	int n1 = 40;
	char *clave_null = NULL;

	pa2m_afirmar(
		diccionario_insertar(d, clave_null, (void *)(intptr_t)n1,
				     NULL) == false,
		"Se intenta introducir en un diciconario un valor con clave NULL, devuelve false");

	pa2m_afirmar(diccionario_cantidad(d) == 0,
		     "La cantidad del diccionario es 0 (obtenido = %ld)",
		     diccionario_cantidad(d));

	pa2m_afirmar(
		diccionario_buscar(d, clave_null) == false,
		"Se intenta buscar un valor con clave NULL, devuelve false");

	pa2m_afirmar(
		diccionario_obtener(d, clave_null) == NULL,
		"Se intenta obtener un valor con clave NULL, devuelve NULL");

	diccionario_destruir(d);
}

void diccionario_pruebas_insertar_y_obtener()
{
	pa2m_nuevo_grupo("PRUEBAS DE INSERTAR, BUSCAR Y OBTENER ELEMENTOS");

	unsigned int capacidad = 1;

	diccionario_t *d = diccionario_crear(capacidad);
	int n1 = 10;
	int n2 = 20;

	char *clave1 = "clave1";
	char *clave2 = "clave2";

	pa2m_afirmar(diccionario_insertar(d, clave1, (void *)(intptr_t)n1,
					  NULL) == true,
		     "Se inserta un primer elemento");

	pa2m_afirmar(diccionario_cantidad(d) == 1,
		     "La cantidad del diccionario es 1 (obtenido = %ld)",
		     diccionario_cantidad(d));

	pa2m_afirmar(
		diccionario_buscar(d, clave1) == true,
		"Se intenta buscar el valor de la primera clave y se encuentra");

	pa2m_afirmar((int)(intptr_t)diccionario_obtener(d, clave1) == n1,
		     "Se obtiene el valor de la primera clave introducida");

	pa2m_afirmar(diccionario_insertar(d, clave2, (void *)(intptr_t)n2,
					  NULL) == true,
		     "Se inserta un segundo elemento'");

	pa2m_afirmar((int)(intptr_t)diccionario_obtener(d, clave2) == n2,
		     "Se intenta obtener el segundo valor insertado ");

	pa2m_afirmar(diccionario_cantidad(d) == 2,
		     "La cantidad del diccionario es 2 (obtenido = %ld)",
		     diccionario_cantidad(d));

	pa2m_afirmar(diccionario_insertar(d, clave2, (void *)(intptr_t)n1,
					  NULL) == true,
		     "Se remplaza el segundo elemento correctamente");

	pa2m_afirmar(
		diccionario_cantidad(d) == 2,
		"La cantidad del diccionario sigue siendo 2 (obtenido = %ld)",
		diccionario_cantidad(d));

	diccionario_destruir(d);
}

// Inserar varias claves - test re-hashing
void diccionario_insertar_varias_claves()
{
	pa2m_nuevo_grupo(
		"PRUEBAS DE INSERTAR Y BUSCAR VARIOS ELEMENTOS EN DICCIONARIO");

	unsigned int capacidad = 1;

	diccionario_t *d = diccionario_crear(capacidad);

	char *claves[MAX_VECTOR] = {
		"abc", "def",	"ghi",	"jkl",	 "mno",
		"pqz", "gamma", "beta", "theta", "epsilon"
	};
	int valores[MAX_VECTOR] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };

	size_t tope = 10;

	bool inserciones_exitosas = true;

	for (int i = 0; i < tope; i++) {
		if (!diccionario_insertar(d, claves[i], &valores[i], NULL)) {
			inserciones_exitosas = false;
		}
	}

	pa2m_afirmar(inserciones_exitosas,
		     "Se insertaron 10 elementos correctamente");

	bool se_encuentran_todos = true;

	for (int j = 0; j < tope; j++) {
		if (!diccionario_buscar(d, claves[j])) {
			se_encuentran_todos = false;
		}

		void *valor_obt = diccionario_obtener(d, claves[j]);

		if (valor_obt == NULL || *(int *)valor_obt != valores[j]) {
			se_encuentran_todos = false;
		}
	}

	pa2m_afirmar(se_encuentran_todos,
		     "Se encuentran todos los elementos insertados");

	diccionario_destruir(d);
}

void diccionario_pruebas_eliminar()
{
	pa2m_nuevo_grupo("PRUEBAS DE ELIMINACIÓN DE ELEMENTOS");

	diccionario_t *d = diccionario_crear(CAPACIDAD_MIN);

	int n1 = 37;
	int n2 = 67;

	char *clave1 = "clave1";
	char *clave2 = "clave2";
	char *clave3 = "clave3";

	diccionario_insertar(d, clave1, (void *)(intptr_t)n1, NULL);

	diccionario_insertar(d, clave2, (void *)(intptr_t)n2, NULL);

	pa2m_afirmar(diccionario_cantidad(d) == 2,
		     "Se introducen 2 claves correctamente");

	void *eliminado = diccionario_eliminar(d, clave1);

	pa2m_afirmar(eliminado != NULL, "Se elimina el primer valor insertado");

	pa2m_afirmar(
		(int)(intptr_t)eliminado == n1,
		"Se obtiene el valor eliminado (esperado = %d), (obtenido = %d)",
		n1, eliminado);

	pa2m_afirmar(
		diccionario_cantidad(d) == 1,
		"La cantidad del diccionario disminuyo con la clave eliminada");

	pa2m_afirmar(
		diccionario_buscar(d, clave1) == false,
		"Se busca la clave eliminada en el diccionario pero no se encuentra");

	pa2m_afirmar(
		diccionario_eliminar(d, clave3) == NULL,
		"Eliminar una clave que no esta en el diccionario devuelve NULL");

	void *eliminado2 = diccionario_eliminar(d, clave2);

	pa2m_afirmar(eliminado2 != NULL,
		     "Se elimina el segundo valor insertado");

	pa2m_afirmar(
		(int)(intptr_t)eliminado2 == n2,
		"Se obtiene el valor eliminado (esperado = %d), (obtenido = %d)",
		n2, eliminado2);

	pa2m_afirmar(
		diccionario_cantidad(d) == 0,
		"Eliminar el ultimo elemento del diccionario vacia el mismo");

	diccionario_destruir(d);
}

void diccionario_pruebas_iterador_interno()
{
	pa2m_nuevo_grupo("PRUEBAS DEL ITERADOR INTERNO DEL");

	diccionario_t *d = diccionario_crear(CAPACIDAD_MIN);
	int n1 = 10;
	int n2 = 20;
	int n3 = 30;

	diccionario_insertar(d, "c1", (void *)(intptr_t)n1, NULL);
	diccionario_insertar(d, "c2", (void *)(intptr_t)n2, NULL);
	diccionario_insertar(d, "c3", (void *)(intptr_t)n3, NULL);

	int suma = 0;
	int suma_esperada = 60;

	size_t iterados =
		diccionario_iterar(d, sumar_elementos_diccionario, &suma);

	pa2m_afirmar(
		diccionario_iterar(d, NULL, &suma) == 0,
		"El iterador interno devuelve 0 cuando no se le pasa una función por parámetro");

	pa2m_afirmar(
		iterados == 3,
		"El iterador interno recorrió los 3 elementos introducidos (obtenido = %ld)",
		iterados);

	pa2m_afirmar(
		suma == suma_esperada,
		"La suma total acumulada por el iterador interno es %i (obtenido = %i)",
		suma_esperada, suma);

	diccionario_destruir(d);
}

void diccionario_pruebas_destruir_todo()
{
	pa2m_nuevo_grupo("PRUEBAS DE DICCIONARIO_DESTRUIR_TODO");

	diccionario_t *d = diccionario_crear(CAPACIDAD_MIN);

	int n1 = 1;
	int n2 = 2;
	char *clave1 = "clave1";
	char *clave2 = "clave2";

	diccionario_insertar(d, clave1, (void *)(intptr_t)n1, NULL);
	diccionario_insertar(d, clave2, (void *)(intptr_t)n2, NULL);

	diccionario_destruir_todo(d, destructor_contador);

	pa2m_afirmar(
		elementos_destruidos == 2,
		"diccionario_destruir_todo invocó la función destructora 2 veces (obtenido = %i)",
		elementos_destruidos);
}

int main()
{
	pa2m_nuevo_grupo("== PRUEBAS TDA DICCIONARIO ==");
	diccionario_pruebas_crear_vacio();
	diccionario_pruebas_null();
	diccionario_pruebas_clave_null();

	diccionario_pruebas_insertar_y_obtener();
	diccionario_insertar_varias_claves();

	diccionario_pruebas_eliminar();
	diccionario_pruebas_iterador_interno();
	diccionario_pruebas_destruir_todo();

	return pa2m_mostrar_reporte();
}