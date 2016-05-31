#include <stdlib.h>
#include <stdio.h>

#define SV_MARKDOWN_IMPLEMENTATION
#include "sv_markdown.h"

#define _CRT_SECURE_NO_WARNING

int main(int argc, char** argv[])
{
	char* headerTest =	"# hai!             \n"
						"##       hai!     \n"
						"### hai!                       \r"
						"####            hai!           \r\n"
						"##### hai!        \r"
						"######        hai!         \n"
						"# \n"
						"## \n"
						"### \n"
						"#### \n"
						"##### \n"
						"###### ";

	printf("%s\n", headerTest);

	sv_compile_ast(headerTest);

	getchar();
}

char* ReadEntireFile(char* filename)
{
	char * result = 0;

	FILE *file = fopen(filename, "r");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		size_t filesize = ftell(file);
		fseek(file, 0, SEEK_SET);

		result = (char*)malloc(filesize + 1);

		fread(result, filesize, 1, file);

		result[filesize] = 0;

		fclose(file);
	}

	return result;
}
