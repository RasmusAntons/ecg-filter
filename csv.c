//
// Created by rasmus on 04/06/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <values.h>
#include "csv.h"

#define MAXLENGTH 80
#define SKIPLINES 2

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
    int n = 0;
    char buffer[MAXLENGTH];

    if (fgets(buffer, MAXLENGTH, instream) != NULL)
    {
        if (attributes->empty)
        {
            attributes->empty = 0;
            attributes->lines = 0;
            attributes->xmin = MAXDOUBLE;
            attributes->xmax = MINDOUBLE;
            attributes->ymin = MAXDOUBLE;
            attributes->ymax = MINDOUBLE;
        }
        strncpy(line->time, buffer, 10);
        int i;
        for (i = 0  ; i < 2; i++)
        {
            double *target;
            switch (i)
            {
                case 0: target = &line->data_0; break;
                case 1: target = &line->data_1; break;
                default: return 0;
            }
            while (buffer[n] != ',')
            {
                if (buffer[n] == '\0') return 0;
                n++;
            }
            *target = atof(&buffer[++n]);
            attributes->lines++;
            if (*target < attributes->ymin) attributes->ymin = *target;
            if (*target > attributes->ymax) attributes->ymax = *target;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

int write_line(struct line_data *line, FILE *outstream)
{
    fprintf(outstream, "%s,%1.3f,%1.3f\n", line->time, line->data_0, line->data_1);
    return 1;
}