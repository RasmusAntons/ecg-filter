//
// Created by rasmus on 04/06/15.
//

#include <stdio.h>
#include <stdlib.h>
#include "filters.h"

void init(struct line_data *line, struct line_data *write)
{
    line->time = malloc(10 * sizeof(int));
    line->data = calloc(MAXCOLUMNS, sizeof(double));
    if (write)
    {
        write->time = line->time;
        write->data = calloc(MAXCOLUMNS, sizeof(double));
    }
}

void cleanup(struct line_data *line, struct line_data *write)
{
    free(line->time);
    free(line->data);
    if (write)
        free(write->data);
}

int filter_void(FILE *instream, FILE *outstream, struct file_attributes *attributes)
{
    struct line_data line;
    init(&line, NULL);
    initialize(instream, outstream);
    while (get_next_line(&line, instream, attributes))
        write_line(&line, attributes, outstream);
    cleanup(&line, NULL);
    return 1;
}

int filter_mean(FILE *instream, FILE *outstream, struct file_attributes *attributes, size_t n)
{
    double *olddata = calloc(MAXCOLUMNS * (n - 1), sizeof(double));
    struct line_data line, write;
    init(&line, &write);
    int i, c, p = 0;
    initialize(instream, outstream);
    while (get_next_line(&line, instream, attributes))
    {
        for (c = 0; c < attributes->columns; c++)
        {
            write.data[c] = line.data[c];
            for (i = 0; i < (n - 1); i++)
                write.data[c] += olddata[i * attributes->columns + c];
            write.data[c] /= n;
        }
        write_line(&write, attributes, outstream);
        for (c = 0; c < attributes->columns; c++)
            olddata[p + c] = line.data[c];
        p += attributes->columns;
        p %= (n - 1) * attributes->columns;
    }
    free(olddata);
    cleanup(&line, &write);
    return 1;
}

int filter_derivative(FILE *instream, FILE *outstream, struct file_attributes *attributes)
{
    struct line_data line, write;
    init(&line, &write);
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
    cleanup(&line, &write);
    return 1;
}

int filter_square(FILE *instream, FILE *outstream, struct file_attributes *attributes)
{
    struct line_data line;
    init(&line, NULL);
    int c;
    initialize(instream, outstream);
    while (get_next_line(&line, instream, attributes))
    {
        for (c = 0; c < attributes->columns; c++)
            line.data[c] = (line.data[c] * line.data[c]) * SQUARE_FACTOR;
        write_line(&line, attributes, outstream);
    }
    cleanup(&line, NULL);
    return 1;
}

int filter_highpass(FILE *instream, FILE *outstream, struct file_attributes *attributes)
{
    struct line_data line, write;
    init(&line, &write);
    int c;
    initialize(instream, outstream);
    while (get_next_line(&line, instream, attributes))
    {
        continue;
    }
    cleanup(&line, &write);
    return 1;
}
