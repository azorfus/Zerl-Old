#include "parse.h"
#include "toktype.h"

void parse_tok(token_lex tok)
{
	printf("%s, %s, %d\n", tok.type_str, tok.token, tok.index);
	

}