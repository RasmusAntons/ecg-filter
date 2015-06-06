//
// Created by rasmus on 04/06/15.
//

#include <stdio.h>
#include <stdlib.h>
#include "filters.h"

int filter_void(FILE *instream, FILE *outstream, struct file_attributes *attributes)
{
    struct line_data line;
    line.time = malloc(10 * sizeof(char));
    line.data = calloc(MAXCOLUMNS, sizeof(double));
    initialize(instream, outstream);
    while (get_next_line(&line, instream, attributes))
    {
        write_line(&line, attributes, outstream);
    }
    free(line.time);
    return 1;
}

int filter_mean(FILE *instream, FILE *outstream, struct file_attributes *attributes, size_t n)
{
    n--;
    double *olddata = calloc(MAXCOLUMNS * n, sizeof(double));
    struct line_data line;
    line.time = malloc(10 * sizeof(char));
    line.data = calloc(MAXCOLUMNS, sizeof(double));
    struct line_data write;
    write.time = line.time;
    write.data = calloc(MAXCOLUMNS, sizeof(double));
    int i, c, p = 0;
    initialize(instream, outstream);
    while (get_next_line(&line, instream, attributes))
    {
        for (c = 0; c < attributes->columns; c++)
        {
            write.data[c] = line.data[c];
            for (i = 0; i < n; i++)
                write.data[c] += olddata[i * attributes->columns + c];
            write.data[c] /= n + 1;
        }
        write_line(&write, attributes, outstream);
        for (c = 0; c < attributes->columns; c++)
            olddata[p + c] = line.data[c];
        p += attributes->columns;
        p %= n * attributes->columns;
    }
    free(line.time);
    free(line.data);
    free(write.data);
    free(olddata);
    return 1;
}

int filter_derivative(FILE *instream, FILE *outstream, struct file_attributes *attributes)
{
    struct line_data line;
    line.time = malloc(10 * sizeof(char));
    line.data = calloc(MAXCOLUMNS, sizeof(double));
    struct line_data write;
    write.time = line.time;
    write.data = calloc(MAXCOLUMNS, sizeof(double));
    int c;
    initialize(instream, outstream);
    while (get_next_line(&line, instream, attributes))
    {
        for (c = 0; c < attributes->columns; c++)
            write.data[c] = (line.data[c] - write.data[c]) * DERIVATIVE_FACTOR;
        write_line(&write, attributes, outstream);
        for (c = 0; c < attributes->columns; c++)
            write.data[c] = line.data[c];
    }
    free(line.time);
    free(line.data);
    free(write.data);
    return 1;
}

int filter_square(FILE *instream, FILE *outstream, struct file_attributes *attributes)
{
	struct line_data line;
    line.time = malloc(10 * sizeof(char));
    line.data = calloc(MAXCOLUMNS, sizeof(double));
    initialize(instream, outstream);
    while (get_next_line(&line, instream, attributes))
    {
		line.data[0] = (line.data[0] * line.data[0]) * SQUARE_FACTOR;
		line.data[1] = (line.data[1] * line.data[1]) * SQUARE_FACTOR;
        write_line(&line, attributes, outstream);
    }
    free(line.time);
    free(line.data);
    return 1;
}
