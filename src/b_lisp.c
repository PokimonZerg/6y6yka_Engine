/*----------------------------------------------------------------------------*/
/*                              6y6yka Engine                                 */
/*----------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "b_assert.h"
#include "b_lisp.h"
/*----------------------------------------------------------------------------*/
#define bLisp_NumDebugTokens 5
#define bLisp_DefaultBoxNum  16
#define bLisp_BoxNotFound    -1
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
	buint   string;
	bvoid  *data  ;
};

typedef union bLisp_TypeValue bLisp_TypeValue;
/*----------------------------------------------------------------------------*/
/* bLisp_Type                                                                 */
/*----------------------------------------------------------------------------*/
struct bLisp_Type
{
	bLisp_TypeClass type ;
	bLisp_TypeValue value;
};

typedef struct bLisp_Type bLisp_Type;
typedef struct bLisp_Type bLisp_Constant;
/*----------------------------------------------------------------------------*/
/* bLisp_StringBox                                                            */
/*----------------------------------------------------------------------------*/
struct bLisp_StringBox
{
	bchar *data;
	buint  size;
	buint  max_size;
};

typedef struct bLisp_StringBox bLisp_StringBox;
/*----------------------------------------------------------------------------*/
/* bLisp_Variable                                                             */
/*----------------------------------------------------------------------------*/
struct bLisp_Variable
{
	bLisp_Type value;
	bchar     *name;
};

typedef struct bLisp_Variable bLisp_Variable;
/*----------------------------------------------------------------------------*/
/* bLisp_VariableBox                                                          */
/*----------------------------------------------------------------------------*/
struct bLisp_VariableBox
{
	bLisp_Variable *data;
	buint           size;
	buint           max_size;
};

typedef struct bLisp_VariableBox bLisp_VariableBox;
/*----------------------------------------------------------------------------*/
/* bLisp_UniversalBox                                                         */
/*----------------------------------------------------------------------------*/
struct bLisp_UniversalBox
{
	bvoid *data;
	buint  size;
	buint  max_size;
};

typedef struct bLisp_UniversalBox bLisp_UniversalBox;
/*----------------------------------------------------------------------------*/
/* bLisp_ConstantBox                                                          */
/*----------------------------------------------------------------------------*/
struct bLisp_ConstantBox
{
	bLisp_Constant *data;
	buint           size;
	buint           max_size;
};

typedef struct bLisp_ConstantBox bLisp_ConstantBox;
/*----------------------------------------------------------------------------*/
/* bLisp_Stack                                                                */
/*----------------------------------------------------------------------------*/
struct bLisp_Stack
{
	bLisp_Type *data;
	bLisp_Type *bottom;
	bLisp_Type *top;
};

typedef struct bLisp_Stack bLisp_Stack;
/*----------------------------------------------------------------------------*/
/* bLisp_CodeClass                                                            */
/*----------------------------------------------------------------------------*/
enum bLisp_CodeClass
{
	bLisp_Push_Code,
	bLisp_Add_Code,
	bLisp_Sub_Code,
	bLisp_Mul_Code,
	bLisp_Div_Code
};

typedef enum bLisp_CodeClass bLisp_CodeClass;
/*----------------------------------------------------------------------------*/
/* bLisp_RefClass                                                             */
/*----------------------------------------------------------------------------*/
enum bLisp_RefClass
{
	bLisp_ConstantRef,
	bLisp_VariableRef,
	bLisp_FunctionRef
};

typedef enum bLisp_RefClass bLisp_RefClass;
/*----------------------------------------------------------------------------*/
/* bLisp_RefValue                                                             */
/*----------------------------------------------------------------------------*/
typedef buint bLisp_RefValue;
/*----------------------------------------------------------------------------*/
/* bLisp_Ref                                                                  */
/*----------------------------------------------------------------------------*/
struct bLisp_Ref
{
	bLisp_RefClass type;
	bLisp_RefValue value;
};

typedef struct bLisp_Ref bLisp_Ref;
/*----------------------------------------------------------------------------*/
/* bLisp_Code                                                                 */
/*----------------------------------------------------------------------------*/
struct bLisp_Code
{
	bLisp_CodeClass type;
	bLisp_Ref       ref;
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
/* bLisp_ErrorInfo                                                            */
/*----------------------------------------------------------------------------*/
struct bLisp_ErrorInfo
{
	bchar *text;
	bchar *file;
	buint  line;
};

typedef struct bLisp_ErrorInfo bLisp_ErrorInfo;
/*----------------------------------------------------------------------------*/
/* bLisp_TempData                                                          */
/*----------------------------------------------------------------------------*/
struct bLisp_TempData
{
	bLisp_Source     source;
	bLisp_ErrorInfo  error;
};

typedef struct bLisp_TempData bLisp_TempData;
/*----------------------------------------------------------------------------*/
/* bLisp_CodeBox                                                              */
/*----------------------------------------------------------------------------*/
struct bLisp_CodeBox
{
	bLisp_Code *code;
	buint       size;
	buint       max_size;
};

typedef struct bLisp_CodeBox bLisp_CodeBox;
/*----------------------------------------------------------------------------*/
/* bLisp_Box                                                                  */
/*----------------------------------------------------------------------------*/
struct bLisp_Box
{
	bLisp_VariableBox variable;
	bLisp_StringBox   string;
	bLisp_ConstantBox constant;
	bLisp_CodeBox     code;
};

typedef struct bLisp_Box bLisp_Box;
/*----------------------------------------------------------------------------*/
/* bLisp_Script                                                               */
/*----------------------------------------------------------------------------*/
struct bLisp_Script
{
	bLisp_Stack stack;

	bLisp_Box box;
};
/*----------------------------------------------------------------------------*/
/* bLisp_ParseExpression                                                      */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseExpression(bLisp_Script *_script, bLisp_Token *_token,
								 bLisp_TempData *_data);
/*----------------------------------------------------------------------------*/
/* bLisp_ParseFunction                                                        */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseFunction(bLisp_Script *_script, bLisp_TempData *_data);
/*----------------------------------------------------------------------------*/
/* bLisp_ParseASMD                                                            */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseASMD(bLisp_Script *_script, bLisp_TokenClass _type,
					  bLisp_TempData *_data);
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
/* bLisp_StackPush                                                            */
/*----------------------------------------------------------------------------*/
bvoid bLisp_StackPush(bLisp_Stack *_stack, bLisp_Type *_item);
/*----------------------------------------------------------------------------*/
/* bLisp_StackPop                                                             */
/*----------------------------------------------------------------------------*/
bvoid bLisp_StackPop(bLisp_Stack *_stack);
/*----------------------------------------------------------------------------*/
/* bLisp_StackTop                                                             */
/*----------------------------------------------------------------------------*/
bLisp_Type *bLisp_StackTop(bLisp_Stack *_stack);
/*----------------------------------------------------------------------------*/
/* bLisp_StackPushRef                                                         */
/*----------------------------------------------------------------------------*/
bvoid bLisp_StackPushRef(bLisp_Script *_script, bLisp_Ref *_ref);
/*----------------------------------------------------------------------------*/
/* bLisp_StackPopRef                                                          */
/*----------------------------------------------------------------------------*/
bvoid bLisp_StackPopRef(bLisp_Script *_script, bLisp_Ref *_ref);
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
/* bLisp_AddItemInBox                                                         */
/*----------------------------------------------------------------------------*/
buint bLisp_AddItemInBox(bLisp_UniversalBox *_box, bvoid *_item,  buint _size);
/*----------------------------------------------------------------------------*/
/* bLisp_CreateBox                                                            */
/*----------------------------------------------------------------------------*/
bvoid bLisp_CreateBox(bLisp_UniversalBox *_box, buint _size, buint _num);
/*----------------------------------------------------------------------------*/
/* bLisp_DeleteBox                                                            */
/*----------------------------------------------------------------------------*/
bvoid bLisp_DeleteBox(bLisp_UniversalBox *_box);
/*----------------------------------------------------------------------------*/
/* bLisp_FindBox                                                              */
/*----------------------------------------------------------------------------*/
buint bLisp_FindBox(bLisp_UniversalBox *_box, bvoid *_item, buint _size);
/*----------------------------------------------------------------------------*/
/* bLisp_ZipBox                                                               */
/*----------------------------------------------------------------------------*/
bvoid bLisp_ZipBox(bLisp_UniversalBox *_box, buint _size);
/*----------------------------------------------------------------------------*/
/* bLisp_GetNextToken                                                         */
/*----------------------------------------------------------------------------*/
bLisp_Token bLisp_GetNextToken(bLisp_Source *_source);
/*----------------------------------------------------------------------------*/
/* bLisp_AddStringExpression                                                  */
/*----------------------------------------------------------------------------*/
bvoid bLisp_AddStringExpression(bLisp_Box *_box, bLisp_Token *_token);
/*----------------------------------------------------------------------------*/
/* bLisp_AddNumberExpression                                                  */
/*----------------------------------------------------------------------------*/
bvoid bLisp_AddNumberExpression(bLisp_Box *_box, bLisp_Token *_token);
/*----------------------------------------------------------------------------*/
/* bLisp_Compile                                                              */
/*----------------------------------------------------------------------------*/
bbool bLisp_Compile(bLisp_Script *_script, bLisp_TempData *_data);
/*----------------------------------------------------------------------------*/
/* bLisp_CreateAllBoxes                                                       */
/*----------------------------------------------------------------------------*/
bvoid bLisp_CreateAllBoxes(bLisp_Box *_box);
/*----------------------------------------------------------------------------*/
/* bLisp_DeleteAllBoxes                                                       */
/*----------------------------------------------------------------------------*/
bvoid bLisp_DeleteAllBoxes(bLisp_Box *_box);
/*----------------------------------------------------------------------------*/
/* bLisp_OpenScript                                                           */
/*----------------------------------------------------------------------------*/
bLisp_Script *bLisp_OpenScript(bchar *_text)
{
	bLisp_Script      *script;
	bLisp_TempData *data;
	bbool              compile_result;

	bAssert(_text);

	script = malloc(sizeof(bLisp_Script));
	data = malloc(sizeof(bLisp_TempData));

	compile_result = bLisp_Compile(script, data);

	free(data);

	return script;
}
/*----------------------------------------------------------------------------*/
/* bLisp_Compile                                                              */
/*----------------------------------------------------------------------------*/
bbool bLisp_Compile(bLisp_Script *_script, bLisp_TempData *_data)
{
	bLisp_Token token;

	bLisp_CreateAllBoxes(&_script->box);

	token = bLisp_GetNextToken(&_data->source);

	while(token.type != bLisp_EofToken)
	{
		if(!bLisp_ParseExpression(_script, &token, _data))
		{
			bLisp_DeleteAllBoxes(&_script->box);
			return bfalse;
		}

		token = bLisp_GetNextToken(&_data->source);
	}

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_AddStringExpression                                                  */
/*----------------------------------------------------------------------------*/
bvoid bLisp_AddStringExpression(bLisp_Box *_box, bLisp_Token *_token)
{
	bLisp_Code     code;
	bLisp_Constant constant;
	buint          id;

	id = bLisp_AddItemInBox((bLisp_UniversalBox *)&_box->string,
		 _token->value.string, wcslen(_token->value.string) + sizeof(bchar));

	constant.type = bLisp_StringType;
	constant.value.string = id;

	id = bLisp_AddItemInBox((bLisp_UniversalBox *)&_box->constant,
		 &constant, sizeof(bLisp_Constant));

	code.type      = bLisp_Push_Code;
	code.ref.type  = bLisp_ConstantRef;
	code.ref.value = id;

	bLisp_AddItemInBox((bLisp_UniversalBox *)&_box->code, 
	&code, sizeof(bLisp_Code));
}
/*----------------------------------------------------------------------------*/
/* bLisp_AddNumberExpression                                                  */
/*----------------------------------------------------------------------------*/
bvoid bLisp_AddNumberExpression(bLisp_Box *_box, bLisp_Token *_token)
{
	bLisp_Code     code;
	bLisp_Constant constant;

	constant.type         = bLisp_NumberType;
	constant.value.number = _token->value.number;

	code.ref.value = bLisp_AddItemInBox((bLisp_UniversalBox *)&_box->constant,
		             &constant, sizeof(bLisp_Constant));

	code.type      = bLisp_Push_Code;
	code.ref.type  = bLisp_ConstantRef;

	bLisp_AddItemInBox((bLisp_UniversalBox *)&_box->code, 
	&code, sizeof(bLisp_Code));
}
/*----------------------------------------------------------------------------*/
/* bLisp_ParseASMD                                                            */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseASMD(bLisp_Script *_script, bLisp_TokenClass _type,
					  bLisp_TempData *_data)
{
	buint       i;
	bLisp_Token token;
	bLisp_Code  code;

	token = bLisp_GetNextToken(&_data->source);

	for(i = 0; token.type != bLisp_RBraceToken; i++)
	{
		if(!bLisp_ParseExpression(_script, &token, _data))
			return bfalse;

		token = bLisp_GetNextToken(&_data->source);
	}

	if(i < 2) /* minimum two argumnets */
	{
		_data->error.text = L"So few argumnets in '+'";
		_data->error.file = token.file;
		_data->error.line = token.line;
	}

	switch(_type)
	{
	case bLisp_AddToken:
		code.type = bLisp_Add_Code;
		break;

	case bLisp_SubToken:
		code.type = bLisp_Sub_Code;
		break;

	case bLisp_MulToken:
		code.type = bLisp_Mul_Code;
		break;

	case bLisp_DivToken:
		code.type = bLisp_Div_Code;
		break;
	}

	while(i-- != 1)
	{
		bLisp_AddItemInBox((bLisp_UniversalBox *)&_script->box.code, 
		&code, sizeof(bLisp_Code));
	}

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_ParseFunction                                                        */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseFunction(bLisp_Script *_script, bLisp_TempData *_data)
{
	bLisp_Token token;

	token = bLisp_GetNextToken(&_data->source);

	switch(token.type)
	{
	case bLisp_NameToken:
		break;

	case bLisp_AddToken:
	case bLisp_SubToken:
	case bLisp_MulToken:
	case bLisp_DivToken:
		return bLisp_ParseASMD(_script, token.type, _data);
		break;

	default:
		_data->error.text = L"Not valid function name";
		_data->error.file = token.file;
		_data->error.line = token.line;
		return bfalse;
	};

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_ParseExpression                                                      */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseExpression(bLisp_Script *_script, bLisp_Token *_token,
								 bLisp_TempData *_data)
{
	switch(_token->type)
	{
	case bLisp_StringToken:
		bLisp_AddStringExpression(&_script->box, _token);
		break;

	case bLisp_NumberToken:
	case bLisp_NameToken:
		bLisp_AddNumberExpression(&_script->box, _token);
		break;

	case bLisp_LBraceToken:
		return bLisp_ParseFunction(_script, _data);

	case bLisp_ErrorToken:
		_data->error.file = _token->file;
		_data->error.line = _token->line;
		return bfalse;

	default:
		_data->error.text = L"Unknown parse error";
		_data->error.file = _token->file;
		_data->error.line = _token->line;
		return bfalse;
	}

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_CreateAllBoxes                                                       */
/*----------------------------------------------------------------------------*/
bvoid bLisp_CreateAllBoxes(bLisp_Box *_box)
{
	bLisp_CreateBox((bLisp_UniversalBox *)&_box->code,
		            sizeof(bLisp_Code), bLisp_DefaultBoxNum);

	bLisp_CreateBox((bLisp_UniversalBox *)&_box->string,
		            sizeof(L"typical string"), bLisp_DefaultBoxNum);

	bLisp_CreateBox((bLisp_UniversalBox *)&_box->constant,
		            sizeof(bLisp_Type), bLisp_DefaultBoxNum);

	bLisp_CreateBox((bLisp_UniversalBox *)&_box->variable,
		            sizeof(bLisp_Variable), bLisp_DefaultBoxNum);
}
/*----------------------------------------------------------------------------*/
/* bLisp_DeleteAllBoxes                                                       */
/*----------------------------------------------------------------------------*/
bvoid bLisp_DeleteAllBoxes(bLisp_Box *_box)
{
	bLisp_DeleteBox((bLisp_UniversalBox *)&_box->code);
	bLisp_DeleteBox((bLisp_UniversalBox *)&_box->constant);
	bLisp_DeleteBox((bLisp_UniversalBox *)&_box->string);
	bLisp_DeleteBox((bLisp_UniversalBox *)&_box->variable);
}
/*----------------------------------------------------------------------------*/
/* bLisp_CreateBox                                                            */
/*----------------------------------------------------------------------------*/
bvoid bLisp_CreateBox(bLisp_UniversalBox *_box, buint _size, buint _num)
{
	buint size = _size * _num;

	_box->data     = malloc(size);
	_box->max_size = size;
	_box->size     = 0;
}
/*----------------------------------------------------------------------------*/
/* bLisp_DeleteBox                                                            */
/*----------------------------------------------------------------------------*/
bvoid bLisp_DeleteBox(bLisp_UniversalBox *_box)
{
	bAssert(_box);

	free(_box->data);
}
/*----------------------------------------------------------------------------*/
/* bLisp_FindBox                                                              */
/*----------------------------------------------------------------------------*/
buint bLisp_FindBox(bLisp_UniversalBox *_box, bvoid *_item, buint _size)
{
	buint id;

	for(id = 0; id < _box->size; id++)
	{
		if(memcmp(_box->data, _item, _size) == 0)
			return id;
	}

	return bLisp_BoxNotFound;
}
/*----------------------------------------------------------------------------*/
/* bLisp_ZipBox                                                               */
/*----------------------------------------------------------------------------*/
bvoid bLisp_ZipBox(bLisp_UniversalBox *_box, buint _size)
{
	bbyte *new_data;
	buint  new_size;

	bAssert(_box);

	new_size = _box->size * _size;
	new_data = malloc(new_size);
	
	memcpy(new_data, _box->data, new_size);

	free(_box->data);

	_box->data     = new_data;
	_box->max_size = new_size;
}
/*----------------------------------------------------------------------------*/
/* bLisp_AddItemInBox                                                         */
/*----------------------------------------------------------------------------*/
buint bLisp_AddItemInBox(bLisp_UniversalBox *_box, bvoid *_item, buint _size)
{
	bbyte *new_data;
	buint  new_size;
	bbyte *cur_pos;
	bbyte *end_ptr    = (bbyte *)_box->data + _box->max_size;
	bbyte *result_ptr = (bbyte *)_box->data + (_box->size + 1) * _size;
	buint  result     = (buint)(end_ptr - result_ptr);

	if(result < _size)
	{
		new_size = (_box->max_size + _size) * 2;
		new_data = malloc(new_size);
		memcpy(new_data, _box->data, _box->max_size);

		free(_box->data);
		_box->data = new_data;
		_box->max_size = new_size;
	}

	cur_pos = (bbyte *)_box->data + (_box->size * _size);
	memcpy(cur_pos, _item, _size);

	_box->max_size += _size;
	_box->size     += 1;

	return 0;
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
/* bLisp_StackPush                                                            */
/*----------------------------------------------------------------------------*/
bvoid bLisp_StackPush(bLisp_Stack *_stack, bLisp_Type *_item)
{
	bAssert(_stack->data < _stack->top);

	*_stack->data = *_item;
	_stack->data++;
}
/*----------------------------------------------------------------------------*/
/* bLisp_StackPop                                                             */
/*----------------------------------------------------------------------------*/
bvoid bLisp_StackPop(bLisp_Stack *_stack)
{
	bAssert(_stack->data > _stack->bottom);

	_stack->data--;
}
/*----------------------------------------------------------------------------*/
/* bLisp_StackTop                                                             */
/*----------------------------------------------------------------------------*/
bLisp_Type *bLisp_StackTop(bLisp_Stack *_stack)
{
	bAssert(_stack->data > _stack->bottom);

	return _stack->data - 1;
}
/*----------------------------------------------------------------------------*/
/* bLisp_StackPushRef                                                         */
/*----------------------------------------------------------------------------*/
bvoid bLisp_StackPushRef(bLisp_Script *_script, bLisp_Ref *_ref)
{
	switch(_ref->type)
	{
	case bLisp_ConstantRef:
		bLisp_StackPush(&_script->stack,
						&_script->box.constant.data[_ref->value]);
		break;

	}
}
/*----------------------------------------------------------------------------*/
/* bLisp_StackPopRef                                                          */
/*----------------------------------------------------------------------------*/
bvoid bLisp_StackPopRef(bLisp_Script *_script, bLisp_Ref *_ref)
{
	switch(_ref->type)
	{
	case bLisp_ConstantRef:
		_script->box.constant.data[_ref->value] = *
		bLisp_StackTop(&_script->stack);
		bLisp_StackPop(&_script->stack);
		break;

	}
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

			case L' ': case L'\t':
				state = bLisp_Space_Lexer_State;
				break;

			default:
				break;
			}
			
			break;

		case bLisp_S_Lexer_State:
			if(*_code == L'e')
			{
				state = bLisp_Se_Lexer_State;
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

		case bLisp_Se_Lexer_State:
			if(*_code == L't')
			{
				state = bLisp_Set_Lexer_State;
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

		case bLisp_Set_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_SetToken);
			}

		case bLisp_Not_Lexer_State:
			if(*_code == L'=')
			{
				state = bLisp_NEqual_Lexer_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_NotToken);
			}

		case bLisp_NEqual_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_NEqualToken);

		case bLisp_NewLine_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_LineToken);

		case bLisp_Space_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_SpaceToken);

		case bLisp_L_Lexer_State:
			if(*_code == L'e')
			{
				state = bLisp_Le_Lexer_State;
				break;
			}
			else if(*_code == L'i')
			{
				state = bLisp_Li_Lexer_State;
				break;
			}
			else if(*_code == L'a')
			{
				state = bLisp_La_Lexer_State;
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

		case bLisp_La_Lexer_State:
			if(*_code == L'm')
			{
				state = bLisp_Lam_Lexer_State;
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

		case bLisp_Lam_Lexer_State:
			if(*_code == L'b')
			{
				state = bLisp_Lamb_Lexer_State;
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

		case bLisp_Lamb_Lexer_State:
			if(*_code == L'd')
			{
				state = bLisp_Lambd_Lexer_State;
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

		case bLisp_Lambd_Lexer_State:
			if(*_code == L'a')
			{
				state = bLisp_Lambda_Lexer_State;
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
			{
				state = bLisp_List_Lexer_State;
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

		case bLisp_List_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_ListToken);
			}

		case bLisp_Le_Lexer_State:
			if(*_code == L't')
			{
				state = bLisp_Let_Lexer_State;
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

		case bLisp_Let_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_LetToken);
			}

		case bLisp_PlusPlus_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_IncToken);

		case bLisp_MinusMinus_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_DecToken);

		case bLisp_C_Lexer_State:
			if(*_code == L'a')
			{
				state = bLisp_Ca_Lexer_State;
				break;
			}
			else if(*_code == L'd')
			{
				state = bLisp_Cd_Lexer_State;
				break;
			}
			else if(*_code == L'o')
			{
				state = bLisp_Co_Lexer_State;
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

		case bLisp_Car_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_CarToken);
			}

		case bLisp_Cdr_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_CdrToken);
			}

		case bLisp_Cons_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_ConsToken);
			}

		case bLisp_Ca_Lexer_State:
			if(*_code == L'r')
			{
				state = bLisp_Car_Lexer_State;
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

		case bLisp_Cd_Lexer_State:
			if(*_code == L'r')
			{
				state = bLisp_Cdr_Lexer_State;
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

		case bLisp_Co_Lexer_State:
			if(*_code == L'n')
			{
				state = bLisp_Con_Lexer_State;
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

		case bLisp_Con_Lexer_State:
			if(*_code == L's')
			{
				state = bLisp_Cons_Lexer_State;
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

		case bLisp_In_Lexer_State:
			if(*_code == L'c')
			{
				state = bLisp_Inc_Lexer_State;
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

		case bLisp_Inc_Lexer_State:
			if(*_code == L'l')
			{
				state = bLisp_Incl_Lexer_State;
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

		case bLisp_Incl_Lexer_State:
			if(*_code == L'u')
			{
				state = bLisp_Inclu_Lexer_State;
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

		case bLisp_Inclu_Lexer_State:
			if(*_code == L'd')
			{
				state = bLisp_Includ_Lexer_State;
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

		case bLisp_Includ_Lexer_State:
			if(*_code == L'e')
			{
				state = bLisp_Include_Lexer_State;
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

		case bLisp_Include_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_IncludeToken);
			}

		case bLisp_Sharp_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_SharpToken);

		case bLisp_Comment_Lexer_State:
			if(*_code == L'\0')
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_CommentToken);
			}
			else
			{
				break;
			}

		case bLisp_W_Lexer_State:
			if(*_code == L'h')
			{
				state = bLisp_Wh_Lexer_State;
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

		case bLisp_Wh_Lexer_State:
			if(*_code == L'i')
			{
				state = bLisp_Whi_Lexer_State;
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

		case bLisp_Whi_Lexer_State:
			if(*_code == L'l')
			{
				state = bLisp_Whil_Lexer_State;
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

		case bLisp_Whil_Lexer_State:
			if(*_code == L'e')
			{
				state = bLisp_While_Lexer_State;
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

		case bLisp_While_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_WhileToken);
			}

		case bLisp_G_Lexer_State:
			if(*_code == L'e')
			{
				state = bLisp_Ge_Lexer_State;
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

		case bLisp_Ge_Lexer_State:
			if(*_code == L't')
			{
				state = bLisp_Get_Lexer_State;
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

		case bLisp_Get_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_GetToken);
			}

		case bLisp_B_Lexer_State:
			if(*_code == L'e')
			{
				state = bLisp_Be_Lexer_State;
				break;
			}
			else if(*_code == L'i')
			{
				state = bLisp_Bi_Lexer_State;
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

		case bLisp_Bi_Lexer_State:
			if(*_code == L'n')
			{
				state = bLisp_Bin_Lexer_State;
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

		case bLisp_Bin_Lexer_State:
			if(*_code == L'd')
			{
				state = bLisp_Bind_Lexer_State;
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

		case bLisp_Bind_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_BindToken);
			}
		
		case bLisp_Be_Lexer_State:
			if(*_code == L'g')
			{
				state = bLisp_Beg_Lexer_State;
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

		case bLisp_Beg_Lexer_State:
			if(*_code == L'i')
			{
				state = bLisp_Begi_Lexer_State;
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

		case bLisp_Begi_Lexer_State:
			if(*_code == L'n')
			{
				state = bLisp_Begin_Lexer_State;
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

		case bLisp_Begin_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_BeginToken);
			}

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
			{
				break;
			}

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
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_SubToken);
			}

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
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_AddToken);
			}

		case bLisp_Mul_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_MulToken);

		case bLisp_Slash_Lexer_State:
			if(*_code == '/')
			{
				state = bLisp_Comment_Lexer_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_DivToken);
			}

		case bLisp_I_Lexer_State:
			if(*_code == L'f')
			{
				state = bLisp_If_Lexer_State;
				break;
			}
			else if(*_code == L'n')
			{
				state = bLisp_In_Lexer_State;
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
			{
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
			}

		default:
			return bLisp_BuildTokenPos(begin, _code, bLisp_ErrorToken);
		}
	} while(*(_code++));

	return bLisp_BuildTokenPos(begin, _code, bLisp_ErrorToken);
}
/*----------------------------------------------------------------------------*/
/* bLisp_BuildTokenPos                                                        */
/*----------------------------------------------------------------------------*/
bLisp_TokenPos bLisp_BuildTokenPos(bchar *_beg, bchar *_end,
								   bLisp_TokenClass _type)
{
	bLisp_TokenPos token = {_beg, _end, _type};

	return token;
}