//
// Created by rasmus on 04/06/15.
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "graph.h"

int test_graph(char *file)
{
    int n, N=100;
    double x[N], y[N], xmin, xmax, ymin, ymax;
    for (n = 0; n < N; n++)
    {
        x[n] = n/10.0;
        y[n] = cos(x[n]);

        if (n==0)
        {
            xmin = x[0];
            xmax = x[0];
            ymin = y[0];
            ymax = y[0];
        }
        if (x[n] < xmin) xmin = x[n];
        if (x[n] > xmax) xmax = x[n];
        if (y[n] < ymin) ymin = y[n];
        if (y[n] > ymax) ymax = y[n];
    }

    int w = 1000, h = 200;

    double left, right, top, bottom;
    left = xmin - 0.1 * (xmax - xmin);
    right = xmax + 0.1 * (xmax - xmin);
    top = ymin - 0.1 * (ymax - ymin);
    bottom = ymax + 0.1 * (ymax - ymin);

    FILE *f = fopen(file, "w");
    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">");
    fprintf(f, "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"%dpx\" height=\"%dpx\"", w, h);
    fprintf(f, " viewBox=\"%lf %lf %lf %lf\"", left, top, right - left, bottom - top);
    fprintf(f, " preserveAspectRatio=\"none\">\n");
    fprintf(f, "<g transform=\"scale(1.0 -1.0)\">\n\n");

    fprintf(f, "<polyline points=\"%lf,%lf %lf,%lf\"", left, 0.0, right, 0.0);
    fprintf(f, " style=\"fill:none;stroke:grey;stroke-width:%lf\" />\n", 2 * (bottom - top) / h);
    fprintf(f, "<polyline points=\"%lf,%lf %lf,%lf\"", 0.0, top, 0.0, bottom);
    fprintf(f, " style=\"fill:none;stroke:grey;stroke-width:%lf\" />\n\n", 2 * (right - left) / w);

    fprintf(f, "<polyline points=\"");
    for (n = 0; n < N; n++) fprintf(f, "%lf,%lf ", x[n], y[n]);
    fprintf(f, "\" style=\"fill:none;stroke:black;stroke-width:%lf\" />", 2 * (right - left) / w);

    fprintf(f, "\n\n</g></svg>\n");
    fclose(f);

    return 1;
}

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
