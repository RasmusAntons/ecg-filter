//
// Created by rasmus on 04/06/15.
//

#ifndef FILTER_CSV_H
#define FILTER_CSV_H

struct line_data {
    char *time;
    double data_0;
    double data_1;
};

struct file_attributes {
    int empty;
    int lines;
    double xmin;
    double xmax;
    double ymin;
    double ymax;
};

int initialize(FILE *instream, FILE *outstream);
int skip_head(FILE *instream);
int get_next_line(struct line_data *line, FILE *instream, struct file_attributes *attributes);
int write_line(struct line_data *line, FILE *outstream);

#endif //FILTER_CSV_H
