/*----------------------------------------------------------------------------*/
/*                              6y6yka Engine                                 */
/*----------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "b_assert.h"
#include "b_box.h"
#include "b_pool.h"
#include "b_lisp.h"
/*----------------------------------------------------------------------------*/
/* Important defines                                                          */
/*----------------------------------------------------------------------------*/
#define bLisp_NumDebugTokens  5
#define bLisp_DefaultBoxSize  16
#define bLisp_DefaultPoolSize 512
#define bLisp_BoxNotFound     -1
#define bLisp_StackSize       1024
/*----------------------------------------------------------------------------*/
/* bLisp_Function                                                             */
/*----------------------------------------------------------------------------*/
struct bFunction
{
	buint temp; // really temp
};

typedef struct bFunction bFunction;
/*----------------------------------------------------------------------------*/
/* bLisp_TypeClass                                                            */
/*----------------------------------------------------------------------------*/
enum bLisp_TypeClass
{
	bLisp_NumberType,    /* число                   */
	bLisp_StringType,    /* строка константа        */
	bLisp_NewStringType, /* строка динамическая     */
	bLisp_DataType,      /* указатель на что угодно */
	bLisp_FunctionType   /* указатель на функцию    */
};

typedef enum bLisp_TypeClass bLisp_TypeClass;
/*----------------------------------------------------------------------------*/
/* bLisp_TypeValue                                                            */
/*----------------------------------------------------------------------------*/
union bLisp_TypeValue
{
	bfloat     number    ;
	bchar     *string    ;
	bchar     *new_string;
	bvoid     *data      ;
	bFunction *function  ;
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
/*----------------------------------------------------------------------------*/
/* bLisp_Variable                                                             */
/*----------------------------------------------------------------------------*/
struct bLisp_Variable
{
	bLisp_Type *value;
	bchar      *name;
};

typedef struct bLisp_Variable bLisp_Variable;
/*----------------------------------------------------------------------------*/
/* bLisp_VariableBox                                                          */
/*----------------------------------------------------------------------------*/
struct bLisp_VariableBox
{
	bLisp_Variable *data;
	buint           item_size;
	buint           item_num;
	buint           item_num_max;
};

typedef struct bLisp_VariableBox bLisp_VariableBox;
/*----------------------------------------------------------------------------*/
/* bLisp_ConstBox                                                             */
/*----------------------------------------------------------------------------*/
struct bLisp_ConstBox
{
	bLisp_Type *data;
	buint       item_size;
	buint       item_num;
	buint       item_num_max;
};

typedef struct bLisp_ConstBox bLisp_ConstBox;
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
	bLisp_Push_Code, /* push type into stack                                  */
	bLisp_Pop_Code,  /* pop type away                                         */
	bLisp_Add_Code,  /* add two types on stack and push result                */
	bLisp_Sub_Code,  /* sub two types on stack and push result                */
	bLisp_Mul_Code,  /* mul two types on stack and push result                */
	bLisp_Div_Code,  /* div two types on stack and push result                */
	bLisp_If_Code,   /* test type on steck                                    */
	bLisp_Goto_Code, /* goto new code position                                */
	bLisp_Set_Code   /* typical set code                                      */
};

typedef enum bLisp_CodeClass bLisp_CodeClass;
/*----------------------------------------------------------------------------*/
/* bLisp_RefClass                                                             */
/*----------------------------------------------------------------------------*/
enum bLisp_RefClass
{
	bLisp_ConstRef,
	bLisp_VariableRef
};

typedef enum bLisp_RefClass bLisp_RefClass;
/*----------------------------------------------------------------------------*/
/* bLisp_RefValue                                                             */
/*----------------------------------------------------------------------------*/
union bLisp_RefValue
{
	bLisp_Type     *constant;
	bLisp_Variable *variable;
};

typedef union bLisp_RefValue bLisp_RefValue;
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
	bLisp_Ref       ref ;
	buint           line;
	bchar          *file;
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
/* bLisp_StringPool                                                           */
/*----------------------------------------------------------------------------*/
struct bLisp_StringPool
{
	bchar *data;
	buint  max_size;
	buint  size;
};

typedef struct bLisp_StringPool bLisp_StringPool;
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
/* bLisp_CodeBox                                                              */
/*----------------------------------------------------------------------------*/
struct bLisp_CodeBox
{
	bLisp_Code *data;
	buint       item_size;
	buint       item_num;
	buint       item_num_max;
};

typedef struct bLisp_CodeBox bLisp_CodeBox;
/*----------------------------------------------------------------------------*/
/* bLisp_Data                                                                 */
/*----------------------------------------------------------------------------*/
struct bLisp_Data
{
	bLisp_VariableBox *variable;
	bLisp_ConstBox    *constant;
	bLisp_CodeBox     *code;

	bLisp_StringPool  *string;
};

typedef struct bLisp_Data bLisp_Data;
/*----------------------------------------------------------------------------*/
/* bLisp_Script                                                               */
/*----------------------------------------------------------------------------*/
struct bLisp_Script
{ 
	bLisp_Stack      stack ; /* runtime data     */
	bLisp_Data       data  ; /* script data      */
	bLisp_ErrorInfo  error ; /* last error data  */
	bLisp_Source    *source; /* compilation data */
};
/*----------------------------------------------------------------------------*/
/* bLisp_StackCreate                                                          */
/*----------------------------------------------------------------------------*/
bvoid bLisp_StackCreate(bLisp_Stack *_stack);
/*----------------------------------------------------------------------------*/
/* bLisp_StackDelete                                                          */
/*----------------------------------------------------------------------------*/
bvoid bLisp_StackDelete(bLisp_Stack *_stack);
/*----------------------------------------------------------------------------*/
/* bLisp_ParseExpression                                                      */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseExpression(bLisp_Script *_script, bLisp_Token *_token);
/*----------------------------------------------------------------------------*/
/* bLisp_ParseFunction                                                        */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseFunction(bLisp_Script *_script);
/*----------------------------------------------------------------------------*/
/* bLisp_ParseASMD                                                            */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseASMD(bLisp_Script *_script, bLisp_TokenClass _type);
/*----------------------------------------------------------------------------*/
/* bLisp_ParseBind                                                            */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseBind(bLisp_Script *_script);
/*----------------------------------------------------------------------------*/
/* bLisp_ParseSet                                                             */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseSet(bLisp_Script *_script);
/*----------------------------------------------------------------------------*/
/* bLisp_ParseName                                                            */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseName(bLisp_Script *_script, bLisp_Token *_token);
/*----------------------------------------------------------------------------*/
/* bLisp_IdentNextToken                                                       */
/*----------------------------------------------------------------------------*/
bLisp_TokenPos bLisp_IdentNextToken(bchar *_code);
/*----------------------------------------------------------------------------*/
/* bLisp_CreateSource                                                         */
/*----------------------------------------------------------------------------*/
bLisp_Source *bLisp_CreateSource(bLisp_Script *_script, bchar *_code);
/*----------------------------------------------------------------------------*/
/* bLisp_DeleteSource                                                         */
/*----------------------------------------------------------------------------*/
bvoid bLisp_DeleteSource(bLisp_Source *_source);
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
bLisp_TokenPos bLisp_BuildTokenPos(bchar *_b, bchar *_e, bLisp_TokenClass _t);
/*----------------------------------------------------------------------------*/
/* bLisp_SqueezeToken                                                         */
/*----------------------------------------------------------------------------*/
bbool bLisp_SqueezeToken(bLisp_TokenPos *_data, bLisp_Token *_token);
/*----------------------------------------------------------------------------*/
/* bLisp_GetNextToken                                                         */
/*----------------------------------------------------------------------------*/
bLisp_Token bLisp_GetNextToken(bLisp_Source *_source);
/*----------------------------------------------------------------------------*/
/* bLisp_AddStringExpression                                                  */
/*----------------------------------------------------------------------------*/
bvoid bLisp_AddStringExpression(bLisp_Script *_script, bLisp_Token *_token);
/*----------------------------------------------------------------------------*/
/* bLisp_AddNumberExpression                                                  */
/*----------------------------------------------------------------------------*/
bvoid bLisp_AddNumberExpression(bLisp_Data *_data, bLisp_Token *_token);
/*----------------------------------------------------------------------------*/
/* bLisp_Compile                                                              */
/*----------------------------------------------------------------------------*/
bbool bLisp_Compile(bLisp_Script *_script, bchar *_code);
/*----------------------------------------------------------------------------*/
/* bLisp_CreateData                                                           */
/*----------------------------------------------------------------------------*/
bvoid bLisp_CreateData(bLisp_Data *_data);
/*----------------------------------------------------------------------------*/
/* bLisp_DeleteData                                                           */
/*----------------------------------------------------------------------------*/
bvoid bLisp_DeleteData(bLisp_Data *_data);
/*----------------------------------------------------------------------------*/
/* bLisp_PrintType                                                            */
/*----------------------------------------------------------------------------*/
bchar *bLisp_PrintType(bLisp_Type *_type);
/*----------------------------------------------------------------------------*/
/* bLisp_Error                                                                */
/*----------------------------------------------------------------------------*/
bbool bLisp_Error(bLisp_Script *_s, bchar *_file, buint _line, bchar *_text);
/*----------------------------------------------------------------------------*/
/* bLisp_Add                                                                  */
/*----------------------------------------------------------------------------*/
bbool bLisp_Add(bLisp_Stack *_stack);
/*----------------------------------------------------------------------------*/
/* bLisp_Set                                                                  */
/*----------------------------------------------------------------------------*/
bvoid bLisp_Set(bLisp_Stack *_stack, bLisp_Variable *_var);
/*----------------------------------------------------------------------------*/
/* bLisp_FindVariable                                                         */
/*----------------------------------------------------------------------------*/
bLisp_Variable *bLisp_FindVariable(bLisp_VariableBox *_vars, bchar *_name);
/*----------------------------------------------------------------------------*/
/* bLisp_DeleteType                                                           */
/*----------------------------------------------------------------------------*/
bvoid bLisp_DeleteType(bLisp_Type *_type);
/*----------------------------------------------------------------------------*/
/* bLisp_OpenScript                                                           */
/*----------------------------------------------------------------------------*/
bLisp_Script *bLisp_OpenScript(bchar *_text)
{
	bLisp_Script      *script;
	bbool              compile_result;

	bAssert(_text);

	script = malloc(sizeof(bLisp_Script));

	compile_result = bLisp_Compile(script, _text);

	if(!compile_result)
		return 0;

	return script;
}
/*----------------------------------------------------------------------------*/
/* bLisp_Run                                                                  */
/*----------------------------------------------------------------------------*/
bbool bLisp_Run(bLisp_Script *_script, bchar **_result)
{
	buint        i;
	buint        code_size;
	bLisp_Code  *code;
	bLisp_Stack *stack;
	bLisp_Type   result;
		
	code_size = _script->data.code->item_num;
	code      = _script->data.code->data;
	stack     = &_script->stack;

	bLisp_StackCreate(stack);

	for(i = 0; i < code_size; i++)
	{
		switch(code[i].type)
		{
		case bLisp_Push_Code:
			bLisp_StackPushRef(_script, &code[i].ref);
			break;

		case bLisp_Pop_Code:
			bLisp_StackPop(stack);
			break;

		case bLisp_Set_Code:
			bLisp_Set(stack, code[i].ref.value.variable);
			break;

		case bLisp_Add_Code:
			if(!bLisp_Add(stack))
			{
				bLisp_StackDelete(stack);
				return bLisp_Error(_script, code[i].file, code[i].line,
					               L"incompatable arguments types");
			}
			break;
		}
	}

	result = *stack->data;

	bLisp_StackDelete(stack);

	if(_result)
	{
		*_result = bLisp_PrintType(&result);
	}

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_Add                                                                  */
/*----------------------------------------------------------------------------*/
bbool bLisp_Add(bLisp_Stack *_stack)
{
	bLisp_Type *arg_one;
	bLisp_Type *arg_two;
	bLisp_Type result;

	arg_one = bLisp_StackTop(_stack);
	          bLisp_StackPop(_stack);
	arg_two = bLisp_StackTop(_stack);
			  bLisp_StackPop(_stack);

	if(arg_one->type != bLisp_NumberType ||
	   arg_two->type != bLisp_NumberType)
	   return bfalse;

	result.value.number = arg_one->value.number + arg_two->value.number;
	result.type         = bLisp_NumberType;

	bLisp_StackPush(_stack, &result);

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_Set                                                                  */
/*----------------------------------------------------------------------------*/
bvoid bLisp_Set(bLisp_Stack *_stack, bLisp_Variable *_var)
{
	bLisp_Type *arg;

	arg = bLisp_StackTop(_stack);
		  bLisp_StackPop(_stack);

    /* free all dynamic type data */
	bLisp_DeleteType(_var->value);

	*_var->value = *arg;

	bLisp_StackPush(_stack, _var->value);
}
/*----------------------------------------------------------------------------*/
/* bLisp_Compile                                                              */
/*----------------------------------------------------------------------------*/
bbool bLisp_Compile(bLisp_Script *_script, bchar *_code)
{
	bLisp_Token token;
	bLisp_Code  code;

	bLisp_CreateData(&_script->data);

	_script->source = bLisp_CreateSource(_script, _code);

	token     = bLisp_GetNextToken(_script->source);
	code.type = bLisp_Pop_Code;

	while(token.type != bLisp_EofToken)
	{
		if(!bLisp_ParseExpression(_script, &token))
		{
			bLisp_DeleteData(&_script->data);

			return bfalse;
		}
		code.file = token.file;
		code.line = token.line;

		/* add pop code and clear stack */
		bBox_Add(_script->data.code, &code);

		token = bLisp_GetNextToken(_script->source);
	}

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_AddStringExpression                                                  */
/*----------------------------------------------------------------------------*/
bvoid bLisp_AddStringExpression(bLisp_Script *_script, bLisp_Token *_token)
{
	bLisp_Code  code    ;
	bLisp_Type  constant;
	bLisp_Type *const_id;
	bchar      *str_id  ;

	str_id = bPool_Add(_script->data.string, _token->value.string,
		wcslen(_token->value.string) + sizeof(L"\0"));

	constant.type = bLisp_StringType;
	constant.value.string = str_id;

	const_id = bBox_Add(_script->data.constant, &constant);

	code.type               = bLisp_Push_Code;
	code.ref.type           = bLisp_ConstRef;
	code.ref.value.constant = const_id;
	code.file               = _token->file;
	code.line               = _token->line;

	bBox_Add(_script->data.code, &code);
}
/*----------------------------------------------------------------------------*/
/* bLisp_AddNumberExpression                                                  */
/*----------------------------------------------------------------------------*/
bvoid bLisp_AddNumberExpression(bLisp_Data *_data, bLisp_Token *_token)
{
	bLisp_Code     code;
	bLisp_Type constant;

	constant.type         = bLisp_NumberType;
	constant.value.number = _token->value.number;

	code.ref.value.constant = bBox_Add(_data->constant, &constant);
	code.type               = bLisp_Push_Code;
	code.ref.type           = bLisp_ConstRef;
	code.file               = _token->file;
	code.line               = _token->line;

	bBox_Add(_data->code, &code);
}
/*----------------------------------------------------------------------------*/
/* bLisp_PrintType                                                            */
/*----------------------------------------------------------------------------*/
bchar *bLisp_PrintType(bLisp_Type *_type)
{
	bchar *string;
	buint  size;
	bchar *default_type = L"unnown type";

	switch(_type->type)
	{
	case bLisp_NumberType:
		size = 32; /* 32 max number length */
		string = malloc(size * sizeof(bchar));
		swprintf(string, size, L"%g", _type->value.number);
		break;

	case bLisp_StringType:
		size = *((buint *)_type->value.string - 1);
		string = malloc(size);
		memcpy(string, _type->value.string, size);
		break;

	case bLisp_DataType:
		size = 32; /* 32 max pointer length */
		string = malloc(size * sizeof(bchar));
		swprintf(string, size, L"%x", _type->value.data);
		break;

	default:
		size = sizeof(default_type);
		string = malloc(size);
		swprintf(string, size, L"%s", default_type);
		break;
	}

	return string;
}
/*----------------------------------------------------------------------------*/
/* bLisp_Error                                                                */
/*----------------------------------------------------------------------------*/
bbool bLisp_Error(bLisp_Script *_s, bchar *_file, buint _line, bchar *_text)
{
	_s->error.text = _wcsdup(_text);
	_s->error.file = _wcsdup(_file);
	_s->error.line = _line;

	return bfalse; /* always return false */
}
/*----------------------------------------------------------------------------*/
/* bLisp_ParseASMD                                                            */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseASMD(bLisp_Script *_script, bLisp_TokenClass _type)
{
	buint       i;
	bLisp_Token token;
	bLisp_Code  code;

	token = bLisp_GetNextToken(_script->source);

	for(i = 0; token.type != bLisp_RBraceToken; i++)
	{
		if(!bLisp_ParseExpression(_script, &token))
			return bfalse;

		token = bLisp_GetNextToken(_script->source);
	}

	if(i < 2) /* minimum two argumnets */
		return bLisp_Error(_script, token.file, token.line,
		                   L"So few argumnets in '+'");

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

	code.file = token.file;
	code.line = token.line;

	while(i-- != 1)
	{
		bBox_Add(_script->data.code, &code);
	}

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_ParseBind                                                            */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseBind(bLisp_Script *_script)
{
	bLisp_Variable *find_result;
	bLisp_Token     token;
	bLisp_Variable  var;
	bchar          *name_id;

	token = bLisp_GetNextToken(_script->source);

	if(token.type != bLisp_NameToken) 
		return bLisp_Error(_script, token.file, token.line,
			   L"Missing variable name");

	find_result = bLisp_FindVariable(_script->data.variable, token.value.name);

	if(find_result) 
		return bLisp_Error(_script, token.file, token.line, 
			   L"Variable with same name already exists");

	name_id = bPool_Add(_script->data.string, token.value.name,
			  wcslen(token.value.name) + sizeof(L"\0"));

	token = bLisp_GetNextToken(_script->source);
	if(!bLisp_ParseExpression(_script, &token))
		return bfalse;

	var.name  = name_id;
	var.value = bBox_GetLast(_script->data.constant);

	bBox_Add(_script->data.variable, &var);

	token = bLisp_GetNextToken(_script->source);

	if(token.type != bLisp_RBraceToken) 
		return bLisp_Error(_script, token.file, token.line, L"Missing ')'");

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_ParseName                                                            */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseName(bLisp_Script *_script, bLisp_Token *_token)
{
	bLisp_Variable *result;
	bLisp_Code      code;

	result = bLisp_FindVariable(_script->data.variable, _token->value.name);

	if(!result)
		return bLisp_Error(_script, _token->file, _token->line,
		L"Can'r find variable with this name");

	code.ref.type           = bLisp_VariableRef;
	code.ref.value.variable = result;
	code.type               = bLisp_Push_Code;
	code.file               = _token->file;
	code.line               = _token->line;

	bBox_Add(_script->data.code, &code);
	
	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_FindVariable                                                         */
/*----------------------------------------------------------------------------*/
bLisp_Variable *bLisp_FindVariable(bLisp_VariableBox *_vars, bchar *_name)
{
	bLisp_Variable *pos;
	buint i;

	pos = _vars->data;

	for(i = 0; i < _vars->item_num; i++)
	{
		if(wcscmp(pos->name, _name) == 0)
			return pos;
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
/* bLisp_DeleteType                                                           */
/*----------------------------------------------------------------------------*/
bvoid bLisp_DeleteType(bLisp_Type *_type)
{
	if(_type->type == bLisp_NewStringType)
	{
		free(_type->value.new_string);
	}
}
/*----------------------------------------------------------------------------*/
/* bLisp_ParseSet                                                             */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseSet(bLisp_Script *_script)
{
	bLisp_Variable *find_result;
	bLisp_Token token;
	bLisp_Code  code;

	token = bLisp_GetNextToken(_script->source);

	if(token.type != bLisp_NameToken)
	{
		return bLisp_Error(_script, token.file, token.line,
		       L"Wrong set syntax. Missing variable name or constant.");
	}

	find_result = bLisp_FindVariable(_script->data.variable, token.value.name);

	if(!find_result)
	{
		return bLisp_Error(_script, token.file, token.line,
			   L"Unknown variable found");
	}

	code.file = token.file;
	code.line = token.line;
	code.type = bLisp_Set_Code;
	code.ref.type = bLisp_VariableRef;
	code.ref.value.variable = find_result;

	token = bLisp_GetNextToken(_script->source);

	/* calculate and add argument */
	if(!bLisp_ParseExpression(_script, &token)) 
		return bfalse;

	/* add set code */
	bBox_Add(_script->data.code, &code); 

	token = bLisp_GetNextToken(_script->source);

	if(token.type != bLisp_RBraceToken)
	{
		return bLisp_Error(_script, token.file, token.line,
			   L"So many arguments in set expression or missing ')'");
	}

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_ParseFunction                                                        */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseFunction(bLisp_Script *_script)
{
	bLisp_Token token;

	token = bLisp_GetNextToken(_script->source);

	switch(token.type)
	{
	case bLisp_NameToken:
		break;

	case bLisp_SetToken:
		return bLisp_ParseSet(_script);

	case bLisp_AddToken:
	case bLisp_SubToken:
	case bLisp_MulToken:
	case bLisp_DivToken:
		return bLisp_ParseASMD(_script, token.type);

	case bLisp_BindToken:
		return bLisp_ParseBind(_script);

	default:
		return bLisp_Error(_script, token.file, token.line,
			               L"Not valid function name");
	};

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_ParseExpression                                                      */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseExpression(bLisp_Script *_script, bLisp_Token *_token)
{
	switch(_token->type)
	{
	case bLisp_StringToken:
		bLisp_AddStringExpression(_script, _token);
		break;

	case bLisp_NumberToken:
		bLisp_AddNumberExpression(&_script->data, _token);
		break;
	case bLisp_NameToken:
		return bLisp_ParseName(_script, _token);

	case bLisp_LBraceToken:
		return bLisp_ParseFunction(_script);

	case bLisp_ErrorToken:
		return bLisp_Error(_script, _token->file, _token->line, 
						   L"Parse error");

	default:
		return bLisp_Error(_script, _token->file, _token->line,
			               L"Unknown parse error");
	}

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_CreateData                                                           */
/*----------------------------------------------------------------------------*/
bvoid bLisp_CreateData(bLisp_Data *_data)
{
	_data->code     = bBox_Create(sizeof(bLisp_Code), bLisp_DefaultBoxSize);
	_data->constant = bBox_Create(sizeof(bLisp_Type), bLisp_DefaultBoxSize);
	_data->variable = bBox_Create(sizeof(bLisp_Variable), bLisp_DefaultBoxSize);

	_data->string   = bPool_Create(bLisp_DefaultPoolSize);
}
/*----------------------------------------------------------------------------*/
/* bLisp_DeleteData                                                           */
/*----------------------------------------------------------------------------*/
bvoid bLisp_DeleteData(bLisp_Data *_data)
{
	bBox_Delete(_data->code);
	bBox_Delete(_data->constant);
	bBox_Delete(_data->variable);

	bPool_Delete(_data->string);
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
		bLisp_SqueezeToken(&token_pos, &token);
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

	while(include->next)
	{
		include = include->next;

		if(!include->complete)
		{
			_source->current_include = include;
			return include;
		}
	};

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
	case bLisp_ConstRef:
		bLisp_StackPush(&_script->stack, _ref->value.constant);
		break;

	case bLisp_VariableRef:
		bLisp_StackPush(&_script->stack, _ref->value.variable->value);
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
	case bLisp_ConstRef:
		*_ref->value.constant = *bLisp_StackTop(&_script->stack);
		bLisp_StackPop(&_script->stack);
		break;

	}
}
/*----------------------------------------------------------------------------*/
/* bLisp_StackCreate                                                          */
/*----------------------------------------------------------------------------*/
bvoid bLisp_StackCreate(bLisp_Stack *_stack)
{
	_stack->data   = malloc(bLisp_StackSize * sizeof(bLisp_Type));
	_stack->bottom = _stack->data;
	_stack->top    = _stack->data + bLisp_StackSize;
}
/*----------------------------------------------------------------------------*/
/* bLisp_StackDelete                                                          */
/*----------------------------------------------------------------------------*/
bvoid bLisp_StackDelete(bLisp_Stack *_stack)
{
	free(_stack->data);
}
/*----------------------------------------------------------------------------*/
/* bLisp_SqueezeToken                                                         */
/*----------------------------------------------------------------------------*/
bbool bLisp_SqueezeToken(bLisp_TokenPos *_data, bLisp_Token *_token)
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
/* bLisp_CreateSource                                                         */
/*----------------------------------------------------------------------------*/
bLisp_Source *bLisp_CreateSource(bLisp_Script *_script, bchar *_code)
{
	bLisp_Source *source;

	source                  = malloc(sizeof(bLisp_Source));
	source->include         = bLisp_AllocInclude(_code, L"main script");
	source->current_include = source->include;

	/* add file name in string pool */
	bPool_Add(_script->data.string, source->include->file, 
			  wcslen(source->include->file) + sizeof(L"\0"));

	return source;
}
/*----------------------------------------------------------------------------*/
/* bLisp_DeleteSource                                                         */
/*----------------------------------------------------------------------------*/
bvoid bLisp_DeleteSource(bLisp_Source *_source)
{
	bLisp_Include *temp;

	while(_source->include)
	{
		temp = _source->include->next;

		free(_source->include);

		_source->include = temp;
	}

	free(_source);
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
bLisp_TokenPos bLisp_BuildTokenPos(bchar *_b, bchar *_e, bLisp_TokenClass _t)
{
	bLisp_TokenPos token;
	
	token.begin = _b;
	token.end   = _e;
	token.type  = _t;

	return token;
}