/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <crtdbg.h>
#include <assert.h>
#include "b_file.h"
#include "b_unicode.h"
#include "b_config.h"
/*------------------------------------------------------------------------------------------------*/
/* bConfig_Fail                                                                                   */
/*------------------------------------------------------------------------------------------------*/
#define bConfig_Fail (bConfig_Value *)-1
/*------------------------------------------------------------------------------------------------*/
/* bConfig_TokenType                                                                              */
/*------------------------------------------------------------------------------------------------*/
typedef enum
{
	bConfig_UnknownToken,
	bConfig_SpaceToken  ,
	bConfig_LBraceToken ,
	bConfig_RBraceToken ,
	bConfig_NumberToken ,
	bConfig_FloatToken  ,
	bConfig_StringToken ,
	bConfig_NameToken   ,
	bConfig_EqualToken  ,
	bConfig_ErrorToken  ,
	bConfig_EofToken
}
bConfig_TokenType;
/*------------------------------------------------------------------------------------------------*/
/* bConfig_Memory                                                                                 */
/*------------------------------------------------------------------------------------------------*/
typedef struct
{
	bbyte *memory ;
	bbyte *current;
	bbyte *maximum;
}
bConfig_Memory;
/*------------------------------------------------------------------------------------------------*/
/* bConfig_Token                                                                                  */
/*------------------------------------------------------------------------------------------------*/
typedef struct bConfig_Token
{
	bConfig_TokenType     type ;
	bchar                *value;
	struct bConfig_Token *next ;
}
bConfig_Token;
/*------------------------------------------------------------------------------------------------*/
/* bConfig_ValuePot                                                                               */
/*------------------------------------------------------------------------------------------------*/
typedef union
{
	bint                  int_pot   ;
	bchar                *string_pot;
	bfloat                float_pot ;
	struct bConfig_Value *section   ;
}
bConfig_ValuePot;
/*------------------------------------------------------------------------------------------------*/
/* bConfig_ValueClass                                                                             */
/*------------------------------------------------------------------------------------------------*/
typedef enum
{
	bConfig_IntValue    ,
	bConfig_FloatValue  ,
	bConfig_StringValue ,
	bConfig_SectionValue
}
bConfig_ValueClass;
/*------------------------------------------------------------------------------------------------*/
/* bConfig_Value                                                                                  */
/*------------------------------------------------------------------------------------------------*/
typedef struct bConfig_Value
{
	bConfig_Token        *name_str ; /* name string    */
	bConfig_Token        *value_str; /* value string   */
	bConfig_ValuePot      value    ; /* item value     */
	bConfig_ValueClass    type     ; /* item type      */
	struct bConfig_Value *next     ; /* next value     */
}
bConfig_Value;
/*------------------------------------------------------------------------------------------------*/
/* bConfig_State                                                                                  */
/*------------------------------------------------------------------------------------------------*/
typedef enum
{
	bConfig_NameState ,
	bConfig_EqualState,
	bConfig_ValueState
}
bConfig_State;
/*------------------------------------------------------------------------------------------------*/
/* bConfig                                                                                        */
/*------------------------------------------------------------------------------------------------*/
struct bConfig
{
	bConfig_Token *token_list;
	bConfig_Value *value_list;
		
	bConfig_Memory memory;
};
/*------------------------------------------------------------------------------------------------*/
/* bConfig_Alloc - simple allocator                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid *bConfig_Alloc(bConfig *_config, buint _size);
/*------------------------------------------------------------------------------------------------*/
/* bConfig_GetToken - simple lexer                                                                */
/*------------------------------------------------------------------------------------------------*/
bConfig_Token *bConfig_GetToken(bchar **_text, bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
/* bConfig_Parser - simple parser                                                                 */
/*------------------------------------------------------------------------------------------------*/
bConfig_Value *bConfig_Parser(bchar **_text, bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
/* bConfig_CreateToken - helper function                                                          */
/*------------------------------------------------------------------------------------------------*/
bConfig_Token *bConfig_CreateToken(bchar *_ptr, bint _size, bConfig_TokenType _type, bConfig *_cfg);
/*------------------------------------------------------------------------------------------------*/
/* bConfig_FindValue - find value in current section                                              */
/*------------------------------------------------------------------------------------------------*/
bConfig_Value *bConfig_FindValue(bchar *_name, buint _size, bConfig_Value *_value);
/*------------------------------------------------------------------------------------------------*/
/* bConfig_Get - find value in all sections                                                       */
/*------------------------------------------------------------------------------------------------*/
bConfig_Value *bConfig_Get(bchar *_key, bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
/* bConfig_WriteToken - write single token in file                                                */
/*------------------------------------------------------------------------------------------------*/
bvoid bConfig_WriteToken(bConfig_Token *_token, bFile _file);
/*------------------------------------------------------------------------------------------------*/
/* bConfig_IsExternal - check pointer location                                                    */
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_IsExternal(bvoid *_ptr, bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
/* bConfig_FreeValue - free random alloced values                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid bConfig_FreeValue(bConfig_Value *_value, bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
/* bConfig_FreeToken - free random alloced tokens                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid bConfig_FreeToken(bConfig_Token *_token, bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
/* bConfig_EraseValue - free value content                                                        */
/*------------------------------------------------------------------------------------------------*/
bvoid bConfig_EraseValue(bConfig_Value *_value, bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
/* bConfig_Create                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bConfig *bConfig_Create(bchar *_name)
{
	bConfig *config    ;
	bchar   *text      ;
	bchar   *text_begin;
	buint    text_size ;

	config = malloc(sizeof(bConfig));

	/* read config from disk */
	text_begin = text = bFile_ReadText(_name);
	/* calculate config size. 32 is a minimum. */
	text_size = wcslen(text);
	text_size = (text_size > 32) ? text_size : 32;
	text_size =  text_size                              +
	             text_size      * sizeof(bConfig_Token) + 
		        (text_size / 4) * sizeof(bConfig_Value) ;
	/* prepare allocator */
	config->memory.memory  = malloc(text_size);
	config->memory.current = config->memory.memory;
	config->memory.maximum = config->memory.memory + text_size;
	/* init token list and parse config */
	config->token_list = 0;
	config->value_list = bConfig_Parser(&text, config);
	/* free config source */
	free(text_begin);

	if(config->value_list == bConfig_Fail)
	{
		free(config->memory.memory);
		free(config);

		config = 0;
	}

	return config;
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_Delete                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid bConfig_Delete(bchar *_name, bConfig *_config)
{
	bConfig_Save(_name, _config);

	/* free randomly alloced tokens */
	bConfig_FreeToken(_config->token_list, _config);

	/* free random alloced values */
	bConfig_FreeValue(_config->value_list, _config);

	/* free allocator memory woth all tokens and values */
	free(_config->memory.memory);
	free(_config);
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_FreeToken - free random alloced tokens                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid bConfig_FreeToken(bConfig_Token *_token, bConfig *_config)
{
	while(_token)
	{
		if(bConfig_IsExternal(_token->value, _config))
		{
			free(_token);
		}

		_token = _token->next;
	}
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_FreeValue                                                                              */
/*------------------------------------------------------------------------------------------------*/
bvoid bConfig_FreeValue(bConfig_Value *_value, bConfig *_config)
{
	while(_value)
	{
		/* free string values */
		if(_value->type == bConfig_StringValue)
		{
			if(bConfig_IsExternal(_value->value.string_pot, _config))
			{
				free(_value->value.string_pot);
			}
		}

		/* free nested sections */
		if(_value->type == bConfig_SectionValue)
		{
			bConfig_FreeValue(_value->value.section, _config);
		}

		_value = _value->next;
	}
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_Alloc                                                                                  */
/*------------------------------------------------------------------------------------------------*/
bvoid *bConfig_Alloc(bConfig *_config, buint _size)
{
	bbyte *new_block;

	/* remember block ptr */
	new_block = _config->memory.current;
	/* add current size */
	_config->memory.current += _size;
	/* check buffer overflow */
	assert(_config->memory.current < _config->memory.maximum);

	return new_block;
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_GetToken                                                                               */
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
		
		case bConfig_FloatToken:
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
				return bConfig_CreateToken(begin, ++*_text - begin, state, _config);
			}

			break;

		case bConfig_LBraceToken:
		case bConfig_RBraceToken:
		case bConfig_EqualToken:
			return bConfig_CreateToken(begin, *_text - begin, state, _config);

		default:
			return bConfig_CreateToken(begin, *_text - begin, bConfig_EofToken, _config);
		}
	}
	while(*((*_text += 1) - 1)); /* skip null character */

	return bConfig_CreateToken(0, 0, bConfig_ErrorToken, _config);
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_Parser                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bConfig_Value *bConfig_Parser(bchar **_text, bConfig *_config)
{
	buint          strpot_len;
	bConfig_Token *token;
	bConfig_Value *value      = 0;
	bConfig_State  state      = bConfig_NameState;
	bConfig_Value *value_list = 0;

	for(token = bConfig_GetToken(_text, _config); token->type != bConfig_EofToken;
		token = bConfig_GetToken(_text, _config))
	{
		token->next         = _config->token_list;
		_config->token_list = token;

		if(token->type == bConfig_RBraceToken) break;
		if(token->type == bConfig_SpaceToken)  continue;

		switch(state)
		{
		case bConfig_NameState:
			if(token->type != bConfig_NameToken) return bConfig_Fail;

			value                = bConfig_Alloc(_config, sizeof(bConfig_Value));
		    value->next          = value_list;
		    value_list           = value;
		    value->name_str      = token;

			state = bConfig_EqualState;
			break;

		case bConfig_EqualState:
			if(token->type != bConfig_EqualToken) return bConfig_Fail;

			state = bConfig_ValueState;
			break;

		case bConfig_ValueState:
			switch(token->type)
			{
			case bConfig_LBraceToken:
				value->value.section = bConfig_Parser(_text, _config);
				value->type          = bConfig_SectionValue;
				break;

			case bConfig_NumberToken:
				value->type          = bConfig_IntValue;
				value->value.int_pot = _wtoi(token->value);
				value->value_str     = token;
				break;

			case bConfig_FloatToken:
				value->type            = bConfig_FloatValue;
				value->value.float_pot = (bfloat)_wtof(token->value);
				value->value_str       = token;
				break;

			case bConfig_StringToken:
				value->type             = bConfig_StringValue;
				strpot_len              = (wcslen(token->value) - 2) * sizeof(bchar);
				value->value.string_pot = bConfig_Alloc(_config, strpot_len + sizeof(bchar));
				memcpy(value->value.string_pot, token->value + 1, strpot_len);
				value->value.string_pot[strpot_len / 2] = L'\0';
				value->value_str        = token;
				break;

			default:
				return bConfig_Fail;
			}

			state = bConfig_NameState;
			break;
		}
	}

	return value_list;
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_CreateToken                                                                            */
/*------------------------------------------------------------------------------------------------*/
bConfig_Token *bConfig_CreateToken(bchar *_ptr, bint _size, bConfig_TokenType _type, bConfig *_cfg)
{
	bConfig_Token *token;

	token = bConfig_Alloc(_cfg, sizeof(bConfig_Token));

	token->type = _type;

	/* add token value */
	if(_size > 0)
	{
		token->value = bConfig_Alloc(_cfg, _size * sizeof(bchar) + sizeof(bchar));
		wcsncpy(token->value, _ptr, _size);
		token->value[_size] = L'\0';
	}

	return token;
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_Save                                                                                   */
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_Save(bchar *_name, bConfig *_config)
{
	bFile file;

	file = bFile_Create(_name, btrue, bFile_WriteMode);
	/* write bom */
	bFile_Write(bUnicode_Utf16_Bom, bUnicode_Utf16_Bom_Size, 0, file);
	/* write token list from tail */
	bConfig_WriteToken(_config->token_list, file);

	bFile_Close(file);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_WriteToken                                                                             */
/*------------------------------------------------------------------------------------------------*/
bvoid bConfig_WriteToken(bConfig_Token *_token, bFile _file)
{
	/* write next token */
	if(_token->next)
		bConfig_WriteToken(_token->next, _file);
	/* then write this token */
	bFile_Write(_token->value, wcslen(_token->value) * sizeof(bchar), 0, _file);
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_FindValue                                                                              */
/*------------------------------------------------------------------------------------------------*/
bConfig_Value *bConfig_FindValue(bchar *_name, buint _size, bConfig_Value *_value)
{
	bConfig_Value *value = _value;

	while(value)
	{
		if(wcsncmp(value->name_str->value, _name, _size) == 0)
		{
			if(value->name_str->value[_size] == L'\0') 
				return value;
		}

		value = value->next;
	}

	return 0;
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_Get                                                                                    */
/*------------------------------------------------------------------------------------------------*/
bConfig_Value *bConfig_Get(bchar *_key,  bConfig *_config)
{
	buint i, k, len;
	bConfig_Value *value = _config->value_list;
	len                  = wcslen(_key) + 1;

	for(i = 0, k = 0; i < len; i++)
	{
		/* if -> found */
		if(_key[i] == L'-' && _key[i + 1] == L'>')
		{
			/* find this section */
			value = bConfig_FindValue(_key + k, ++i - 1 - k, value);
			
			if(!value) return 0;
			/* enter in this section */
			value = value->value.section;
			k     = i + 1;
		}

		/* if last value name found */
		if(_key[i] == L'\0')
		{
			return bConfig_FindValue(_key + k, i - k, value);
		}
	}

	return 0;
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_IsExternal                                                                             */
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_IsExternal(bvoid *_ptr, bConfig *_config)
{
	/* check _ptr location */
	return !(_ptr > (bvoid *)_config->memory.memory && _ptr < (bvoid *)_config->memory.maximum);
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_GetString                                                                              */
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_GetString(bchar **_value, bchar *_key, bConfig *_config)
{
	bConfig_Value *value;

	value = bConfig_Get(_key, _config);

	if(!value || value->type != bConfig_StringValue) return bfalse;

	*_value = _wcsdup(value->value.string_pot);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_GetInt                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_GetInt(bint   *_value, bchar *_key, bConfig *_config)
{
	bConfig_Value *value;

	value = bConfig_Get(_key, _config);

	if(!value || value->type != bConfig_IntValue) return bfalse;

	*_value = value->value.int_pot;

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_GetFloat                                                                               */
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_GetFloat(bfloat *_value, bchar *_key, bConfig *_config)
{
	bConfig_Value *value;

	value = bConfig_Get(_key, _config);

	if(!value || value->type != bConfig_FloatValue) return bfalse;

	*_value = value->value.float_pot;

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_GetBool                                                                                */
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_GetBool(bbool *_value, bchar *_key, bConfig *_config)
{
	bConfig_Value *value;

	value = bConfig_Get(_key, _config);

	if(!value || value->type != bConfig_IntValue) return bfalse;

	*_value = value->value.int_pot != 0;

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_EraseValue - free value content                                                        */
/*------------------------------------------------------------------------------------------------*/
bvoid bConfig_EraseValue(bConfig_Value *_value, bConfig *_config)
{
	if(bConfig_IsExternal(_value->value_str->value, _config))
	{
		free(_value->value_str->value);
	}

	if(_value->type == bConfig_StringValue)
	{
		if(bConfig_IsExternal(_value->value.string_pot, _config))
		{
			free(_value->value.string_pot);
		}
	}
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_SetString                                                                              */
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_SetString(bchar *_value, bchar *_key, bConfig *_config)
{
	bConfig_Value *value;
	buint          value_len;

	/* find key */
	value = bConfig_Get(_key, _config);

	if(!value) return bfalse;

	bConfig_EraseValue(value, _config);
    /* 3 = " + " + 0 */
	value_len               = wcslen(_value) + 3;
	value->value_str->value = malloc(value_len * sizeof(bchar));
	value->type             = bConfig_StringValue;
	value->value.string_pot = _wcsdup(_value);

	swprintf(value->value_str->value, value_len, L"\"%s\"", _value);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_SetInt                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_SetInt(bint _value, bchar *_key, bConfig *_config)
{
	bConfig_Value *value;

	value = bConfig_Get(_key, _config);

	if(!value) return bfalse;

	bConfig_EraseValue(value, _config);
    /* 32 - number string length */
	value->value_str->value = malloc(32 * sizeof(bchar));
	value->type             = bConfig_IntValue;
	value->value.int_pot    = _value;

	swprintf(value->value_str->value, 32, L"%d", _value);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_SetFloat                                                                               */
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_SetFloat(bfloat _value, bchar *_key, bConfig *_config)
{
	bConfig_Value *value;

	value = bConfig_Get(_key, _config);

	if(!value) return bfalse;

	bConfig_EraseValue(value, _config);
    /* 32 - float string length */
	value->value_str->value = malloc(32 * sizeof(bchar));
	value->type             = bConfig_FloatValue;
	value->value.float_pot  = _value;

	swprintf(value->value_str->value, 32, L"%g", _value);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bConfig_SetBool                                                                                */
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_SetBool(bbool _value, bchar *_key, bConfig *_config)
{
	bConfig_Value *value;

	value = bConfig_Get(_key, _config);

	if(!value) return bfalse;

	bConfig_EraseValue(value, _config);
    /* 2 - bool string length */
	value->value_str->value = malloc(2 * sizeof(bchar));
	value->type             = bConfig_IntValue;
	value->value.int_pot    = _value;

	swprintf(value->value_str->value, 2, L"%d", _value != 0);

	return btrue;
}