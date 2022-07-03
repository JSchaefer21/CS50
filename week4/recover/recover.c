#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //si no hay dos comandos, envio mensaje de error
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE");
        return 1;
    }

    // Abro el archivo del segundo comando y lo guardo en un pointer llamado file
    // el primer parametro indica el archivo que quieres abrir, y el segundo (r) inidca que quieres leer (read)
    FILE *file = fopen(argv[1], "r");

    // si el nombre no existe recivo un NULL, y aviso de que no existe el archivo
    if (file == NULL)
    {
        printf("IMAGE not found\n");
        return 2;
    }


    //declaro la variable como unsigned char para que vaya de 0-255
    //unsigned char data[512];
    //o creo un typedef de 8bits
    BYTE data[512];

    //inicio contador para los numeros de las fotos
    int num = 0;

    //creo el archivo de salida en el que ire pasando la info
    FILE *outp = NULL;

    bool flag = false;


    // !feof(file) / feof devuelve 0 si aún no se ha llegado al final del fichero.
    // Si se ha llegado, devuelve un entero distinto de cero.
    /*Read 512 bytes from input_file and store on the buffer*/
    /*
    fread(data, size, number, inptr);
    data= pointer de donde quieres guardar la informacion
    size= tamaño de cada elemento que lees
    number= numero de elementos para leer
    inptr= FILE * to read from)

    cuando acaba de leer o da error devuelve 0 o menos, por lo tanto el while se cierra
    */
    while (fread(data, sizeof(char), 512, file))
    {
        // miro si los tres primeros empiezan igual que las especificaciones de JPEG
        // pone a 0 el ultimo digito y por lo tanto es igual a 0xe0
        //check if bytes is start of a JPEG
        if (data[0]==0xff && data[1]==0xd8 && data[2]==0xff && (data[3] & 0xf0) == 0xe0)
        {
            //creo un nombre para el archivo de salida
            //usando malloc deberia liberar al final de programa free(filename);
            //char *filename = malloc(8 * sizeof(char));
            char filename[8];
            sprintf(filename, "%03i.jpg", num);

            //abro el archivo de salida
            outp = fopen(filename, "w");

            //compruebo que no hayan errores de apertura en el archivo antes de guardar
            if (outp == NULL)
            {
                printf("error de apertura\n");
                flag=false;
            }
            else
            {
                flag=true;
            }

            //escribo data en el archivo de salida  NO FUNCONA, no llena bien las imagenes
            //aqui dentro solo guardo el primer paquete, ademas que no estaba comprobando que estuviese abierto sin errores
            //fwrite(data, sizeof(data), 512, outp);

            //subo 1 al contador de imagenes
            num++;
        }

        //si tengo el visto bueno, empiezo a guardar fuera del if anterior, ya que guardo mas de 1 paquete
        if (flag)
        {
            fwrite(data, sizeof(char), 512, outp);
        }
    }
    
    fclose(outp);
    fclose(file);
    return 0;
}



