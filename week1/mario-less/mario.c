#include <stdio.h>
#include <cs50.h>

int main(void){
    int height=0;
    int n=1;

    while (height<1 || height>8){
        height = get_int("Height: ");
    }

    while (height>0){
        for (int i=height; i>1; i--){
            printf(" ");
        }
        for (int i=0; i<n; i++){
            printf("#");
        }
        printf("\n");
        n++;
        height--;
    }

}
