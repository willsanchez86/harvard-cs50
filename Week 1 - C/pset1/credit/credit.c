#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Prompt user for credit card number
    long number = get_long("Number: ");
    
    //Determine length of user input
    int length = 0;
    for (long num = number; num > 0; num /= 10)
    {
        length ++;
    }
    
    //Luhn's Algorithm 
    int doubled_sum = 0;
    int digit_sum = 0;
    
    for (long num = number; num > 0; num /= 100)
    {
        int digit = num % 10;
        digit_sum += digit;
        int n = (num % 100) / 10 * 2;
        
        if (n < 10)
        {
            doubled_sum += n;
        }
        else
        {
            doubled_sum += (n % 10);
            doubled_sum += (n / 10);
        }
    }
    
    int total_sum = (doubled_sum + digit_sum);
    
    // // CHECK!
    // printf("%i\n", doubled_sum);
    // printf("%i\n", digit_sum);
    // printf("%i\n", total_sum);
    // // CHECK!

    //Check if last digit of total sum is 0
    if (total_sum % 10 == 0)
    {
        //Check if card matches credit card company details such as length or starting digits, then print company name if valid.
        
        
        if (length == 16)
        {
            if ((number / 1000000000000000) == 4)
            {
                printf("VISA\n");
            }
            else if ((number / 100000000000000) >= 51 && (number / 100000000000000) <= 55)
            {
                printf("MASTERCARD\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        
        else if (length == 15)
        {
            if ((number / 10000000000000) == 34 || (number / 10000000000000) == 37)
            {
                printf("AMEX\n");
            }
            else
            {
                printf("INVALID\n");
            }
            
        }
        
        else if (length == 13)
        {
            if ((number / 1000000000000) == 4)
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
    
    else
    {
        printf("INVALID\n"); // Checking if Algorithm was issue for failure
        
    }
}