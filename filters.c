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
    initialize(instream, outstream);
    while (get_next_line(&line, instream, attributes))
    {
        write_line(&line, outstream);
    }
    free(line.time);
    return 1;
}

int filter_mean(FILE *instream, FILE *outstream, struct file_attributes *attributes, size_t n)
{
    n--;
    double *olddata = calloc(2 * n, sizeof(double));
    struct line_data line;
    line.time = malloc(10 * sizeof(char));
    struct line_data write;
    int i = 0, p = 0;
    initialize(instream, outstream);
    while (get_next_line(&line, instream, attributes))
    {
        write.time = line.time;
        write.data_0 = line.data_0;
        write.data_1 = line.data_1;
        for (i = 0; i < n; i++)
        {
            write.data_0  += olddata[i * 2];
            write.data_1 += olddata[i * 2 + 1];
        }
        write.data_0 /= n + 1;
        write.data_1 /= n + 1;
        write_line(&write, outstream);
        olddata[p] = line.data_0;
        olddata[p + 1] = line.data_1;
        p += 2;
        p %= n * 2;
    }
    free(line.time);
    free(olddata);
    return 1;
}