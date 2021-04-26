#ifndef LEX_H
#define LEX_H
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parse.h"
#include "toktype.h"

token_lex tokenize(char* src, int PI);

#endif
