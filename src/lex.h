#ifndef LEX_H
#define LEX_H
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "toktype.h"

void tokenize(char* src);

#endif