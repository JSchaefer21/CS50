#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main (void)
{
    //recojo todo el texto en un string y lo muestro tal cual
    string text = get_string("Text: ");
    //printf("%i letters\n", count_letters(text));
    //printf("%i words\n", count_words(text));
    //printf("%i sentences\n", count_sentences(text));
    float words=count_words(text);
    //conviento un numero en float antes de la division pork sino
    // al ser todo valores int se pierde informacion
    float L=count_letters(text)/words*100.0;
    float S=count_sentences(text)/words*100.0;
    float index= round(0.0588*L-0.296*S-15.8);

//el enunciado te pide separar los grados menores de 1 y mayores de 16
    if (index<1)
    {
        printf("Before Grade 1\n");
    }
    else if (index>16)
    {
        printf("Grade 16+\n");
    }
    else
    {
    printf("Grade %.0f\n", index);
    }
}


//funcion especifica para contar el numero de letras
int count_letters(string text)
{
    int count=0;
    for ( int i=0, n=strlen(text); i<n; i++)
    {
        if ((text[i]>='a' && text[i]<='z')||(text[i]>='A' && text[i]<='Z'))
        {
            count++;
        }
    }
    return count;
}

//cada espacio equivale a una palabra
int count_words(string text)
{
    int count=1;
    for ( int i=0, n=strlen(text); i<n; i++)
    {
        if(text[i]==' ')
        {
            count++;
        }
    }
    return count;
}

//cada punto o signo equivale a una frase
int count_sentences(string text)
{
    int count=0;
    for ( int i=0, n=strlen(text); i<n; i++)
    {
        if(text[i]=='.' || text[i]=='!' || text[i]=='?')
        {
            count++;
        }
    }
    return count;
}