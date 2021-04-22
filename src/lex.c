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
token_lex Tok;
int line = 1;

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
	printf("[%s] %d, %d - %s\n", val(Tok.type), i, j, Tok.token);
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

void tokenize(char* src)
{
	for(int i=0;src[i]!=0;i++)
	{
		if (src[i] == '\n') {
    		++line;
		}
		else if (src[i] == '#') {
	    	while (src[i] != 0 && src[i] != '\n') {
    	    	i++;
   			}
		}
		else if(src[i]=='"' || src[i] == '\'')
		{
			for(int j=i+1;src[j]!=0;j++)
			{
				if(src[j]=='\\')
				{
					switch(src[j+1])
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
				if(src[j]=='"' || src[j]=='\'')
				{
					Tok.token = substr(src, i, j+1);
					Tok.type = TYPE_STR;
					parse(i, j);
					i=j;
					break;
				}
				else if(src[j]=='\n')
				{
					printf("Expected [\"] at the end of string! LineNo: %d\n", line);
					free(Tok.token);
					exit(0);				
				}
			}
		}
		else if(src[i] >= '0' && src[i] <= '9')
		{
			for(size_t j=i;src[j]!=0;j++)
			{
				if(!(src[j] >= '0' && src[j] <= '9'))
				{
					Tok.token = substr(src, i, j);
					Tok.type = TYPE_INT;
					parse(i, j-1);
					i = j-1;
					break;
				}
			}
		}else if(src[i]=='(')
		{
			Tok.type = TYPE_OP;
			Tok.token = "(";
			parse(i, i);
		}
		else if(src[i]==')')
		{
			Tok.type = TYPE_CP;
			Tok.token = ")";
			parse(i, i);
		}
		else if(src[i]=='{')
		{
			Tok.type = TYPE_OCB;
			Tok.token = "{";
			parse(i, i);
		}
		else if(src[i-1]=='}')
		{
			Tok.type = TYPE_CCB;
			Tok.token = "}";
			parse(i, i);
		}
		else if(strncmp(src + i, "print", 5) == 0)
		{
			Tok.type = TYPE_PRINT;
			Tok.token = "print";
			parse(i, i+5);
			i = i+4;
		}
		else if(strncmp(src + i, "get", 3) == 0)
		{
			Tok.type = TYPE_GET;
			Tok.token = "get";
			parse(i, i+3);
			i = i+2;
		}
		else if(strncmp(src + i, "str", 3) == 0)
		{
			Tok.type = TYPE_I_STR;
			Tok.token = "str";
			parse(i, i+3);
			i = i+2;
		}
		else if(strncmp(src + i, "int", 3) == 0)
		{
			Tok.type = TYPE_I_INT;
			Tok.token = "int";
			parse(i, i+3);
			i = i+2;
		}
		else if(strncmp(src + i, "loop", 4) == 0)
		{
			Tok.type = TYPE_LOOP;
			Tok.token = "loop";
			parse(i, i+4);
			i = i+3;
		}
		else if(strncmp(src + i, "else", 4) == 0)
		{
			Tok.type = TYPE_ELSE;
			Tok.token = "else";
			parse(i, i+4);
			i = i+3;
		}
		else if(strncmp(src + i, "break", 5) == 0)
		{
			Tok.type = TYPE_BREAK;
			Tok.token = "break";
			parse(i, i+5);
			i = i+4;	
		}
		else if(src[i]=='=' && src[i+1]!='=')
		{
			Tok.type = TYPE_EQ;
			Tok.token = "=";
			parse(i, i);
		}
		else if(src[i]=='=' && src[i]=='=')
		{
			Tok.type = TYPE_ASGN;
			Tok.token = "==";
			parse(i, i+1);
			i++;			
		}
		else if(src[i]=='<')
		{
			Tok.type = TYPE_OAB;
			Tok.token = "<";
			parse(i, i);
		}
		else if(src[i]=='>')
		{
			Tok.type = TYPE_CAB;
			Tok.token = ">";
			parse(i, i);
		}
		else if(src[i]=='!' && src[i+1]=='=')
		{
			Tok.type = TYPE_NE;
			Tok.token = "!=";
			parse(i, i+1);
		}
		else if(src[i]=='+')
		{
			Tok.type = TYPE_ADD;
			Tok.token = "+";
			parse(i, i);
		}
		else if(src[i]=='-')
		{
			Tok.type = TYPE_SUB;
			Tok.token = "-";
			parse(i, i);
		}
		else if(src[i]=='*')
		{
			Tok.type = TYPE_MUL;
			Tok.token = "*";
			parse(i, i);
		}
		else if(src[i]=='/')
		{
			Tok.type = TYPE_DIV;
			Tok.token = "/";
			parse(i, i);
		}
		else if(src[i]==',')
		{
			Tok.type = TYPE_CMA;
			Tok.token = ",";
			parse(i, i);
		}
		else if((src[i] >= 'A' && src[i] <= 'Z') || (src[i] >= 'a' && src[i] <= 'z'))
		{
			for(size_t j=i;src[i]!=0;j++)
			{
				if(!(src[j] >= 'A' && src[j] <= 'z'))
				{
					Tok.token = substr(src, i, j);
					Tok.type = TYPE_IDEN;
					parse(i, j-1);
					i = j-1;
					break;
				}
			}
		}
	}
}
