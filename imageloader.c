/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include "imageloader.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Check line end has no extra character.
static inline int checkLineEnd(FILE *file)
{
    int ch;
    if ((ch = fgetc(file)) != '\n' && ch != EOF) return 0;
    return 1;
}

// Wrong file type and exit;
static inline void wrongFileType()
{
    fprintf(stderr, "Wrong file type");
    exit(EXIT_FAILURE);
}

// Opens a .ppm P3 image file, and constructs an Image object.
// You may find the function fscanf useful.
// Make sure that you close the file with fclose before returning.
Image *readData(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char type[3];
    if (fscanf(file, "%2s", type) != 1 || strcmp(type, "P3") != 0 ||
        !checkLineEnd(file))
        wrongFileType();

    uint32_t rows, cols;
    if (fscanf(file, "%u %u", &cols, &rows) != 2 || !checkLineEnd(file))
        wrongFileType();

    uint32_t maximum;
    if (fscanf(file, "%u", &maximum) != 1 || maximum != 255 ||
        !checkLineEnd(file))
        wrongFileType();

    Image *image = (Image *)malloc(sizeof(*image));
    if (image == NULL)
    {
        perror("Error malloc memory");
        exit(EXIT_FAILURE);
    }

    image->rows = rows;
    image->cols = cols;

    Color **colors = (Color **)malloc(rows * cols * sizeof(*colors));
    if (colors == NULL)
    {
        perror("Error malloc memory");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            //? need to check the overflow?
            uint32_t r, g, b;
            if (fscanf(file, "%u %u %u", &r, &g, &b) != 3 || r > 255 ||
                g > 255 || b > 255)
                wrongFileType();
            Color *color = (Color *)malloc(sizeof(*color));
            if (color == NULL)
            {
                perror("Error malloc memory");
                exit(EXIT_FAILURE);
            }
            color->R = r;
            color->G = g;
            color->B = b;
            colors[index++] = color;
        }
        if (!checkLineEnd(file)) wrongFileType();
    }
    image->image = colors;

    if (fclose(file) != 0)
    {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    return image;
}

// Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the
// image's data.
void writeData(Image *image)
{
    printf("P3\n");
    printf("%u %u\n", image->cols, image->rows);
    printf("255\n");
    int index = 0;
    Color **colors = image->image;
    for (int i = 0; i < image->rows; i++)
    {
        for (int j = 0; j < image->cols; j++)
        {
            Color *color = colors[index++];
            printf("%3u %3u %3u", color->R, color->G, color->B);
            if (j != image->cols - 1) printf("   ");
        }
        printf("\n");
    }
}

// Frees an image
void freeImage(Image *image)
{
    Color **colors = image->image;
    int pixels = image->cols * image->rows;
    for (int i = 0; i < pixels; i++) free(colors[i]);
    free(colors);
    free(image);
}
