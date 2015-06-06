//
// Created by rasmus on 04/06/15.
//

#ifndef FILTER_CSV_H
#define FILTER_CSV_H

#define MAXLENGTH 80
#define SKIPLINES 2
#define MAXCOLUMNS 2

struct line_data {
    char *time;
    double *data;
};

struct file_attributes {
    int empty;
    int error;
    int lines;
    int columns;
    double ymin;
    double ymax;
};

int initialize(FILE *instream, FILE *outstream);
int skip_head(FILE *instream);
int get_next_line(struct line_data *line, FILE *instream, struct file_attributes *attributes);
int write_line(struct line_data *line, struct file_attributes *attributes, FILE *outstream);

#endif //FILTER_CSV_H
