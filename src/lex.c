#include "lex.h"
#include "parse.h"
#include "toktype.h"

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
			typestr = "TYPE_VAR";
			break;
		case 9:
			typestr = "TYPE_LOOP";
			break;
		case 10:
			typestr = "TYPE_IDEN";
			break;
		case 11:
			typestr = "TYPE_IF";
			break;
		case 12:
			typestr = "TYPE_ELSE";
			break;
		case 13:
			typestr = "TYPE_BREAK";
			break;
		case 14:
			typestr = "TYPE_EQ";
			break;
		case 15:
			typestr = "TYPE_ASGN";
			break;
		case 16:
			typestr = "TYPE_GRT";
			break;
		case 17:
			typestr = "TYPE_LSS";
			break;
		case 18:
			typestr = "TYPE_NE";
			break;
		case 19:
			typestr = "TYPE_ADD";
			break;
		case 20:
			typestr = "TYPE_SUB";
			break;
		case 21:
			typestr = "TYPE_MUL";
			break;
		case 22:
			typestr = "TYPE_DIV";
			break;
		case 23:
			typestr = "TYPE_CMA";
			break;
		case 24:
			typestr = "TYPE_GEQ";
			break;
		case 25:
			typestr = "TYPE_LEQ";
			break;
	
	
		default:
			typestr = "(NULL)";
			break;
	}
	return typestr;
}

void toktable(int i, int j)
{
	printf("[%s]\t%d, %d - %s\n", val(Tok.type), i, j, Tok.token);
	Tok.type_str = val(Tok.type);
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

token_lex tokenize(char* src, int PI)
{
	for(int i=PI;src[i]!=0;i++)
	{
		if (src[i] == '\n') {
    		++line;
		}
		else if (src[i] == '#') {
	    	while (src[i] != 0 && src[i] != '\n') {
    	    	i++;
   			}
		}
		else if(src[i]==' '||src[i]=='\t'||src[i]=='\v'||src[i]=='\f'||src[i]=='\r')
		{}
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
					toktable(i, j);
					i=j;
					Tok.index = i+1;
					return Tok;
				}
				else if(src[j]=='\n')
				{
					printf("Expected [\"] at the end of string! LineNo: %d\n", line);
					free(Tok.token);
					exit(0);
				}
			}
		}

/*
	You can use these methods instead of isdeigit
	src[i] >= '0' && src[i] <= '9'

		(and)

	!(src[j] >= '0' && src[j] <= '9')

*/
		else if(isdigit(src[i]))
		{
			for(size_t j=i;src[i]!=0;j++)
			{
				if(!isdigit(src[j]))
				{
					Tok.token = substr(src, i, j);
					Tok.type = TYPE_INT;
					toktable(i, j-1);
					i = j-1;
					Tok.index = i+1;
					break;
				}
			}
			return Tok;
		}
		else if(src[i]=='(')
		{
			Tok.type = TYPE_OP;
			Tok.token = "(";
			Tok.index = i+1;
			toktable(i, i);
			return Tok;
		}
		else if(src[i]==')')
		{
			Tok.type = TYPE_CP;
			Tok.token = ")";
			Tok.index = i+1;
			toktable(i, i);
			return Tok;
		}
		else if(src[i]=='{')
		{
			Tok.type = TYPE_OCB;
			Tok.token = "{";
			Tok.index = i+1;
			toktable(i, i);
			return Tok;
		}
		else if(src[i-1]=='}')
		{
			Tok.type = TYPE_CCB;
			Tok.token = "}";
			Tok.index = i+1;
			toktable(i, i);							
			return Tok;
		}
		else if(strncmp(src + i, "print", 5) == 0)
		{
			Tok.type = TYPE_PRINT;
			Tok.token = "print";
			toktable(i, i+5);
			i = i+4;
			Tok.index = i+1;
			return Tok;
		}
		else if(strncmp(src + i, "get", 3) == 0)
		{
			Tok.type = TYPE_GET;
			Tok.token = "get";
			toktable(i, i+3);
			i = i+2;
			Tok.index = i+1;
			return Tok;
		}
		else if(strncmp(src + i, "var", 3) == 0)
		{
			Tok.type = TYPE_VAR;
			Tok.token = "var";
			toktable(i, i+3);	
			i = i+2;
			Tok.index = i+1;
			return Tok;
		}
		else if(strncmp(src + i, "loop", 4) == 0)
		{
			Tok.type = TYPE_LOOP;
			Tok.token = "loop";
			toktable(i, i+4);
			i = i+3;
			Tok.index = i+1;
			return Tok;
		}
		else if(strncmp(src + i, "else", 4) == 0)
		{
			Tok.type = TYPE_ELSE;
			Tok.token = "else";
			Tok.index = i+1;
			toktable(i, i+4);
			
			i = i+3;
			return Tok;
		}
		else if(strncmp(src + i, "break", 5) == 0)
		{
			Tok.type = TYPE_BREAK;
			Tok.token = "break";
			toktable(i, i+5);
			i = i+4;	
			Tok.index = i+1;
			return Tok;
		}
		else if(src[i]=='=' && src[i+1]=='=')
		{
			Tok.type = TYPE_EQ;
			Tok.token = "==";
			toktable(i, i);
			i+=1;
			Tok.index = i+1;
			return Tok;
		}
		else if(src[i]=='=')
		{
			Tok.type = TYPE_ASGN;
			Tok.token = "=";
			Tok.index = i+1;
			toktable(i, i+1);	
			return Tok;
		}
		else if(src[i]=='<' && src[i+1]!='=')
		{
			Tok.type = TYPE_OAB;
			Tok.token = "<";
			toktable(i, i);
			Tok.index = i+1;
			return Tok;
		}
		else if(src[i]=='>' && src[i+1]!='=')
		{
			Tok.type = TYPE_CAB;
			Tok.token = ">";
			Tok.index = i+1;
			toktable(i, i);
			return Tok;
		}
		else if(src[i]=='!' && src[i+1]=='=')
		{
			Tok.type = TYPE_NE;
			Tok.token = "!=";
			toktable(i, i+1);
			i+=1;
			Tok.index = i+1;
			return Tok;
		}
		else if(src[i]=='>' && src[i+1]=='=')
		{
			Tok.type = TYPE_GEQ;
			Tok.token = ">=";
			toktable(i, i+1);
			i+=1;
			Tok.index = i+1;
			return Tok;
		}
		else if(src[i]=='<' && src[i+1]=='=')
		{
			Tok.type = TYPE_LEQ;
			Tok.token = "<=";
			toktable(i, i+1);
			i+=1;
			Tok.index = i+1;
			return Tok;
		}
		else if(src[i]=='+')
		{
			Tok.type = TYPE_ADD;
			Tok.token = "+";
			toktable(i, i);
			Tok.index = i+1;
			return Tok;
		}
		else if(src[i]=='-')
		{
			Tok.type = TYPE_SUB;
			Tok.token = "-";
			toktable(i, i);
			Tok.index = i+1;
			return Tok;
		}
		else if(src[i]=='*')
		{
			Tok.type = TYPE_MUL;
			Tok.token = "*";
			Tok.index = i+1;
			toktable(i, i);
			return Tok;
		}
		else if(src[i]=='/')
		{
			Tok.type = TYPE_DIV;
			Tok.token = "/";
			Tok.index = i+1;
			toktable(i, i);
			return Tok;
		}
		else if(src[i]==',')
		{
			Tok.type = TYPE_CMA;
			Tok.token = ",";
			Tok.index = i+1;
			toktable(i, i);
			return Tok;
		}

/*
	You can use these methods instead of isalpha
	(src[j] >= 'A' && src[j] <= 'Z') || !(src[j] >= 'a' && src[j] <= 'z')

		(or)

	(src[j] >= 'A' && src[j] <= 'z')
*/

		else if(isalpha(src[i]))
		{
			for(size_t j=i;src[i]!=0;j++)
			{
				if(!isalpha(src[j]))
				{
					Tok.token = substr(src, i, j);
					Tok.type = TYPE_IDEN;
					toktable(i, j-1);
					i = j-1;
					Tok.index = i+1;
					break;
				}
			}
			return Tok;
		}
		else
		{
			printf("Token Error! [%c] token not known\n", src[i]);
			exit(0);
		}
	}
	return Tok;
}