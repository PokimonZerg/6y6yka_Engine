/*----------------------------------------------------------------------------*/
/*                              6y6yka Engine                                 */
/*----------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "b_assert.h"
#include "b_stack.h"
#include "b_box.h"
#include "b_pool.h"
#include "b_lisp.h"
/*----------------------------------------------------------------------------*/
/* Important defines                                                          */
/*----------------------------------------------------------------------------*/
#define bLisp_DefaultBoxSize  512
#define bLisp_DefaultPoolSize 512
#define bLisp_BoxNotFound     -1
#define bLisp_StackSize       1024
#define bLisp_LocalVarMax     32
/*----------------------------------------------------------------------------*/
/* bLisp_Code                                                                 */
/*----------------------------------------------------------------------------*/
typedef struct bLisp_Code bLisp_Code;
/*----------------------------------------------------------------------------*/
/* bLisp_FunctionClass                                                        */
/*----------------------------------------------------------------------------*/
enum bLisp_FunctionClass
{
	bLisp_UsualFunction,
	bLisp_NativeFunction
};

typedef enum bLisp_FunctionClass bLisp_FunctionClass;
/*----------------------------------------------------------------------------*/
/* bLisp_FunctionValue                                                        */
/*----------------------------------------------------------------------------*/
union bLisp_FunctionValue
{
	bLisp_Code    *usual_ptr;
	bLisp_CallBack native_ptr;
};

typedef union bLisp_FunctionValue bLisp_FunctionValue;
/*----------------------------------------------------------------------------*/
/* bLisp_Function                                                             */
/*----------------------------------------------------------------------------*/
struct bLisp_Function
{
	buint               arg_num;
	bLisp_FunctionClass type   ;
	bLisp_FunctionValue value  ;
};

typedef struct bLisp_Function bLisp_Function;
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
	bfloat          number    ;
	bchar          *string    ;
	bchar          *new_string;
	bvoid          *data      ;
	bLisp_Function *function  ;
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
/* bLisp_VariableClass                                                        */
/*----------------------------------------------------------------------------*/
enum bLisp_VariableClass
{
	bLisp_GlobalVariable,
	bLisp_LocalVariable
};

typedef enum bLisp_VariableClass bLisp_VariableClass;
/*----------------------------------------------------------------------------*/
/* bLisp_VariableValue                                                        */
/*----------------------------------------------------------------------------*/
union bLisp_VariableValue
{
	bLisp_Type *gloabal;
	buint       local;
};

typedef union bLisp_VariableValue bLisp_VariableValue;
/*----------------------------------------------------------------------------*/
/* bLisp_Variable                                                             */
/*----------------------------------------------------------------------------*/
struct bLisp_Variable
{
	bLisp_VariableClass type;
	bLisp_VariableValue value;
	bchar              *name;
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
/* bLisp_CallInfo                                                             */
/*----------------------------------------------------------------------------*/
struct bLisp_CallInfo
{
	bLisp_Code *r_ptr; /* return pointer */
	bLisp_Type *frame; /* stack frame    */
	//buint       arg  ; /* arg number     */
};

typedef struct bLisp_CallInfo bLisp_CallInfo;
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
/* bLisp_CallStack                                                            */
/*----------------------------------------------------------------------------*/
struct bLisp_CallStack
{
	bLisp_CallInfo *data;
	bLisp_CallInfo *bottom;
	bLisp_CallInfo *top;
};


typedef struct bLisp_CallStack bLisp_CallStack;
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
	bLisp_Set_Code,  /* typical set code                                      */
	bLisp_Func_Code, /* function begin                                        */
	bLisp_Call_Code, /* function call                                         */
	bLisp_Ret_Code   /* return from function                                  */
};

typedef enum bLisp_CodeClass bLisp_CodeClass;
/*----------------------------------------------------------------------------*/
/* bLisp_RefClass                                                             */
/*----------------------------------------------------------------------------*/
enum bLisp_RefClass
{
	bLisp_ConstRef,    /* constant ptr     */
	bLisp_VariableRef, /* variable ptr     */
	bLisp_FunctionRef, /* function end ptr */
	bLisp_CallInfoRef, /* call info struct */
	bLisp_ArgNumRef    /* argument number  */
};

typedef enum bLisp_RefClass bLisp_RefClass;
/*----------------------------------------------------------------------------*/
/* bLisp_RefValue                                                             */
/*----------------------------------------------------------------------------*/
union bLisp_RefValue
{
	bLisp_Type     *constant;
	bLisp_Variable *variable;
	bLisp_Code     *function;
	bLisp_CallInfo *callinfo;
	buint           arg_num ;
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
	bchar *data    ;
	buint  max_size;
	buint  size    ;
};

typedef struct bLisp_StringPool bLisp_StringPool;
/*----------------------------------------------------------------------------*/
/* bLisp_LocalBlock                                                           */
/*----------------------------------------------------------------------------*/
struct bLisp_LocalBlock
{
	bLisp_Variable *local_var[bLisp_LocalVarMax];
	buint           count                       ;
};

typedef struct bLisp_LocalBlock bLisp_LocalBlock;
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
/* bLisp_CallInfoBox                                                          */
/*----------------------------------------------------------------------------*/
struct bLisp_CallInfoBox
{
	bLisp_CallInfo *data;
	buint           item_size;
	buint           item_num;
	buint           item_num_max;
};

typedef struct bLisp_CallInfoBox bLisp_CallInfoBox;
/*----------------------------------------------------------------------------*/
/* bLisp_FunctionBox                                                          */
/*----------------------------------------------------------------------------*/
struct bLisp_FunctionBox
{
	bLisp_Function *data;
	buint       item_size;
	buint       item_num;
	buint       item_num_max;
};

typedef struct bLisp_FunctionBox bLisp_FunctionBox;
/*----------------------------------------------------------------------------*/
/* bLisp_Data                                                                 */
/*----------------------------------------------------------------------------*/
struct bLisp_Data
{
	bLisp_VariableBox *variable;
	bLisp_FunctionBox *function;
	bLisp_ConstBox    *constant;
	bLisp_CallInfoBox *callinfo;
	bLisp_CodeBox     *code;

	bLisp_StringPool  *string;
};

typedef struct bLisp_Data bLisp_Data;
/*----------------------------------------------------------------------------*/
/* bLisp_LocalBlockList                                                       */
/*----------------------------------------------------------------------------*/
typedef struct bLisp_LocalBlockList bLisp_LocalBlockList;

struct bLisp_LocalBlockList
{
	bLisp_LocalBlock      item;
	bLisp_LocalBlockList *next;
};
/*----------------------------------------------------------------------------*/
/* bLisp_Script                                                               */
/*----------------------------------------------------------------------------*/
struct bLisp_Script
{ 
	/* runtime data */
	bLisp_Stack     *stack;

	/* script data */
	bLisp_Data *data ;     

    /* last error data */
	bLisp_ErrorInfo error;    

	/* compilation data */
	bLisp_Source         *source;
	bLisp_LocalBlockList *local_block;
	bLisp_Type           *stack_frame;
};
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
/* bLisp_ParseLambda                                                          */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseLambda(bLisp_Script *_script);
/*----------------------------------------------------------------------------*/
/* bLisp_ParseCall                                                            */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseCall(bLisp_Script *_script, bLisp_Token *_token);
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
bLisp_Data *bLisp_CreateData();
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
/* bLisp_Push                                                                 */
/*----------------------------------------------------------------------------*/
bvoid bLisp_Push(bLisp_Script *_script, bLisp_Ref *_ref);
/*----------------------------------------------------------------------------*/
/* bLisp_Set                                                                  */
/*----------------------------------------------------------------------------*/
bvoid bLisp_Set(bLisp_Script *_script, bLisp_Variable *_var);
/*----------------------------------------------------------------------------*/
/* bLisp_Call                                                                 */
/*----------------------------------------------------------------------------*/
bvoid bLisp_Call(bLisp_Script *_script, bLisp_Code *_code);
/*----------------------------------------------------------------------------*/
/* bLisp_FindVariable                                                         */
/*----------------------------------------------------------------------------*/
bLisp_Variable *bLisp_FindVariable(bLisp_VariableBox *_vars, bchar *_name);
/*----------------------------------------------------------------------------*/
/* bLisp_FindLocalVariable                                                    */
/*----------------------------------------------------------------------------*/
bLisp_Variable *bLisp_FindLocalVariable(bLisp_LocalBlock *_block, bchar *_name);
/*----------------------------------------------------------------------------*/
/* bLisp_DeleteType                                                           */
/*----------------------------------------------------------------------------*/
bvoid bLisp_DeleteType(bLisp_Type *_type);
/*----------------------------------------------------------------------------*/
/* bLisp_CreateLocalBlock                                                     */
/*----------------------------------------------------------------------------*/
bLisp_LocalBlock *bLisp_CreateLocalBlock(bLisp_Script *_script);
/*----------------------------------------------------------------------------*/
/* bLisp_DeleteLocalBlock                                                     */
/*----------------------------------------------------------------------------*/
bvoid bLisp_DeleteLocalBlock(bLisp_Script *_script);
/*----------------------------------------------------------------------------*/
/* bLisp_DeleteAllLocalBlocks                                                 */
/*----------------------------------------------------------------------------*/
bvoid bLisp_DeleteAllLocalBlocks(bLisp_Script *_script);
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
	bLisp_Code      *code, *end;
	bLisp_CallInfo  *cur_info;
	bLisp_Type      *cur_type, result;
		
	code      = _script->data->code->data;
	end       = code + _script->data->code->item_num;

	_script->stack = bStack_Create(sizeof(bLisp_Type), bLisp_StackSize);

	while(code < end)
	{
		switch(code->type)
		{
		case bLisp_Push_Code:
			bLisp_Push(_script, &code->ref);
			break;

		case bLisp_Pop_Code:
			bStack_Pop(_script->stack);
			break;

		case bLisp_Func_Code:
			code = code->ref.value.function;
			break;

		case bLisp_Ret_Code:
			cur_info = (bLisp_CallInfo *)(_script->stack->data - 2);
			cur_type = _script->stack_frame - 1;

			*cur_type = *(_script->stack->data - 1);
			_script->stack->data = _script->stack_frame;
			_script->stack_frame = cur_info->frame;
			code = cur_info->r_ptr;
			break;

		case bLisp_Call_Code:
			cur_info = (bLisp_CallInfo *)(_script->stack->data - 1);
			/* save current frame */
			cur_info->frame = _script->stack_frame;

			cur_type = (_script->stack->data - code->ref.value.arg_num - 1);
			_script->stack_frame = cur_type;
			code = (cur_type - 1)->value.function->value.usual_ptr;
			break;

		case bLisp_Set_Code:
			bLisp_Set(_script, code->ref.value.variable);
			break;

		case bLisp_Add_Code:
			if(!bLisp_Add(_script->stack))
			{
				bStack_Delete(_script->stack);
				return bLisp_Error(_script, code->file, code->line,
					               L"incompatable arguments types");
			}
			break;
		}

		code++;
	}

	result = *_script->stack->data;

	bStack_Delete(_script->stack);

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
	bLisp_Type *one, *two, result;

	one = bStack_Pop(_stack);
	two = bStack_Pop(_stack);

	if(one->type != bLisp_NumberType || two->type != bLisp_NumberType)
		return bfalse;

	result.value.number = one->value.number + two->value.number;
	result.type         = bLisp_NumberType;

	bStack_Push(_stack, &result);

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_Push                                                                 */
/*----------------------------------------------------------------------------*/
bvoid bLisp_Push(bLisp_Script *_script, bLisp_Ref *_ref)
{
	bLisp_Type     *item;

	if(_ref->type == bLisp_VariableRef)
	{
		if(_ref->value.variable->type == bLisp_GlobalVariable)
		{
			item = _ref->value.variable->value.gloabal;
		}
		else
		{
			item = _script->stack_frame + _ref->value.variable->value.local;
		}
	}
	else if(_ref->type == bLisp_CallInfoRef)
	{
		item = (bLisp_Type *)_ref->value.callinfo;
	}
	else
	{
		item = _ref->value.constant;
	}

	bStack_Push(_script->stack, item);
}
/*----------------------------------------------------------------------------*/
/* bLisp_Set                                                                  */
/*----------------------------------------------------------------------------*/
bvoid bLisp_Set(bLisp_Script *_script, bLisp_Variable *_var)
{
	bLisp_Type *arg, *var;

	arg = bStack_Pop(_script->stack);

	if(_var->type == bLisp_GlobalVariable)
	{
		var = _var->value.gloabal;
	}
	else
	{
		var = _script->stack_frame + _var->value.local;
	}

	bLisp_DeleteType(var);

	*var = *arg;
	
	bStack_Push(_script->stack, var);
}
/*----------------------------------------------------------------------------*/
/* bLisp_Compile                                                              */
/*----------------------------------------------------------------------------*/
bbool bLisp_Compile(bLisp_Script *_script, bchar *_code)
{
	bLisp_Token token;
	bLisp_Code  code;

	_script->data        = bLisp_CreateData();
	_script->source      = bLisp_CreateSource(_script, _code);
	_script->local_block = 0; /* init local block list */

	token     = bLisp_GetNextToken(_script->source);
	code.type = bLisp_Pop_Code;

	while(token.type != bLisp_EofToken)
	{
		if(!bLisp_ParseExpression(_script, &token))
		{
			bLisp_DeleteData(_script->data);
			bLisp_DeleteSource(_script->source);
			bLisp_DeleteAllLocalBlocks(_script);

			return bfalse;
		}
		code.file = token.file;
		code.line = token.line;

		/* add pop code and clear stack */
		bBox_Add(_script->data->code, &code);

		token = bLisp_GetNextToken(_script->source);
	}

	bLisp_DeleteSource(_script->source);
	bLisp_DeleteAllLocalBlocks(_script);

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

	str_id = bPool_Add(_script->data->string, _token->value.string,
		wcslen(_token->value.string) + sizeof(L"\0"));

	constant.type = bLisp_StringType;
	constant.value.string = str_id;

	const_id = bBox_Add(_script->data->constant, &constant);

	code.type               = bLisp_Push_Code;
	code.ref.type           = bLisp_ConstRef;
	code.ref.value.constant = const_id;
	code.file               = _token->file;
	code.line               = _token->line;

	bBox_Add(_script->data->code, &code);
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
		bBox_Add(_script->data->code, &code);
	}

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_ParseBind                                                            */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseBind(bLisp_Script *_script)
{
	bLisp_Token     token;
	bLisp_Type      type, *type_id;
	bLisp_Code      code;
	bLisp_Variable  var, *find_result, *var_id;
	bchar          *name_id;

	token = bLisp_GetNextToken(_script->source);

	if(token.type != bLisp_NameToken)
	{
		return bLisp_Error(_script, token.file, token.line,
			   L"Missing variable name");
	}

	find_result = bLisp_FindVariable(_script->data->variable, token.value.name);

	/* if variable already exists */
	if(find_result)
	{
		return bLisp_Error(_script, token.file, token.line, 
			   L"Variable with same name already exists");
	}

	/* preprae new empty value */
	type.type         = bLisp_NumberType;
	type.value.number = 0;
	type_id           = bBox_Add(_script->data->constant, &type);

	/* add new variable name */
	name_id = bPool_Add(_script->data->string, token.value.name,
			  wcslen(token.value.name) * sizeof(bchar) + sizeof(L"\0"));

	/* prepare new variable */
	var.name          = name_id;
	var.type          = bLisp_GlobalVariable;
	var.value.gloabal = type_id;
	var_id            = bBox_Add(_script->data->variable, &var);

	/* prepare set code */
	code.file               = token.file;
	code.line               = token.line;
	code.type               = bLisp_Set_Code;
	code.ref.type           = bLisp_VariableRef;
	code.ref.value.variable = var_id;

	token = bLisp_GetNextToken(_script->source);

	/* parse and push variable value */
	if(!bLisp_ParseExpression(_script, &token))
		return bfalse;

	/* add set code */
	bBox_Add(_script->data->code, &code);

	/* get right brace token */
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

	result = bLisp_FindLocalVariable(&_script->local_block->item,
		                              _token->value.name);
	if(!result)
	{
		result = bLisp_FindVariable(_script->data->variable,
			                        _token->value.name);
		if(!result)
			return bLisp_Error(_script, _token->file, _token->line,
				   L"Can'r find variable with this name");
	}

	code.ref.type           = bLisp_VariableRef;
	code.ref.value.variable = result;
	code.type               = bLisp_Push_Code;
	code.file               = _token->file;
	code.line               = _token->line;

	bBox_Add(_script->data->code, &code);
	
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
		if(wcscmp(pos[i].name, _name) == 0)
			return pos;
	}

	return 0;
}
/*----------------------------------------------------------------------------*/
/* bLisp_FindLocalVariable                                                    */
/*----------------------------------------------------------------------------*/
bLisp_Variable *bLisp_FindLocalVariable(bLisp_LocalBlock *_block, bchar *_name)
{
	buint i;

	/* if no local variables */
	if(!_block) return 0;

	for(i = 0; i < _block->count; i++)
	{
		if(wcscmp(_block->local_var[i]->name, _name) == 0)
			return _block->local_var[i];
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
/* bLisp_CreateLocalBlock                                                     */
/*----------------------------------------------------------------------------*/
bLisp_LocalBlock *bLisp_CreateLocalBlock(bLisp_Script *_script)
{
	bLisp_LocalBlockList *new_block;

	new_block = malloc(sizeof(bLisp_LocalBlockList));
	new_block->next = _script->local_block;
	new_block->item.count = 0;

	_script->local_block = new_block;

	return &new_block->item;
}
/*----------------------------------------------------------------------------*/
/* bLisp_DeleteLocalBlock                                                     */
/*----------------------------------------------------------------------------*/
bvoid bLisp_DeleteLocalBlock(bLisp_Script *_script)
{
	bLisp_LocalBlockList *temp;

	bAssert(_script->local_block);

	temp = _script->local_block->next;

	free(_script->local_block);

	_script->local_block = temp;
}
/*----------------------------------------------------------------------------*/
/* bLisp_DeleteAllLocalBlocks                                                 */
/*----------------------------------------------------------------------------*/
bvoid bLisp_DeleteAllLocalBlocks(bLisp_Script *_script)
{
	while(_script->local_block)
	{
		bLisp_DeleteLocalBlock(_script);
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

	find_result = bLisp_FindVariable(_script->data->variable, token.value.name);

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
	bBox_Add(_script->data->code, &code); 

	token = bLisp_GetNextToken(_script->source);

	if(token.type != bLisp_RBraceToken)
	{
		return bLisp_Error(_script, token.file, token.line,
			   L"So many arguments in set expression or missing ')'");
	}

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_ParseLambda                                                          */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseLambda(bLisp_Script *_script)
{
	bLisp_Token       token;
	bLisp_Type        type, *type_id;
	bLisp_Code        code, *end_code;
	bLisp_Function    function, *function_id;
	bLisp_LocalBlock *local_block;
	bLisp_Variable   *find_result, new_var, *var_id;
	bchar            *name_id;

	/* start parse arg section */
	token = bLisp_GetNextToken(_script->source);

	if(token.type != bLisp_LBraceToken)
	{
		bLisp_Error(_script, token.file, token.line, L"Missing argument list");
	}

	token            = bLisp_GetNextToken(_script->source);
	function.arg_num = 0;
	function.type    = bLisp_UsualFunction;

	local_block = bLisp_CreateLocalBlock(_script);

	while(token.type != bLisp_RBraceToken)
	{
		if(token.type != bLisp_NameToken)
		{
			bLisp_Error(_script, token.file, token.line,
				        L"Function argument list can contains only names");
		}

		find_result = bLisp_FindLocalVariable(local_block, token.value.name);

		if(find_result)
		{
			bLisp_Error(_script, token.file, token.line,
				        L"Duplicate name in argument list");
		}

		/* add new variable name */
		name_id = bPool_Add(_script->data->string, token.value.name,
			      wcslen(token.value.name) * sizeof(bchar) + sizeof(L'\0'));

		/* add new local variable */
		new_var.name = name_id;
		new_var.type = bLisp_LocalVariable;
		new_var.value.local = local_block->count;

		var_id = bBox_Add(_script->data->variable, &new_var);

		/* add new variable to local var list */
		local_block->local_var[local_block->count] = var_id;
		local_block->count++;
		
		function.arg_num++;

		token = bLisp_GetNextToken(_script->source);
	}
	/* end parse arg section */

	/* prepare func code. It marks function strat */
	code.file = token.file;
	code.line = token.line;
	code.type = bLisp_Func_Code;

	function.value.usual_ptr = bBox_Add(_script->data->code, &code);

	/* now parse function body */
	token = bLisp_GetNextToken(_script->source);

	while(token.type != bLisp_RBraceToken)
	{
		/* parse one expression */
		if(!bLisp_ParseExpression(_script, &token))
			return bfalse;

		token = bLisp_GetNextToken(_script->source);

		/* stupid double check */
		if(token.type != bLisp_RBraceToken)
		{
			/* prepare pop code */
			code.file = token.file;
			code.line = token.line;
			code.type = bLisp_Pop_Code;

			/* add pop code */
			bBox_Add(_script->data->code, &code);
		}
	}

	/* prepare ret code */
	code.file = token.file;
	code.line = token.line;
	code.type = bLisp_Ret_Code;

	end_code = bBox_Add(_script->data->code, &code);

	/* mark function end */
	function.value.usual_ptr->ref.type = bLisp_FunctionRef;
	function.value.usual_ptr->ref.value.function = end_code;

	/* add new function */
	function_id = bBox_Add(_script->data->function, &function);

	/* prepare new type with function value */
	type.type           = bLisp_FunctionType;
	type.value.function = function_id;

	type_id = bBox_Add(_script->data->constant, &type);

	/* prepare push code */
	code.file               = token.file;
	code.line               = token.line;
	code.type               = bLisp_Push_Code;
	code.ref.type           = bLisp_ConstRef;
	code.ref.value.constant = type_id;

	bBox_Add(_script->data->code, &code);

	/* don't forget delete it */
	bLisp_DeleteLocalBlock(_script);

	return btrue;
}
/*----------------------------------------------------------------------------*/
/* bLisp_ParseCall                                                            */
/*----------------------------------------------------------------------------*/
bbool bLisp_ParseCall(bLisp_Script *_script, bLisp_Token *_token)
{
	bLisp_Code     code,     *code_id    ;
	bLisp_CallInfo callinfo, *callinfo_id;
	bLisp_Token    token                 ;
	buint          i                     ;

	/* push function variable */
	if(!bLisp_ParseExpression(_script, _token))
		return bfalse;

	/* now we push function arg */
	token = bLisp_GetNextToken(_script->source);

	for(i = 0; token.type != bLisp_RBraceToken; i++)
	{
		if(!bLisp_ParseExpression(_script, &token))
			return bfalse;

		token = bLisp_GetNextToken(_script->source);
	}

	/* add new callinfo */
	callinfo_id = bBox_Add(_script->data->callinfo, &callinfo);

	/* build call info code */
	code.file               = _token->file;
	code.line               = _token->line;
	code.type               = bLisp_Push_Code;
	code.ref.type           = bLisp_CallInfoRef;
	code.ref.value.callinfo = callinfo_id;

	bBox_Add(_script->data->code, &code);

	/* buind call code */
	code.file = token.file;
	code.line = token.line;
	code.type = bLisp_Call_Code;
	//code.ref.type = bLisp_CallInfoRef;
	code.ref.type = bLisp_ArgNumRef;
	code.ref.value.arg_num = i;

	code_id = bBox_Add(_script->data->code, &code);

	callinfo_id->r_ptr = code_id;

	/* buind call info structure */
	//callinfo.arg  = i;

	//code_id     = bBox_Add(_script->data->code, &code);
	//callinfo_id = bBox_Add(_script->data->callinfo, &callinfo);

	//code_id->ref.value.callinfo = callinfo_id;
	//callinfo_id->r_ptr          = code_id;

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
	case bLisp_LBraceToken:
		return bLisp_ParseCall(_script, &token);

	case bLisp_LambdaToken:
		return bLisp_ParseLambda(_script);

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
		bLisp_AddNumberExpression(_script->data, _token);
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
bLisp_Data *bLisp_CreateData()
{
	bLisp_Data *data = malloc(sizeof(bLisp_Data));

	data->code     = bBox_Create(sizeof(bLisp_Code), bLisp_DefaultBoxSize);
	data->constant = bBox_Create(sizeof(bLisp_Type), bLisp_DefaultBoxSize);
	data->variable = bBox_Create(sizeof(bLisp_Variable), bLisp_DefaultBoxSize);
	data->function = bBox_Create(sizeof(bLisp_Function), bLisp_DefaultBoxSize);
	data->callinfo = bBox_Create(sizeof(bLisp_CallInfo), bLisp_DefaultBoxSize);

	data->string   = bPool_Create(bLisp_DefaultPoolSize);

	return data;
}
/*----------------------------------------------------------------------------*/
/* bLisp_DeleteData                                                           */
/*----------------------------------------------------------------------------*/
bvoid bLisp_DeleteData(bLisp_Data *_data)
{
	bBox_Delete(_data->code);
	bBox_Delete(_data->constant);
	bBox_Delete(_data->variable);
	bBox_Delete(_data->function);
	bBox_Delete(_data->callinfo);

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
	bPool_Add(_script->data->string, source->include->file, 
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