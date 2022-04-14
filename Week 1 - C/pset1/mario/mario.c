#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Ask the user for an integer between 1-8 to use as pyramid height
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    //Loop through rows
    for (int i = 0; i < n; i++)
    {
        //Nested loop for initial spaces
        for (int space = n - i; space > 1; space--)
        {
            printf(" ");
        }
        
        //Nested loop for left #'s'
        for (int j = -1; j < i; j++)
        {
            printf("#");
        }
        
        //Constant spacing in between #'s'
        printf("  ");
        
        //Nested loop for right #'s'
        for (int j = -1; j < i; j++)
        {
            printf("#");
        }
        
        //Print a new line to move onto next row
        printf("\n");
    }
}