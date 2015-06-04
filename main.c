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
        {"graph", 'g', 0, 0, "graph it, doesn't work yet :^)"},
        {"output", 'o', "file", 0, "output to file"},
        {0}
};
struct arguments {
    char *file;
    char *filter;
    int graph;
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
            arguments->graph = 1;
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
    arguments.graph = 0;
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
        else if (!strcmp(arguments.filter, "none"))
        {
            filter_void(instream, outstream, &attributes);
        }
        else
        {
            printf("Error: filter %s does not exist!\n", arguments.filter);
            return EXIT_FAILURE;
        }
    }
    else
    {
        filter_void(instream, outstream, &attributes);
    }

    if (arguments.graph)
    {
        export_graph("test.svg", &attributes, instream, outstream, 1500);
    }

    fclose(instream);
    fclose(outstream);

    printf("finished\n");
    return EXIT_SUCCESS;
}