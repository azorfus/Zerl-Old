#include "lex.h"

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
	TYPE_LOOP,
	TYPE_IF,
	TYPE_ELSE,
    TYPE_EQ,
    TYPE_ASGN,
    TYPE_GRT,
    TYPE_LSS,
    TYPE_NE,
    TYPE_ADD,
    TYPE_SUB,
    TYPE_MUL,
    TYPE_DIV
};

typedef struct token_lex
{
    enum tokentype type;
	const char* type_str;
	char *token;
	int index;
    
} token_lex;

token_lex wTOK;

int count = 0;
char* block;

void parse(int i, int j)
{
	// wTOK.type_str = TypeToString(TYPE_STR); TODO
	printf("[%d] %d, %d - %s\n", wTOK.type, i, j, wTOK.token);
}

char* substr(char* source, int start, int end)
{
	size_t size = end - start;
	char* retstr = malloc(size+1);
	int dex = 0;

	for(int i=start;i<end;i++)
	{
		retstr[dex] = source[i];
		dex++;
	}
	retstr[size+1] = '\0';
	return retstr;
}

void tokenize_code(char* source_code)
{
	size_t source_len = strlen(source_code);
	int end_quote = 0;
	int end_int = 0;
	for(int i=0;i<source_len;i++)
	{
		if(source_code[i]=='\"' && i!=end_quote)
		{
			for(int j=i+1;j<source_len;j++)
			{
				if(source_code[j]=='\"')
				{
					end_quote=j;
					wTOK.token = substr(source_code, i, j);
					wTOK.type = TYPE_STR;
					parse(i, j+1);
					break;
				}
			}
		}
		else if(source_code[i] >= '0' && source_code[i] <= '9' && i > end_int && i > end_quote)
		{
			for(size_t j=i;j<source_len;j++)
			{
				if(!(source_code[j] >= '0' && source_code[j] <= '9'))
				{
					wTOK.token = substr(source_code, i, j);
					wTOK.type = TYPE_INT;
					end_int = j;
					parse(i, j);
					break;
				}
			}
		}
	}
}
