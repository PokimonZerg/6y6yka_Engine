/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>
#include "b_file.h"
#include "b_define.h"
#include "b_unicode.h"
#include "b_lisp.h"
/*------------------------------------------------------------------------------------------------*/
/* Important defines                                                                              */
/*------------------------------------------------------------------------------------------------*/
#define bLisp_DefaultBoxSize  16
#define bLisp_DefaultPoolSize 2
#define bLisp_BoxNotFound     -1
#define bLisp_StackSize       1024
#define bLisp_LocalVarMax     32
#define bLisp_NotFound        (bLisp_Variable *)UINT_MAX
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Code                                                                                     */
/*------------------------------------------------------------------------------------------------*/
typedef struct bLisp_Code bLisp_Code;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FunctionClass                                                                            */
/*------------------------------------------------------------------------------------------------*/
enum bLisp_FunctionClass
{
	bLisp_UsualFunction,
	bLisp_NativeFunction
};

typedef enum bLisp_FunctionClass bLisp_FunctionClass;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FunctionValue                                                                            */
/*------------------------------------------------------------------------------------------------*/
union bLisp_FunctionValue
{
	bLisp_Code    *usual_ptr;
	bLisp_CallBack native_ptr;
};

typedef union bLisp_FunctionValue bLisp_FunctionValue;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Function                                                                                 */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_Function
{
	buint               arg_num;
	bLisp_FunctionClass type   ;
	bLisp_FunctionValue value  ;
};

typedef struct bLisp_Function bLisp_Function;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_TypeClass                                                                                */
/*------------------------------------------------------------------------------------------------*/
enum bLisp_TypeClass
{
	bLisp_NumberType,    /* number              */
	bLisp_StringType,    /* string        */
	bLisp_DataType,      /* any ptr             */
	bLisp_FunctionType,  /* function ptr        */
	bLisp_PtrType,       /* ptr to list element */
	bLisp_ArrayType      /* array               */
};

typedef enum bLisp_TypeClass bLisp_TypeClass;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_TypeValue                                                                                */
/*------------------------------------------------------------------------------------------------*/
typedef struct bLisp_Array bLisp_Array;
typedef struct bLisp_Type bLisp_Type;

union bLisp_TypeValue
{
	bfloat                 number  ;
	bchar                 *string  ;
	bvoid                 *data    ;
	bLisp_Function        *function;
	bLisp_Array           *array   ;
	bLisp_Type            *type    ;
};

typedef union bLisp_TypeValue bLisp_TypeValue;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Type                                                                                     */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_Type
{
	bLisp_TypeClass type ;
	bLisp_TypeValue value;
};
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Array                                                                                    */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_Array
{
	bLisp_Type *ptr;
	buint       size;
};
/*------------------------------------------------------------------------------------------------*/
/* bLisp_VariableClass                                                                            */
/*------------------------------------------------------------------------------------------------*/
enum bLisp_VariableClass
{
	bLisp_GlobalVariable,
	bLisp_LocalVariable
};

typedef enum bLisp_VariableClass bLisp_VariableClass;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_VariableValue                                                                            */
/*------------------------------------------------------------------------------------------------*/
union bLisp_VariableValue
{
	bLisp_Type *gloabal;
	buint       local;
};

typedef union bLisp_VariableValue bLisp_VariableValue;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Variable                                                                                 */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_Variable
{
	bLisp_VariableClass type;
	bLisp_VariableValue value;
	bchar              *name;
};

typedef struct bLisp_Variable bLisp_Variable;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ArrayBox                                                                                 */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_ArrayBox
{
	bLisp_Array *data;
	buint        item_size;
	buint        item_num;
	buint        item_num_max;
};

typedef struct bLisp_ArrayBox bLisp_ArrayBox;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_VariableBox                                                                              */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_VariableBox
{
	bLisp_Variable *data;
	buint           item_size;
	buint           item_num;
	buint           item_num_max;
};

typedef struct bLisp_VariableBox bLisp_VariableBox;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ConstBox                                                                                 */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_ConstBox
{
	bLisp_Type *data;
	buint       item_size;
	buint       item_num;
	buint       item_num_max;
};

typedef struct bLisp_ConstBox bLisp_ConstBox;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Box                                                                                      */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_Box
{
	bbyte *data;
	buint  item_size;
	buint  item_num;
	buint  item_num_max;
};

typedef struct bLisp_Box bLisp_Box;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_CallInfo                                                                                 */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_CallInfo
{
	bLisp_Code *r_ptr; /* return pointer */
	bLisp_Type *frame; /* stack frame    */
};

typedef struct bLisp_CallInfo bLisp_CallInfo;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Stack                                                                                    */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_Stack
{
	bLisp_Type *data;
	bLisp_Type *bottom;
	bLisp_Type *top;
};
typedef struct bLisp_Stack bLisp_Stack;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_CodeClass                                                                                */
/*------------------------------------------------------------------------------------------------*/
enum bLisp_CodeClass
{
	bLisp_PushC_Code,      /* push constant                                         */
	bLisp_PushG_Code,      /* push global variable                                  */
	bLisp_PushL_Code,      /* push local variable                                   */
	bLisp_PushI_Code,      /* push callinfo                                         */
	bLisp_Pop_Code,        /* pop type away                                         */
	bLisp_Add_Code,        /* add two types on stack and push result                */
	bLisp_Sub_Code,        /* sub two types on stack and push result                */
	bLisp_Mul_Code,        /* mul two types on stack and push result                */
	bLisp_Div_Code,        /* div two types on stack and push result                */
	bLisp_If_Code,         /* test type on steck                                    */
	bLisp_Goto_Code,       /* goto new code position                                */
	bLisp_SetG_Code,       /* set global variable                                   */
	bLisp_SetL_Code,       /* set local variable                                    */
	bLisp_SetQ_Code,       /* classic set                                           */
	bLisp_Get_Code,        /* universal get                                         */
	bLisp_GetSet_Code,     /* get for set                                           */
	bLisp_Call_Code,       /* function call                                         */
	bLisp_Run_Code,        /* run native function                                   */
	bLisp_Ret_Code,        /* return from function                                  */
	bLisp_Cmp_Code,        /* compare type with null                                */
	bLisp_Equal_Code,      /* compare two types                                     */
	bLisp_Less_Code,       /* simply <                                              */
	bLisp_More_Code,       /* simply >                                              */
	bLisp_NotEqual_Code    /* compare two type and ! result                         */
};

typedef enum bLisp_CodeClass bLisp_CodeClass;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_RefClass                                                                                 */
/*------------------------------------------------------------------------------------------------*/
enum bLisp_RefClass
{
	bLisp_ConstRef,    /* constant ptr     */
	bLisp_VariableRef, /* variable ptr     */
	bLisp_CallInfoRef, /* call info struct */
	bLisp_ArgNumRef,   /* argument number  */
	bLisp_CodeRef      /* code ptr         */
};

typedef enum bLisp_RefClass bLisp_RefClass;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_RefValue                                                                                 */
/*------------------------------------------------------------------------------------------------*/
union bLisp_RefValue
{
	bLisp_Type     *constant;
	bLisp_Variable *variable;
	bLisp_CallInfo *callinfo;
	bLisp_Code     *code    ;
	buint           arg_num ;
};

typedef union bLisp_RefValue bLisp_RefValue;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Ref                                                                                      */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_Ref
{
	bLisp_RefClass type;
	bLisp_RefValue value;
};

typedef struct bLisp_Ref bLisp_Ref;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Code                                                                                     */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_Code
{
	bLisp_CodeClass type;
	bLisp_Ref       ref ;
	buint           line;
	bchar          *file;
};

typedef struct bLisp_Code bLisp_Code;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_TokenClass                                                                               */
/*------------------------------------------------------------------------------------------------*/
enum bLisp_TokenClass
{
	bLisp_NameToken,    /* variable name  */
	bLisp_DigitToken,   /* integer        */
	bLisp_FloatToken,   /* float          */
	bLisp_NumberToken,  /* number         */
	bLisp_StringToken,  /* unicode string */
	bLisp_CommentToken, /* pure comment   */
	bLisp_NativeToken,  /* native         */
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
	bLisp_LambdaToken,  /* lambda         */
	bLisp_ArrayToken,   /* array          */
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
/*------------------------------------------------------------------------------------------------*/
/* bLisp_TokenValue                                                                               */
/*------------------------------------------------------------------------------------------------*/
union bLisp_TokenValue
{
	bfloat  number; 
	bchar  *string;
	bchar  *name;
};

typedef union bLisp_TokenValue bLisp_TokenValue;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Token                                                                                    */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_Token
{
	bLisp_TokenClass type ;
	bLisp_TokenValue value;

	buint            line ;
	bchar           *file ;
};

typedef struct bLisp_Token bLisp_Token;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_TokenPos                                                                                 */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_TokenPos
{
	bchar           *begin;
	bchar           *end  ;

	bLisp_TokenClass type ;
};

typedef struct bLisp_TokenPos bLisp_TokenPos;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_TokenClassList                                                                           */
/*------------------------------------------------------------------------------------------------*/
typedef struct bLisp_TokenClassList bLisp_TokenClassList;

struct bLisp_TokenClassList
{
	bLisp_TokenClass      type;
	bLisp_TokenClassList *next;
};
/*------------------------------------------------------------------------------------------------*/
/* bLisp_LexerState                                                                               */
/*------------------------------------------------------------------------------------------------*/
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
	bLisp_N_Lexer_State,
	bLisp_Na_Lexer_State,
	bLisp_Nat_Lexer_State,
	bLisp_Nati_Lexer_State,
	bLisp_Nativ_Lexer_State,
	bLisp_Native_Lexer_State,
	bLisp_A_Lexer_State,
	bLisp_Ar_Lexer_State,
	bLisp_Arr_Lexer_State,
	bLisp_Arra_Lexer_State,
	bLisp_Array_Lexer_State,
	bLisp_Bi_Lexer_State,
	bLisp_Bin_Lexer_State,
	bLisp_Bind_Lexer_State,
	bLisp_L_Lexer_State,
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
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Include                                                                                  */
/*------------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------------*/
/*  bLisp_Source                                                                                  */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_Source
{
	bLisp_Include *include;
	bLisp_Include *current_include;
};

typedef struct bLisp_Source bLisp_Source;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_StringPool                                                                               */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_StringPool
{
	bchar *data    ;
	buint  max_size;
	buint  size    ;
};

typedef struct bLisp_StringPool bLisp_StringPool;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Block                                                                                    */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_Block
{
	bLisp_Variable *local_var[bLisp_LocalVarMax];
	buint           count                       ;
};

typedef struct bLisp_Block bLisp_Block;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ErrorInfo                                                                                */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_ErrorInfo
{
	bchar *text;
	bchar *file;
	buint  line;
};

typedef struct bLisp_ErrorInfo bLisp_ErrorInfo;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_CodeBox                                                                                  */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_CodeBox
{
	bLisp_Code *data;
	buint       item_size;
	buint       item_num;
	buint       item_num_max;
};

typedef struct bLisp_CodeBox bLisp_CodeBox;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_CallInfoBox                                                                              */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_CallInfoBox
{
	bLisp_CallInfo *data;
	buint           item_size;
	buint           item_num;
	buint           item_num_max;
};

typedef struct bLisp_CallInfoBox bLisp_CallInfoBox;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FunctionBox                                                                              */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_FunctionBox
{
	bLisp_Function *data;
	buint       item_size;
	buint       item_num;
	buint       item_num_max;
};

typedef struct bLisp_FunctionBox bLisp_FunctionBox;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FindResult                                                                               */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_FindResult
{
	bbool           result  ;
	bbool           local   ;
	bLisp_Variable *variable;
};

typedef struct bLisp_FindResult bLisp_FindResult;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Data                                                                                     */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_Data
{
	bLisp_VariableBox *variable;
	bLisp_FunctionBox *function;
	bLisp_ConstBox    *constant;
	bLisp_CallInfoBox *callinfo;
	bLisp_ArrayBox    *array;
	bLisp_CodeBox     *code;

	bLisp_StringPool  *string;
};

typedef struct bLisp_Data bLisp_Data;
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FunctionList                                                                             */
/*------------------------------------------------------------------------------------------------*/
typedef struct bLisp_Barrel bLisp_Barrel;

struct bLisp_Barrel
{
	bLisp_Function     *function;
	bLisp_Barrel       *next;
};
/*------------------------------------------------------------------------------------------------*/
/* bLisp_BlockList                                                                                */
/*------------------------------------------------------------------------------------------------*/
typedef struct bLisp_BlockList bLisp_BlockList;

struct bLisp_BlockList
{
	bLisp_Block      item;
	bLisp_BlockList *next;
};
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Script                                                                                   */
/*------------------------------------------------------------------------------------------------*/
struct bLisp_Script
{ 
	/* script data */
	bLisp_Data *data;

    /* last error data */
	bLisp_ErrorInfo *error;

	/* compilation data */
	bLisp_Source    *source;
	bLisp_BlockList *local_block;
};
/*------------------------------------------------------------------------------------------------*/
/* bLisp_GenEmptyPush                                                                             */
/*------------------------------------------------------------------------------------------------*/
bLisp_Code *bLisp_GenEmptyPush(bLisp_Script *_script, bLisp_Token *_token);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseExpression                                                                          */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseExpression(bLisp_Script *_script, bLisp_Token *_token);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseFunction                                                                            */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseFunction(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseASMD                                                                                */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseASMD(bLisp_Script *_script, bLisp_TokenClass _type);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseLogic                                                                               */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseLogic(bLisp_Script *_script, bLisp_TokenClass _type);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseBind                                                                                */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseBind(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseBegin                                                                               */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseBegin(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseSet                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseSet(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseGet                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseGet(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseGetForSet                                                                           */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseGetForSet(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseArray                                                                               */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseArray(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseName                                                                                */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseName(bLisp_Script *_script, bLisp_Token *_token);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseLambda                                                                              */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseLambda(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseNative                                                                              */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseNative(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseCall                                                                                */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseCall(bLisp_Script *_script, bLisp_Token *_token);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseIf                                                                                  */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseIf(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseWhile                                                                               */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseWhile(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_IdentNextToken                                                                           */
/*------------------------------------------------------------------------------------------------*/
bLisp_TokenPos bLisp_IdentNextToken(bchar *_code);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_CreateSource                                                                             */
/*------------------------------------------------------------------------------------------------*/
bLisp_Source *bLisp_CreateSource(bLisp_Script *_script, bchar *_code);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_DeleteSource                                                                             */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_DeleteSource(bLisp_Source *_source);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_AddInclude                                                                               */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_AddInclude(bchar *_code, bchar *_file, bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_AllocInclude                                                                             */
/*------------------------------------------------------------------------------------------------*/
bLisp_Include *bLisp_AllocInclude(bchar *_code, bchar *_file);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_GetNewInclude                                                                            */
/*------------------------------------------------------------------------------------------------*/
bLisp_Include *bLisp_GetNewInclude(bLisp_Source *_source);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FindInclude                                                                              */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_FindInclude(bLisp_Script *_script, bchar *_name);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FreeSource                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_FreeSource(bLisp_Source *_source);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_BuildTokenPos                                                                            */
/*------------------------------------------------------------------------------------------------*/
bLisp_TokenPos bLisp_BuildTokenPos(bchar *_b, bchar *_e, bLisp_TokenClass _t);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_SqueezeToken                                                                             */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_SqueezeToken(bLisp_TokenPos *_data, bLisp_Token *_token);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_GetNextToken                                                                             */
/*------------------------------------------------------------------------------------------------*/
bLisp_Token bLisp_GetNextToken(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_AddStringExpression                                                                      */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_AddStringExpression(bLisp_Script *_script, bLisp_Token *_token);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_AddNumberExpression                                                                      */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_AddNumberExpression(bLisp_Data *_data, bLisp_Token *_token);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Compile                                                                                  */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_Compile(bLisp_Script *_script, bchar *_code);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_CreateData                                                                               */
/*------------------------------------------------------------------------------------------------*/
bLisp_Data *bLisp_CreateData();
/*------------------------------------------------------------------------------------------------*/
/* bLisp_DeleteData                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_DeleteData(bLisp_Data *_data);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_PrintType                                                                                */
/*------------------------------------------------------------------------------------------------*/
bchar *bLisp_PrintType(bLisp_Type *_type);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Error                                                                                    */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_Error(bLisp_Script *_s, bLisp_Token *_token, bchar *_text);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_RuntimeError                                                                             */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_RuntimeError(bLisp_Script *_s, bLisp_Code *_code, bchar **_error, bchar *_text);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Add                                                                                      */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_Add(bLisp_Stack *_stack);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Push                                                                                     */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_Push(bLisp_Script *_script, bLisp_Ref *_ref);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Set                                                                                      */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_Set(bLisp_Script *_script, bLisp_Variable *_var);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Call                                                                                     */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_Call(bLisp_Script *_script, bLisp_Code *_code);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_NativeProcessor                                                                          */
/*------------------------------------------------------------------------------------------------*/
bLisp_Token bLisp_NativeProcessor(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_IncludeProcessor                                                                         */
/*------------------------------------------------------------------------------------------------*/
bLisp_Token bLisp_IncludeProcessor(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FindNativeVariable                                                                       */
/*------------------------------------------------------------------------------------------------*/
bLisp_Variable *bLisp_FindNativeVariable(bLisp_Script *_script, bchar *_name);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FindVariable                                                                             */
/*------------------------------------------------------------------------------------------------*/
bLisp_FindResult bLisp_FindVariable(bLisp_Script *_script, bchar *_name);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FindGlobalVariable                                                                       */
/*------------------------------------------------------------------------------------------------*/
bLisp_Variable *bLisp_FindGlobalVariable(bLisp_Script *_script, bchar *_name);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FindLocalVariable                                                                        */
/*------------------------------------------------------------------------------------------------*/
bLisp_Variable *bLisp_FindLocalVariable(bLisp_Script *_script, bchar *_name);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_CreateLocalBlock                                                                         */
/*------------------------------------------------------------------------------------------------*/
bLisp_Block *bLisp_CreateLocalBlock(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_DeleteLocalBlock                                                                         */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_DeleteLocalBlock(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_DeleteAllLocalBlocks                                                                     */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_DeleteAllLocalBlocks(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_PackData                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_PackData(bLisp_Data *_data);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_UnpackData                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_UnpackData(bLisp_Data *_data);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Box_Create                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid *bLisp_Box_Create(buint _size, buint _num);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Box_Delete                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_Box_Delete(bvoid *_box);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Box_Add                                                                                  */
/*------------------------------------------------------------------------------------------------*/
bvoid *bLisp_Box_Add(bvoid *_box, bvoid *_item);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Box_Read                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid *bLisp_Box_Read(bFile _file);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Box_Save                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_Box_Save(bvoid *_box, bFile _file);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_StringPool_Create                                                                        */
/*------------------------------------------------------------------------------------------------*/
bLisp_StringPool *bLisp_StringPool_Create(buint _size);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_StringPool_Delete                                                                        */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_StringPool_Delete(bLisp_StringPool *_pool);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_StringPool_Add                                                                           */
/*------------------------------------------------------------------------------------------------*/
bchar *bLisp_StringPool_Add(bLisp_StringPool *_pool, bchar *_string);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_StringPool_Save                                                                          */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_StringPool_Save(bvoid *_pool, bFile _file);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_StringPool_Read                                                                          */
/*------------------------------------------------------------------------------------------------*/
bLisp_StringPool *bLisp_StringPool_Read(bFile _file);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Stack_Create                                                                             */
/*------------------------------------------------------------------------------------------------*/
bLisp_Stack *bLisp_Stack_Create(buint _size);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Stack_Delete                                                                             */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_Stack_Delete(bLisp_Stack *_stack);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Preprocessor                                                                             */
/*------------------------------------------------------------------------------------------------*/
bLisp_Token bLisp_Preprocessor(bLisp_Script *_script);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_CheckASMDType                                                                            */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_CheckASMDType(bLisp_Stack *_stack);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_CheckArrayLength                                                                         */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_CheckArrayLength(bLisp_Stack *_stack);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FreeToken                                                                                */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_FreeToken(bLisp_Token *_token);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ErrorInfo_Print                                                                          */
/*------------------------------------------------------------------------------------------------*/
bchar *bLisp_ErrorInfo_Print(bLisp_ErrorInfo *_einfo);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ErrorInfo_Create                                                                         */
/*------------------------------------------------------------------------------------------------*/
bLisp_ErrorInfo *bLisp_ErrorInfo_Create();
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ErrorInfo_Delete                                                                         */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_ErrorInfo_Delete(bLisp_ErrorInfo *_info);
/*------------------------------------------------------------------------------------------------*/
/* bLisp_OpenScript                                                                               */
/*------------------------------------------------------------------------------------------------*/
bLisp_Script *bLisp_OpenScript(bchar *_text, bchar **error)
{
	bLisp_Script *script;
	bbool         compile_result;

	assert(_text);

	script = malloc(sizeof(bLisp_Script));

	script->error = bLisp_ErrorInfo_Create();

	compile_result = bLisp_Compile(script, _text);

	if(!compile_result)
	{
		if(error)
			*error = bLisp_ErrorInfo_Print(script->error);
		bLisp_ErrorInfo_Delete(script->error);
		free(script);

		return 0;
	}

	return script;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_OpenCode                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bLisp_Script *bLisp_OpenCode(bchar *_filename)
{
	bLisp_Script *script;
	bFile         file;

	assert(_filename);

	file = bFile_Open(_filename, bFile_ReadMode);

	if(!file) return 0;

	script = malloc(sizeof(bLisp_Script));

	script->data = malloc(sizeof(bLisp_Data));

	script->data->variable = bLisp_Box_Read(file);
	script->data->function = bLisp_Box_Read(file);
	script->data->constant = bLisp_Box_Read(file);
	script->data->callinfo = bLisp_Box_Read(file);
	script->data->array    = bLisp_Box_Read(file);
	script->data->code     = bLisp_Box_Read(file);

	script->data->string   = bLisp_StringPool_Read(file);

	bFile_Close(file);

	bLisp_UnpackData(script->data);

	return script;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Run                                                                                      */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_Run(bLisp_Script *_script, bchar **_result, bchar **_error)
{
	bLisp_Code      *code, *end;
	bLisp_CallInfo  *cur_info;
	bLisp_Type      *cur_type, result, *frame;
	bLisp_Stack     *stack;
		
	code  = _script->data->code->data;
	end   = code + _script->data->code->item_num;
	stack = bLisp_Stack_Create(bLisp_StackSize);
	frame = 0;

	while(code < end)
	{
		switch(code->type)
		{
		case bLisp_PushC_Code:
			*stack->data++ = *code->ref.value.constant;
			break;

		case bLisp_PushG_Code:
			*stack->data++ = *code->ref.value.variable->value.gloabal;
			break;

		case bLisp_PushL_Code:
			*stack->data++ = *(frame + code->ref.value.variable->value.local);
			break;

		case bLisp_PushI_Code:
			*stack->data++ = *(bLisp_Type *)code->ref.value.callinfo;
			break;

		case bLisp_Pop_Code:
			--stack->data;
			break;

		case bLisp_Cmp_Code:
			if((--stack->data)->value.number) code++;
			break;

		case bLisp_Goto_Code:
			code = code->ref.value.code;
			break;

		case bLisp_Ret_Code:
			cur_info     = (bLisp_CallInfo *)(stack->data - 2); /* get call info */
			*(frame - 1) = *(stack->data - 1);                  /* copy result   */
			stack->data  = frame;                               /* pop all trash */
			frame        = cur_info->frame;                     /* restore frame */
			code         = cur_info->r_ptr;                     /* restore code  */
			break;

		case bLisp_Call_Code:
			cur_type = (stack->data - code->ref.value.arg_num - 2);

			if(cur_type->value.function->type == bLisp_NativeFunction)
			{
				cur_type->value.function->arg_num = code->ref.value.arg_num;
				stack->data = cur_type + 1;
				cur_type->value.function->value.native_ptr(stack->data);
			}
			else
			{
#ifndef bRelease
				if(code->ref.value.arg_num != cur_type->value.function->arg_num)
					bLisp_RuntimeError(_script, code, _error, L"Wrong arg number");
#endif /* bRelease */

				((bLisp_CallInfo *)stack->data - 1)->frame = frame;
				frame = cur_type + 1;
				code  = cur_type->value.function->value.usual_ptr;
			}
			break;

		case bLisp_SetL_Code:
			*(frame + code->ref.value.variable->value.local) = *(stack->data - 1);
			break;

		case bLisp_SetG_Code:
			*code->ref.value.variable->value.gloabal = *(stack->data - 1);
			break;

		case bLisp_SetQ_Code:
			*(stack->data - 2)->value.type = *(stack->data - 1);
			*(stack->data-- - 2) = *(stack->data - 1);
			break;

		case bLisp_Get_Code:
			if(!bLisp_CheckArrayLength(stack))
				return bLisp_RuntimeError(_script, code, _error, L"array overflow!");

			*(stack->data-- - 2) = *((stack->data - 2)->value.array->ptr + 
					          (buint)(stack->data - 1)->value.number);
			break;

		case bLisp_GetSet_Code:
			if(!bLisp_CheckArrayLength(stack))
				return bLisp_RuntimeError(_script, code, _error, L"array overflow!");

			(stack->data-- - 2)->type = bLisp_PtrType;
			(stack->data   - 1)->value.type = (stack->data - 1)->value.array->ptr + 
					                           (buint)(stack->data)->value.number ;
			break;

		case bLisp_Equal_Code:
			if(!bLisp_CheckASMDType(stack))
				return bLisp_RuntimeError(_script, code, _error, L"Incompatable types in '='");

			(stack->data-- - 2)->value.number = (bfloat)((stack->data - 1)->value.number ==
				                                         (stack->data - 2)->value.number) ;
			break;

		case bLisp_NotEqual_Code:
			if(!bLisp_CheckASMDType(stack))
				return bLisp_RuntimeError(_script, code, _error, L"Incompatable types in '!='");

			(stack->data-- - 2)->value.number = (bfloat)((stack->data - 1)->value.number !=
				                                         (stack->data - 2)->value.number) ;
			break;

		case bLisp_Less_Code:
			if(!bLisp_CheckASMDType(stack))
				return bLisp_RuntimeError(_script, code, _error, L"Incompatable types in '<'");

			(stack->data-- - 2)->value.number = (bfloat)((stack->data - 1)->value.number >
				                                         (stack->data - 2)->value.number);
			break;

		case bLisp_More_Code:
			if(!bLisp_CheckASMDType(stack))
				return bLisp_RuntimeError(_script, code, _error, L"Incompatable types in '>'");

			(stack->data-- - 2)->value.number = (bfloat)((stack->data - 1)->value.number <
				                                         (stack->data - 2)->value.number);
			break;

		case bLisp_Add_Code:
			if(!bLisp_CheckASMDType(stack))
				return bLisp_RuntimeError(_script, code, _error, L"Incompatable types in '+'");

			(stack->data-- - 2)->value.number = (stack->data - 1)->value.number + 
				                                (stack->data - 2)->value.number ;
			break;

		case bLisp_Sub_Code:
			if(!bLisp_CheckASMDType(stack))
				return bLisp_RuntimeError(_script, code, _error, L"Incompatable types in '-'");

			(stack->data-- - 2)->value.number = (stack->data - 2)->value.number - 
				                                (stack->data - 1)->value.number ;
			break;

		case bLisp_Mul_Code:
			if(!bLisp_CheckASMDType(stack))
				return bLisp_RuntimeError(_script, code, _error, L"Incompatable types in '*'");

			(stack->data-- - 2)->value.number = (stack->data - 1)->value.number * 
				                                (stack->data - 2)->value.number ;
			break;

		case bLisp_Div_Code:
			if(!bLisp_CheckASMDType(stack))
				return bLisp_RuntimeError(_script, code, _error, L"Incompatable types in '/'");

			(stack->data-- - 2)->value.number = (stack->data - 2)->value.number / 
				                                (stack->data - 1)->value.number ;
			break;
		}

		code++;
	}

	result = *stack->data;

	if(stack->data != stack->bottom)
	{
		bLisp_RuntimeError(_script, code - 1, _error, L"Unknown internal error");
	}

	bLisp_Stack_Delete(stack);

	/* write result string */
	if(_result)
	{
		*_result = bLisp_PrintType(&result);
	}

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Save                                                                                     */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_Save(bchar *_filename, bLisp_Script *_script)
{
	bFile file;

	file = bFile_Create(_filename, btrue, bFile_WriteMode);

	if(!file) return bfalse;

	bLisp_PackData(_script->data);

	bLisp_Box_Save(_script->data->variable, file);
	bLisp_Box_Save(_script->data->function, file);
	bLisp_Box_Save(_script->data->constant, file);
	bLisp_Box_Save(_script->data->callinfo, file);
	bLisp_Box_Save(_script->data->array,    file);
	bLisp_Box_Save(_script->data->code,     file);

	bLisp_StringPool_Save(_script->data->string, file);

	bLisp_UnpackData(_script->data);

	bFile_Close(file);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Close                                                                                    */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_Close(bLisp_Script *_script)
{
	bLisp_DeleteData(_script->data);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_StringPool_Save                                                                          */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_StringPool_Save(bvoid *_pool, bFile _file)
{
	buint size;
	bLisp_StringPool *pool = _pool;

	size = pool->size * sizeof(bchar);
	bFile_Write(&size, sizeof(buint), 0, _file);
	bFile_Write(pool->data, size, 0, _file);
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_StringPool_Read                                                                          */
/*------------------------------------------------------------------------------------------------*/
bLisp_StringPool *bLisp_StringPool_Read(bFile _file)
{
	buint size;
	bLisp_StringPool *pool;

	pool = malloc(sizeof(bLisp_StringPool));

	bFile_Read(&size, sizeof(buint), 0, _file);

	if(!size)
	{
		size = bLisp_DefaultPoolSize / sizeof(bchar);
		pool->max_size = size;
		pool->size = 0;
		pool->data = malloc(bLisp_DefaultPoolSize);
	}
	else
	{
		pool->size     = size / sizeof(bchar);
		pool->max_size = pool->size;
		pool->data     = malloc(size);

		bFile_Read(pool->data, size, 0, _file);
	}

	return pool;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Box_Read                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid *bLisp_Box_Read(bFile _file)
{
	buint size;
	bLisp_Box *box;

	box = malloc(sizeof(bLisp_Box));

	bFile_Read(&box->item_num,  sizeof(buint), 0, _file);
	bFile_Read(&box->item_size, sizeof(buint), 0, _file);

	if(!box->item_num)
	{
		size              = bLisp_DefaultBoxSize;
		box->data         = malloc(bLisp_DefaultBoxSize * box->item_size);
		box->item_num_max = size;
		box->item_num     = 0;
	}
	else
	{
		size              = box->item_num * box->item_size;
		box->item_num_max = box->item_num;
		box->data         = malloc(size);

		bFile_Read(box->data, size, 0, _file);
	}

	return box;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Box_Save                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_Box_Save(bvoid *_box, bFile _file)
{
	buint size;
	bLisp_Box *box = _box;

	size = box->item_num * box->item_size;

	bFile_Write(&box->item_num,  sizeof(buint), 0, _file);
	bFile_Write(&box->item_size, sizeof(buint), 0, _file);
	bFile_Write(box->data, size, 0, _file);
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_CheckASMDType                                                                            */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_CheckASMDType(bLisp_Stack *_stack)
{
#ifndef bRelease

	if((_stack->data - 1)->type != bLisp_NumberType ||
	   (_stack->data - 2)->type != bLisp_NumberType )
	{
		bLisp_Stack_Delete(_stack);
		return btrue;
	}

#endif /* bRelease */

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_CheckArrayLength                                                                         */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_CheckArrayLength(bLisp_Stack *_stack)
{
#ifndef bRelease

	if((_stack->data - 1)->type != bLisp_NumberType)
	{
		bLisp_Stack_Delete(_stack);
		return bfalse;
	}

	if((_stack->data - 2)->value.array->size <= (_stack->data - 1)->value.number)
	{
		bLisp_Stack_Delete(_stack);
		return bfalse;
	}

#endif /* bRelease */

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FreeToken                                                                                */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_FreeToken(bLisp_Token *_token)
{
	if(_token->type == bLisp_StringToken || _token->type == bLisp_NameToken)
	{
		free(_token->value.string); /* this free name value too */
	}
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ErrorInfo_Print                                                                          */
/*------------------------------------------------------------------------------------------------*/
bchar *bLisp_ErrorInfo_Print(bLisp_ErrorInfo *_info)
{
	bchar *text;
	buint  size;

	size = wcslen(_info->text) + wcslen(_info->file) + 256;

	text = malloc(size * sizeof(bchar));

	swprintf(text, size, L"6y6yka Lisp error: \r\n file: %s \r\n line: %d \r\n test: %s", 
		     _info->file, _info->line, _info->text);

	return text;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ErrorInfo_Create                                                                         */
/*------------------------------------------------------------------------------------------------*/
bLisp_ErrorInfo *bLisp_ErrorInfo_Create()
{
	bLisp_ErrorInfo *info;

	info       = malloc(sizeof(bLisp_ErrorInfo));
	info->file = 0;
	info->line = 0;
	info->text = 0;

	return info;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ErrorInfo_Delete                                                                         */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_ErrorInfo_Delete(bLisp_ErrorInfo *_info)
{
	free(_info->file);
	free(_info->text);
	free(_info);
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Add                                                                                      */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_Add(bLisp_Stack *_stack)
{
	bLisp_Type *one, *two, result;

	one = --_stack->data;
	two = --_stack->data; 

	if(one->type != bLisp_NumberType || two->type != bLisp_NumberType)
		return bfalse;

	result.value.number = one->value.number + two->value.number;
	result.type         = bLisp_NumberType;

	*_stack->data++ = result;

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Compile                                                                                  */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_Compile(bLisp_Script *_script, bchar *_code)
{
	bLisp_Token token;
	bLisp_Code  code;

	_script->data        = bLisp_CreateData();
	_script->source      = bLisp_CreateSource(_script, _code);
	_script->local_block = 0; /* init local block list */

	token     = bLisp_GetNextToken(_script);
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
		bLisp_Box_Add(_script->data->code, &code);

		token = bLisp_GetNextToken(_script);
	}

	bLisp_DeleteSource(_script->source);
	bLisp_DeleteAllLocalBlocks(_script);

	bLisp_UnpackData(_script->data);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_AddStringExpression                                                                      */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_AddStringExpression(bLisp_Script *_script, bLisp_Token *_token)
{
	bLisp_Code  code    ;
	bLisp_Type  constant;
	bLisp_Type *const_id;
	bchar      *str_id  ;

	str_id = bLisp_StringPool_Add(_script->data->string, _token->value.string);

	constant.type = bLisp_StringType;
	constant.value.string = str_id;

	const_id = bLisp_Box_Add(_script->data->constant, &constant);

	code.type               = bLisp_PushC_Code;
	code.ref.type           = bLisp_ConstRef;
	code.ref.value.constant = const_id;
	code.file               = _token->file;
	code.line               = _token->line;

	bLisp_Box_Add(_script->data->code, &code);
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_AddNumberExpression                                                                      */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_AddNumberExpression(bLisp_Data *_data, bLisp_Token *_token)
{
	bLisp_Code     code;
	bLisp_Type constant;

	constant.type         = bLisp_NumberType;
	constant.value.number = _token->value.number;

	code.ref.value.constant = bLisp_Box_Add(_data->constant, &constant);
	code.type               = bLisp_PushC_Code;
	code.ref.type           = bLisp_ConstRef;
	code.file               = _token->file;
	code.line               = _token->line;

	bLisp_Box_Add(_data->code, &code);
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_PrintType                                                                                */
/*------------------------------------------------------------------------------------------------*/
bchar *bLisp_PrintType(bLisp_Type *_type)
{
	bchar *string;
	buint  size;
	bchar *default_type = L"unnown type";

	switch(_type->type)
	{
	case bLisp_NumberType:
		size = 64; /* 64 max number length */
		string = malloc(size);
		swprintf(string, size, L"%g", _type->value.number);
		break;

	case bLisp_StringType:
		size = wcslen(_type->value.string) * sizeof(bchar) + sizeof('\0');
		string = malloc(size);
		swprintf(string, size, L"%s", _type->value.string);
		break;

	case bLisp_DataType:
		size = 64; /* 64 max pointer length */
		string = malloc(size);
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
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Error                                                                                    */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_Error(bLisp_Script *_s, bLisp_Token *_token, bchar *_text)
{
	_s->error->text = _wcsdup(_text);
	_s->error->file = _wcsdup(_s->data->string->data + (buint)_token->file);
	_s->error->line = _token->line;

	return bfalse; /* always return false */
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_RuntimeError                                                                             */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_RuntimeError(bLisp_Script *_s, bLisp_Code *_code, bchar **_error, bchar *_text)
{
	_s->error->text = _text;
	_s->error->file = _code->file;
	_s->error->line = _code->line;

	if(_error)
	{
		*_error = bLisp_ErrorInfo_Print(_s->error);
	}

	return bfalse; /* always return false */
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_RegisterFunction                                                                         */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_RegisterFunction(bLisp_Script *_script, bchar *_name, bLisp_CallBack _ptr)
{
	bLisp_Variable *find_result;

	find_result = bLisp_FindNativeVariable(_script, _name);

	if(find_result == bLisp_NotFound)
		return bfalse;

	find_result->value.gloabal->value.function->value.native_ptr = _ptr;

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_GetNumberArg                                                                             */
/*------------------------------------------------------------------------------------------------*/
bfloat bLisp_GetNumberArg(bvoid *_stack, buint _arg)
{
	bLisp_Type *stack = _stack;

#ifndef bRelease

	assert((stack + _arg)->type == bLisp_NumberType);
	assert((stack - 1)->value.function->arg_num > _arg);

#endif /* bRelease */

	return (stack + _arg)->value.number;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_GetStringArg                                                                             */
/*------------------------------------------------------------------------------------------------*/
bchar *bLisp_GetStringArg(bvoid *_stack, buint _arg)
{
	bLisp_Type *stack = _stack;

#ifndef bRelease

	assert((stack + _arg)->type == bLisp_StringType);
	assert((stack - 1)->value.function->arg_num > _arg);

#endif /* bRelease */

	return (stack + _arg)->value.string;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_GetDataArg                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid *bLisp_GetDataArg(bvoid *_stack, buint _arg)
{
	bLisp_Type *stack = _stack;

#ifndef bRelease

	assert((stack + _arg)->type == bLisp_DataType);
	assert((stack - 1)->value.function->arg_num > _arg);

#endif /* bRelease */

	return (stack + _arg)->value.data;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ReturnNumber                                                                             */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_ReturnNumber(bvoid *_stack, bfloat _ret)
{
	bLisp_Type *stack = _stack;

	stack--;

	stack->type         = bLisp_NumberType;
	stack->value.number = _ret;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ReturnString                                                                             */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_ReturnString(bvoid *_stack, bchar *_ret)
{
	bLisp_Type *stack = _stack;

	stack--;

	stack->type         = bLisp_StringType;
	stack->value.string = _ret;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ReturnData                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_ReturnData(bvoid *_stack, bvoid *_ret)
{
	bLisp_Type *stack = _stack;

	stack--;

	stack->type         = bLisp_DataType;
	stack->value.data   = _ret;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseLogic                                                                               */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseLogic(bLisp_Script *_script, bLisp_TokenClass _type)
{
	bLisp_Token token;
	bLisp_Code  code ;

	/* get first argument */
	token = bLisp_GetNextToken(_script);
	if(!bLisp_ParseExpression(_script, &token))
		return bfalse;

	/* get second argument */
	token = bLisp_GetNextToken(_script);
	if(!bLisp_ParseExpression(_script, &token))
		return bfalse;

	token = bLisp_GetNextToken(_script);
	if(token.type != bLisp_RBraceToken)
		return bLisp_Error(_script, &token, L"Missing ')'");

	/* choose operator type */
	switch(_type)
	{
	case bLisp_EqualToken:
		code.type = bLisp_Equal_Code;
		break;

	case bLisp_NEqualToken:
		code.type = bLisp_NotEqual_Code;
		break;

	case bLisp_LessToken:
		code.type = bLisp_Less_Code;
		break;

	case bLisp_MoreToken:
		code.type = bLisp_More_Code;
		break;
	}

	code.file = token.file;
	code.line = token.line;

	bLisp_Box_Add(_script->data->code, &code);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseASMD                                                                                */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseASMD(bLisp_Script *_script, bLisp_TokenClass _type)
{
	buint       i;
	bLisp_Token token;
	bLisp_Code  code;

	token = bLisp_GetNextToken(_script);

	for(i = 0; token.type != bLisp_RBraceToken; i++)
	{
		if(!bLisp_ParseExpression(_script, &token))
			return bfalse;

		token = bLisp_GetNextToken(_script);
	}

	switch(_type)
	{
	case bLisp_AddToken:
		code.type = bLisp_Add_Code;
		break;

	case bLisp_SubToken:
		code.type = bLisp_Add_Code;
		break;
	case bLisp_MulToken:
		code.type = bLisp_Mul_Code;
		break;

	case bLisp_DivToken:
		code.type = bLisp_Mul_Code;
		break;
	}
	
	if(i < 2) /* minimum two argumnets */
		return bLisp_Error(_script, &token, L"So few argumnets in '+'");

	code.file = token.file;
	code.line = token.line;

	while(i-- != 2)
	{
		bLisp_Box_Add(_script->data->code, &code);
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

	/* add last code */
	bLisp_Box_Add(_script->data->code, &code);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseBind                                                                                */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseBind(bLisp_Script *_script)
{
	bLisp_Token     token;
	bLisp_Type      type, *type_id;
	bLisp_Code      code;
	bLisp_Variable  var, *find_result, *var_id;
	bchar          *name_id;

	token = bLisp_GetNextToken(_script);

	if(token.type != bLisp_NameToken)
	{
		return bLisp_Error(_script, &token, L"Missing variable name");
	}

	find_result = bLisp_FindGlobalVariable(_script, token.value.name);

	/* if variable already exists */
	if(find_result != bLisp_NotFound)
	{
		return bLisp_Error(_script, &token, L"Variable with same name already exists");
	}

	/* preprae new empty value */
	type.type         = bLisp_NumberType;
	type.value.number = 0;
	type_id           = bLisp_Box_Add(_script->data->constant, &type);

	/* add new variable name */
	name_id = bLisp_StringPool_Add(_script->data->string, token.value.name);

	/* prepare new variable */
	var.name          = name_id;
	var.type          = bLisp_GlobalVariable;
	var.value.gloabal = type_id;
	var_id            = bLisp_Box_Add(_script->data->variable, &var);

	/* prepare set code */
	code.file               = token.file;
	code.line               = token.line;
	code.type               = bLisp_SetG_Code;
	code.ref.type           = bLisp_VariableRef;
	code.ref.value.variable = var_id;

	token = bLisp_GetNextToken(_script);

	/* parse and push variable value */
	if(!bLisp_ParseExpression(_script, &token))
		return bfalse;

	/* add set code */
	bLisp_Box_Add(_script->data->code, &code);

	/* get right brace token */
	token = bLisp_GetNextToken(_script);

	if(token.type != bLisp_RBraceToken) 
		return bLisp_Error(_script, &token, L"Missing ')'");

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseName                                                                                */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseName(bLisp_Script *_script, bLisp_Token *_token)
{
	bLisp_FindResult find;
	bLisp_Code       code;

	find = bLisp_FindVariable(_script, _token->value.name);

	if(!find.result)
		return bLisp_Error(_script, _token, L"Can't find variable with this name");

	code.ref.type           = bLisp_VariableRef;
	code.ref.value.variable = find.variable;
	code.type               = find.local ? bLisp_PushL_Code : bLisp_PushG_Code;
	code.file               = _token->file;
	code.line               = _token->line;

	bLisp_Box_Add(_script->data->code, &code);
	
	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FindNativeVariable                                                                       */
/*------------------------------------------------------------------------------------------------*/
bLisp_Variable *bLisp_FindNativeVariable(bLisp_Script *_script, bchar *_name)
{
	bLisp_VariableBox *var;
	buint i;

	var = _script->data->variable;

	for(i = 0; i < var->item_num; i++)
	{
		if(wcscmp(var->data[i].name, _name) == 0)
			return var->data + i;
	}

	return bLisp_NotFound;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FindVariable                                                                             */
/*------------------------------------------------------------------------------------------------*/
bLisp_FindResult bLisp_FindVariable(bLisp_Script *_script, bchar *_name)
{
	bLisp_FindResult find_result;

	find_result.variable = bLisp_FindLocalVariable(_script, _name);
	find_result.local    = btrue;

	if(find_result.variable == bLisp_NotFound)
	{
		find_result.variable = bLisp_FindGlobalVariable(_script, _name);
		find_result.local    = bfalse;

		find_result.result   = (find_result.variable != bLisp_NotFound);
	}

	return find_result;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FindGlobalVariable                                                                       */
/*------------------------------------------------------------------------------------------------*/
bLisp_Variable *bLisp_FindGlobalVariable(bLisp_Script *_script, bchar *_name)
{
	bLisp_VariableBox *var;
	bchar *var_name;
	buint i;

	var = _script->data->variable;

	for(i = 0; i < var->item_num; i++)
	{
		var_name = _script->data->string->data + (buint)var->data[i].name;

		if(wcscmp(var_name, _name) == 0)
			return (bLisp_Variable *)i;
	}

	return bLisp_NotFound;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FindLocalVariable                                                                        */
/*------------------------------------------------------------------------------------------------*/
bLisp_Variable *bLisp_FindLocalVariable(bLisp_Script *_script, bchar *_name)
{
	buint           i       ;
	bchar          *var_name;
	bLisp_Variable *var     ;
	bLisp_Block    *block   ;

	block = &_script->local_block->item;

	/* if no local variables */
	if(!block)
		return bLisp_NotFound;

	for(i = 0; i < block->count; i++)
	{
		var      = _script->data->variable->data + (buint)block->local_var[i];
		var_name = _script->data->string->data   + (buint)var->name          ;

		if(wcscmp(var_name, _name) == 0)
			return block->local_var[i];
	}

	return bLisp_NotFound;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_CreateLocalBlock                                                                         */
/*------------------------------------------------------------------------------------------------*/
bLisp_Block *bLisp_CreateLocalBlock(bLisp_Script *_script)
{
	bLisp_BlockList *new_block;

	new_block = malloc(sizeof(bLisp_BlockList));
	new_block->next = _script->local_block;
	new_block->item.count = 0;

	_script->local_block = new_block;

	return &new_block->item;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_DeleteLocalBlock                                                                         */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_DeleteLocalBlock(bLisp_Script *_script)
{
	bLisp_BlockList *temp;

	assert(_script->local_block);

	temp = _script->local_block->next;

	free(_script->local_block);

	_script->local_block = temp;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_DeleteAllLocalBlocks                                                                     */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_DeleteAllLocalBlocks(bLisp_Script *_script)
{
	while(_script->local_block)
	{
		bLisp_DeleteLocalBlock(_script);
	}
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_PackData                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_PackData(bLisp_Data *_data)
{
	buint i;
	bLisp_Function *f;
	bLisp_CallInfo *info;
	bLisp_Variable *var;
	bLisp_Type     *type;
	bLisp_Ref      *ref;
	bLisp_Array    *array;

	for(i = 0; i < _data->array->item_num; i++)
	{
		array = _data->array->data + i;

		array->ptr = (bLisp_Type *)(array->ptr - _data->constant->data);
	}

	for(i = 0; i < _data->function->item_num; i++)
	{
		f = _data->function->data + i;

		if(f->type == bLisp_UsualFunction)
		{
			f->value.usual_ptr = (bLisp_Code *)(f->value.usual_ptr - _data->code->data);
		}
	}

	for(i = 0; i < _data->callinfo->item_num; i++)
	{
		info = _data->callinfo->data + i;

		info->r_ptr = (bLisp_Code *)(info->r_ptr - _data->code->data);
	}

	for(i = 0; i < _data->variable->item_num; i++)
	{
		var = _data->variable->data + i;

		var->name = (bchar *)(var->name - _data->string->data);

		if(var->type == bLisp_GlobalVariable)
		{
			var->value.gloabal = (bLisp_Type *)(var->value.gloabal - _data->constant->data);
		}
	}

	for(i = 0; i < _data->constant->item_num; i++)
	{
		type = _data->constant->data + i;

		if(type->type == bLisp_FunctionType)
		{
			type->value.function = (bLisp_Function *)(type->value.function - _data->function->data);
		}

		if(type->type == bLisp_StringType)
		{
			type->value.string = (bchar *)(type->value.string - _data->string->data);
		}

		if(type->type == bLisp_ArrayType)
		{
			type->value.array = (bLisp_Array *)(type->value.array - _data->array->data);
		}
	}

	for(i = 0; i < _data->code->item_num; i++)
	{
		ref = &_data->code->data[i].ref;

		_data->code->data[i].file = (bchar *)(_data->code->data[i].file - _data->string->data);

		switch(ref->type)
		{
			case bLisp_CodeRef:
				ref->value.code = (bLisp_Code *)(ref->value.code - _data->code->data);
				break;

			case bLisp_ConstRef:
				ref->value.constant = (bLisp_Type *)(ref->value.constant - _data->constant->data);
				break;

			case bLisp_VariableRef:
				ref->value.variable=(bLisp_Variable *)(ref->value.variable - _data->variable->data);
				break;

			case bLisp_CallInfoRef:
				ref->value.callinfo=(bLisp_CallInfo *)(ref->value.callinfo - _data->callinfo->data);
				break;
		}
	}
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_UnpackData                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_UnpackData(bLisp_Data *_data)
{
	buint i;
	bLisp_Ref *ref;
	bLisp_Type *type;
	bLisp_Variable *var;
	bLisp_Function *func;
	bLisp_CallInfo *info;
	bLisp_Array    *array;

	for(i = 0; i < _data->array->item_num; i++)
	{
		array = _data->array->data + i;

		array->ptr = _data->constant->data + (buint)array->ptr;
	}

	for(i = 0; i < _data->function->item_num; i++)
	{
		func = _data->function->data + i;

		if(func->type == bLisp_UsualFunction)
		{
			func->value.usual_ptr = _data->code->data + (buint)func->value.usual_ptr;
		}
	}

	for(i = 0; i < _data->callinfo->item_num; i++)
	{
		info = _data->callinfo->data + i;

		info->r_ptr = _data->code->data + (buint)info->r_ptr;
	}

	for(i = 0; i < _data->variable->item_num; i++)
	{
		var = (_data->variable->data + i);

		var->name = _data->string->data + (buint)(var->name);

		if(var->type == bLisp_GlobalVariable)
		{
			var->value.gloabal = _data->constant->data + (buint)var->value.gloabal;
		}
	}

	for(i = 0; i < _data->constant->item_num; i++)
	{
		type = (_data->constant->data + i);

		if(type->type == bLisp_FunctionType)
		{
			type->value.function = _data->function->data + (buint)type->value.function;
		}

		if(type->type == bLisp_StringType)
		{
			type->value.string = _data->string->data + (buint)(type->value.string);
		}

		if(type->type == bLisp_ArrayType)
		{
			type->value.array = _data->array->data + (buint)type->value.array;
		}
	}

	for(i = 0; i < _data->code->item_num; i++)
	{
		ref = &(_data->code->data + i)->ref;

		_data->code->data[i].file = _data->string->data + (buint)_data->code->data[i].file;

		switch(ref->type)
		{
		case bLisp_CodeRef:
			ref->value.code = _data->code->data + (buint)ref->value.code;
			break;

		case bLisp_ConstRef:
			ref->value.constant = _data->constant->data + (buint)ref->value.constant;
			break;

		case bLisp_VariableRef:
			ref->value.variable = _data->variable->data + (buint)ref->value.variable;
			break;

		case bLisp_CallInfoRef:
			ref->value.callinfo = _data->callinfo->data + (buint)ref->value.callinfo;
			break;
		}
	}
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Box_Create                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid *bLisp_Box_Create(buint _size, buint _num)
{
	bLisp_Box *box;

	assert(_size);
	assert(_num);

	box = malloc(sizeof(bLisp_Box));

	box->item_size    = _size;
	box->item_num     = 0;
	box->item_num_max = _num;
	box->data         = malloc(box->item_size * box->item_num_max);

	return box;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Box_Delete                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_Box_Delete(bvoid *_box)
{
	bLisp_Box *ptr = _box;

	free(ptr->data);
	free(ptr);
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Box_Add                                                                                  */
/*------------------------------------------------------------------------------------------------*/
bvoid *bLisp_Box_Add(bvoid *_box, bvoid *_item)
{
	bbyte      *new_data  ;
	bLisp_Box  *box = _box;

	if(box->item_num == box->item_num_max)
	{
		new_data = malloc((box->item_num_max * box->item_size) * 2);

		memcpy(new_data, box->data, box->item_num_max * box->item_size);

		free(box->data);

		box->data = new_data;
		box->item_num_max *= 2;
	}

	memcpy(box->data + box->item_num * box->item_size, _item, box->item_size);

	return (bvoid *)(box->item_num++);
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_StringPool_Create                                                                        */
/*------------------------------------------------------------------------------------------------*/
bLisp_StringPool *bLisp_StringPool_Create(buint _size)
{
	bLisp_StringPool *pool;

	pool = malloc(sizeof(bLisp_StringPool));

	pool->data     = malloc(_size);
	pool->max_size = _size;
	pool->size     = 0;

	return pool;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_StringPool_Delete                                                                        */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_StringPool_Delete(bLisp_StringPool *_pool)
{
	bLisp_StringPool *pool = _pool;

	free(pool->data);
	free(pool);
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_StringPool_Add                                                                           */
/*------------------------------------------------------------------------------------------------*/
bchar *bLisp_StringPool_Add(bLisp_StringPool *_pool, bchar *_string)
{
	buint             free_space;
	buint             new_size  ;
	buint             str_len   ;
	bchar            *new_data  ;
	bchar            *cur_pos   ;
	bLisp_StringPool *pool      ;
	
	pool = _pool;
	str_len = wcslen(_string) + sizeof(L'\0');

	free_space = pool->max_size - pool->size;

	if(free_space < str_len)
	{
		new_size = (pool->max_size + str_len) * 2;
		new_data = malloc(new_size * sizeof(bchar));

		memcpy(new_data, pool->data, pool->size * sizeof(bchar));

		free(pool->data);

		pool->data = new_data;
		pool->max_size = new_size;
	}

	cur_pos = pool->data + pool->size;

	memcpy(cur_pos, _string, str_len * sizeof(bchar));
	pool->size += str_len;

	return (bchar *)(cur_pos - pool->data);
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Stack_Create                                                                             */
/*------------------------------------------------------------------------------------------------*/
bLisp_Stack *bLisp_Stack_Create(buint _size)
{
	bLisp_Stack *stack;

	stack         = malloc(sizeof(bLisp_Stack));
	stack->data   = malloc(_size * sizeof(bLisp_Type));
	stack->bottom = stack->data;
	stack->top    = stack->data + _size;

	return stack;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Stack_Delete                                                                             */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_Stack_Delete(bLisp_Stack *_stack)
{
	free(_stack->bottom);
	free(_stack);
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseWhile                                                                               */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseWhile(bLisp_Script *_script)
{
	bLisp_Token token;
	bLisp_Code  code, *cond_code, *goto_id;

	/* remember while position */
	cond_code = (bLisp_Code *)(_script->data->code->item_num - 1);

	token = bLisp_GetNextToken(_script);

	/* calculate and push condition */
	if(!bLisp_ParseExpression(_script, &token)) 
		return bfalse;

	/* prepare compare code */
	code.file = token.file;
	code.line = token.line;
	code.type = bLisp_Cmp_Code;

	/* add compare code */
	bLisp_Box_Add(_script->data->code, &code);

	/* prepare goto code (while end) */
	code.file     = token.file;
	code.line     = token.line;
	code.type     = bLisp_Goto_Code;
	code.ref.type = bLisp_CodeRef;

	/* add goto code */
	goto_id = bLisp_Box_Add(_script->data->code, &code);

	token = bLisp_GetNextToken(_script);

	/* calculate and push expression */
	if(!bLisp_ParseExpression(_script, &token)) 
		return bfalse;

	/* prepare pop code */
	code.file = token.file;
	code.line = token.line;
	code.type = bLisp_Pop_Code;

	/* add pop code */
	bLisp_Box_Add(_script->data->code, &code);

	/* prepare goto code (while begin) */
	code.file = token.file;
	code.line = code.line;
	code.type = bLisp_Goto_Code;
	code.ref.type = bLisp_CodeRef;
	code.ref.value.code = cond_code;

	/* add goto to while begin code */
	bLisp_Box_Add(_script->data->code, &code);

	/* write code adress to go_one ref */
	(_script->data->code->data + (buint)(goto_id))->ref.value.code = 
	(bLisp_Code *)(_script->data->code->item_num - 1);

	/* prepare null type */
	bLisp_GenEmptyPush(_script, &token);

	/* right brace */
	token = bLisp_GetNextToken(_script);

	if(token.type != bLisp_RBraceToken)
	{
		return bLisp_Error(_script, &token, L"Missing ')'");
	}

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseIf                                                                                  */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseIf(bLisp_Script *_script)
{
	bLisp_Token token;
	bLisp_Type  type, *type_id;
	bLisp_Code  code, *go_one, *go_two, *code_id;

	token = bLisp_GetNextToken(_script);

	/* calculate and push condition */
	if(!bLisp_ParseExpression(_script, &token)) 
		return bfalse;

	/* add cmp code */
	code.file = token.file;
	code.line = token.line;
	code.type = bLisp_Cmp_Code;

	bLisp_Box_Add(_script->data->code, &code);

	/* add goto code. goto center */
	code.file     = token.file;
	code.line     = token.line;
	code.type     = bLisp_Goto_Code;
	code.ref.type = bLisp_CodeRef;

	go_one = bLisp_Box_Add(_script->data->code, &code);

	token = bLisp_GetNextToken(_script);

	/* add first expression code */
	if(!bLisp_ParseExpression(_script, &token)) 
		return bfalse;

	/* add goto code. goto end */
	code.file     = token.file;
	code.line     = token.line;
	code.type     = bLisp_Goto_Code;
	code.ref.type = bLisp_CodeRef;

	go_two = bLisp_Box_Add(_script->data->code, &code);

	/* write code adress to go_one ref */
	(_script->data->code->data + (buint)(go_one))->ref.value.code = 
	(bLisp_Code *)(_script->data->code->item_num - 1);

	token = bLisp_GetNextToken(_script);

	if(token.type == bLisp_RBraceToken)
	{
		type.type = bLisp_NumberType;
		type.value.number = 0;

		type_id = bLisp_Box_Add(_script->data->constant, &type);

		code_id = bLisp_GenEmptyPush(_script, &token);
	}
	else
	{
		/* add second expression code */
		if(!bLisp_ParseExpression(_script, &token)) 
			return bfalse;

		/* extract last code pos */
		code_id = (bLisp_Code *)(_script->data->code->item_num - 1);

		if(bLisp_GetNextToken(_script).type != bLisp_RBraceToken)
		{
			return bLisp_Error(_script, &token, L"Missing ')'");
		}
	}

	/* write code adress to go_two ref */
	(_script->data->code->data + (buint)(go_two))->ref.value.code = code_id;

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseGet                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseGet(bLisp_Script *_script)
{
	bLisp_Token token;
	bLisp_Code  code;

	/* parse variable */
	token = bLisp_GetNextToken(_script);

	if(!bLisp_ParseExpression(_script, &token))
		return bfalse;

	/* parse name or index */
	token = bLisp_GetNextToken(_script);

	if(!bLisp_ParseExpression(_script, &token))
		return bfalse;

	code.file = token.file;
	code.line = token.line;
	code.type = bLisp_Get_Code;

	bLisp_Box_Add(_script->data->code, &code);

	token = bLisp_GetNextToken(_script);

	if(token.type != bLisp_RBraceToken)
	{
		return bLisp_Error(_script, &token, L"Missing ')'");
	}

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseGetForSet                                                                           */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseGetForSet(bLisp_Script *_script)
{
	bLisp_Token token;
	bLisp_Code  code;

	/* parse variable */
	token = bLisp_GetNextToken(_script);

	if(!bLisp_ParseExpression(_script, &token))
		return bfalse;

	/* parse name or index */
	token = bLisp_GetNextToken(_script);

	if(!bLisp_ParseExpression(_script, &token))
		return bfalse;

	code.file = token.file;
	code.line = token.line;
	code.type = bLisp_GetSet_Code;

	bLisp_Box_Add(_script->data->code, &code);

	token = bLisp_GetNextToken(_script);

	if(token.type != bLisp_RBraceToken)
	{
		return bLisp_Error(_script, &token, L"Missing ')'");
	}

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseArray                                                                               */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseArray(bLisp_Script *_script)
{
	bLisp_Array array, *array_id;
	bLisp_Token token;
	bLisp_Code  code ;
	bLisp_Type  type , *type_id ;
	buint       i    ;

	token = bLisp_GetNextToken(_script);

	if(token.type != bLisp_NumberToken)
	{
		return bLisp_Error(_script, &token, L"Array size must have digit format");
	}

	array.size = (buint)token.value.number;
	array.ptr  = (bLisp_Type *)_script->data->constant->item_num;

	/* prepare empty type */
	type.type         = bLisp_NumberType;
	type.value.number = 0;

	/* add some array elements */
	for(i = 0; i < array.size; i++)
	{
		bLisp_Box_Add(_script->data->constant, &type);
	}

	/* add our array */
	array_id = bLisp_Box_Add(_script->data->array, &array);

	/* add new array type */
	type.type        = bLisp_ArrayType;
	type.value.array = array_id;

	type_id = bLisp_Box_Add(_script->data->constant, &type);

	/* prepare push code */
	code.file               = token.file;
	code.line               = token.line;
	code.type               = bLisp_PushC_Code;
	code.ref.type           = bLisp_ConstRef;
	code.ref.value.constant = type_id;

	bLisp_Box_Add(_script->data->code, &code);

	token = bLisp_GetNextToken(_script);

	if(token.type != bLisp_RBraceToken)
	{
		return bLisp_Error(_script, &token, L"Missing ')'");
	}

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseSet                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseSet(bLisp_Script *_script)
{
	bLisp_FindResult find ;
	bLisp_Token      token;
	bLisp_Code       code ;

	token = bLisp_GetNextToken(_script);

	if(token.type == bLisp_NameToken)
	{
		find = bLisp_FindVariable(_script, token.value.name);

		if(!find.result)
			return bLisp_Error(_script, &token, L"Unknown variable found");

		code.file               = token.file;
		code.line               = token.line;
		code.type               = find.local ? bLisp_SetL_Code : bLisp_SetG_Code;
		code.ref.type           = bLisp_VariableRef;
		code.ref.value.variable = find.variable;

	}
	else
	{
		token = bLisp_GetNextToken(_script);

		if(token.type != bLisp_GetToken)
			return bLisp_Error(_script, &token, L"Wrong set syntax. Missing variable or array.");

		if(!bLisp_ParseGetForSet(_script))
			return bfalse;

		code.file = token.file;
		code.line = token.line;
		code.type = bLisp_SetQ_Code;
	}

	token = bLisp_GetNextToken(_script);

	/* calculate and add argument */
	if(!bLisp_ParseExpression(_script, &token)) 
		return bfalse;

	/* add set code */
	bLisp_Box_Add(_script->data->code, &code); 

	if(bLisp_GetNextToken(_script).type != bLisp_RBraceToken)
		return bLisp_Error(_script, &token, L"Missing ')'");

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_GenEmptyPush                                                                             */
/*------------------------------------------------------------------------------------------------*/
bLisp_Code *bLisp_GenEmptyPush(bLisp_Script *_script, bLisp_Token *_token)
{
	bLisp_Type type, *type_id;
	bLisp_Code code;

	/* prepare empty type */
	type.type         = bLisp_NumberType;
	type.value.number = 0;

	/* add empty type */
	type_id = bLisp_Box_Add(_script->data->constant, &type);

	/* prepare push code */
	code.file               = _token->file;
	code.line               = _token->line;
	code.type               = bLisp_PushC_Code;
	code.ref.type           = bLisp_ConstRef;
	code.ref.value.constant = type_id;

	/* add push code */
	return bLisp_Box_Add(_script->data->code, &code);
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseBegin                                                                               */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseBegin(bLisp_Script *_script)
{
	bLisp_Token token;
	bLisp_Code  code;

	token = bLisp_GetNextToken(_script);

	while(token.type != bLisp_RBraceToken)
	{
		/* parse one expression */
		if(!bLisp_ParseExpression(_script, &token))
			return bfalse;

		token = bLisp_GetNextToken(_script);

		if(token.type != bLisp_RBraceToken)
		{
			/* prepare pop code */
			code.file = token.file;
			code.line = token.line;
			code.type = bLisp_Pop_Code;

			/* add pop code */
			bLisp_Box_Add(_script->data->code, &code);
		}
	}

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseLambda                                                                              */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseLambda(bLisp_Script *_script)
{
	bLisp_Token       token;
	bLisp_Type        type,       *type_id;
	bLisp_Code        code,       *end_code;
	bLisp_Function    function,   *function_id;
	bLisp_Block      *local_block;
	bLisp_Variable   *find_result, new_var, *var_id;
	bLisp_Ref        *ref;
	bchar            *name_id;

	/* start parse arg section */
	token = bLisp_GetNextToken(_script);

	if(token.type != bLisp_LBraceToken)
	{
		bLisp_Error(_script, &token, L"Missing argument list");
	}

	token            = bLisp_GetNextToken(_script);
	function.arg_num = 0;
	function.type    = bLisp_UsualFunction;

	local_block = bLisp_CreateLocalBlock(_script);

	while(token.type != bLisp_RBraceToken)
	{
		if(token.type != bLisp_NameToken)
		{
			bLisp_Error(_script, &token, L"Function argument list can contains only names");
		}

		find_result = bLisp_FindLocalVariable(_script, token.value.name);

		if(find_result != bLisp_NotFound)
		{
			bLisp_Error(_script, &token, L"Duplicate name in argument list");
		}

		/* add new variable name */
		name_id = bLisp_StringPool_Add(_script->data->string, token.value.name);

		/* add new local variable */
		new_var.name = name_id;
		new_var.type = bLisp_LocalVariable;
		new_var.value.local = local_block->count;

		var_id = bLisp_Box_Add(_script->data->variable, &new_var);

		/* add new variable to local var list */
		local_block->local_var[local_block->count] = var_id;
		local_block->count++;
		
		function.arg_num++;

		token = bLisp_GetNextToken(_script);
	} 
	/* end parse arg section */

	/* prepare func code. It marks function strat */
	code.file = token.file;
	code.line = token.line;
	code.type = bLisp_Goto_Code;

	function.value.usual_ptr = bLisp_Box_Add(_script->data->code, &code);

	/* now parse function body */
	token = bLisp_GetNextToken(_script);

	while(token.type != bLisp_RBraceToken)
	{
		/* parse one expression */
		if(!bLisp_ParseExpression(_script, &token))
			return bfalse;

		token = bLisp_GetNextToken(_script);

		/* stupid double check */
		if(token.type != bLisp_RBraceToken)
		{
			/* prepare pop code */
			code.file = token.file;
			code.line = token.line;
			code.type = bLisp_Pop_Code;

			/* add pop code */
			bLisp_Box_Add(_script->data->code, &code);
		}
	}

	/* prepare ret code */
	code.file = token.file;
	code.line = token.line;
	code.type = bLisp_Ret_Code;

	end_code = bLisp_Box_Add(_script->data->code, &code);

	/* mark function end */
	ref             = &(_script->data->code->data + (buint)function.value.usual_ptr)->ref;
	ref->type       = bLisp_CodeRef;
	ref->value.code = end_code;

	/* add new function */
	function_id = bLisp_Box_Add(_script->data->function, &function);

	/* prepare new type with function value */
	type.type           = bLisp_FunctionType;
	type.value.function = function_id;

	type_id = bLisp_Box_Add(_script->data->constant, &type);

	/* prepare push code */
	code.file               = token.file;
	code.line               = token.line;
	code.type               = bLisp_PushC_Code;
	code.ref.type           = bLisp_ConstRef;
	code.ref.value.constant = type_id;

	bLisp_Box_Add(_script->data->code, &code);

	/* don't forget delete it */
	bLisp_DeleteLocalBlock(_script);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseCall                                                                                */
/*------------------------------------------------------------------------------------------------*/
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
	token = bLisp_GetNextToken(_script);

	for(i = 0; token.type != bLisp_RBraceToken; i++)
	{
		if(!bLisp_ParseExpression(_script, &token))
			return bfalse;

		token = bLisp_GetNextToken(_script);
	}

	/* add new callinfo */
	callinfo_id = bLisp_Box_Add(_script->data->callinfo, &callinfo);

	/* build call info code */
	code.file               = _token->file;
	code.line               = _token->line;
	code.type               = bLisp_PushI_Code;
	code.ref.type           = bLisp_CallInfoRef;
	code.ref.value.callinfo = callinfo_id;

	bLisp_Box_Add(_script->data->code, &code);

	/* buind call code */
	code.file = token.file;
	code.line = token.line;
	code.type = bLisp_Call_Code;
	code.ref.type = bLisp_ArgNumRef;
	code.ref.value.arg_num = i;

	code_id = bLisp_Box_Add(_script->data->code, &code);

	(_script->data->callinfo->data + (buint)callinfo_id)->r_ptr = code_id;

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseFunction                                                                            */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_ParseFunction(bLisp_Script *_script)
{
	bLisp_Token token;

	token = bLisp_GetNextToken(_script);

	switch(token.type)
	{
	case bLisp_NameToken:
	case bLisp_LBraceToken:
		return bLisp_ParseCall(_script, &token);

	case bLisp_IfToken:
		return bLisp_ParseIf(_script);

	case bLisp_WhileToken:
		return bLisp_ParseWhile(_script);

	case bLisp_LambdaToken:
		return bLisp_ParseLambda(_script);

	case bLisp_BeginToken:
		return bLisp_ParseBegin(_script);

	case bLisp_SetToken:
		return bLisp_ParseSet(_script);

	case bLisp_GetToken:
		return bLisp_ParseGet(_script);

	case bLisp_ArrayToken:
		return bLisp_ParseArray(_script);

	case bLisp_AddToken:
	case bLisp_SubToken:
	case bLisp_MulToken:
	case bLisp_DivToken:
		return bLisp_ParseASMD(_script, token.type);

	case bLisp_EqualToken:
	case bLisp_MoreToken:
	case bLisp_LessToken:
	case bLisp_NEqualToken:
		return bLisp_ParseLogic(_script, token.type);

	case bLisp_BindToken:
		return bLisp_ParseBind(_script);

	default:
		return bLisp_Error(_script, &token, L"Not valid function name");
	};
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_ParseExpression                                                                          */
/*------------------------------------------------------------------------------------------------*/
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
		return bLisp_Error(_script, _token, L"Parse error");

	default:
		return bLisp_Error(_script, _token, L"Unknown parse error");
	}

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_CreateData                                                                               */
/*------------------------------------------------------------------------------------------------*/
bLisp_Data *bLisp_CreateData()
{
	bLisp_Data *data = malloc(sizeof(bLisp_Data));

	data->code     = bLisp_Box_Create(sizeof(bLisp_Code), bLisp_DefaultBoxSize);
	data->array    = bLisp_Box_Create(sizeof(bLisp_Array), bLisp_DefaultBoxSize);
	data->constant = bLisp_Box_Create(sizeof(bLisp_Type), bLisp_DefaultBoxSize);
	data->variable = bLisp_Box_Create(sizeof(bLisp_Variable), bLisp_DefaultBoxSize);
	data->function = bLisp_Box_Create(sizeof(bLisp_Function), bLisp_DefaultBoxSize);
	data->callinfo = bLisp_Box_Create(sizeof(bLisp_CallInfo), bLisp_DefaultBoxSize);

	data->string   = bLisp_StringPool_Create(bLisp_DefaultPoolSize);

	return data;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_DeleteData                                                                               */
/*------------------------------------------------------------------------------------------------*/
bvoid bLisp_DeleteData(bLisp_Data *_data)
{
	bLisp_Box_Delete(_data->code);
	bLisp_Box_Delete(_data->array);
	bLisp_Box_Delete(_data->constant);
	bLisp_Box_Delete(_data->variable);
	bLisp_Box_Delete(_data->function);
	bLisp_Box_Delete(_data->callinfo);

	bLisp_StringPool_Delete(_data->string);
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_Preprocessor                                                                             */
/*------------------------------------------------------------------------------------------------*/
bLisp_Token bLisp_Preprocessor(bLisp_Script *_script)
{
	bLisp_Token token;

	token = bLisp_GetNextToken(_script);

	switch(token.type)
	{
	case bLisp_IncludeToken:
		return bLisp_IncludeProcessor(_script);

	case bLisp_NativeToken:
		return bLisp_NativeProcessor(_script);

	default:
		return token;
	}
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_IncludeProcessor                                                                         */
/*------------------------------------------------------------------------------------------------*/
bLisp_Token bLisp_IncludeProcessor(bLisp_Script *_script)
{
	bLisp_Token token;
	bchar *code;

	token = bLisp_GetNextToken(_script);

	/* check file name string */
	if(token.type != bLisp_StringToken)
	{
		bLisp_Error(_script, &token, L"Missing file name");
		token.type = bLisp_ErrorToken;
		return token;
	}

	/* if file already exists */
	if(bLisp_FindInclude(_script, token.value.name))
		return bLisp_GetNextToken(_script);

	/* read file in memory */
	code = bFile_ReadText(token.value.name);

	if(!code)
	{
		bLisp_Error(_script, &token, L"Can't open source file");
		token.type = bLisp_ErrorToken;
		return token;
	}

	bLisp_AddInclude(code, token.value.name, _script);

	/* return next token */
	return bLisp_GetNextToken(_script);
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_NativeProcessor                                                                          */
/*------------------------------------------------------------------------------------------------*/
bLisp_Token bLisp_NativeProcessor(bLisp_Script *_script)
{
	bLisp_Token token;
	bLisp_Function function, *function_id;
	bLisp_Type type, *type_id;
	bLisp_Variable var, *find_result;
	bchar *name_id;

	token = bLisp_GetNextToken(_script);

	/* check file name string */
	if(token.type != bLisp_StringToken)
	{
		bLisp_Error(_script, &token, L"Missing native function name");
		token.type = bLisp_ErrorToken;
		return token;
	}

	find_result = bLisp_FindGlobalVariable(_script, token.value.string);

	if(find_result != bLisp_NotFound)
	{
		/* return next token */
		return bLisp_GetNextToken(_script);
	}

	/* remember native function name */
	name_id = bLisp_StringPool_Add(_script->data->string, token.value.string);

	/* prepare function data */
	function.type = bLisp_NativeFunction;
	
	/* add new native function */
	function_id = bLisp_Box_Add(_script->data->function, &function);

	/* prepare new type */
	type.type = bLisp_FunctionType;
	type.value.function = function_id;

	/* add new type */
	type_id = bLisp_Box_Add(_script->data->constant, &type);

	/* prepare new variable */
	var.name = name_id;
	var.type = bLisp_GlobalVariable;
	var.value.gloabal = type_id;

	/* add new variable */
	bLisp_Box_Add(_script->data->variable, &var);

	/* return next token */
	return bLisp_GetNextToken(_script);
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_GetNextToken                                                                             */
/*------------------------------------------------------------------------------------------------*/
bLisp_Token bLisp_GetNextToken(bLisp_Script *_script)
{
	bLisp_Token    token;
	bLisp_TokenPos token_pos;
	bLisp_Include *include;

	include      = _script->source->current_include;
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
		return bLisp_Preprocessor(_script);

	case bLisp_EofToken:
		include->complete = btrue;
		include = bLisp_GetNewInclude(_script->source);
		if(include)
			return bLisp_GetNextToken(_script);
		break;

	case bLisp_LineToken:
		include->line++;
		return bLisp_GetNextToken(_script);

	case bLisp_SpaceToken:
		return bLisp_GetNextToken(_script);

	default:
		bLisp_SqueezeToken(&token_pos, &token);
		break;
	}	

	return token;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_GetNewInclude                                                                            */
/*------------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------------*/
/* bLisp_FindInclude                                                                              */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_FindInclude(bLisp_Script *_script, bchar *_name)
{
	bchar         *name;
	bLisp_Source  *source = _script->source;
	bLisp_Include *start = source->include;

	while(start->next)
	{
		name = _script->data->string->data + (buint)start->file;

		if(wcscmp(name, _name) == 0)
			return btrue;

		start = start->next;
	}

	return bfalse;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_SqueezeToken                                                                             */
/*------------------------------------------------------------------------------------------------*/
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
		size = (_data->end - _data->begin) * sizeof(bchar) - (sizeof(L'"') * 2) + sizeof(L'\0');
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
/*------------------------------------------------------------------------------------------------*/
/* bLisp_CreateSource                                                                             */
/*------------------------------------------------------------------------------------------------*/
bLisp_Source *bLisp_CreateSource(bLisp_Script *_script, bchar *_code)
{
	bLisp_Source *source;

	source                  = malloc(sizeof(bLisp_Source));
	source->include         = bLisp_AllocInclude(_code, L"main script");
	source->current_include = source->include;

	/* add file name in string pool */
	source->include->file = bLisp_StringPool_Add(_script->data->string, source->include->file);

	return source;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_DeleteSource                                                                             */
/*------------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------------*/
/* bLisp_AddInclude                                                                               */
/*------------------------------------------------------------------------------------------------*/
bbool bLisp_AddInclude(bchar *_code, bchar *_file, bLisp_Script *_script)
{
	bLisp_Source  *source    = _script->source;
	bLisp_Include *temp      = source->include;

	source->include         = bLisp_AllocInclude(_code, _file);
	source->include->next   = temp;
	source->current_include = source->include;

	source->include->file = bLisp_StringPool_Add(_script->data->string, source->include->file);

	return btrue;
}
/*------------------------------------------------------------------------------------------------*/
/* bLisp_AllocInclude                                                                             */
/*------------------------------------------------------------------------------------------------*/
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
/*------------------------------------------------------------------------------------------------*/
/* bLisp_IdentNextToken                                                                           */
/*------------------------------------------------------------------------------------------------*/
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

			if(iswalpha(*_code) || *_code == L'_')
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

			case L'a':
				state = bLisp_A_Lexer_State;
				break;

			case L'b':
				state = bLisp_B_Lexer_State;
				break;

			case L'l':
				state = bLisp_L_Lexer_State;
				break;

			case L'n':
				state = bLisp_N_Lexer_State;
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
			if(*_code == L'a')
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

		case bLisp_PlusPlus_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_IncToken);

		case bLisp_MinusMinus_Lexer_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_DecToken);

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

		case bLisp_A_Lexer_State:
			if(*_code == L'r')
			{
				state = bLisp_Ar_Lexer_State;
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

		case bLisp_Ar_Lexer_State:
			if(*_code == L'r')
			{
				state = bLisp_Arr_Lexer_State;
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

		case bLisp_Arr_Lexer_State:
			if(*_code == L'a')
			{
				state = bLisp_Arra_Lexer_State;
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

		case bLisp_Arra_Lexer_State:
			if(*_code == L'y')
			{
				state = bLisp_Array_Lexer_State;
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

		case bLisp_Array_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_ArrayToken);
			}

		case bLisp_N_Lexer_State:
			if(*_code == L'a')
			{
				state = bLisp_Na_Lexer_State;
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

		case bLisp_Na_Lexer_State:
			if(*_code == L't')
			{
				state = bLisp_Nat_Lexer_State;
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

		case bLisp_Nat_Lexer_State:
			if(*_code == L'i')
			{
				state = bLisp_Nati_Lexer_State;
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

		case bLisp_Nati_Lexer_State:
			if(*_code == L'v')
			{
				state = bLisp_Nativ_Lexer_State;
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

		case bLisp_Nativ_Lexer_State:
			if(*_code == L'e')
			{
				state = bLisp_Native_Lexer_State;
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

		case bLisp_Native_Lexer_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_Lexer_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_NativeToken);
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
			if(iswalpha(*_code) || iswdigit(*_code) || *_code == L'_')
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
/*------------------------------------------------------------------------------------------------*/
/* bLisp_BuildTokenPos                                                                            */
/*------------------------------------------------------------------------------------------------*/
bLisp_TokenPos bLisp_BuildTokenPos(bchar *_b, bchar *_e, bLisp_TokenClass _t)
{
	bLisp_TokenPos token;
	
	token.begin = _b;
	token.end   = _e;
	token.type  = _t;

	return token;
}