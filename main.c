#include <stdio.h>
#include "lista.h"
#include "mapeo.h"
#include <stdlib.h>
#include <stdbool.h>

void fEliminar(tElemento e){
    free(e);
    e = NULL;
}

size_t fHash(char* clave, size_t tam){
   unsigned long number = 5381;
   int c;
   while ((c = *clave++))
	   number = ((number << 5) + number) + c;
   return number % tam;
}

int fComparacion(char* key1,char*key2){
    int toRet=1;
    if (strcmp(key1,key2)!=0){
        toRet=0;
    }
    return toRet;
}

int cantidadPalabras(FILE *f){
    int c;
    int cantidad=0;
    bool auxiliarCuenta=true;
    while ((c = fgetc(f)) != EOF){
                if (c!=32 && auxiliarCuenta){
                     cantidad++;
                     auxiliarCuenta=false;
                } else if (c==32){
                    auxiliarCuenta=true;
                    }
           }
    fclose(f);
    return cantidad;
}

int main(int argc, char *argv[]){
    tMapeo map =NULL;
    FILE *fp;
    char *nombreArchivo;
    int c;
    int cantPalabras=0;

    // Se fija que el archivo no tenga espacios, si los tiene es porque el nombre del archivo es invalido.
    if (argc>2){
        printf("El nombre del archivo es invalido o no existe.\n");
        return(1);
        }
    else{
        nombreArchivo = argv[1];
        printf("Nombre de archivo : %s\n", nombreArchivo);
        }

        // Abro el archivo en read-only para que el usuario no pueda modificarlo.
        fp = fopen(nombreArchivo,"r");

        // Si el archivo se pudo abrir correctamente se empieza a printear el mismo (la idea acá seria empezar a meterlo en el mapeo PERO NO LO PUEDO INICIALIZAR PORQUE TENGO PROBLEMAS EN LA CABEZA)
        if (fp){
            printf("El archivo contiene:\n");
            cantPalabras= cantidadPalabras(fp);
            map = crear_mapeo(&map,cantPalabras,fHash*(void*),fComparacion*(void*,void*));
            fp = fopen(nombreArchivo,"r");
            while ((c = fgetc(fp)) != EOF){
                printf("%c",c);
            }
           printf("Cantidad de palabras: %i\n",cantPalabras);
       }
        else{
         printf("No se pudo abrir el archivo\n");
            }
    fclose(fp);
    return(0);
}
