#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int population;
    do
    {
        population = get_int("Start size: ");

    }
    while (population < 9);
    
    // TODO: Prompt for end size
    int end;
    do
    {
        end = get_int("End size: ");
    }
    while (end < population);

    //Calculat number of years until we reach threshold
    int n = 0;
    while (population < end)
    {
        population += (population / 3) - (population / 4);
        n++;
    }
    // TODO: Print number of years
    printf("Years: %i\n", n);
}