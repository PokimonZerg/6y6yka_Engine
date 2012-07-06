/*----------------------------------------------------------------------------*/
/*                              6y6yka Engine                                 */
/*----------------------------------------------------------------------------*/
#include <stdlib.h>
#include <ctype.h>
#include "b_assert.h"
#include "b_lisp.h"
/*----------------------------------------------------------------------------*/
#define bLisp_NumDebugTokens 5
/*----------------------------------------------------------------------------*/
/* bLisp_TypeClass                                                            */
/*----------------------------------------------------------------------------*/
enum bLisp_TypeClass
{
	bLisp_NumberType,
	bLisp_StringType,
	bLisp_DataType
};

typedef enum bLisp_TypeClass bLisp_TypeClass;
/*----------------------------------------------------------------------------*/
/* bLisp_TypeValue                                                            */
/*----------------------------------------------------------------------------*/
union bLisp_TypeValue
{
	bfloat  number;
	bchar  *string;
	bvoid  *data  ;
};

typedef union bLisp_TypeValue bLisp_TypeValue;
/*----------------------------------------------------------------------------*/
/* bLisp_Type                                                                 */
/*----------------------------------------------------------------------------*/
struct bList_Type
{
	bLisp_TypeClass type ;
	bLisp_TypeValue value;
};

typedef struct bLisp_Type bLisp_Type;
/*----------------------------------------------------------------------------*/
/* bLisp_Stack                                                                */
/*----------------------------------------------------------------------------*/
struct bLisp_Stack
{
	bLisp_Type *data;
};

typedef struct bLisp_Stack bLisp_Stack;
/*----------------------------------------------------------------------------*/
/* bLisp_CodeClass                                                            */
/*----------------------------------------------------------------------------*/
enum bLisp_CodeClass
{
	bLisp_PushConst_Code,
	bLisp_PushVar_Code,
	bLisp_PushFunc_Code,
	bLisp_Add_Code,
	bLisp_Sub_Code,
	bLisp_Mul_Code,
	bLisp_Div_Code
};

typedef enum bLisp_CodeClass bLisp_CodeClass;
/*----------------------------------------------------------------------------*/
/* bLisp_CodeValue                                                            */
/*----------------------------------------------------------------------------*/
union bLisp_CodeValue
{
	bLisp_Type *value;
};

typedef union bLisp_CodeValue bLisp_CodeValue;
/*----------------------------------------------------------------------------*/
/* bLisp_Code                                                                 */
/*----------------------------------------------------------------------------*/
struct bLisp_Code
{
	bLisp_CodeClass type;
	bLisp_CodeValue value;
};

typedef struct bLisp_Code bLisp_Code;
/*----------------------------------------------------------------------------*/
/* bLisp_TokenClass                                                           */
/*----------------------------------------------------------------------------*/
enum bLisp_TokenClass
{
	bLisp_NameToken,    /* variable name  */
	bLisp_DigitToken,   /* integer        */
	bLisp_FloatToken,   /* float          */
	bLisp_NumberToken,  /* number         */
	bLisp_StringToken,  /* unicode string */
	bLisp_CommentToken, /* pure comment   */
	bLisp_AddToken,     /* +              */
	bLisp_SubToken,     /* -              */
	bLisp_MulToken,     /* *              */
	bLisp_DivToken,     /* /              */
	bLisp_IncToken,     /* ++             */
	bLisp_DecToken,     /* --             */
	bLisp_EqualToken,   /* =              */
	bLisp_NEqualToken,  /* !=             */
	bLisp_NotToken,     /* !              */
	bLisp_LessToken,    /* <              */
	bLisp_MoreToken,    /* >              */
	bLisp_LBraceToken,  /* (              */
	bLisp_RBraceToken,  /* )              */
	bLisp_BindToken,    /* bind           */
	bLisp_IfToken,      /* if             */
	bLisp_SetToken,     /* set            */
	bLisp_GetToken,     /* get            */
	bLisp_LetToken,     /* let            */
	bLisp_LambdaToken,  /* lambda         */
	bLisp_ListToken,    /* list           */
	bLisp_CarToken,     /* car            */
	bLisp_CdrToken,     /* cdr            */
	bLisp_ConsToken,    /* cons           */
	bLisp_BeginToken,   /* begin          */
	bLisp_WhileToken,   /* while          */
	bLisp_SharpToken,   /* #              */
	bLisp_IncludeToken, /* include        */
	bLisp_LineToken,    /* new line       */
	bLisp_SpaceToken,   /* any space      */
	bLisp_ErrorToken,   /* lexer error    */
	bLisp_EofToken      /* service        */
};

typedef enum bLisp_TokenClass bLisp_TokenClass;
/*----------------------------------------------------------------------------*/
/* bLisp_TokenValue                                                           */
/*----------------------------------------------------------------------------*/
union bLisp_TokenValue
{
	bfloat  number; 
	bchar  *string;
	bchar  *name;
};

typedef union bLisp_TokenValue bLisp_TokenValue;
/*----------------------------------------------------------------------------*/
/* bLisp_Token                                                                */
/*----------------------------------------------------------------------------*/
struct bLisp_Token
{
	bLisp_TokenClass type ;
	bLisp_TokenValue value;

	buint            line ;
	bchar           *file ;
};

typedef struct bLisp_Token bLisp_Token;
/*----------------------------------------------------------------------------*/
/* bLisp_TokenPos                                                             */
/*----------------------------------------------------------------------------*/
struct bLisp_TokenPos
{
	bchar           *begin;
	bchar           *end  ;

	bLisp_TokenClass type ;
};

typedef struct bLisp_TokenPos bLisp_TokenPos;
/*----------------------------------------------------------------------------*/
/* bLisp_TokenClassList                                                       */
/*----------------------------------------------------------------------------*/
typedef struct bLisp_TokenClassList bLisp_TokenClassList;

struct bLisp_TokenClassList
{
	bLisp_TokenClass      type;
	bLisp_TokenClassList *next;
};
/*----------------------------------------------------------------------------*/
/* bLisp_TokenCircle                                                          */
/*----------------------------------------------------------------------------*/
struct bLisp_TokenCircle
{
	bLisp_TokenClassList circle[bLisp_NumDebugTokens];
	buint                pos;
};

typedef struct bLisp_TokenCircle bLisp_TokenCircle;
/*----------------------------------------------------------------------------*/
/* bLisp_LexerState                                                           */
/*----------------------------------------------------------------------------*/
enum bLisp_LexerState
{
	bLisp_Unknown_Lexer_State,
	bLisp_Name_Lexer_State,
	bLisp_PlusPlus_Lexer_State,
	bLisp_MinusMinus_Lexer_State,
	bLisp_Digit_Lexer_State,
	bLisp_Float_Lexer_State,
	bLisp_String_Lexer_State,
	bLisp_Comment_Lexer_State,
	bLisp_Slash_Lexer_State,
	bLisp_Add_Lexer_State,
	bLisp_Sub_Lexer_State,
	bLisp_Mul_Lexer_State,
	bLisp_I_Lexer_State,
	bLisp_If_Lexer_State,
	bLisp_LBrace_Lexer_State,
	bLisp_RBrace_Lexer_State,
	bLisp_Equal_Lexer_State,
	bLisp_Less_Lexer_State,
	bLisp_More_Lexer_State,
	bLisp_S_Lexer_State,
	bLisp_Se_Lexer_State,
	bLisp_Set_Lexer_State,
	bLisp_B_Lexer_State,
	bLisp_Be_Lexer_State,
	bLisp_Beg_Lexer_State,
	bLisp_Begi_Lexer_State,
	bLisp_Begin_Lexer_State,
	bLisp_C_Lexer_State,
	bLisp_Ca_Lexer_State,
	bLisp_Car_Lexer_State,
	bLisp_Cd_Lexer_State,
	bLisp_Cdr_Lexer_State,
	bLisp_Co_Lexer_State,
	bLisp_Con_Lexer_State,
	bLisp_Cons_Lexer_State,
	bLisp_L_Lexer_State,
	bLisp_Li_Lexer_State,
	bLisp_Lis_Lexer_State,
	bLisp_List_Lexer_State,
	bLisp_Le_Lexer_State,
	bLisp_Let_Lexer_State,
	bLisp_G_Lexer_State,
	bLisp_Ge_Lexer_State,
	bLisp_Get_Lexer_State,
	bLisp_Sharp_Lexer_State,
	bLisp_In_Lexer_State,
	bLisp_Inc_Lexer_State,
	bLisp_Incl_Lexer_State,
	bLisp_Inclu_Lexer_State,
	bLisp_Includ_Lexer_State,
	bLisp_Include_Lexer_State,
	bLisp_W_Lexer_State,
	bLisp_Wh_Lexer_State,
	bLisp_Whi_Lexer_State,
	bLisp_Whil_Lexer_State,
	bLisp_While_Lexer_State,
	bLisp_Bi_Lexer_State,
	bLisp_Bin_Lexer_State,
	bLisp_Bind_Lexer_State,
	bLisp_La_Lexer_State,
	bLisp_Lam_Lexer_State,
	bLisp_Lamb_Lexer_State,
	bLisp_Lambd_Lexer_State,
	bLisp_Lambda_Lexer_State,
	bLisp_NewLine_Lexer_State,
	bLisp_Space_Lexer_State,
	bLisp_NEqual_Lexer_State,
	bLisp_Not_Lexer_State,
	bLisp_EndString_Lexer_State
};

typedef enum bLisp_LexerState bLisp_LexerState;
/*----------------------------------------------------------------------------*/
/* bLisp_Include                                                              */
/*----------------------------------------------------------------------------*/
typedef struct bLisp_Include bLisp_Include;

struct bLisp_Include
{
	bLisp_Include *next;

	bchar *source;
	bchar *pos;

	bchar *file;
	buint  line;

	bbool complete;
};
/*----------------------------------------------------------------------------*/
/*  bLisp_Source                                                              */
/*----------------------------------------------------------------------------*/
struct bLisp_Source
{
	bLisp_Include *include;
	bLisp_Include *current_include;
};

typedef struct bLisp_Source bLisp_Source;
/*----------------------------------------------------------------------------*/
/* bLisp_CompileData                                                          */
/*----------------------------------------------------------------------------*/
struct bLisp_CompileData
{
	bLisp_Source     source;
	bchar           *last_error;
};

typedef struct bLisp_CompileData bLisp_CompileData;
/*----------------------------------------------------------------------------*/
/* bLisp_Script                                                               */
/*----------------------------------------------------------------------------*/
struct bLisp_Script
{
	bLisp_Stack stack;
};
/*----------------------------------------------------------------------------*/
/* bLisp_IdentNextToken                                                       */
/*----------------------------------------------------------------------------*/
bLisp_TokenPos bLisp_IdentNextToken(bchar *_code);
/*----------------------------------------------------------------------------*/
/* bLisp_InitSource                                                           */
/*----------------------------------------------------------------------------*/
bbool bLisp_InitSource(bchar *_code, bLisp_Source *_source);
/*----------------------------------------------------------------------------*/
/* bLisp_AddInclude                                                           */
/*----------------------------------------------------------------------------*/
bbool bLisp_AddInclude(bchar *_code, bchar *_file, bLisp_Source *_source);
/*----------------------------------------------------------------------------*/
/* bLisp_AllocInclude                                                         */
/*----------------------------------------------------------------------------*/
bLisp_Include *bLisp_AllocInclude(bchar *_code, bchar *_file);
/*----------------------------------------------------------------------------*/
/* bLisp_GetNewInclude                                                        */
/*----------------------------------------------------------------------------*/
bLisp_Include *bLisp_GetNewInclude(bLisp_Source *_source);
/*----------------------------------------------------------------------------*/
/* bLisp_FreeSource                                                           */
/*----------------------------------------------------------------------------*/
bvoid bLisp_FreeSource(bLisp_Source *_source);
/*----------------------------------------------------------------------------*/
/* bLisp_BuildTokenPos                                                        */
/*----------------------------------------------------------------------------*/
bLisp_TokenPos bLisp_BuildTokenPos(bchar *_begin, bchar *_end,
								   bLisp_TokenClass _type);
/*----------------------------------------------------------------------------*/
/* bLisp_SqueezeToken                                                         */
/*----------------------------------------------------------------------------*/
bbool bLisp_SqueezeToken(bLisp_TokenPos *_data, bLisp_Token *_token,
						 bLisp_Include *_include);
/*----------------------------------------------------------------------------*/
/* bLisp_GetNextToken                                                         */
/*----------------------------------------------------------------------------*/
bLisp_Token bLisp_GetNextToken(bLisp_Source *_source);
/*----------------------------------------------------------------------------*/
/* bLisp_OpenScript                                                           */
/*----------------------------------------------------------------------------*/
bLisp_Script *bLisp_OpenScript(bchar *_text)
{
	bLisp_Script *script;

	bAssert(_text);

	script = malloc(sizeof(bLisp_Script));

	return script;
}
/*----------------------------------------------------------------------------*/
/* bLisp_GetNextToken                                                         */
/*----------------------------------------------------------------------------*/
bLisp_Token bLisp_GetNextToken(bLisp_Source *_source)
{
	bLisp_Token    token;
	bLisp_TokenPos token_pos;
	bLisp_Include *include;

	include      = _source->current_include;
	token_pos    = bLisp_IdentNextToken(include->pos);
	token.type   = token_pos.type;
	token.line   = include->line;
	token.file   = include->file;
	include->pos = token_pos.end;

	switch(token.type)
	{
	case bLisp_ErrorToken:
		return token;

	case bLisp_SharpToken:
		/* preprocessor */
		break;

	case bLisp_EofToken:
		include->complete = btrue;
		include = bLisp_GetNewInclude(_source);
		if(include)
			return bLisp_GetNextToken(_source);
		break;

	case bLisp_LineToken:
		include->line++;
		break;

	case bLisp_SpaceToken:
		return bLisp_GetNextToken(_source);

	default:
		bLisp_SqueezeToken(&token_pos, &token, include);
		break;
	}	

	return token;
}
/*----------------------------------------------------------------------------*/
/* bLisp_GetNewInclude                                                        */
/*----------------------------------------------------------------------------*/
bLisp_Include *bLisp_GetNewInclude(bLisp_Source *_source)
{
	bLisp_Include *include = _source->current_include;

	do
	{
		include = include->next;

		if(! include->complete)
		{
			_source->current_include = include;
			return include;
		}
	} while(include->next);

	return 0;
}
/*----------------------------------------------------------------------------*/
/* bLisp_SqueezeToken                                                         */
/*----------------------------------------------------------------------------*/
bbool bLisp_SqueezeToken(bLisp_TokenPos *_data, bLisp_Token *_token,
						 bLisp_Include *_include)
{
	buint size;

	switch(_token->type)
	{
	case bLisp_DigitToken:
		_token->value.number = (bfloat)wcstol(_data->begin, 0, 10);
		_token->type = bLisp_NumberToken;
		break;

	case bLisp_FloatToken:
		_token->value.number = (bfloat)wcstod(_data->begin, 0);
		_token->type = bLisp_NumberToken;
		break;

	case bLisp_StringToken:
		size = (_data->end - _data->begin) * sizeof(bchar) - (sizeof(L'"') * 2)
			                               + sizeof(L'\0');
		_token->value.string = malloc(size);
		memcpy(_token->value.string, _data->begin + 1, size);
		_token->value.string[(size - sizeof(bchar)) / sizeof(bchar)] = L'\0';
		break;

	case bLisp_NameToken:
		size = (_data->end - _data->begin) * sizeof(bchar) + sizeof(L'\0');
		_token->value.string = malloc(size);
		memcpy(_token->value.string, _data->begin, size);
		_token->value.string[(size - sizeof(bchar)) / sizeof(bchar)] = L'\0';
		break;
	}

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_InitSource                                                           */
/*----------------------------------------------------------------------------*/
bbool bLisp_InitSource(bchar *_code, bLisp_Source *_source)
{
	_source->include         = bLisp_AllocInclude(_code, L"main script");
	_source->current_include = _source->include;

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_AddInclude                                                           */
/*----------------------------------------------------------------------------*/
bbool bLisp_AddInclude(bchar *_code, bchar *_file, bLisp_Source *_source)
{
	bLisp_Include *temp      = _source->include;

	_source->include         = bLisp_AllocInclude(_code, _file);
	_source->include->next   = temp;
	_source->current_include = _source->include;

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_AllocInclude                                                         */
/*----------------------------------------------------------------------------*/
bLisp_Include *bLisp_AllocInclude(bchar *_code, bchar *_file)
{
	bLisp_Include *first_include;

	first_include            = malloc(sizeof(bLisp_Include));
	first_include->next      = 0;
	first_include->source    = _code;
	first_include->pos       = _code;
	first_include->file      = _file;
	first_include->line      = 1;
	first_include->complete  = bfalse;

	return first_include;
}
/*----------------------------------------------------------------------------*/
/* bLisp_FreeSource                                                           */
/*----------------------------------------------------------------------------*/
bvoid bLisp_FreeSource(bLisp_Source *_source)
{
	bLisp_Include *temp;

	while(_source->include)
	{
		temp = _source->include->next;

		free(_source->include);

		_source->include = temp;
	}
}
/*----------------------------------------------------------------------------*/
/* bLisp_IdentNextToken                                                       */
/*----------------------------------------------------------------------------*/
bLisp_TokenPos bLisp_IdentNextToken(bchar *_code)
{
	bLisp_LexerState state;
	bchar           *begin;

	begin = _code;
	state = bLisp_Unknown_Lexer_State;

	do
	{
		switch(state) /* epic switch */
		{
		case bLisp_Unknown_Lexer_State:
			if(iswdigit(*_code))
			{
				state = bLisp_Digit_Lexer_State;
				break;
			}

			if(iswalpha(*_code))
			{
				state = bLisp_Name_Lexer_State;
			}

			switch(*_code)
			{
			case L'\0':
				return bLisp_BuildTokenPos(begin, _code, bLisp_EofToken);

			case L'\n':
				state = bLisp_NewLine_Lexer_State;
				break;

			case L'-':
				state = bLisp_Sub_Lexer_State;
				break;

			case L'+':
				state = bLisp_Add_Lexer_State;
				break;

			case L'*':
				state = bLisp_Mul_Lexer_State;
				break;

			case L'/':
				state = bLisp_Slash_Lexer_State;
				break;

			case L'"':
				state = bLisp_String_Lexer_State;
				break;

			case L'(':
				state = bLisp_LBrace_Lexer_State;
				break;

			case L')':
				state = bLisp_RBrace_Lexer_State;
				break;

			case L'#':
				state = bLisp_Sharp_Lexer_State;
				break;

			case L'=':
				state = bLisp_Equal_Lexer_State;
				break;

			case L'!':
				state = bLisp_Not_Lexer_State;
				break;

			case L'<':
				state = bLisp_Less_Lexer_State;
				break;

			case L'>':
				state = bLisp_More_Lexer_State;
				break;

			case L'i':
				state = bLisp_I_Lexer_State;
				break;

			case L'b':
				state = bLisp_B_Lexer_State;
				break;

			case L'l':
				state = bLisp_L_Lexer_State;
				break;

			case L's':
				state = bLisp_S_Lexer_State;
				break;

			case L'w':
				state = bLisp_W_Lexer_State;
				break;

			case L'g':
				state = bLisp_G_Lexer_State;
				break;

			case L'c':
				state = bLisp_C_Lexer_State;
				break;

			case L' ':
			case L'\t':
				state = bLisp_Space_Lexer_State;
				break;

			default:
				break;
			}
			
			break;

		case bLisp_S_Lexer_State:
			if(*_code == L'e')
				state = bLisp_Se_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Se_Lexer_State:
			if(*_code == L't')
				state = bLisp_Set_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Set_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_SetToken);

		case bLisp_Not_Lexer_State:
			if(*_code == L'=')
			{
				state = bLisp_NEqual_Lexer_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_NotToken);

		case bLisp_NEqual_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_NEqualToken);

		case bLisp_NewLine_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_LineToken);

		case bLisp_Space_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_SpaceToken);

		case bLisp_L_Lexer_State:
			if(*_code == L'e')
				state = bLisp_Le_Lexer_State;
			else if(*_code == L'i')
				state = bLisp_Li_Lexer_State;
			else if(*_code == L'a')
				state = bLisp_La_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_La_Lexer_State:
			if(*_code == L'm')
				state = bLisp_Lam_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Lam_Lexer_State:
			if(*_code == L'b')
				state = bLisp_Lamb_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Lamb_Lexer_State:
			if(*_code == L'd')
				state = bLisp_Lambd_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Lambd_Lexer_State:
			if(*_code == L'a')
				state = bLisp_Lambda_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Lambda_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_LambdaToken);
			}

		case bLisp_Li_Lexer_State:
			if(*_code == L's')
			{
				state = bLisp_Lis_Lexer_State;
				break;
			}
			else if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else 
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			}

		case bLisp_Lis_Lexer_State:
			if(*_code == L't')
				state = bLisp_List_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_List_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_ListToken);

		case bLisp_Le_Lexer_State:
			if(*_code == L't')
				state = bLisp_Let_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Let_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_LetToken);

		case bLisp_PlusPlus_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_IncToken);

		case bLisp_MinusMinus_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_DecToken);

		case bLisp_C_Lexer_State:
			if(*_code == L'a')
				state = bLisp_Ca_Lexer_State;
			else if(*_code == L'd')
				state = bLisp_Cd_Lexer_State;
			else if(*_code == L'o')
				state = bLisp_Co_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Car_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_CarToken);

		case bLisp_Cdr_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_CdrToken);

		case bLisp_Cons_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_ConsToken);

		case bLisp_Ca_Lexer_State:
			if(*_code == L'r')
				state = bLisp_Car_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Cd_Lexer_State:
			if(*_code == L'r')
				state = bLisp_Cdr_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Co_Lexer_State:
			if(*_code == L'n')
				state = bLisp_Con_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Con_Lexer_State:
			if(*_code == L's')
				state = bLisp_Cons_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_In_Lexer_State:
			if(*_code == L'c')
				state = bLisp_Inc_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Inc_Lexer_State:
			if(*_code == L'l')
				state = bLisp_Incl_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Incl_Lexer_State:
			if(*_code == L'u')
				state = bLisp_Inclu_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Inclu_Lexer_State:
			if(*_code == L'd')
				state = bLisp_Includ_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Includ_Lexer_State:
			if(*_code == L'e')
				state = bLisp_Include_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Include_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_IncludeToken);

		case bLisp_Sharp_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_SharpToken);

		case bLisp_Comment_Lexer_State:
			if(*_code == L'\0')
				return bLisp_BuildTokenPos(begin, _code, bLisp_CommentToken);
			break;

		case bLisp_W_Lexer_State:
			if(*_code == L'h')
				state = bLisp_Wh_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Wh_Lexer_State:
			if(*_code == L'i')
				state = bLisp_Whi_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Whi_Lexer_State:
			if(*_code == L'l')
				state = bLisp_Whil_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Whil_Lexer_State:
			if(*_code == L'e')
				state = bLisp_While_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_While_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_WhileToken);

		case bLisp_G_Lexer_State:
			if(*_code == L'e')
				state = bLisp_Ge_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Ge_Lexer_State:
			if(*_code == L't')
				state = bLisp_Get_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Get_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_GetToken);

		case bLisp_B_Lexer_State:
			if(*_code == L'e')
				state = bLisp_Be_Lexer_State;
			else if(*_code == L'i')
				state = bLisp_Bi_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);		
			break;

		case bLisp_Bi_Lexer_State:
			if(*_code == L'n')
				state = bLisp_Bin_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Bin_Lexer_State:
			if(*_code == L'd')
				state = bLisp_Bind_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Bind_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_BindToken);
		
		case bLisp_Be_Lexer_State:
			if(*_code == L'g')
				state = bLisp_Beg_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Beg_Lexer_State:
			if(*_code == L'i')
				state = bLisp_Begi_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Begi_Lexer_State:
			if(*_code == L'n')
				state = bLisp_Begin_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_Begin_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_BeginToken);

		case bLisp_More_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_MoreToken);

		case bLisp_Less_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_LessToken);

		case bLisp_Equal_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_EqualToken);

		case bLisp_LBrace_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_LBraceToken);

		case bLisp_RBrace_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_RBraceToken);

		case bLisp_EndString_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_StringToken);
			break;

		case bLisp_String_Lexer_State:
			if(*_code == L'"')
			{
				state = bLisp_EndString_Lexer_State;
				break;
			}
			else if(*_code == L'\n' || *_code == L'\0')
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_ErrorToken);
			}
			else
				break;

		case bLisp_Digit_Lexer_State:
			if(iswdigit(*_code))
			{
				break;
			}
			else if(*_code == L'.')
			{
				state = bLisp_Float_Lexer_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_DigitToken);
			}

		case bLisp_Float_Lexer_State:
			if(iswdigit(*_code))
			{
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_FloatToken);
			}

		case bLisp_Sub_Lexer_State:
			if(iswdigit(*_code))
			{
				state = bLisp_Digit_Lexer_State;
				break;
			}
			else if(*_code == L'-')
			{
				state = bLisp_MinusMinus_Lexer_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_SubToken);

		case bLisp_Add_Lexer_State:
			if(iswdigit(*_code))
			{
				state = bLisp_Digit_Lexer_State;
				break;
			}
			else if(*_code == L'+')
			{
				state = bLisp_PlusPlus_Lexer_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_AddToken);

		case bLisp_Mul_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_MulToken);

		case bLisp_Slash_Lexer_State:
			if(*_code == '/')
			{
				state = bLisp_Comment_Lexer_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_DivToken);

		case bLisp_I_Lexer_State:
			if(*_code == L'f')
				state = bLisp_If_Lexer_State;
			else if(*_code == L'n')
				state = bLisp_In_Lexer_State;
			else if(iswalpha(*_code) || iswdigit(*_code))
				state = bLisp_Name_Lexer_State;
			else 
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			break;

		case bLisp_If_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_IfToken);
			}

		case bLisp_Name_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
				break;
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);

		default:
			return bLisp_BuildTokenPos(begin, _code, bLisp_ErrorToken);
		}
	} while(*(_code++));

	return bLisp_BuildTokenPos(begin, _code, bLisp_ErrorToken);
}
/*----------------------------------------------------------------------------*/
/* bLisp_BuildTokenPos                                                        */
/*----------------------------------------------------------------------------*/
bLisp_TokenPos
bLisp_BuildTokenPos(bchar *_beg, bchar *_end, bLisp_TokenClass _type)
{
	bLisp_TokenPos token = {_beg, _end, _type};

	return token;
}