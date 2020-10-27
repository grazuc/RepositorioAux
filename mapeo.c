#include <stdio.h>
#include <stdlib.h>

#include "lista.h"
#include "mapeo.h"

void re_hash(tMapeo m);

int max(int n1, int n2){
    int toreturn = 0;

    if (n1 == n2){
        toreturn = n1;
    }
    else if (n1 > n2){
        toreturn = n1;
    }
    else{
        toreturn = n2;
    }
    return toreturn;
}

/**
 Inicializa un mapeo vacío, con capacidad inicial igual al MAX(10, CI).
 Una referencia al mapeo creado es referenciada en *M.
 A todo efecto, el valor hash para las claves será computado mediante la función fHash.
 A todo efecto, la comparación de claves se realizará mediante la función fComparacion.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
extern void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)){

    int capacidad_inicial = max(10, ci);

    (*m) = (tMapeo) malloc(sizeof (struct mapeo));

    if (*m == NULL){
        exit (MAP_ERROR_MEMORIA);
    }

    tLista * arraylists = (tLista*) malloc(sizeof(tLista) * capacidad_inicial);

    for (int i=0; i < capacidad_inicial; i++){
        crear_lista(&arraylists[i]);
    }

    (*m)->cantidad_elementos = 0;
    (*m)->longitud_tabla = capacidad_inicial;
    (*m)->hash_code = fHash;
    (*m)->comparador = fComparacion;
    (*m)->tabla_hash = arraylists;
}

/**
 Inserta una entrada con clave C y valor V, en M.
 Si una entrada con clave C y valor Vi ya existe en M, modifica Vi por V.
 Retorna NULL si la clave C no existía en M, o Vi en caso contrario.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
extern tValor m_insertar(tMapeo m, tClave c, tValor v){
    tLista  lista;
    tPosicion pos;
    tPosicion pos_fin;
    int encontre = 0;
    int bucket;
    tValor toRet = NULL;
    tEntrada entry;
    tEntrada nuevo;

    if(m == NULL || c == NULL || v == NULL) {
        exit(MAP_ERROR_MEMORIA);
    }

    bucket = m->hash_code(c) % m->longitud_tabla;
    lista = (tLista) (m->tabla_hash + bucket); //obtiene el arreglo en la posición bucket
    pos = l_primera(lista);
    pos_fin = l_fin(lista);

    while (pos != pos_fin && !encontre) {
        entry = (tEntrada)l_recuperar(lista,pos);
        tClave *clave = entry->clave;

        if(m->comparador(clave,c) != 0){//Si es true
            toRet = entry->valor;
            entry->valor = v; //modifico el valor
            encontre = 1;
        }
        pos = l_siguiente(lista,pos);
    }

    if(!encontre){
        if(m->cantidad_elementos +1 / m->longitud_tabla >  0.9){ //Si supera el factor de carga
            re_hash(m);
        }
        nuevo = (tEntrada) malloc(sizeof(struct entrada));
        nuevo->clave = c;
        nuevo->valor = v;
        l_insertar(lista,pos,nuevo);
    }

    return toRet;
}
tValor m_recuperar(tMapeo m, tClave c){
    int bucket = m->hash_code(c) % m->longitud_tabla;
    int encontre = 0;
    tLista mi_lista = m->tabla_hash[bucket];
    tValor toReturn = NULL;
    tPosicion pos_actual, pos_fin;
    tEntrada entrada;

    pos_actual = l_primera(mi_lista);
    pos_fin = l_fin(mi_lista);
    while(!encontre && pos_actual!=pos_fin){ //Se recorre la lista correspondiente al bucket.
        entrada = (tEntrada) l_recuperar(mi_lista, pos_actual);
        if (m->comparador(c, entrada->clave) == 0){ //Las claves son iguales
            encontre = 1;
            toReturn = entrada->valor;
        }
        else
            pos_actual = l_siguiente(mi_lista, pos_actual);
    }

    return toReturn;
}


void re_hash(tMapeo m){
    tEntrada entry;
    m->cantidad_elementos = 0;
    tPosicion pos;
    tPosicion pos_fin;
    int size = 2 * m->longitud_tabla;
	tLista *nuevo_arraylists[size];
    (*nuevo_arraylists) = malloc(sizeof(tLista) * size);

	for (int i=0; i < size; i++){
        crear_lista(nuevo_arraylists[i]);
	}

    for(int i = 0; i<m->longitud_tabla;i++){//Recorro las tablas del mapeo original
        pos = l_primera(m->tabla_hash[i]);//primer posicion de la lista ubicada en arraylists[i]
        pos_fin = l_fin(m->tabla_hash[i]);//ultima posicion de la lista ubicada en arraylists[i]

        while (pos != pos_fin) {
            entry = (tEntrada) l_recuperar(m->tabla_hash[i],pos);
            m_insertar(m,entry->clave, entry->valor);
            pos = l_siguiente(m->tabla_hash[i], pos);
        }
    }
}




