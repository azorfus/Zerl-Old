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

token_lex wTOK;

int count = 0;
char* block;

char* val(enum tokentype type)
{
	char* typestr;
	switch(type)
	{
		case 0:
			typestr = "TYPE_STR";
			break;
		case 1:
			typestr = "TYPE_INT";
			break;
		case 2:
			typestr = "TYPE_OP";
			break;
		case 3:
			typestr = "TYPE_CP";
			break;
		case 4:
			typestr = "TYPE_OCB";
			break;
		case 5:
			typestr = "TYPE_CCB";
			break;
		case 6:
			typestr = "TYPE_PRINT";
			break;
		case 7:
			typestr = "TYPE_GET";
			break;
		case 8:
			typestr = "TYPE_I_STR";
			break;
		case 9:
			typestr = "TYPE_I_INT";
			break;
		case 10:
			typestr = "TYPE_LOOP";
			break;
		case 11:
			typestr = "TYPE_IDEN";
			break;
		case 12:
			typestr = "TYPE_IF";
			break;
		case 13:
			typestr = "TYPE_ELSE";
			break;
		case 14:
			typestr = "TYPE_BREAK";
			break;
		case 15:
			typestr = "TYPE_EQ";
			break;
		case 16:
			typestr = "TYPE_ASGN";
			break;
		case 17:
			typestr = "TYPE_GRT";
			break;
		case 18:
			typestr = "TYPE_LSS";
			break;
		case 19:
			typestr = "TYPE_NE";
			break;
		case 20:
			typestr = "TYPE_ADD";
			break;
		case 21:
			typestr = "TYPE_SUB";
			break;
		case 22:
			typestr = "TYPE_MUL";
			break;
		case 23:
			typestr = "TYPE_DIV";
			break;
		case 24:
			typestr = "TYPE_CMA";
			break;
	
	
		default:
			typestr = "(NULL)";
			break;
	}
	return typestr;
}

void parse(int i, int j)
{
	// wTOK.type_str = TypeToString(TYPE_STR); todo
	printf("[%s] %d, %d - %s\n", val(wTOK.type), i, j, wTOK.token);
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
	int o;
	size_t source_len = strlen(source_code);
	for(int i=0;i<source_len;i++)
	{
		o = i-1;
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
					i = j+1;
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
		else if(source_code[i-1]==')')
		{
			wTOK.type = TYPE_CP;
			wTOK.token = ")";
			parse(i, i+1);
		}
		else if(source_code[i]=='{')
		{
			wTOK.type = TYPE_OCB;
			wTOK.token = "{";
			parse(i, i+1);
		}
		else if(source_code[i-1]=='}')
		{
			wTOK.type = TYPE_CCB;
			wTOK.token = "}";
			parse(i, i+1);
		}
		else if(strncmp(source_code + i, "print", 5) == 0)
		{
			wTOK.type = TYPE_PRINT;
			wTOK.token = "print";
			parse(i, i+5);
			i = i+5;
		}
		else if(strncmp(source_code + i, "get", 3) == 0)
		{
			wTOK.type = TYPE_GET;
			wTOK.token = "get";
			parse(i, i+3);
			i = i+3;
		}
		else if(strncmp(source_code + i, "str", 3) == 0)
		{
			wTOK.type = TYPE_I_STR;
			wTOK.token = "str";
			parse(i, i+3);
			i = i+3;
		}
		else if(strncmp(source_code + i, "int", 3) == 0)
		{
			wTOK.type = TYPE_I_INT;
			wTOK.token = "int";
			parse(i, i+3);
			i = i+3;
		}
		else if(strncmp(source_code + i, "loop", 4) == 0)
		{
			wTOK.type = TYPE_LOOP;
			wTOK.token = "loop";
			parse(i, i+4);
			i = i+3;
		}
		else if(strncmp(source_code + i, "else", 4) == 0)
		{
			wTOK.type = TYPE_ELSE;
			wTOK.token = "else";
			parse(i, i+4);
			i = i+3;
		}
		else if(strncmp(source_code + i, "break", 5) == 0)
		{
			wTOK.type = TYPE_BREAK;
			wTOK.token = "break";
			parse(i, i+5);
			i = i+4;	
		}
		else if(source_code[i]=='=' && source_code[i+1]!='=')
		{
			wTOK.type = TYPE_EQ;
			wTOK.token = "=";
			parse(i, i+1);
			i = i+1;
		}
		else if(source_code[i]=='=' && source_code[i]=='=')
		{
			wTOK.type = TYPE_ASGN;
			wTOK.token = "==";
			parse(i, i+1);
			i = i+1;
		}
		else if(source_code[i]=='<')
		{
			wTOK.type = TYPE_OAB;
			wTOK.token = "<";
			parse(i, i+1);
			i = i+1;
		}
		else if(source_code[i]=='>')
		{
			wTOK.type = TYPE_CAB;
			wTOK.token = ">";
			parse(i, i+1);
			i = i+1;
		}
		else if(source_code[i]=='!' && source_code[i+1]=='=')
		{
			wTOK.type = TYPE_NE;
			wTOK.token = "!=";
			parse(i, i+1);
			i = i+1;
		}
		else if(source_code[i]=='+')
		{
			wTOK.type = TYPE_ADD;
			wTOK.token = "+";
			parse(i, i+1);
			i = i+1;
		}
		else if(source_code[i]=='-')
		{
			wTOK.type = TYPE_SUB;
			wTOK.token = "-";
			parse(i, i+1);
			i = i+1;
		}
		else if(source_code[i]=='*')
		{
			wTOK.type = TYPE_MUL;
			wTOK.token = "*";
			parse(i, i+1);
			i = i+1;
		}
		else if(source_code[i]=='/')
		{
			wTOK.type = TYPE_DIV;
			wTOK.token = "/";
			parse(i, i+1);
			i = i+1;
		}
		else if(source_code[i]==',')
		{
			wTOK.type = TYPE_CMA;
			wTOK.token = ",";
			parse(i, i+1);
			i = i+1;
		}
		else if((source_code[i] >= 'A' && source_code[i] <= 'Z') || (source_code[i] >= 'a' && source_code[i] <= 'z'))
		{
			for(size_t j=i;j<source_len;j++)
			{
				if(!(source_code[j] >= 'A' && source_code[j] <= 'z'))
				{
					wTOK.token = substr(source_code, i, j);
					wTOK.type = TYPE_IDEN;
					parse(i, j);
					i = j;
					break;
				}
			}
		}
		//printf("[%c][%d]\n", source_code[i], i);
	}
}
