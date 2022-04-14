#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(void)
{
    //Ask user for text input
    string text = get_string("Text: ");
    

    int num_letters = 0;
    int num_words = 0;
    int num_sentences = 0;
    
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        //Calculate number of words by counting the number of single spaces between characters
        if (isspace(text[i]))
        {
            num_words++;
        }
        
        //Calculate number of sentences by counting punctation marks
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            num_sentences++;
        }
        
        //Convert character to lowercase, and increase number of letters by 1 if character is a letter
        int letter = tolower(text[i]);
        if (letter >= 97 && letter <= 122)
        {
            num_letters++;
        }
    }
    
    //Add the final word to your word count because the sentence will end with punctation rather than a space
    if (num_words > 0)
    {
        num_words++;
    }
    
    //Calculate the average number of letters and sentences per 100 words to use in  Coleman-Liau index
    double l = (num_letters * (100.00 / num_words));
    double s = (num_sentences * (100.00 / num_words));
    double index = 0.0588 * (l) - 0.296 * (s) - 15.8;
    
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(index));
    }
}
