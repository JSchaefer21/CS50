#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    for (int i=0; i<candidate_count; i++)
    {
        if (strcmp(candidates[i].name, name)==0)
        {
            preferences[voter][rank]=i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
// comprueba si el camdidato esta eliminado
// si no lo esta, suma la cantidad de votos k tiene cada uno
void tabulate(void)
{
    //busco entre todos los votantes de max 100
    for (int i=0; i<voter_count; i++)
    {
        //para todos los candidatos max 9
        for (int j=0; j<candidate_count; j++)
        {
            //si no esta eliminado
            if (!candidates[preferences[i][j]].eliminated)
            {
                //le sumo 1 voto al candidato de esa casilla y salgo de ESTE for y sigo con las i
                // si esta eliminado sigo con las j
                candidates[preferences[i][j]].votes++;;
                break;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // mas de la mitad del total de votos es mayoria
    float vot=voter_count/2;

    // con otro for miro si alguno tiene mas de la mitad de votos y lo imprimo
    for (int i=0; i<voter_count; i++)
    {
        if (candidates[i].votes>vot)
        {
//            printf("hay ganador\n");
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
//    printf("no gana nadie\n");
    return false;
}

// Return the minimum number of votes any remaining candidate has
// entre todos los canidatos no eliminados busco el de menos puntuaciom
// devuelvo la puntuacion mas baja
int find_min(void)
{
    int n=MAX_VOTERS;
    for (int i=0; i<candidate_count; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes<n)
        {
            n=candidates[i].votes;
        }
    }
//    printf("el minimo tiene: %i\n", n);
    return n;
}

// Return true if the election is tied between all candidates, false otherwise
// entre todos los candidatos no eliminados
//busco cuando hay no eliminados y cuandos k ademas tienen la minima puntuacion
// si son los mismos devuelvo true
bool is_tie(int min)
{
    int n=0,e=0;
    for (int i=0; i<candidate_count; i++)
    {
        if (!candidates[i].eliminated)
        {
            e++;
        }
        if (!candidates[i].eliminated && candidates[i].votes==min)
        {
            n++;
        }
    }
    if (n==e)
    {
        return true;
    }
    return false;

    // seria mas rapido mirar que no este eliminado y tenga la puntuacion distinta a la minima
    // con que uno ya sea diferente ya sabemos que no hay empate
    // porque lo eliminados no se cuentan
    // y si hay mayor puntuacion no hay empate
}

// Eliminate the candidate (or candidates) in last place
// a todos los candidatos k tienen la puntuanion minima los elimino
void eliminate(int min)
{
    for (int i=0; i<candidate_count; i++)
    {
        if (candidates[i].votes==min)
        {
            candidates[i].eliminated=true;
//            printf("elimino a: %s\n", candidates[i].name);
        }
    }
    return;
}