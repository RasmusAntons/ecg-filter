//
// Created by rasmus on 04/06/15.
//

#ifndef FILTER_FILTERS_H
#define FILTER_FILTERS_H

#include "csv.h"

int filter_void(FILE *instream, FILE *outstream, struct file_attributes *attributes);
int filter_mean(FILE *instream, FILE *outstream, struct file_attributes *attributes, size_t n);
int filter_derivative(FILE *instream, FILE *outstream, struct file_attributes *attributes);
int filter_square(FILE *instream, FILE *outstream, struct file_attributes *attributes);

#endif //FILTER_FILTERS_H
