<div align="left">
    <img width="32px" src="img/algo2.svg">
</div>

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