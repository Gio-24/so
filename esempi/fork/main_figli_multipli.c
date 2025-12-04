#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() 
{
    pid_t padre; // signed integer

    for(int i = 0; i < 5; i++)
    {
        padre = fork();

        printf("sono il ...");

        if(!padre)
        {
            printf("figlio\n");
            exit(0);
        }

        printf("\n");
    }

    return 0;
}