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
#include <math.h>

typedef uint32_t sv_uint32;
typedef int32_t sv_int32;

typedef enum
{
	SV_TOKEN_TEST,
	SV_TOKEN_HEADING1,
	SV_TOKEN_HEADING2,
	SV_TOKEN_HEADING3,
	SV_TOKEN_HEADING4,
	SV_TOKEN_HEADING5,
	SV_TOKEN_HEADING6,
	SV_TOKEN_THEMATIC_BREAK,
	SV_TOKEN_PARAGRAPH,
	SV_TOKEN_EOF
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
		while (token->text[0] == ' ')
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
		while (token->text[0] == ' ' ||
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

		while (token->text[pos] == ' ')
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

		while (token->text[pos] == ' ' ||
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

static char* sv_normalize_line_endings(char* text)
{
	size_t length = strlen(text);

	sv_uint32 sourceAt = 0;
	sv_uint32 destAt = 0;

	char* cleaned = malloc(length);

	while (sourceAt < length)
	{
		if (text[sourceAt] == '\r' && text[sourceAt + 1] == '\n')
		{
			cleaned[destAt++] = '\n';
			sourceAt += 2;
		}
		else if (text[sourceAt] == '\r')
		{
			cleaned[destAt++] = '\n';
			sourceAt++;;
		}
		else
		{
			cleaned[destAt++] = text[sourceAt++];
		}
	}

	cleaned[destAt] = '\0';
	realloc(cleaned, destAt + 1);

	return cleaned;
}

static void parse_thematic_break(sv_tokenizer* tokenizer, sv_token* token, const char breakCharacter)
{
	token->type = SV_TOKEN_THEMATIC_BREAK;

	uint32_t breakTokenCount = 1;
	uint32_t charCount = 1;

	while (tokenizer->at[charCount] != '\0' &&
		tokenizer->at[charCount] != '\n')
	{
		if (tokenizer->at[charCount] == breakCharacter)
		{
			breakTokenCount++;
			charCount++;
		}
		else if (tokenizer->at[charCount] == ' ')
		{
			charCount++;
		}
		else
		{
			token->type = SV_TOKEN_PARAGRAPH;
		}
	}

	if (breakTokenCount < 3)
	{
		token->type = SV_TOKEN_PARAGRAPH;
	}
	else
	{
		tokenizer->at += charCount + 1;
	}
}

static sv_token sv_get_token(sv_tokenizer* tokenizer)
{
	sv_token token = {0};
	token.length = 1;
	token.text = &tokenizer->at[0];

	switch(tokenizer->at[0])
	{
		case '\0': {token.type = SV_TOKEN_EOF;} break;
		case '#': //ATX headings
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
						if (tokenizer->at[0] == '\n')
						{
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
					token.type = SV_TOKEN_PARAGRAPH;
				}
			}
			else
			{
				token.type = SV_TOKEN_PARAGRAPH;
			}
		} break;
		case '*': //Thematic breaks
		{
			parse_thematic_break(tokenizer, &token, '*');
		} break;
		case '-':
		{
			parse_thematic_break(tokenizer, &token, '-');
		} break;
		case '_':
		{
			parse_thematic_break(tokenizer, &token, '_');
		} break;

		default: {} break;
	}

	return token;
}

extern char* sv_compile_ast(char* markdown)
{
	sv_tokenizer tokenizer = {0};
	char* cleaned = sv_normalize_line_endings(markdown);
	tokenizer.at = cleaned;

	bool parsing = true;

	char* result = "";

	while (parsing)
	{
		sv_token token = sv_get_token(&tokenizer);

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

			case SV_TOKEN_THEMATIC_BREAK: {printf("<hr />\n"); } break;
			case SV_TOKEN_PARAGRAPH: {printf("paragraph!\n"); } break;

			default:
			{

			} break;
		}
	}

	return result;
}
#endif
