#include "diccionario.h"
#include <ctype.h>

#define ARCHIVO 2

#define TEXTO 3

#define ERROR -1
#define NULO "NULL"

int no_case_sensitive_strcmp(const char *string_1, const char *string_2)
{
	bool diferentes = false;

	int diferencia = tolower(*string_1) - tolower(*string_2);
	while (*string_1 && *string_2 && !diferentes) {
		//Si es distinto de 0 significa que lo son iguales, por ende no es necesario seguir el bucle
		if (diferencia != 0) {
			diferentes = true;
		} else {
			//Avanzo haciendo aritmetica de punteros
			string_1++;
			string_2++;
			diferencia = tolower(*string_1) - tolower(*string_2);
		}
	}

	if (diferentes) {
		return diferencia;
	}

	return diferencia;
}


int main(int argc, char *argv[])
{
    //if (argc < 3) {
    //    //Mensaje
    //    return ERROR;
    //}

    if (no_case_sensitive_strcmp(argv[ARCHIVO], NULO) == 0) {
        return ERROR;
    }

    //Funcion de parsear (Cuando encuentra un espacio).

    // Averiguar si se puede usar un DS cola
		
		//Simplemente desencolo secuencialmente y veo si la palabra es una clave o no, si no lo es lo printeo nomal, si lo es printeo su valor 
}