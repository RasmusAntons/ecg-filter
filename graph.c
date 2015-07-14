//
// Created by rasmus on 04/06/15.
//

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int print_values(int column, struct file_attributes *attributes, FILE *source, FILE *dest, int n_max)
{
    int n;
    struct line_data line;
    line.time = malloc(80 * sizeof(long));
    line.data = calloc(MAXCOLUMNS, sizeof(double));
    rewind(source);
    skip_head(source);
    for (n = 0; n < n_max; n++)
    {
        if (!get_next_line(&line, source, attributes)) {
            free(line.time);
            return 0;
        }
        fprintf(dest, "%lf,%lf ", (double) n/200.0, line.data[column]);
    }
    free(line.time);
    free(line.data);
    return 1;
}

int export_graph(char *file,struct file_attributes *attributes, FILE *instream, FILE *outstream, int limit) {
    if (limit > attributes->lines) limit = attributes->lines;
    int w = limit, h = 600;

    FILE *f = fopen(file, "w");
    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">");
    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%dpx\" height=\"%dpx\"", w, h);
    fprintf(f, " viewBox=\"%lf %lf %lf %lf\">", 0.0, attributes->ymin, limit/200.0, attributes->ymax);
    fprintf(f, "<g transform=\"scale(1.0 -1.0)\">\n");

    fprintf(f, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\"", 0.0, 0.0, limit/200.0, 0.0);
    fprintf(f, " style=\"fill:none;stroke:grey;stroke-width:0.02\" />\n");
    fprintf(f, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\"", 0.0, attributes->ymin, 0.0, attributes->ymax);
    fprintf(f, " style=\"fill:none;stroke:grey;stroke-width:0.02\" />\n");

    int c;
    for (c = 0; c < 2; c++)
    {
        fprintf(f, "<path d=\"M ");
        if (!print_values(c, attributes, instream, f, limit))
        {
            fclose(f);
            return 0;
        }
        fprintf(f, "\" style=\"fill:none;stroke:black;stroke-opacity:%1.3f;stroke-width:0.015\" />\n", 1.0 / (c + 1));

        fprintf(f, "<path d=\"M ");
        if (!print_values(c, attributes, outstream, f, limit))
        {
            fclose(f);
            return 0;
        }
        fprintf(f, "\" style=\"fill:none;stroke:red;stroke-opacity:%1.3f;stroke-width:0.015\" />\n", 1.0 / (c + 1));
    }

    fprintf(f, "</g></svg>\n");
    fclose(f);

    return 1;
}
