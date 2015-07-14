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
const char *argp_program_version = "ecg-filter 0.01";
const char *argp_program_bug_address = "<mail@rasmusantons.de>";
static char doc[] = "Applies a filter to an existing set of ecg data\ndata format is csv (time, data_0, data_1)";
static char args_doc[] = "file";
static struct argp_option options[] = {
        {"filters", 'f', "f0,f1,...,fn", 0, "apply these filters"},
        {"graph", 'g', "file", 0, "export graph as svg"},
        {"limit", 'l', "limit", 0, "maximum points in graph"},
        {"output", 'o', "file", 0, "output to file"},
        {0}
};
struct arguments {
    char *file;
    char *filter;
    char *graph;
    int limit;
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
        case 'l':
            arguments->limit = atoi(arg);
            if (arguments->limit <= 0)
            {
                printf("%s ist not a positive number (idiot)\n", arg);
                arguments->limit = GRAPHLIMIT;
            }
            else
                printf("limit graph to %d points\n", arguments->limit);
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
    arguments.filter = "void";
    arguments.graph = NULL;
    arguments.limit = GRAPHLIMIT;
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

    if (!choose_filter(arguments.filter, instream, outstream, &attributes)) {
        // one filter does not exist, should already be printed
        fclose(instream);
        fclose(outstream);
        return EXIT_FAILURE;
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
        if (!export_graph(arguments.graph, &attributes, instream, outstream, arguments.limit))
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
