#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void createTable(int n)
{
    int table[n][n];// = new int[n][ n];

    //Random random = new Random();

    printf("Plansza do sudoku");
    for (int i = 0; i < n; i++)
    {
        printf("\n");
        if (i % (int)sqrt(n) == 0 && i != 0)
        {
            //printf(strcat("-", n * 5));
            printf("--------------------------------------------------------");
            printf("\n" );
        }
        for (int j = 0; j < n; j++)
        {
            if (j % (int)sqrt(n) == 0 && j != 0)
            {
                //tableNine[i, j] = random.Next(1,n+1);
                printf("|");
            }
            printf("  %d  ", table[i, j]);
        }
        printf("\n");
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            free(table[i][j]);
        }
        free(table[i]);
    }
    free(table);
}

int main(void) {
    createTable(4);
    return 0;
}