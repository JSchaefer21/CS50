#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool bloqueo(int g, int p);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    //me entra el numero de preferencia de cada votante
    // el nombre de la preferencia rank
    // el array donde los pongo en orden de preferencia
    for (int i=0; i<candidate_count; i++)
    {
        if (strcmp(candidates[i], name)==0)
        // si el nombre existe
        {
            ranks[rank]=i;
            //meto el numero correspondiente al nombre
            //dentro del array
            //en la posicion de la preferencia enviada
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
// cuando ha votado 1 persona paso sus datos al array general
void record_preferences(int ranks[])
{
    for (int i=0; i<candidate_count-1; i++)
    {
        //estan ya ordenados de antes
        // prefiere el primero sobre el siguiente y sobre todos los demas hasta el final
        // y asi con cada uno de los siguientes hasta el final
        //sobre esa persona de preferencia uno con el otro le sumo 1
        int b=i+1;
        while (b<candidate_count)
        {
            preferences[ranks[i]][ranks[b]]++;
            b++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
// cuando ya han votado todos hago recuento de parejas ganadores con perdedores
void add_pairs(void)
{
    for (int i=0; i<MAX; i++)
    {
        for (int j=0; j<MAX; j++)
        {
            //para todo el array de preferencia introducidas
            //si uno es mejor que el otro le sumo
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner=i;
                pairs[pair_count].loser=j;
                pair_count++;
            }
/*            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner=j;
                pairs[pair_count].loser=i;
                pair_count++;
            }
            esto no hace falta ya que hare toda la matriz y se compararan los contrarios
            */
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
// ordeno el array de parejas segun la diferencia entre ellos, bas dif van primero
void sort_pairs(void)
{
    pair reserva;
    int n=0, p=0, dif=0, dif1=0, dif2=0;
    while(n<pair_count)
    {
        p=0;
        for (int i=n; i<pair_count; i++)
        {
            dif1=(preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner]);
            dif2=(preferences[pairs[i+1].winner][pairs[i+1].loser] - preferences[pairs[i+1].loser][pairs[i+1].winner]);
            dif=dif1-dif2;
            if (dif>p)
            {
                p=i;
            }
        }
        reserva=pairs[n];
        pairs[n]=pairs[p];
        pairs[p]=reserva;
        n++;
    }
    return;
}


// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    //bool locked[MAX][MAX];
    //voy pasando por todo el array de pairs
    for (int i=0; i<pair_count; i++)
    {
        //para cada uno de los perdedores miro si alguien esta bloqueado y ha ganado
        if(bloqueo(pairs[i].winner,pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser]=true;
        }
    }
    return;
}


//para cada uno de los perdedores miro si alguien esta bloqueado y ha ganado
bool bloqueo(int g, int p)
{
    for (int i=0; i<candidate_count; i++)//corresponde a columna de ganadores CREO
    {
        for (int j=0; j<candidate_count; j++)//corresponde columna de perdedores CREO
        {
            //busco entre toda la tabla si hay alguno bloqueado
            if (locked[i][j])
            {
                //si lo hay miro si alguna empieza con el perdedor recibido
                if (i==p)
                {
                    //si hay una con ganador
                    //miro si su perdedor coincide con mi ganador
                    //si coincide devuelvo false
                    if (j==g)
                    {
                        return false;
                    }
                    //si no coincide sigo buscando otro que este bloqueado y su perdedor sea el ganador anterior
                    if (!bloqueo(g,j))
                    {
                        return false;
                    }
                }
            }
        }
    }
    //si no hay le dejo acabar todo el ciclo buscando y devuelve true porque puede bloquear
    return true;

    //se puede optimizar juntando if mirando si es igual justo al principio
    // pero asi muestra el seguimiento del analisis mental del proceso
    //el primer if lo puedo borrar ya que solo me interesa uno que el ganador siempre sea la P
}



// Print the winner of the election
void print_winner(void)
{
    for ( int i=0; i<candidate_count; i++)
    {
        int count=0;
        for (int j=0; j<candidate_count; j++)
        {
            if(!locked[j][i])
            {
                count++;
            }
        }
        if (count==candidate_count)
        {
            printf("%s\n",candidates[i]);
        }
    }
    return;
}