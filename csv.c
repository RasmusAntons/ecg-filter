//
// Created by rasmus on 04/06/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <values.h>
#include "csv.h"

#define COLUMNS 2 /*TODO: detect number of columns*/

int initialize(FILE *instream, FILE *outstream)
{
    char buffer[MAXLENGTH];
    int i = 0;
    while (i++ < SKIPLINES)
    {
        if (fgets(buffer, MAXLENGTH, instream) != NULL)
            fprintf(outstream, buffer);
        else
            return 0;
    }
    return 1;
}

int skip_head(FILE *instream)
{
    char buffer[MAXLENGTH];
    int i = 0;
    while (i++ < SKIPLINES)
    {
        if (fgets(buffer, MAXLENGTH, instream) != NULL)
            continue;
        else
            return 0;
    }
    return 1;
}

int get_next_line(struct line_data *line, FILE *instream, struct file_attributes *attributes)
{
    char buffer[MAXLENGTH];

    if (fgets(buffer, MAXLENGTH, instream) != NULL)
    {
        if (attributes->empty)
        {
            attributes->empty = 0;
            attributes->error = 0;
            attributes->lines = 0;
            attributes->ymin = MAXDOUBLE;
            attributes->ymax = MINDOUBLE;
            attributes->columns = 2;
        }
        int n = 0;
        strncpy(line->time, buffer, 10);
        int i;
        for (i = 0; i < COLUMNS; i++)
        {
            while (buffer[n] != ',')
            {
                if (buffer[n] == '\0')
                {
                    attributes->error = 1;
                    return 0;
                }
                n++;
            }
            line->data[i] = atof(&buffer[++n]);
            attributes->lines++;
            if (line->data[i] < attributes->ymin) attributes->ymin = line->data[i];
            if (line->data[i] > attributes->ymax) attributes->ymax = line->data[i];
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

int write_line(struct line_data *line, struct file_attributes *attributes, FILE *outstream)
{
    fprintf(outstream, "%s", line->time);
    int c;
    for (c = 0; c < attributes->columns; c++)
        fprintf(outstream, ",%1.3f", line->data[c]);
    fprintf(outstream, "\n");
    return 1;
}
