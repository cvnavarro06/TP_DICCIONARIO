# TP - TDA Diccionario

## Información personal

* **Nombre** = Constantino Valentín Navarro
* **Padrón** = 114821
* **E-mail** = cvnavarro@fi.uba.ar
* **User de github** = cvnavarro06

---

## Índice
* [1. Instrucciones para el uso correcto del programa](#1-instrucciones-para-el-uso-correcto-del-programa)
  * [Compilación](#compilación)
  * [Correr programa](#correr-programa)
  * [Correr valgrind](#correr-valgrind)
* [2. Funcionamiento](#2-Funcionamiento)
* [3. Estructura](#3-Estructura)
  * [3.1. Diagrama de memoria](#31-Diagrama-de-memoria)
  * [3.2. Análisis de complejidades](#32-Análisis-de-complejidades)
* [4. Decisiones de diseño y/o complejidades de implementación](#4-Decisiones-de-diseño-yo-complejidades-de-implementación)
* [5. Respuestas a las preguntas teóricas](#5-Respuestas-a-las-preguntas-teóricas)
---
<br>

# 1. Instrucciones para el uso correcto del programa

## Compilación
```
make compile NOMBRE=[opcional]
```
---
## Correr programa
```
./<ejecutable> <archivo> <texto>
```
---
### Mas detalles de compilación
```
make exp
```

---
## Correr pruebas

### Primero compilo las mismas
```
make compile_test NOMBRE_TEST=[Opcional]
```

### Correr pruebas
```
make test
```
---

## Correr valgrind
```
./valgrind --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes -s ./<ejecutable> <archivo> <texto>
```
---

## Para más información
```
make manual
```
---
<br>

# 2. Funcionamiento del programa
Este programa funciona como una especie de traductor de palabras, **¿Cómo funciona por dentro?**

- Para el correcto funcionamiento del programa, el mismo espera entrada especifica por linea de comandos, más especificamente un archivo con el formato implementado [acá](files/test.csv).<br>
El mismo formato será de la siguiente manera: `<clave,valor>`.<br><br>
Por otro lado, el segundo parámetro que espera el programa es un texto, el mismo pordrá tener una longitud deseada por el usuario.

**¿Cómo "traduce" el programa?**

## Algorimo interno

- La primera tarea del programa será cargar todas las claves del archivo a un diccionario, siempre y cuando no ocurra ningún error en el proceso la carga será exitosa.<br>
Luego de haber cargado un diccionario con todas sus `<clave,valor>` se parseará el texto enviado por el usuario, utilizado como separador del texto el espacio `" "`. Mientras se va parseando el texto las palabras separadas del mismo se cargarán en una estrutura de datos `cola`. **¿Por qué se decidio esta estrutura de datos?**

### Desición de la estructura de datos
Previamente a entender el por qué se eligió esta estructura, se debe considerar la salida del programa esperado. Del mismo se espera que muestre el texto que el usuario envió en el mismo orden de palabras con el cambio de que si hay una clave presente en el mismo texto, se cambie por el valor representado por la llave puesta en el archivo anteriormente enviado por el usuario. <br>
Luego de la previa explicación, se opto por la estructura de datos `cola` para aprovechar el principio de **(FIFO)** ya que la primera palabra introducida en la cola será la primera en ser motrada por terminal. ¿Cómo el programa sabe cual palabra es la clave y cual no?

### Mostrar por la terminal
Hay una función dedicada a evaluar cuál palabra es una clave o no, dicha función, dentro del [main.c](main.c), es `mostrar_por_pantalla()`. La misma se encarga de ir desencolando y evaluar si dicha palabra extraída se encuentra en el diccionario, si es así se mostrará por pantalla el valor que representa la misma clave, caso contrario se mostrará la misma palabra extraída.

## Funciones de [`diccionario.c`](src/diccionario.c)

### Estructura de `diccionario`

```
struct diccionario
{
  struct entrada *entradas; -> Representa <clave;valor>
  size_t capacidad_hash;
  size_t cantidad;
};
```

Para implementar el `diccionario` se optó de tener el la propia estructura, una estructura externa que representa las claves y valores que se vayan almacenando en el mismo diccionario.

Visualización del mismo:
```
struct entrada
{
  char *clave;
  void *valor;
};
```
Para implementar esta estructura general se requiere de utilizar una función hash, la cuál se encargará de ir poniendo cada entrada en una posición especifica respecto a la `capacidad` del hash.

Para comprender mejor esta estructura veamos qué es y que otras implementaciones de dicha estructura puede tener.

### ¿Qué es un diccionario?
Un Diccionario es un Tipo de Dato Abstracto diseñado para almacenar una colección de pares/entradas de datos `<clave,valor>` en donde cada clave es única.

### Formas distintas de implementar uno diccionario

#### Lista enlazada
Los pares (clave, valor) se van guardando de forma secuencial a medida que se insertan, sin mantener ningún orden específico.
```
[Cabeza]
     │
     ▼
┌──────────────┐     ┌──────────────┐     ┌──────────────┐
│ Clave: "juan"│ --> │ Clave: "pato"│ --> │Clave: "alfa" │ --> NULL
│ Valor:  nito │     │ Valor: donald│     │ Valor:  romeo│
└──────────────┘     └──────────────┘     └──────────────┘
```
Si bien esta implementación sería sencilla tiene una desventaja respecto a una tabla de hash, dicha desventaja es que sus primitivas escenciales como insertar y eliminar tendrán una complejidad de **O(n)** volviendolo una opción poco elegida.

#### Arbol binario
Las entradas `<clave,valor>` se organizan jerárquicamente en un árbol binario ordenado según la clave, el mismo orden será de la siguiente manera. (nodo_izq (menor) <-- nodo_padre --> nodo_der (mayor))
```
                     ┌──────────────────┐
                     │  Clave: "j_pato" │
                     │  Valor:   67     │
                     └────────┬─────────┘
                              │
             ┌────────────────┴────────────────┐
             ▼                                 ▼
   ┌──────────────────┐              ┌──────────────────┐
   │  Clave: "six"    │              │  Clave: "zeven"  │
   │  Valor:   6      │              │  Valor:   7      │
   └──────────────────┘              └──────────────────┘
```
Como se ve en el diagrama, los nodos estarán ordenados de menor a mayor, dicha estructura deberá de ser así para lograr una complejidad en las funciones escenciales de **O(log n)** al ser una busqueda binaria.
> Importante: Para que tenga dicha complejidad, el arbol deberá de estar balanceado, por ende será un **arbol AVL, rojo-negro, etc.**

#### Tabla hash
Esta implementación usa una función de hash $hash(clave)$ que convierte cualquier clave en un índice numérico indicando un posición en un "vector".
```

       0               1               2               3           4
  ┌──────────┐    ┌──────────┐    ┌──────────┐    ┌──────────┐    ┌───┐
  │   NULL   │    │ "juan":37│    │ "pato":67│    │   NULL   │    │...│
  └──────────┘    └──────────┘    └──────────┘    └──────────┘    └───┘
```

Para esta implementación de diccionario, como se dijo anteriormente, se utiliza una función **hash** para determinar un índice especifico, el por qué de la función es para que a la hora de "hashear" una clave, el resultado de la misma función siempre sea el mismo índice en el vector, volviendolo una solución deterministica, cuyo factor es clave para esta implementación, ya que vuelve a la misma una solución ideal para un diccionario, a causa de que sus funciones escenciales se vuelve, en un caso promeido, **O(1)**.

Para este programa se decidio utilizar el algoritmo **DJB2** como función hash el cuál tiene como fin, ser deterministica cumpliendo con la principal caracteristica de una tabla hash.





