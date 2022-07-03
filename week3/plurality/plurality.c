#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
// pasa la lista de nombre y cuando lo encuentra suma el voto y devuelve true
// si no esta ese nombre devuelve false al llegar al final
bool vote(string name)
{
    for (int i=0; i<MAX; i++)
    {
        if (candidates[i].name!=NULL && strcmp(candidates[i].name, name)==0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
// busco la puntuacion mas alta entre todos los candidatos
void print_winner(void)
{
    int pmax=0;
    for (int i=0; i<MAX; i++)
    {
        if(candidates[i].votes > pmax)
        {
            pmax=candidates[i].votes;
        }
    }

//muestro todos los candidatos que tienen la puntuacion mas alta
    for (int i=0; i<MAX; i++)
    {
        if (candidates[i].votes == pmax)
        printf("%s\n", candidates[i].name);
    }

    return;
}



/*

intento de ordenar y luego mostrar los primeros


// imprime el nombre que tiene mas votos, si hay empate imprime los dos
// ordeno todos los numeros
// paso de uno al otro, y si tiene misma puntuacion que el primero los imprimo, sino fuera


    candidate reserva;
    int n=0, p=0, r=0;
    while(n<MAX)
    {
        p=0;
        for (int i=n; i<MAX; i++)
        {
            if (candidates[i].name!=NULL && candidates[i].votes>p)
            {
                r=i;
            }
        }
        if (n!=p && p!=0)
        {
            reserva=candidates[n];
            candidates[n]=candidates[r];
            candidates[r]=reserva;
        }
        n++;
    }

    // imprimo el primero
    printf("%s\n", candidates[0].name);
    //reviso si los siguientes tienes la misma puntuacion y los imprimo
    //si el siguiente no es igual me salgo
    for (int i=1; i<MAX; i++)
    {
        if (candidates[0].votes==candidates[i].votes && candidates[i].name!=NULL)
        {
            printf("%s\n", candidates[i].name);
        }
        else
        {
            return;
        }
    }





//Intento de merge fail porque no me deja pasar la lista a traves de la funcion
// es un tema de pointer que todavia no he llegado



void merge(candidate candidates)
{
    //compruevo el largo de las lista, y si es menor de 2 vuelvo
    int n=strlen(lista);
    if (strlen(n)<2)
    {
        return;
    }
    //creo dos listas para hacer las mitades
    candidate lista_1[n/2];
    candidate lista_2[n/2];

    //hasta la mitad del largo lo paso a la primera lista
    for (int i=0; i<n/2; i++)
    {
        candidate lista_1[i]=lista[i];
    }
    //de la mitad al final lo paso a la segunda
    for (int i=n/2; i<n; i++)
    {
        candidate lista_2[i]=lista[i];
    }

    //envio a ordenar las dos mitades
    merge(lista_1);
    merge(lista_2);

    //ordeno las dos mitades ya ordenadas
    int n1=0; n2=0;
    for (int i=0; i<n; i++)
    {
        if (lista_1[n1].votes >= lista_2[n2].votes)
        {
            lista[i]==lista_1[n1];
            n1++;
        }
        else if (lista_2[n2].votes > lista_1[n1].votes)
        {
            lista[i]==lista_2[n2];
            n2++;
        }
    }
    return lista;
}

*/