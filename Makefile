GCC_FLAGS=-Wall -Wconversion -Wtype-limits -pedantic -Werror -g
#Creo un nombre por predeterminado por si el usuario se olvida de mandarlo por parametro
NOMBRE ?= programa
NOMBRE_TEST ?= testing


manual:
	@echo "=========================================================================="
	@echo "              MANUAL DE USO Y COMANDOS DEL PROGRAMA"
	@echo "==========================================================================\n"
	@echo "    1. Compilación:"
	@echo "    • make compile                  -> Compila el programa principal (Por defecto: $(NOMBRE))"
	@echo "    • make compile NOMBRE=nombre    -> Compila asignándole un nombre personalizado"
	@echo "    • make compile_test             -> Compila el set de pruebas (Por defecto: $(NOMBRE_TEST))\n"
	@echo "    2. Ejecución:"
	@echo "    • ./$(NOMBRE) <archivo> <texto>  -> Ejecuta el programa principal (requiere compilar antes)"
	@echo "    • make test                     -> Ejecuta las pruebas unitarias\n"
	@echo "   > Nota: Recuerda compilar siempre antes de ejecutar el progrma."
	@echo "=========================================================================="

compile:
	gcc $(GCC_FLAGS) main.c src/*.c -o $(NOMBRE)
	@echo "\nSe ha creado un archivo .o llamado $(NOMBRE)\n\nSi quieres saber como ejercutarlo escribe el siguiente comando:\n"

	@echo "make exp\n"

compile_test:
	gcc ${GCC_FLAGS} pruebas/*.c src/*.c -o ${NOMBRE_TEST}
	@echo "La compilación ha sido exitosa!"

test:
	./${NOMBRE_TEST}
	
exp:
	@echo "=========================================================================="
	@echo "                        COMO CORRER EL PROGRAMA"
	@echo "==========================================================================\n"
	@echo " 	1. En el parámetro de archivo se necesitará pasar un archivo"
	@echo " 	   con el siguiente parámetro:"
	@echo " 	   • <llave,valor>"
	@echo " 	2. En el parámetro de texto, se deberá de enviar un texto"
	@echo " 	   de tamaño deseado.\n"
	@echo " 	- ¿Como funciona el programa?\n"
	@echo " 	  El programa leéra todo el texto e irá comparando"
	@echo " 	  palabra por palabra encontrado si alguna es una llave"
	@echo " 	  pertieneciente al archivo brindado, si se encuentra"
	@echo " 	  se remplazará la palabra del texto por el valor"
	@echo " 	  almacenado en por la llave."


