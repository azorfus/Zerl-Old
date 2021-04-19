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
	// wTOK.type_str = TypeToString(TYPE_STR); todo
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

// main function that tokenizes the source code.
void tokenize_code(char* source_code)
{
	size_t source_len = strlen(source_code);
	for(int i=0;i<source_len;i++)
	{
		if(source_code[i]=='"')
		{
			for(int j=i+1;j<source_len;j++)
			{
				if(source_code[j]=='\\')
				{
					switch(source_code[j+1])
					{
						case '"':
							j+=2;
							break;
						case '\\':
							j+=2;
							break;
					
						default:
							continue;
					}
				}
				if(source_code[j]=='"')
				{
					wTOK.token = substr(source_code, i, j+1);
					wTOK.type = TYPE_STR;
					parse(i, j+1);
					i = j;
					break;
				}
			}
		}
		else if(source_code[i] >= '0' && source_code[i] <= '9')
		{
			for(size_t j=i;j<source_len;j++)
			{
				if(!(source_code[j] >= '0' && source_code[j] <= '9'))
				{
					wTOK.token = substr(source_code, i, j);
					wTOK.type = TYPE_INT;
					parse(i, j);
					i = j;
					break;
				}
			}
		}
		else if(source_code[i]=='(')
		{
					wTOK.type = TYPE_OP;
					wTOK.token = "(";
					parse(i, i+1);
		}
		else if(source_code[i]==')')
		{
					wTOK.type = TYPE_CP;
					wTOK.token = ")";
					parse(i, i+1);
		}
	}
}
