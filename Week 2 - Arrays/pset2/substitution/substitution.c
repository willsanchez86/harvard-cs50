#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

char ALPHABET[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};


int main(int argc, string argv[])
{   
    //Check if user entered the proper number of arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    
    //Check if length of argument is 26
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        //Confirm that argument only contains alphabetic characters
        int lower = tolower(argv[1][i]);
        if (lower < 97 || lower > 122)
        {
            printf("Key must contain only alphabetic characters.\n");
            return 1;
        }
        //Check for Duplicates
        for (int j = i + 1; j < n; j++)
        {
            if (argv[1][j] == argv[1][i])
            {
                printf("Key cannot contain duplicate characters\n");
                return 1;
            }
        }
    }
    
    //Encode User input and print out encrypted text
    string text = get_string("plaintext:");
    int l = strlen(text);
    char str[l - 1];
    printf("ciphertext:");
    for (int i = 0; i < l; i++)
    {
        //If character is not an english letter, print as is
        int lowercase = tolower(text[i]);
        if (lowercase < 97 || lowercase > 122)
        {
            str[i] = text[i];
        }
        else
        {
            for (int j = 0, n = strlen(ALPHABET); j < n; j++)
            {
                //Find the corresponding letter of the text to the encryption key, convert to proper case if necessary, then print character
                int lower = tolower(argv[1][j]);
                int upper = toupper(argv[1][j]);
                if (lowercase == ALPHABET[j])
                {
                    str[i] = islower(text[i]) ? lower : upper;
                }
            }
        }
        printf("%c", str[i]);
    }
    
    printf("\n");
}

