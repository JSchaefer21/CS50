// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdbool.h>

#include "dictionary.h"
/*
el ejercicio se baja en un array table sobre el cual en cada posicion
del array hay una lista de nodos
*/

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 4000;

// Hash table
node *table[N];

int count=0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int clave = hash(word); //siempre se enviara al hash en minuscula

    node *cursor = table[clave]; // con un nodo nuevo llamado cursor que contenga el primer nodo de mi lista correspondiente a la clave
    while (cursor != NULL)  // voy buscando durante los siguientes nodos hasta que llegue al next == NULL
    {                        // y compruebo si la plabra de cursor es igual a la que busco
        if (strcasecmp(cursor->word, word) == 0) //la funcion devuelve 0 si son iguales
        {
            return true;
        }
        cursor = cursor -> next;  // mientras next no sea NULL voy buscando en los siguientes
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int n = 0;
    int limite = strlen(word);
    for (int i=0; i<limite; i++)
    {
        //combierto a minusculas, entrarla en minuscular ralentiza muchisimo y da errores
        // es mas rapido sumar sin mas, antes que multiplicar o cosas raras
        // sumar sin mas, con N=4000
        n = n + tolower(word[i]);
        //n = n + 37*tolower(word[i]); // copiada de collin's Note con N= 2000
    }
    n = n % N; // % N asegura que el valor no sobrepasa el valor de N
    return n;
    // ni idea de cuando puede llegar la tabla
    // si una palabra tiene 8 letras, entonces 10.000.000  ¿?
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //abro el diccionario
    FILE *file = fopen (dictionary, "r");

    // si el nombre no existe recivo un NULL, y aviso de que no existe el archivo
    if (file == NULL)
    {
        printf("Dictionary not found\n");
        return false;
    }

    //creo una variable para ir guardando palabras
    char buf[LENGTH + 1];;
    while (fscanf(file, "%s", buf) != EOF) // cuando iguala EOF es el final del archivo
    {
        node *new= malloc(sizeof(node)); //busco espacio a un nodo donde guardo cada palabra
        if (new == NULL)
        {
            return false;
        }
        strcpy(new->word, buf); // guardo la palabra en el nodo
        new->next= NULL;

        int clave= hash(buf); //devuelve index para guardar

        if (table[clave] == NULL)  //si el nodo de table esta vacio
        {
            table[clave] = new; //coloco el nuevo nodo en la lista
        } // si table esta ocupado, lo añado en la lista
        else  //no puedo añadir el nodo al final de la lista porque no se cual es el final
        {     // no se cuantos next he de hacer
            new -> next = table[clave];  //lo actual se lo paso al siguiente del nuevo
            table[clave] = new; // y el nuevo lo pongo al principio
        }
        count++;
    }
    fclose(file); //cerramos archivo al
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return count; // si hemos empezado a contar devolvera el valor
}                 // si no hemos contado nada la variable sera 0, y por lo tanto devuelve 0

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i=0; i<N; i++) // para cada archivo de la tabla
    {
        if (table[i] != NULL) // solo si hay algo dentro
        {
            node *cursor = table[i];
            while(cursor != NULL) //miro que la tabla no sea null
            {
                node *tmp = cursor;  // si no lo es me guardo el cursor en una variable
                cursor = cursor->next; // y lo paso al siguiente
                free(tmp); // borro el anterior
            }
            free(cursor); // y cuando llego al ultimo de todos borro ese tambien
        }
    }
    //free(table); // no tiene sentido borrar la tabla de array, solo se borran nodos
    return true; // y devuelvo true
}

// son necesarios tants free???? cuando hace falta y cuando no?