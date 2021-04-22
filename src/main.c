#include <stdio.h>
#include "lex.h"
#include <stdbool.h>
#include <string.h>

void src_read(char** source_code, FILE* source_file)
{
	fseek(source_file, 0, SEEK_END);
	size_t fileSize = ftell(source_file);
	*source_code = malloc(fileSize + 1);
	rewind(source_file);
	fread(*source_code, 1, fileSize, source_file);
}

int main(int argc, char *argv[])
{
	FILE* source_file;
	char *source_code;

	source_file = fopen(argv[1], "r");
	if(source_file==NULL)
	{
		printf("File \"%s\" does not exist!\n", argv[1]);
		printf("Usage:\n\tzerl <filename>\n");
		exit(0);
	}
	

	src_read(&source_code, source_file);
	//fwrite(source_code, sizeof(char), strlen(source_code), stdout);

	tokenize(source_code);
	fclose(source_file);

	return 0;
}
