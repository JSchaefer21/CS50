#include <stdio.h>
#include <cs50.h>

int main(void)
{
    long number = get_long("Number: ");
    long n=number;

    // calculo el numero de digitos
    int digi=0;
    while (n>0)
    {
        n=n/10;
        digi++;
    }
    //printf("digitos: %i \n", digi);


    // calcula si es numero de tarjeta real
    n=number;
    int x, n1=0, n2=0;
    while (n>0)
    {
        n1=n1+n%10;
        n=n/10;
        if(n>0)
        {
            //se soman los digitos individuales, si al multiplicar por 2 tiene 2 digitos
            //los sumamos de forma separada
            x=(n%10)*2;
            if(x>9)
            {
                n2=n2+(x/10)+x%10;
            }
            else
            {
                n2=n2+(2*n%10);
            }
            n=n/10;
        }
    }
    n2=n2+n1;


    // operaciones finales
    if(n2%10 == 0)  //confirmando que el numero es real
    {
        long z=number;
        while (z>100)
        {
            z=z/10;
        }
        // los dos primeros numeros deben ser igual que 51,52,53,54,55 y tener 16 digitos
        if ((z==51 || z==52 || z==53 || z==54 || z==55) && digi==16)
        {
            printf("MASTERCARD\n");
        }
        else if ((z==34 || z==37) && digi==15)
        {
            printf("AMEX\n");
        }
        //solo el prier numero debe ser igual a 4 y tiene 13 o 16 digitos
        else if (z/10==4 && (digi==13 || digi==16))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
