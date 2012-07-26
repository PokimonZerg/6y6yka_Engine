/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "b_file.h"
#include "b_unicode.h"
#include "b_config.h"
/*------------------------------------------------------------------------------------------------*/
typedef enum
{
	bConfig_UnknownToken,
	bConfig_SpaceToken,
	bConfig_LBraceToken,
	bConfig_RBraceToken,
	bConfig_NumberToken,
	bConfig_FloatToken,
	bConfig_StringToken,
	bConfig_NameToken,
	bConfig_EqualToken,
	bConfig_ErrorToken,
	bConfig_EofToken
}
bConfig_TokenType;
/*------------------------------------------------------------------------------------------------*/
typedef struct
{
	bbyte *memory;
	bbyte *current;
	bbyte *maximum;
}
bConfig_Memory;
/*------------------------------------------------------------------------------------------------*/
typedef struct bConfig_Token
{
	bConfig_TokenType     type ;
	bchar                *value;
	struct bConfig_Token *next ;
}
bConfig_Token;
/*------------------------------------------------------------------------------------------------*/
typedef struct
{
	bConfig_Token *token;
	buint          line ;
	bchar         *text ;
}
bConfig_GetResult;
/*------------------------------------------------------------------------------------------------*/
typedef union
{
	bint   int_pot   ;
	bchar *string_pot;
	bfloat float_pot ;
	bbool  bool_pot  ;
	struct bConfig_Value *section;
}
bConfig_ValuePot;
/*------------------------------------------------------------------------------------------------*/
typedef enum
{
	bConfig_IntValue,
	bConfig_FloatValue,
	bConfig_StringValue,
	bConfig_BoolValue,
	bConfig_SectionValue
}
bConfig_ValueClass;
/*------------------------------------------------------------------------------------------------*/
typedef struct bConfig_Value
{
	bConfig_Token        *name_str;
	bConfig_Token        *value_str;
	bConfig_ValuePot      value;
	bConfig_ValueClass    type ;
	struct bConfig_Value *next ;
}
bConfig_Value;
/*------------------------------------------------------------------------------------------------*/
typedef enum
{
	bConfig_NameState,
	bConfig_EqualState,
	bConfig_ValueState
}
bConfig_State;
/*------------------------------------------------------------------------------------------------*/
struct bConfig
{
	bConfig_Token *token_list;
	bConfig_Value *value_list;
		
	bConfig_Memory memory;
};
/*------------------------------------------------------------------------------------------------*/
bvoid *bConfig_Alloc(bConfig_Memory *_memory, buint _size);
/*------------------------------------------------------------------------------------------------*/
bConfig_Token *bConfig_GetToken(bchar **_text, bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
bConfig_Value *bConfig_Parser(bchar **_text, bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
bConfig_Token *bConfig_CreateToken(bchar *_ptr, bint _size, bConfig_TokenType _type, bConfig *_cfg);
/*------------------------------------------------------------------------------------------------*/
bConfig_Value *bConfig_FindValue(const bchar *_name, buint _size, bConfig_Value *_value);
/*------------------------------------------------------------------------------------------------*/
bConfig_Value *bConfig_Get(const bchar *_key, const bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
bvoid bConfig_WriteToken(bConfig_Token *_token, bFile *_file);
/*------------------------------------------------------------------------------------------------*/
bConfig *bConfig_Load(const bchar *_name)
{
	bConfig *config;
	bchar   *text;
	buint    text_size;

	config = malloc(sizeof(bConfig));

	text      = bFile_ReadText(_name);
	text_size = wcslen(text);
	
	config->memory.memory  = malloc(text_size * sizeof(bConfig_Value));
	config->memory.current = config->memory.memory;
	config->memory.maximum = config->memory.memory + text_size * sizeof(bConfig_Value);
	config->token_list     = 0;
	config->value_list     = bConfig_Parser(&text, config);

	return config;
}
/*------------------------------------------------------------------------------------------------*/
bvoid bConfig_Unload(bchar *_name, bConfig *_config)
{
	free(_config);
}
/*------------------------------------------------------------------------------------------------*/
bvoid *bConfig_Alloc(bConfig_Memory *_memory, buint _size)
{
	bbyte *new_block;

	new_block = _memory->current;
	
	_memory->current += _size;
	assert(_memory->current < _memory->maximum);

	return new_block;
}
/*------------------------------------------------------------------------------------------------*/
bConfig_Token *bConfig_GetToken(bchar **_text, bConfig *_config)
{
	bConfig_TokenType state = bConfig_UnknownToken;
	bchar            *begin = *_text;

	do
	{
		switch(state)
		{
		case bConfig_UnknownToken:	
			if(iswalpha(**_text) || **_text == L'_')
			{
				state = bConfig_NameToken;
				break;
			}

			if(iswdigit(**_text) || **_text == L'+' || **_text == L'-')
			{
				state = bConfig_NumberToken;
				break;
			}
			
			if(iswspace(**_text) && **_text != '\n')
			{
				state = bConfig_SpaceToken;
				break;
			}

			switch(**_text)
			{
			case L'"':
				state = bConfig_StringToken;
				break;

			case L'{':
				state = bConfig_LBraceToken;
				break;

			case L'}':
				state = bConfig_RBraceToken;
				break;

			case L'=':
				state = bConfig_EqualToken;
				break;

			case L'\0':
				return bConfig_CreateToken(begin, *_text - begin, bConfig_EofToken, _config);

			default:
				state = bConfig_EofToken;
				break;
			}

			break;

		case bConfig_NameToken:
			if(!iswalpha(**_text) && **_text != L'_')
			{
				return bConfig_CreateToken(begin, *_text - begin, state, _config);
			}

			break;

		case bConfig_NumberToken:
			if(**_text == '.')
			{
				state = bConfig_FloatToken;
				break;
			}

			if(!iswdigit(**_text))
			{
				return bConfig_CreateToken(begin, *_text - begin, state, _config);
			}

			break;

		case bConfig_SpaceToken:
			if(!iswspace(**_text))
			{
				return bConfig_CreateToken(begin, *_text - begin, state, _config);
			}

			break;

		case bConfig_StringToken:
			if(**_text == '"')
			{
				return bConfig_CreateToken(begin, *_text++ - begin, state, _config);
			}

			break;

		case bConfig_LBraceToken:
		case bConfig_RBraceToken:
		case bConfig_EqualToken:
		case bConfig_EofToken:
			return bConfig_CreateToken(begin, *_text - begin, state, _config);

		default:
			return bConfig_CreateToken(begin, *_text - begin, bConfig_EofToken, _config);
		}
	}
	while(*((*_text += 1) - 1)); /* skip null character */

	return bConfig_CreateToken(0, 0, bConfig_ErrorToken, _config);
}
/*------------------------------------------------------------------------------------------------*/
bConfig_Value *bConfig_Parser(bchar **_text, bConfig *_config)
{
	bConfig_Token *token;
	bConfig_Value *value = 0;
	bConfig_State  state = bConfig_NameState;
	bConfig_Value *value_list = 0;

	for(token = bConfig_GetToken(_text, _config);
		token->type != bConfig_RBraceToken && token->type != bConfig_EofToken;
		token = bConfig_GetToken(_text, _config))
	{
		token->next         = _config->token_list;
		_config->token_list = token;

		if(token->type == bConfig_SpaceToken) continue;

		switch(state)
		{
		case bConfig_NameState:
			if(token->type != bConfig_NameToken) return 0;

			value                = bConfig_Alloc(&_config->memory, sizeof(bConfig_Value));
		    value->next          = value_list;
		    value_list           = value;
		    value->name_str      = token;

			state = bConfig_EqualState;
			break;

		case bConfig_EqualState:
			if(token->type != bConfig_EqualToken) return 0;

			state = bConfig_ValueState;
			break;

		case bConfig_ValueState:
			switch(token->type)
			{
			case bConfig_LBraceToken:
				token                = bConfig_GetToken(_text, _config);
				token->next          = _config->token_list;
		        _config->token_list  = token;
				value->value.section = bConfig_Parser(_text, _config);
				value->type          = bConfig_SectionValue;
				break;

			case bConfig_NumberToken:
				value->type          = bConfig_IntValue;
				value->value.int_pot = _wtoi(token->value);
				value->value_str     = token;
				break;

			default:
				return 0;
			}

			state = bConfig_NameState;
			break;
		}
	}
	if(token->type == bConfig_RBraceToken)
	{
		token->next          = _config->token_list;
		_config->token_list  = token;
	}

	return value_list;
}
/*------------------------------------------------------------------------------------------------*/
bConfig_Token *bConfig_CreateToken(bchar *_ptr, bint _size, bConfig_TokenType _type, bConfig *_cfg)
{
	bConfig_Token *token;

	token = bConfig_Alloc(&_cfg->memory, sizeof(bConfig_Token));

	token->type = _type;

	if(_size > 0)
	{
		token->value = bConfig_Alloc(&_cfg->memory, _size * sizeof(bchar) + sizeof(bchar));
		wcsncpy(token->value, _ptr, _size);
		token->value[_size] = L'\0';
	}

	return token;
}
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_Save(bchar *_name, bConfig *_config)
{
	bFile *file;

	file = bFile_Create(_name, btrue, bfalse, btrue);

	bFile_Write(bUnicode_Utf16_Bom, bUnicode_Utf16_Bom_Size, file);

	bConfig_WriteToken(_config->token_list, file);

	bFile_Close(file);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
bvoid bConfig_WriteToken(bConfig_Token *_token, bFile *_file)
{
	if(_token->next)
		bConfig_WriteToken(_token->next, _file);

	bFile_Write(_token->value, wcslen(_token->value) * sizeof(bchar), _file);
}
/*------------------------------------------------------------------------------------------------*/
bConfig_Value *bConfig_FindValue(const bchar *_name, buint _size, bConfig_Value *_value)
{
	bConfig_Value *value = _value;

	while(value)
	{
		if(wcsncmp(value->name_str->value, _name, _size) == 0)
		{
			if(value->name_str->value[_size] == L'\0') return value;
		}
	}

	return 0;
}
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_GetString(bchar **_value, const bchar *_key, const bConfig *_config);
bbool bConfig_GetInt   (bint   *_value, const bchar *_key, const bConfig *_config)
{
	buint i, k, len;
	bConfig_Value *value = _config->value_list;

	len = wcslen(_key) + 1;

	for(i = 0, k = 0; i < len; i++)
	{
		if(_key[i] == L'-' && _key[i + 1] == L'>')
		{
			value = bConfig_FindValue(_key + k, ++i - 1 - k, value);
			value = value->value.section;

			if(!value) return bfalse;

			k = i + 1;
		}

		if(_key[i] == L'\0')
		{
			value = bConfig_FindValue(_key + k, i - k, value);

			if(!value) return bfalse;
			if(value->type != bConfig_IntValue) return bfalse;

			*_value = value->value.int_pot;
			return btrue;
		}
	}

	return bfalse;
}
bbool bConfig_GetFloat (bfloat *_value, const bchar *_key, const bConfig *_config);
bbool bConfig_GetBool  (bbool  *_value, const bchar *_key, const bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_SetString(bchar *_value, const bchar *_key, const bConfig *_config);
bbool bConfig_SetInt   (bint   _value, const bchar *_key, const bConfig *_config);
bbool bConfig_SetFloat (bfloat _value, const bchar *_key, const bConfig *_config);
bbool bConfig_SetBool  (bbool  _value, const bchar *_key, const bConfig *_config);