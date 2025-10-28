/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"


static inline void getNeighbors(Image *image, int row, int col, Color** neighbors)
{
    int rows, cols;
    rows = image->rows;
    cols = image->cols;
    int rowMinusOne = (row - 1 + rows) % rows;
    int rowAddOne = (row + 1) % rows;
    int colMinusOne = (col - 1 + cols) % cols;
    int colAddOne = (col + 1) % cols;
    Color **colors = image->image;

    int firstRowOffset = rowMinusOne * cols;
    int secondRowOffset = row * cols;
    int thirdRowOffset = rowAddOne * cols;

    neighbors[0] = colors[firstRowOffset + colMinusOne];
    neighbors[1] = colors[firstRowOffset + col];
    neighbors[2] = colors[firstRowOffset + colAddOne];

    neighbors[3] = colors[secondRowOffset + colMinusOne];
    neighbors[4] = colors[secondRowOffset + colAddOne];

    neighbors[5] = colors[thirdRowOffset + colMinusOne];
    neighbors[6] = colors[thirdRowOffset + col];
    neighbors[7] = colors[thirdRowOffset + colAddOne];
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
    int index = row * image->cols + col;
    Color *color = image->image[index];
    Color *newColor = (Color *)malloc(sizeof(*newColor));
    if (newColor == NULL) 
    { 
        perror("Error malloc memory");
        exit(EXIT_FAILURE);
    }

    Color *neighbors[8];
    getNeighbors(image, row, col, neighbors);
    int aliveRNeighbors, aliveGNeighbors, aliveBNeighbors;
    aliveRNeighbors = 8;
    aliveGNeighbors = 8;
    aliveBNeighbors = 8;
    for(int i = 0; i < 8; i++)
    {
        aliveRNeighbors -= !neighbors[i]->R;
        aliveGNeighbors -= !neighbors[i]->G;
        aliveBNeighbors -= !neighbors[i]->B;
    }

    int offsetR, offsetG, offsetB;
    offsetR = color->R ? aliveRNeighbors + 9 : aliveRNeighbors;
    offsetG = color->G ? aliveGNeighbors + 9 : aliveGNeighbors;
    offsetB = color->B ? aliveBNeighbors + 9 : aliveBNeighbors;

    newColor->R = rule & (1 << offsetR) ? 255 : 0;
    newColor->G = rule & (1 << offsetG) ? 255 : 0;
    newColor->B = rule & (1 << offsetB) ? 255 : 0;
    return newColor;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
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
            colors[index++] = evaluateOneCell(image, i, j, rule);
    newImage->image = colors;
    return newImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc != 3)
    {
        printf("    usage: ./gameOfLife filename rule\n");
        printf("    filename is an ASCII PPM file (type P3) with maximum value 255.\n");
        printf("    rule is a hex number beginning with 0x; Life is 0x1808.");
        exit(EXIT_FAILURE);
    }
    Image *image = readData(argv[1]);
    uint32_t rule =  (uint32_t) strtol(argv[2], NULL, 16);
    Image *newImage = life(image, rule);
    writeData(newImage);
    freeImage(image);
    freeImage(newImage);
    exit(EXIT_SUCCESS);
}
