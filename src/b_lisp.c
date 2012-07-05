/*----------------------------------------------------------------------------*/
/*                              6y6yka Engine                                 */
/*----------------------------------------------------------------------------*/
#include <stdlib.h>
#include <ctype.h>
#include "b_assert.h"
#include "b_lisp.h"
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
/* bLisp_TokenClass                                                           */
/*----------------------------------------------------------------------------*/
enum bLisp_TokenClass
{
	bLisp_NameToken,    /* variable name  */
	bLisp_DigitToken,   /* integer        */
	bLisp_FloatToken,   /* float          */
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
	bint   int_value; 
	bfloat float_value;
	bchar *string_value;
	bchar *name_value;
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
/* bLisp_LexerState                                                           */
/*----------------------------------------------------------------------------*/
enum bLisp_LexerState
{
	bLisp_Unknown_State,
	bLisp_Name_State,
	bLisp_PlusPlus_State,
	bLisp_MinusMinus_State,
	bLisp_Digit_State,
	bLisp_Float_State,
	bLisp_String_State,
	bLisp_Comment_State,
	bLisp_Slash_State,
	bLisp_Add_State,
	bLisp_Sub_State,
	bLisp_Mul_State,
	bLisp_I_State,
	bLisp_If_State,
	bLisp_LBrace_State,
	bLisp_RBrace_State,
	bLisp_Equal_State,
	bLisp_Less_State,
	bLisp_More_State,
	bLisp_S_State,
	bLisp_Se_State,
	bLisp_Set_State,
	bLisp_B_State,
	bLisp_Be_State,
	bLisp_Beg_State,
	bLisp_Begi_State,
	bLisp_Begin_State,
	bLisp_C_State,
	bLisp_Ca_State,
	bLisp_Car_State,
	bLisp_Cd_State,
	bLisp_Cdr_State,
	bLisp_Co_State,
	bLisp_Con_State,
	bLisp_Cons_State,
	bLisp_L_State,
	bLisp_Li_State,
	bLisp_Lis_State,
	bLisp_List_State,
	bLisp_Le_State,
	bLisp_Let_State,
	bLisp_G_State,
	bLisp_Ge_State,
	bLisp_Get_State,
	bLisp_Sharp_State,
	bLisp_In_State,
	bLisp_Inc_State,
	bLisp_Incl_State,
	bLisp_Inclu_State,
	bLisp_Includ_State,
	bLisp_Include_State,
	bLisp_W_State,
	bLisp_Wh_State,
	bLisp_Whi_State,
	bLisp_Whil_State,
	bLisp_While_State,
	bLisp_Bi_State,
	bLisp_Bin_State,
	bLisp_Bind_State,
	bLisp_La_State,
	bLisp_Lam_State,
	bLisp_Lamb_State,
	bLisp_Lambd_State,
	bLisp_Lambda_State,
	bLisp_NewLine_State,
	bLisp_Space_State,
	bLisp_NEqual_State,
	bLisp_Not_State
};

typedef enum bLisp_LexerState bLisp_LexerState;
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
/* bLisp_BuildTokenPos                                                        */
/*----------------------------------------------------------------------------*/
bLisp_TokenPos 
bLisp_BuildTokenPos(bchar *_begin, bchar *_end, bLisp_TokenClass _type);
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
/* bLisp_IdentNextToken                                                       */
/*----------------------------------------------------------------------------*/
bLisp_TokenPos bLisp_IdentNextToken(bchar *_code)
{
	bLisp_LexerState state;
	bchar           *begin;

	begin = _code;
	state = bLisp_Unknown_State;

	do
	{
		switch(state) /* epic switch */
		{
		case bLisp_Unknown_State:
			if(iswdigit(*_code))
			{
				state = bLisp_Digit_State;
				break;
			}

			if(iswalpha(*_code))
			{
				state = bLisp_Name_State;
			}

			switch(*_code)
			{
			case L'\0':
				return bLisp_BuildTokenPos(begin, _code, bLisp_EofToken);

			case L'\n':
				state = bLisp_NewLine_State;
				break;

			case L'-':
				state = bLisp_Sub_State;
				break;

			case L'+':
				state = bLisp_Add_State;
				break;

			case L'*':
				state = bLisp_Mul_State;
				break;

			case L'/':
				state = bLisp_Slash_State;
				break;

			case L'"':
				state = bLisp_String_State;
				break;

			case L'(':
				state = bLisp_LBrace_State;
				break;

			case L')':
				state = bLisp_RBrace_State;
				break;

			case L'#':
				state = bLisp_Sharp_State;
				break;

			case L'=':
				state = bLisp_Equal_State;
				break;

			case L'!':
				state = bLisp_Not_State;
				break;

			case L'<':
				state = bLisp_Less_State;
				break;

			case L'>':
				state = bLisp_More_State;
				break;

			case L'i':
				state = bLisp_I_State;
				break;

			case L'b':
				state = bLisp_B_State;
				break;

			case L'l':
				state = bLisp_L_State;
				break;

			case L's':
				state = bLisp_S_State;
				break;

			case L'w':
				state = bLisp_W_State;
				break;

			case L'g':
				state = bLisp_G_State;
				break;

			case L'c':
				state = bLisp_C_State;
				break;

			default:
				state = bLisp_Space_State;
				break;
			}
			
			break;

		case bLisp_S_State:
			if(*_code == L'e')
				state = bLisp_Se_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Se_State:
			if(*_code == L't')
				state = bLisp_Set_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Set_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_SetToken);

		case bLisp_Not_State:
			if(*_code == L'=')
			{
				state = bLisp_NEqual_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_NotToken);

		case bLisp_NEqual_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_NEqualToken);

		case bLisp_NewLine_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_LineToken);

		case bLisp_Space_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_SpaceToken);

		case bLisp_L_State:
			switch(*_code)
			{
			case L'e':
				state = bLisp_Le_State;
				break;

			case L'i':
				state = bLisp_Li_State;
				break;

			case L'a':
				state = bLisp_La_State;
				break;

			default:
				state = bLisp_Name_State;
				break;
			}
			break;

		case bLisp_La_State:
			if(*_code == L'm')
				state = bLisp_Lam_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Lam_State:
			if(*_code == L'b')
				state = bLisp_Lamb_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Lamb_State:
			if(*_code == L'd')
				state = bLisp_Lambd_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Lambd_State:
			if(*_code == L'a')
				state = bLisp_Lambda_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Lambda_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_LambdaToken);

		case bLisp_Li_State:
			if(*_code == L's')
				state = bLisp_Lis_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Lis_State:
			if(*_code == L't')
				state = bLisp_List_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_List_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_ListToken);

		case bLisp_Le_State:
			if(*_code == L't')
				state = bLisp_Let_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Let_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_LetToken);

		case bLisp_PlusPlus_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_IncToken);

		case bLisp_MinusMinus_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_DecToken);

		case bLisp_C_State:
			switch(*_code)
			{
			case L'a':
				state = bLisp_Ca_State;
				break;

			case L'd':
				state = bLisp_Cd_State;
				break;

			case L'o':
				state = bLisp_Co_State;
				break;

			default:
				state = bLisp_Name_State;
				break;
			}
			break;

		case bLisp_Car_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_CarToken);

		case bLisp_Cdr_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_CdrToken);

		case bLisp_Cons_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_ConsToken);

		case bLisp_Ca_State:
			if(*_code == L'r')
				state = bLisp_Car_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Cd_State:
			if(*_code == L'r')
				state = bLisp_Cdr_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Co_State:
			if(*_code == L'n')
				state = bLisp_Con_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Con_State:
			if(*_code == L's')
				state = bLisp_Cons_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_In_State:
			if(*_code == L'c')
				state = bLisp_Inc_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Inc_State:
			if(*_code == L'l')
				state = bLisp_Incl_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Incl_State:
			if(*_code == L'u')
				state = bLisp_Inclu_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Inclu_State:
			if(*_code == L'd')
				state = bLisp_Includ_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Includ_State:
			if(*_code == L'e')
				state = bLisp_Include_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Include_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_IncludeToken);

		case bLisp_Sharp_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_SharpToken);

		case bLisp_Comment_State:
			if(*_code == L'\0')
				return bLisp_BuildTokenPos(begin, _code, bLisp_CommentToken);
			break;

		case bLisp_W_State:
			if(*_code == L'h')
				state = bLisp_Wh_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Wh_State:
			if(*_code == L'i')
				state = bLisp_Whi_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Whi_State:
			if(*_code == L'l')
				state = bLisp_Whil_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Whil_State:
			if(*_code == L'e')
				state = bLisp_While_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_While_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_WhileToken);

		case bLisp_G_State:
			if(*_code == L'e')
				state = bLisp_Ge_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Ge_State:
			if(*_code == L't')
				state = bLisp_Get_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Get_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_GetToken);

		case bLisp_B_State:
			if(*_code == L'e')
				state = bLisp_Be_State;
			else if(*_code == L'i')
				state = bLisp_Bi_State;
			else
				state = bLisp_Name_State;		
			break;

		case bLisp_Bi_State:
			if(*_code == L'n')
				state = bLisp_Bin_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Bin_State:
			if(*_code == L'd')
				state = bLisp_Bind_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Bind_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_BindToken);
		
		case bLisp_Be_State:
			if(*_code == L'g')
				state = bLisp_Beg_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Beg_State:
			if(*_code == L'i')
				state = bLisp_Begi_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Begi_State:
			if(*_code == L'n')
				state = bLisp_Begin_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_Begin_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_State;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_BeginToken);

		case bLisp_More_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_MoreToken);

		case bLisp_Less_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_LessToken);

		case bLisp_Equal_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_EqualToken);

		case bLisp_LBrace_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_LBraceToken);

		case bLisp_RBrace_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_RBraceToken);

		case bLisp_String_State:
			if(*_code == L'"')
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_StringToken);
			}
			else if(*_code == L'\n' || *_code == L'\0')
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_ErrorToken);
			}
			else
				break;

		case bLisp_Digit_State:
			if(iswdigit(*_code))
			{
				break;
			}
			else if(*_code == L'.')
			{
				state = bLisp_Float_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_DigitToken);
			}

		case bLisp_Float_State:
			if(iswdigit(*_code))
			{
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_FloatToken);
			}

		case bLisp_Sub_State:
			if(iswdigit(*_code))
			{
				state = bLisp_Digit_State;
				break;
			}
			else if(*_code == L'-')
			{
				state = bLisp_MinusMinus_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_SubToken);

		case bLisp_Add_State:
			if(iswdigit(*_code))
			{
				state = bLisp_Digit_State;
				break;
			}
			else if(*_code == L'+')
			{
				state = bLisp_PlusPlus_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_AddToken);

		case bLisp_Mul_State:
			return bLisp_BuildTokenPos(begin, _code, bLisp_MulToken);

		case bLisp_Slash_State:
			if(*_code == '/')
			{
				state = bLisp_Comment_State;
				break;
			}
			else
				return bLisp_BuildTokenPos(begin, _code, bLisp_DivToken);

		case bLisp_I_State:
			if(*_code == L'f')
				state = bLisp_If_State;
			else if(*_code == L'n')
				state = bLisp_In_State;
			else
				state = bLisp_Name_State;
			break;

		case bLisp_If_State:
			if(iswalpha(*_code) || iswdigit(*_code))
			{
				state = bLisp_Name_State;
				break;
			}
			else
			{
				return bLisp_BuildTokenPos(begin, _code, bLisp_IfToken);
			}

		default:
			return bLisp_BuildTokenPos(begin, _code, bLisp_ErrorToken);
		}
	} while(*(_code++));

	return bLisp_BuildTokenPos(begin, _code, bLisp_NameToken);
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