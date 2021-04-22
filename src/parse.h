#ifndef PARSE_H
#define PARSE_H
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "toktype.h"

void parse_tok(token_lex tok);

#endif