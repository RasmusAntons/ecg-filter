//
// Created by rasmus on 04/06/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filters.h"

void init(struct line_data *line, struct line_data *write)
{
    line->time = malloc(80 * sizeof(long));
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

int filter_detect(FILE *instream, FILE *outstream, struct file_attributes *attributes)
{
    struct line_data line;
    init(&line, NULL);
    int c;
    initialize(instream, outstream);
    while (get_next_line(&line, instream, attributes))
    {
        for (c = 0; c < attributes->columns; c++)
            line.data[c] = line.data[c] > .1 ? 1 : 0;
        write_line(&line, attributes, outstream);
    }
    cleanup(&line, NULL);
    return 1;
}

/*
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
*/

int choose_filter(char *filters, FILE *instream, FILE *outstream, struct file_attributes *attributes)
{
    FILE *read, *write;
    int success;
    size_t i = 0, j = 0;
    char *buffer = calloc(15, sizeof(int));
    while (filters[i] != '\0') {
        attributes->lines = 0;
        i = j;
        while (filters[j] != ',' && filters[j] != '\0')
            j++;
        if (j - i > 15)
        {
            printf("Error: invalid filter(s): %s\n", filters + i);
            return 0;
        }
        memcpy(buffer, filters + i, j - i);
        buffer[j - i] = '\0';
        if (i)
        {
            read = write;
            rewind(read);
        }
        else
            read = instream;
        if (filters[j] != '\0')
            write = tmpfile();
        else
            write = outstream;

        printf("Apply filter %s", buffer);
        if (!strcmp(buffer, "mean2"))
        {
            success = filter_mean(read, write, attributes, 2);
        }
        else if (!strcmp(buffer, "mean5"))
        {
            success = filter_mean(read, write, attributes, 5);
        }
        else if (!strcmp(buffer, "mean10"))
        {
            success = filter_mean(read, write, attributes, 10);
        }
        else if (!strcmp(buffer, "derivative"))
        {
            success = filter_derivative(read, write, attributes);
        }
        else if (!strcmp(buffer, "square"))
        {
            success = filter_square(read, write, attributes);
        }
        else if (!strcmp(buffer, "detect"))
        {
            success = filter_detect(read, write, attributes);
        }
        else if (!strcmp(buffer, "void"))
        {
            success = filter_void(read, write, attributes);
        }
        else
        {
            printf("\nError: filter %s does not exist!\n", buffer);
            return 0;
        }
        if (!success)
        {
            printf("\nError: something went wrong while applying filter %s!\n", buffer);
            return 0;
        }
        printf(" [OK]\n");

        if (i)
            fclose(read);
        i = j++;
    }
    free(buffer);
    return 1;
}
