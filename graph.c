//
// Created by rasmus on 04/06/15.
//

#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int export_graph(char *file,struct file_attributes *attributes, FILE *instream, FILE *outstream, int limit) {
    int n, N = attributes->lines;
    if (N > limit) N = limit;

    struct line_data line;
    struct file_attributes void_attributes;
    line.time = malloc(10 * sizeof(char));

    int w = N, h = 600;

    FILE *f = fopen(file, "w");
    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">");
    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%dpx\" height=\"%dpx\"", w, h);
    fprintf(f, " viewBox=\"%lf %lf %lf %lf\">", 0.0, attributes->ymin, N/200.0, attributes->ymax);
    fprintf(f, "<g transform=\"scale(1.0 -1.0)\">\n\n");

    fprintf(f, "<polyline points=\"%lf,%lf %lf,%lf\"", 0.0, 0.0, N/200.0, 0.0);
    fprintf(f, " style=\"fill:none;stroke:grey;stroke-width:%lf\" />\n", 0.02);
    fprintf(f, "<polyline points=\"%lf,%lf %lf,%lf\"", 0.0, attributes->ymin, 0.0, attributes->ymax);
    fprintf(f, " style=\"fill:none;stroke:grey;stroke-width:%lf\" />\n\n", 0.02);

    fprintf(f, "<polyline points=\"");
    rewind(instream);
    skip_head(instream);
    for (n = 0; n < N; n++)
    {
        get_next_line(&line, instream, &void_attributes);
        fprintf(f, "%lf,%lf ", (double) n/200.0, line.data_0);
    }
    fprintf(f, "\" style=\"fill:none;stroke:black;stroke-width:%lf\" />", 0.015);

    fprintf(f, "<polyline points=\"");
    rewind(outstream);
    skip_head(outstream);
    for (n = 0; n < N; n++)
    {
        get_next_line(&line, outstream, &void_attributes);
        fprintf(f, "%lf,%lf ", (double) n/200.0, line.data_0);
    }
    fprintf(f, "\" style=\"fill:none;stroke:red;stroke-width:%lf\" />", 0.015);

    fprintf(f, "\n\n</g></svg>\n");
    fclose(f);
    free(line.time);

    return 1;
}
