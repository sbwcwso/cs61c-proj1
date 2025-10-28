/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
    int index = row * image->cols + col;
    Color *color = image->image[index];
    Color *newColor = (Color *)malloc(sizeof(*newColor));
    if (newColor == NULL) 
    { 
        perror("Error malloc memory");
        exit(EXIT_FAILURE);
    }
    uint8_t num = (color->B % 2) ? 255: 0; 
    newColor->R = num;
    newColor->G = num;
    newColor->B = num;
    return newColor;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
    Image *newImage = (Image *)malloc(sizeof(*newImage));
    if (newImage == NULL)
    {
        perror("Error malloc memory");
        exit(EXIT_FAILURE);
    }

    int rows = image->rows;
    int cols = image->cols;
    newImage->rows = rows;
    newImage->cols = cols;

    Color **colors = (Color **)malloc(rows * cols * sizeof(*colors));
    if (colors == NULL)
    {
        perror("Error malloc memory");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            colors[index++] = evaluateOnePixel(image, i, j);
    newImage->image = colors;
    return newImage;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	if (argc != 2)
    {
        printf("usage: steganography [ppm file]");
        exit(EXIT_FAILURE);
    }
    Image *image = readData(argv[1]);
    Image *newImage = steganography(image);
    writeData(newImage);
    freeImage(image);
    freeImage(newImage);
    exit(EXIT_SUCCESS);
}
