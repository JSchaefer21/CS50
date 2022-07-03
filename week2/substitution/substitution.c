#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//creo una funcion que recoge solo el nombre y el codigo
int main(int argc, string argv[])
{
    //compruebo que realmente solo hay dos argumentos
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    //compruebo que el segundo argumento son 26 cartacteres
    else if (strlen(argv[1])!=26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else
    //compruebo que son todos letras, tanto mayusculas como minusculas
    {
        for (int i=0, n=strlen(argv[1]); i<n; i++)
        {
            //miro que no este fuera del rango de letras
            if (argv[1][i]<'A' || argv[1][i]>'z')
            {
                printf("Key must not contain special characters.\n");
                return 1;
            }
            //y que no este en el espacio intermecio de simbolos segun tabla ascii
            else if (argv[1][i]>'Z' && argv[1][i]<'a')
            {
                printf("Key must not contain special characters.\n");
                return 1;
            }
        }
    }

    char codigo[26];
    //combierto el codigo a mayusculas
    for (int i=0; i<26; i++)
    {
        if (argv[1][i]>'Z')
        {
            codigo[i]=argv[1][i]-32;
        }
        else
        {
            codigo[i]=argv[1][i];
        }
    }

    // y reviso que no tenga letras duplicadas
    for (int i=0; i<26; i++)
    {
        int c=0;
        for (int j=0; j<26; j++)
        {
            if (codigo[i]==codigo[j])
            {
                c++;
                if (c>1)
                {
                printf("Key cointains repeated letters.\n");
                return 1;
                }
            }
        }
    }

    //pido el texto para cifrar
    string text = get_string("plaintext:  ");

    //recorro todo el texto y la cambio por su equivalente
    //solo si es letra
    // y si esta en mminuscula le sumo 32 para pasar mayus
    for (int i=0, n=strlen(text); i<n; i++)
    {
        if (text[i]>='a' && text[i]<='z')
        {
            text[i]=codigo[text[i]-97]+32;
            //si esta entre las minusculas cojo la letra en la posicion sacando el numero de la tabla ascii
        }
        else if (text[i]>='A' && text[i]<='Z')
        {
            text[i]=codigo[text[i]-65];
            //si  esta entre las mayusculas igual
        }
    }

    //muestro el texto convertido
    printf("ciphertext: %s\n", text);
    return 0;
}
