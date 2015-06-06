#include <stdio.h>
#include <argp.h>
#include <stdlib.h>
#include <string.h>
#include "filters.h"
#include "graph.h"

/**
 * Some argp stuff
 * Documentation at http://www.gnu.org/software/libc/manual/html_node/Argp.html
 */
const char *argp_program_version = "filter 0.01";
const char *argp_program_bug_address = "<mail@rasmusantons.de>";
static char doc[] = "Applies a filter to an existing set of ecg data\ndata format is csv (time, data_0, data_1)";
static char args_doc[] = "file";
static struct argp_option options[] = {
        {"filter", 'f', "filter", 0, "apply this filter"},
        {"graph", 'g', "file", 0, "export graph as svg"},
        {"output", 'o', "file", 0, "output to file"},
        {0}
};
struct arguments {
    char *file;
    char *filter;
    char *graph;
    char *output;
};
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch (key)
    {
        case 'f':
            arguments->filter = arg;
            break;
        case 'g':
            arguments->graph = arg;
            break;
        case 'o':
            arguments->output = arg;
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num >= 1)
                argp_usage(state);
            arguments->file = arg;
            break;
        case ARGP_KEY_END:
            if (state->arg_num < 1)
                argp_usage(state);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}
static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char **argv) {
    FILE *instream;
    FILE *outstream;
    struct arguments arguments;

    arguments.file = NULL;
    arguments.filter = NULL;
    arguments.graph = NULL;
    arguments.output = NULL;
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    instream = fopen(arguments.file, "r");
    if (!instream)
    {
        printf("Error: file %s does not exist!\n", arguments.file);
        return EXIT_FAILURE;
    }

    if (arguments.output)
    {
        outstream = fopen(arguments.output, "wb+");
    }
    else {
        outstream = tmpfile();
    }
    if (!outstream)
    {
        printf("Error: failed to create output file!\n");
        fclose(instream);
        return EXIT_FAILURE;
    }

    struct file_attributes attributes;
    attributes.empty = 1;

    if (arguments.filter)
    {
        if (!strcmp(arguments.filter, "mean2"))
        {
            filter_mean(instream, outstream, &attributes, 2);
        }
        else if (!strcmp(arguments.filter, "mean5"))
        {
            filter_mean(instream, outstream, &attributes, 5);
        }
        else if (!strcmp(arguments.filter, "mean10"))
        {
            filter_mean(instream, outstream, &attributes, 10);
        }
        else if (!strcmp(arguments.filter, "derivative"))
        {
            filter_derivative(instream, outstream, &attributes);
        }
        else if (!strcmp(arguments.filter, "square"))
        {
            filter_square(instream, outstream, &attributes);
        }
        else if (!strcmp(arguments.filter, "void"))
        {
            filter_void(instream, outstream, &attributes);
        }
        else if (!strcmp(arguments.filter, "all"))
        {
            FILE *tmp0 = tmpfile();
            FILE *tmp1 = tmpfile();
            filter_mean(instream, tmp0, &attributes, 5);
            rewind(tmp0);
            filter_mean(tmp0, tmp1, &attributes, 5);
            rewind(tmp0);
            rewind(tmp1);
            filter_mean(tmp1, tmp0, &attributes, 5);
            rewind(tmp0);
            rewind(tmp1);
            filter_derivative(tmp0, tmp1, &attributes);
            rewind(tmp0);
            rewind(tmp1);
            filter_square(tmp1, outstream, &attributes);
            fclose(tmp0);
            fclose(tmp1);
        }
        else
        {
            printf("Error: filter %s does not exist!\n", arguments.filter);
            fclose(instream);
            fclose(outstream);
            return EXIT_FAILURE;
        }
    }
    else
    {
        filter_void(instream, outstream, &attributes);
    }
    if (attributes.error)
    {
        printf("Error: filter failed!\n");
        fclose(instream);
        fclose(outstream);
        return EXIT_FAILURE;
    }

    if (arguments.graph)
    {
        if (!export_graph(arguments.graph, &attributes, instream, outstream, GRAPHLIMIT))
        {
            printf("Error: could not create graph!\n");
            fclose(instream);
            fclose(outstream);
            return EXIT_FAILURE;
        }
    }

    fclose(instream);
    fclose(outstream);

    printf("finished\n");
    return EXIT_SUCCESS;
}
