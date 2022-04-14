#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int n = 0; n < width; n++)
        {
            RGBTRIPLE pixel = image[i][n];
            int average = round((pixel.rgbtBlue + pixel.rgbtGreen + pixel.rgbtRed) / 3.0);
            image[i][n].rgbtBlue = image[i][n].rgbtGreen =  image[i][n].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int n = 0, num_pixels = width - 1; n < width / 2; n++)
        {
            RGBTRIPLE temp = image[i][n];
            image[i][n] = image[i][num_pixels - n];
            image[i][num_pixels - n] = temp;
        }

    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int n = 0; n < width; n++)
        {
            // Initialize the sum variables for each color along with the counter
            int sum_green = 0;
            int sum_blue = 0;
            int sum_red = 0;
            float counter = 0.0;

            // Current Row
            sum_green += image[i][n].rgbtGreen;
            sum_blue += image[i][n].rgbtBlue;
            sum_red += image[i][n].rgbtRed;
            counter++;


            // Top Row Check
            if (i - 1 >= 0)
            {
                // Top-Middle box
                sum_green += image[i - 1][n].rgbtGreen;
                sum_blue += image[i - 1][n].rgbtBlue;
                sum_red += image[i - 1][n].rgbtRed;
                counter++;

                // Top-Left box
                if (n - 1 >= 0)
                {
                    sum_green += image[i - 1][n - 1].rgbtGreen;
                    sum_blue += image[i - 1][n - 1].rgbtBlue;
                    sum_red += image[i - 1][n - 1].rgbtRed;
                    counter++;
                }

                // Top-Right box
                if (n + 1 < width)
                {
                    sum_green += image[i - 1][n + 1].rgbtGreen;
                    sum_blue += image[i - 1][n + 1].rgbtBlue;
                    sum_red += image[i - 1][n + 1].rgbtRed;
                    counter++;
                }
            }

            //MIddle-Left box
            if (n - 1 >= 0)
            {
                sum_green += image[i][n - 1].rgbtGreen;
                sum_blue += image[i][n - 1].rgbtBlue;
                sum_red += image[i][n - 1].rgbtRed;
                counter++;
            }

            // Middle=Right box
            if (n + 1 < width)
            {
                sum_green += image[i][n + 1].rgbtGreen;
                sum_blue += image[i][n + 1].rgbtBlue;
                sum_red += image[i][n + 1].rgbtRed;
                counter++;
            }

            // Bottom Row
            if (i + 1 < height)
            {
                // Bottom-Middle box
                sum_green += image[i + 1][n].rgbtGreen;
                sum_blue += image[i + 1][n].rgbtBlue;
                sum_red += image[i + 1][n].rgbtRed;
                counter++;

                // Bottom-Left box
                if (n - 1 >= 0)
                {
                    sum_green += image[i + 1][n - 1].rgbtGreen;
                    sum_blue += image[i + 1][n - 1].rgbtBlue;
                    sum_red += image[i + 1][n - 1].rgbtRed;
                    counter++;
                }

                // Bottom-Right box
                if (n + 1 < width)
                {
                    sum_green += image[i + 1][n + 1].rgbtGreen;
                    sum_blue += image[i + 1][n + 1].rgbtBlue;
                    sum_red += image[i + 1][n + 1].rgbtRed;
                    counter++;
                }
            }

            temp[i][n].rgbtGreen = round(sum_green / counter);
            temp[i][n].rgbtBlue = round(sum_blue / counter);
            temp[i][n].rgbtRed = round(sum_red / counter);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int n = 0; n < width; n++)
        {
            image[i][n].rgbtGreen = temp[i][n].rgbtGreen;
            image[i][n].rgbtBlue = temp[i][n].rgbtBlue;
            image[i][n].rgbtRed = temp[i][n].rgbtRed;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    
    for (int i = 0; i < height; i++)
    {
        for (int n = 0; n < width; n++)
        {

            // Initialize the sum color variables as 0
            float sum_green_gx = 0, sum_green_gy = 0;
            float sum_blue_gx = 0, sum_blue_gy = 0;
            float sum_red_gx = 0, sum_red_gy = 0;

            // For each qualifying condition, add the RGB values to their respective sums of gx and gy

            // Top Row Check
            if (i - 1 >= 0)
            {
                // Top-Middle box
                sum_green_gy += (image[i - 1][n].rgbtGreen * -2);
                sum_blue_gy += (image[i - 1][n].rgbtBlue * -2);
                sum_red_gy += (image[i - 1][n].rgbtRed * -2);


                // Top-Left box
                if (n - 1 >= 0)
                {
                    sum_green_gx += (image[i - 1][n - 1].rgbtGreen * -1);
                    sum_blue_gx += (image[i - 1][n - 1].rgbtBlue * -1);
                    sum_red_gx += (image[i - 1][n - 1].rgbtRed * -1);

                    sum_green_gy += (image[i - 1][n - 1].rgbtGreen * -1);
                    sum_blue_gy += (image[i - 1][n - 1].rgbtBlue * -1);
                    sum_red_gy += (image[i - 1][n - 1].rgbtRed * -1);

                }

                // Top-Right box
                if (n + 1 < width)
                {
                    sum_green_gx += image[i - 1][n + 1].rgbtGreen;
                    sum_blue_gx += image[i - 1][n + 1].rgbtBlue;
                    sum_red_gx += image[i - 1][n + 1].rgbtRed;

                    sum_green_gy += (image[i - 1][n + 1].rgbtGreen * -1);
                    sum_blue_gy += (image[i - 1][n + 1].rgbtBlue * -1);
                    sum_red_gy += (image[i - 1][n + 1].rgbtRed * -1);

                }
            }

            //MIddle-Left box
            if (n - 1 >= 0)
            {
                sum_green_gx += (image[i][n - 1].rgbtGreen * -2);
                sum_blue_gx += (image[i][n - 1].rgbtBlue * -2);
                sum_red_gx += (image[i][n - 1].rgbtRed * -2);

            }

            // Middle=Right box
            if (n + 1 < width)
            {
                sum_green_gx += (image[i][n + 1].rgbtGreen * 2);
                sum_blue_gx += (image[i][n + 1].rgbtBlue * 2);
                sum_red_gx += (image[i][n + 1].rgbtRed * 2);

            }

            // Bottom Row
            if (i + 1 < height)
            {
                // Bottom-Middle box
                sum_green_gy += (image[i + 1][n].rgbtGreen * 2);
                sum_blue_gy += (image[i + 1][n].rgbtBlue * 2);
                sum_red_gy += (image[i + 1][n].rgbtRed * 2);


                // Bottom-Left box
                if (n - 1 >= 0)
                {
                    sum_green_gx += (image[i + 1][n - 1].rgbtGreen * -1);
                    sum_blue_gx += (image[i + 1][n - 1].rgbtBlue * -1);
                    sum_red_gx += (image[i + 1][n - 1].rgbtRed * -1);

                    sum_green_gy += image[i + 1][n - 1].rgbtGreen;
                    sum_blue_gy += image[i + 1][n - 1].rgbtBlue;
                    sum_red_gy += image[i + 1][n - 1].rgbtRed;

                }

                // Bottom-Right box
                if (n + 1 < width)
                {
                    sum_green_gx += image[i + 1][n + 1].rgbtGreen;
                    sum_blue_gx += image[i + 1][n + 1].rgbtBlue;
                    sum_red_gx += image[i + 1][n + 1].rgbtRed;

                    sum_green_gy += image[i + 1][n + 1].rgbtGreen;
                    sum_blue_gy += image[i + 1][n + 1].rgbtBlue;
                    sum_red_gy += image[i + 1][n + 1].rgbtRed;
                }
            }
            
            // Calculate new values
            int green = round(sqrt((sum_green_gx * sum_green_gx) + (sum_green_gy * sum_green_gy)));
            int blue = round(sqrt((sum_blue_gx * sum_blue_gx) + (sum_blue_gy * sum_blue_gy)));
            int red = round(sqrt((sum_red_gx * sum_red_gx) + (sum_red_gy * sum_red_gy)));

            // Apply new color values to the pixel. If value is greater than 255, just enter 255
            temp[i][n].rgbtGreen = green > 255 ? 255 : green;
            temp[i][n].rgbtBlue = blue > 255 ? 255 : blue;
            temp[i][n].rgbtRed = red > 255 ? 255 : red;
        }
    }
    // Change each pixel of the original array to match the new temp array
    for (int i = 0; i < height; i++)
    {
        for (int n = 0; n < width; n++)
        {
            image[i][n].rgbtGreen = temp[i][n].rgbtGreen;
            image[i][n].rgbtBlue = temp[i][n].rgbtBlue;
            image[i][n].rgbtRed = temp[i][n].rgbtRed;
        }
    }
    return;
}
