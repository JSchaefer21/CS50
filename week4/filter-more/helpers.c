#include "helpers.h"
#include <math.h>
#include <cs50.h> 

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i=0; i<height; i++) //filas de arriba a abajo
    {
        for (int j=0; j<width; j++) //columnas de izquierda a derecha
        {
            //hago la media de los 3 colores, al tener todos el mismo valor sera una escala de grises
            int n=round((image[i][j].rgbtRed+image[i][j].rgbtGreen+image[i][j].rgbtBlue)/3.0);
            image[i][j].rgbtRed=n;
            image[i][j].rgbtGreen=n;
            image[i][j].rgbtBlue=n;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i=0; i<height; i++) //filas de arriba a abajo
    {
        for (int j=0, n=width/2; j<n; j++) //columnas de izquierda a derecha
        {
            //le resto 1 pork sino (en un largo de 40) empieza en 40, y deberia emprezar en 39 (rel primero es 0)
            RGBTRIPLE t0=image[i][width-j-1];
            image[i][width-j-1]=image[i][j];
            image[i][j]=t0;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    float n1=0, n2=0, n3=0, d=1.0;
    int mh=height-1, mw=width-1;

    //hago una copia de la imagen, para trabajar siempre con los pixeles originales despues de modificar los de al lado
    RGBTRIPLE copy[height][width];
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            copy[i][j] = image[i][j];
        }
    }


    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
           n1=copy[i][j].rgbtRed;
           n2=copy[i][j].rgbtGreen;
           n3=copy[i][j].rgbtBlue;

           // calcula primero si estan en el centro, que es lo mas probable
           if(i>0 && j>0 && i<mh && j<mw)
           {
              n1=n1+copy[i-1][j-1].rgbtRed+copy[i-1][j].rgbtRed+copy[i-1][j+1].rgbtRed+copy[i][j-1].rgbtRed+copy[i][j+1].rgbtRed+copy[i+1][j-1].rgbtRed+copy[i+1][j].rgbtRed+copy[i+1][j+1].rgbtRed;
              n2=n2+copy[i-1][j-1].rgbtGreen+copy[i-1][j].rgbtGreen+copy[i-1][j+1].rgbtGreen+copy[i][j-1].rgbtGreen+copy[i][j+1].rgbtGreen+copy[i+1][j-1].rgbtGreen+copy[i+1][j].rgbtGreen+copy[i+1][j+1].rgbtGreen;
              n3=n3+copy[i-1][j-1].rgbtBlue+copy[i-1][j].rgbtBlue+copy[i-1][j+1].rgbtBlue+copy[i][j-1].rgbtBlue+copy[i][j+1].rgbtBlue+copy[i+1][j-1].rgbtBlue+copy[i+1][j].rgbtBlue+copy[i+1][j+1].rgbtBlue;
              d=d+8;
           } else


           if(i>0 && i<mh && j==0){ // si esta en bordes
              n1=n1+copy[i-1][j].rgbtRed+copy[i-1][j+1].rgbtRed+copy[i][j+1].rgbtRed+copy[i+1][j].rgbtRed+copy[i+1][j+1].rgbtRed;
              n2=n2+copy[i-1][j].rgbtGreen+copy[i-1][j+1].rgbtGreen+copy[i][j+1].rgbtGreen+copy[i+1][j].rgbtGreen+copy[i+1][j+1].rgbtGreen;
              n3=n3+copy[i-1][j].rgbtBlue+copy[i-1][j+1].rgbtBlue+copy[i][j+1].rgbtBlue+copy[i+1][j].rgbtBlue+copy[i+1][j+1].rgbtBlue;
              d=d+5;
           } else
           if(i>0 && i<mh && j==mw){
              n1=n1+copy[i-1][j-1].rgbtRed+copy[i-1][j].rgbtRed+copy[i][j-1].rgbtRed+copy[i+1][j-1].rgbtRed+copy[i+1][j].rgbtRed;
              n2=n2+copy[i-1][j-1].rgbtGreen+copy[i-1][j].rgbtGreen+copy[i][j-1].rgbtGreen+copy[i+1][j-1].rgbtGreen+copy[i+1][j].rgbtGreen;
              n3=n3+copy[i-1][j-1].rgbtBlue+copy[i-1][j].rgbtBlue+copy[i][j-1].rgbtBlue+copy[i+1][j-1].rgbtBlue+copy[i+1][j].rgbtBlue;
              d=d+5;
           }else
           if(i==0 && j>0 && j<mw){
              n1=n1+copy[i][j-1].rgbtRed+copy[i][j+1].rgbtRed+copy[i+1][j-1].rgbtRed+copy[i+1][j].rgbtRed+copy[i+1][j+1].rgbtRed;
              n2=n2+copy[i][j-1].rgbtGreen+copy[i][j+1].rgbtGreen+copy[i+1][j-1].rgbtGreen+copy[i+1][j].rgbtGreen+copy[i+1][j+1].rgbtGreen;
              n3=n3+copy[i][j-1].rgbtBlue+copy[i][j+1].rgbtBlue+copy[i+1][j-1].rgbtBlue+copy[i+1][j].rgbtBlue+copy[i+1][j+1].rgbtBlue;
              d=d+5;
           }else
           if(i==mh && j>0 && j<mw){
              n1=n1+copy[i-1][j-1].rgbtRed+copy[i-1][j].rgbtRed+copy[i-1][j+1].rgbtRed+copy[i][j-1].rgbtRed+copy[i][j+1].rgbtRed;
              n2=n2+copy[i-1][j-1].rgbtGreen+copy[i-1][j].rgbtGreen+copy[i-1][j+1].rgbtGreen+copy[i][j-1].rgbtGreen+copy[i][j+1].rgbtGreen;
              n3=n3+copy[i-1][j-1].rgbtBlue+copy[i-1][j].rgbtBlue+copy[i-1][j+1].rgbtBlue+copy[i][j-1].rgbtBlue+copy[i][j+1].rgbtBlue;
              d=d+5;
           }else


           if(i==0 && j==0){ // si esta en esquinas
              n1=n1+copy[i+1][j+1].rgbtRed+copy[i+1][j].rgbtRed+copy[i][j+1].rgbtRed;
              n2=n2+copy[i+1][j+1].rgbtGreen+copy[i+1][j].rgbtGreen+copy[i][j+1].rgbtGreen;
              n3=n3+copy[i+1][j+1].rgbtBlue+copy[i+1][j].rgbtBlue+copy[i][j+1].rgbtBlue;
              d=d+3;
           }else
           if(i==mw && j==mh){
              n1=n1+copy[i-1][j-1].rgbtRed+copy[i-1][j].rgbtRed+copy[i][j-1].rgbtRed;
              n2=n2+copy[i-1][j-1].rgbtGreen+copy[i-1][j].rgbtGreen+copy[i][j-1].rgbtGreen;
              n3=n3+copy[i-1][j-1].rgbtBlue+copy[i-1][j].rgbtBlue+copy[i][j-1].rgbtBlue;
              d=d+3;
           } else
           if(i==0 && j==mh){
              n1=n1+copy[i][j-1].rgbtRed+copy[i+1][j-1].rgbtRed+copy[i+1][j].rgbtRed;
              n2=n2+copy[i][j-1].rgbtGreen+copy[i+1][j-1].rgbtGreen+copy[i+1][j].rgbtGreen;
              n3=n3+copy[i][j-1].rgbtBlue+copy[i+1][j-1].rgbtBlue+copy[i+1][j].rgbtBlue;
              d=d+3;
           }else
           if(i==mw && j==0){
              n1=n1+copy[i][j+1].rgbtRed+copy[i-1][j+1].rgbtRed+copy[i-1][j].rgbtRed;
              n2=n2+copy[i][j+1].rgbtGreen+copy[i-1][j+1].rgbtGreen+copy[i-1][j].rgbtGreen;
              n3=n3+copy[i][j+1].rgbtBlue+copy[i-1][j+1].rgbtBlue+copy[i-1][j].rgbtBlue;
              d=d+3;
           }


           image[i][j].rgbtRed=round(n1/d);
           image[i][j].rgbtGreen=round(n2/d);
           image[i][j].rgbtBlue=round(n3/d);
           d=1.0;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int mtr[3][3]={{-1, 0, 1},{-2, 0, 2},{-1, 0, 1}};
    int mtry[3][3]={{-1, -2, -1},{0, 0, 0},{1, 2, 1}};
    int G[]={0,0,0,0,0,0};
    bool bo=false;

    float n1=0.0, n2=0.0, n3=0.0;
    int mh=height-1, mw=width-1;

    RGBTRIPLE copy[height][width];
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            copy[i][j] = image[i][j];
        }
    }


    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){

           // modifico la tabla segun la posicion, es decir, si esta en las esquinas o bordes pasa a cero
           if(i==0){
               mtr[0][0]=0;
               mtr[0][2]=0;
               mtry[0][0]=0;
               mtry[0][1]=0;
               mtry[0][2]=0;
               bo=true;
           }
           if(j==0){
               mtr[0][0]=0;
               mtr[1][0]=0;
               mtr[2][0]=0;
               mtry[0][0]=0;
               mtry[2][0]=0;
               bo=true;
           }
           if(i==mh){
               mtr[2][0]=0;
               mtr[2][2]=0;
               mtry[2][0]=0;
               mtry[2][1]=0;
               mtry[2][2]=0;
               bo=true;
           }
           if(j==mw){
               mtr[0][2]=0;
               mtr[1][2]=0;
               mtr[2][2]=0;
               mtry[0][2]=0;
               mtry[2][2]=0;
               bo=true;
           }

           for(int a=0; a<3; a++){
               for(int b=0; b<3; b++){
                   if(mtr[a][b]!=0){ // si el valor de la matriz es 0 no hace falque que multiplique
                       G[0]=G[0]+copy[i+(a-1)][j+(b-1)].rgbtRed*mtr[a][b];
                       G[1]=G[1]+copy[i+(a-1)][j+(b-1)].rgbtGreen*mtr[a][b];
                       G[2]=G[2]+copy[i+(a-1)][j+(b-1)].rgbtBlue*mtr[a][b];
                   }
                   if(mtry[a][b]!=0){
                       G[3]=G[3]+copy[i+(a-1)][j+(b-1)].rgbtRed*mtry[a][b];
                       G[4]=G[4]+copy[i+(a-1)][j+(b-1)].rgbtGreen*mtry[a][b];
                       G[5]=G[5]+copy[i+(a-1)][j+(b-1)].rgbtBlue*mtry[a][b];
                   }
               }
           }
           n1=sqrt(pow(G[0],2)+(pow(G[3],2)));
           n2=sqrt(pow(G[1],2)+(pow(G[4],2)));
           n3=sqrt(pow(G[2],2)+(pow(G[5],2)));
           if(n1>255){
               image[i][j].rgbtRed=255;
           }else{image[i][j].rgbtRed=round(n1);}
           if(n2>255){
               image[i][j].rgbtGreen=255;
           }else{image[i][j].rgbtGreen=round(n2);}
           if(n3>255){
                   image[i][j].rgbtBlue=255;
           }else{image[i][j].rgbtBlue=round(n3);}



           for(int s=0; s<6; s++){
               G[s]=0;
           }
           if(bo){ // si el valor de matriz se ha cambiado, lo vuelvo a poner
               mtr[0][0]=-1;
               mtr[1][0]=-2;
               mtr[2][0]=-1;
               mtr[0][2]=1;
               mtr[1][2]=2;
               mtr[2][2]=1;
               mtry[0][0]=-1;
               mtry[0][1]=-2;
               mtry[0][2]=-1;
               mtry[2][0]=1;
               mtry[2][1]=2;
               mtry[2][2]=1;
               bo=false;
           }


        }
    }
    return;
}