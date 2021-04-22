#ifndef TOKTYPE_H
#define TOKTYPE_H

enum tokentype
{
	TYPE_STR, 
	TYPE_INT, 
	TYPE_OP,
	TYPE_CP,
	TYPE_OCB,
	TYPE_CCB,	
	TYPE_PRINT,
	TYPE_GET,
	TYPE_I_STR,
	TYPE_I_INT,
	TYPE_LOOP,
	TYPE_IDEN,
	TYPE_IF,
	TYPE_ELSE,
	TYPE_BREAK,
	TYPE_EQ,
	TYPE_ASGN,
	TYPE_OAB,
	TYPE_CAB,
	TYPE_NE,
	TYPE_ADD,
	TYPE_SUB,
	TYPE_MUL,
	TYPE_DIV,
	TYPE_CMA
};

typedef struct token_lex
{
    enum tokentype type;
	const char* type_str;
	char *token;
	int index;
    
} token_lex;

#endif