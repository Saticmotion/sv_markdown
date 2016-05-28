// LICENSE
//
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.

#ifdef SV_MARKDOWN_IMPLEMENTATION
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef uint32_t sv_uint32;
typedef int32_t sv_int32;

typedef enum
{
	SV_TOKEN_TEST = 1,
	SV_TOKEN_HEADING1 = 2,
	SV_TOKEN_HEADING2 = 3,
	SV_TOKEN_HEADING3 = 4,
	SV_TOKEN_HEADING4 = 5,
	SV_TOKEN_HEADING5 = 6,
	SV_TOKEN_HEADING6 = 7,
	SV_TOKEN_PARAGRAPH = 8,
	SV_TOKEN_EOF = 9
} sv_token_type;

typedef struct sv_token
{
	sv_token_type type;
	sv_uint32 length;
	char* text;
} sv_token;

typedef struct sv_tokenizer
{
	char* at;
} sv_tokenizer;

static void sv_trim_space_begin(sv_token* token)
{
	if (token->length > 0)
	{
		while(token->text[0] == ' ')
		{
			token->text++;
			token->length--;
		}
	}
}

static void sv_trim_whitespace_begin(sv_token* token)
{
	if (token->length > 0)
	{
		while(token->text[0] == ' ' ||
			token->text[0] == '\n' ||
			token->text[0] == '\r' ||
			token->text[0] == '\t' ||
			token->text[0] == '\v' ||
			token->text[0] == '\r')
		{
			token->text++;
			token->length--;
		}
	}
}

static void sv_trim_space_end(sv_token* token)
{
	if (token->length > 0)
	{
		sv_uint32 pos = token->length - 1;

		while(token->text[pos] == ' ')
		{
			pos--;
		}

		token->length = pos + 1;
	}
}

static void sv_trim_whitespace_end(sv_token* token)
{
	if (token->length > 0)
	{
		sv_uint32 pos = token->length - 1;

		while(token->text[pos] == ' ' ||
			token->text[pos] == '\n' ||
			token->text[pos] == '\r' ||
			token->text[pos] == '\t' ||
			token->text[pos] == '\v' ||
			token->text[pos] == '\r')
		{
			pos--;
		}

		token->length = pos + 1;
	}
}

static sv_token sv_getToken(sv_tokenizer* tokenizer)
{
	sv_token token = {0};
	token.length = 1;
	token.text = &tokenizer->at[0];

	switch(tokenizer->at[0])
	{
		case '\0': {token.type = SV_TOKEN_EOF;} break;
		case '#':
		{
			sv_uint32 headingCounter = 1;
			while (tokenizer->at[headingCounter] &&
				tokenizer->at[headingCounter] == '#')
			{
				headingCounter++;
			}

			if (headingCounter <= 6)
			{
				if (tokenizer->at[headingCounter] == ' ')
				{
					token.type = SV_TOKEN_HEADING1 + (headingCounter - 1);
					token.text = &tokenizer->at[headingCounter + 1];
					tokenizer->at += headingCounter + 1;

					sv_uint32 textLength = 0;

					while (tokenizer->at[0] != '\0')
					{
						if ((tokenizer->at[0] == '\n') ||
							(tokenizer->at[0] == '\r'))
						{
							if (tokenizer->at[0] == '\r' && tokenizer->at[1] == '\n')
							{
								tokenizer->at++;
							}
							break;
						}
						else
						{
							tokenizer->at++;
							textLength++;
						}
					}

					tokenizer->at++;

					token.length = textLength;

					sv_trim_space_end(&token);
					sv_trim_space_begin(&token);
				}
				else
				{
					//paragraph
				}
			}
			else
			{
				//paragraph
			}
		} break;

		default: {} break;
	}

	return token;
}

static char* sv_emitToken(sv_token* token)
{
	char* result;
	switch (token->type)
	{
		case SV_TOKEN_HEADING1:
		{
			size_t resultLength = token->length + 10; //9 heading, 1 null
			result = (char*)malloc(resultLength);
			sprintf(result, "<h1>%.*s</h1>\n", token->length, token->text);

		} break;
		case SV_TOKEN_HEADING2: {} break;
		case SV_TOKEN_HEADING3: {} break;
		case SV_TOKEN_HEADING4: {} break;
		case SV_TOKEN_HEADING5: {} break;
		case SV_TOKEN_HEADING6: {} break;
	}
	return result;
}

extern char* sv_compile_ast(char* markdown)
{
	sv_tokenizer tokenizer = {0};
	tokenizer.at = markdown;

	bool parsing = true;

	char* result = "";

	while(parsing)
	{
		sv_token token = sv_getToken(&tokenizer);

		switch(token.type)
		{
			case SV_TOKEN_EOF:
			{
				parsing = false;
			} break;
			case SV_TOKEN_HEADING1: {printf("<h1>%.*s</h1>\n", token.length, token.text); } break;
			case SV_TOKEN_HEADING2: {printf("<h2>%.*s</h2>\n", token.length, token.text); } break;
			case SV_TOKEN_HEADING3: {printf("<h3>%.*s</h3>\n", token.length, token.text); } break;
			case SV_TOKEN_HEADING4: {printf("<h4>%.*s</h4>\n", token.length, token.text); } break;
			case SV_TOKEN_HEADING5: {printf("<h5>%.*s</h5>\n", token.length, token.text); } break;
			case SV_TOKEN_HEADING6: {printf("<h6>%.*s</h6>\n", token.length, token.text); } break;

			default:
			{

			} break;
		}
	}

	return result;
}
#endif
