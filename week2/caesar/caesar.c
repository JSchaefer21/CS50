#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//creo una funcion que recoge solo el nombre y una variable
int main(int argc, string argv[])
{
    //compruebo que realmente solo hay dos argumentos
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    //compruebo que el segundo argumento son numeros
    else
    {
        for (int i=0, n=strlen(argv[1]); i<n; i++)
        {
            if (argv[1][i]<'0' || argv[1][i]>'9')
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }
    }

    //pido el texto para cifrar
    string ptext = get_string("plaintext:  ");
    //convierto el texto anterior a un int
    int k = atoi(argv[1]);
    //reviso que no pase de 1 vuelta y la elimino si pasa
    while (k>26)
    {
        k=k-26;
    }

    //combierto el texto este en mayusculas o minusculas
    //pero solo las letras, lo demas se pasa igual
    string ctext=ptext;
    for (int i=0, n=strlen(ptext); i<n; i++)
    {
        // mayusculas y minusculas por separado, para que no me mezcle al sobrepasar
        if (ptext[i]>='a' && ptext[i]<='z')
        {
            //si se pasa de z lo modifico, sino simplemente sumo
            if (ptext[i]+k>'z')
            {
                ctext[i]=ptext[i]+k-26;
            }
            else
            {
                ctext[i]=ptext[i]+k;
            }
        }
        //lo mismo para las mayusculas
        else if (ptext[i]>='A' && ptext[i]<='Z')
        {
            if (ptext[i]+k>'Z')
            {
                ctext[i]=ptext[i]+k-26;
            }
            else
            {
                ctext[i]=ptext[i]+k;
            }
        }
    }

    //muestro el texto convertido
    printf("ciphertext: %s\n", ctext);
    return 0;
}
