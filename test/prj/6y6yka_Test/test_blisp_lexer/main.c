/*----------------------------------------------------------------------------*/
/*                           6y6yka Engine test                               */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include "b_lisp.c"
/*----------------------------------------------------------------------------*/
#define num_tests 35
/*----------------------------------------------------------------------------*/
void build_data(bLisp_TokenClass *_result, wchar_t *_data[num_tests]);
/*----------------------------------------------------------------------------*/
int main()
{
	bLisp_TokenClass right_result[num_tests];
	wchar_t         *input_data[num_tests];
    bLisp_TokenPos   bad_result;
	int i, result = 0;

	build_data(right_result, input_data);
	
	for(i = 0; i < num_tests; i++)
	{
		bad_result = bLisp_IdentNextToken(input_data[i]);

		if(bad_result.type != right_result[i])
		{
			result = 1;
			break;
		}
	}

	return result;
}
/*----------------------------------------------------------------------------*/
void build_data(bLisp_TokenClass *_result, wchar_t *input_data[num_tests])
{
	_result[0] = bLisp_EofToken;     input_data[0] = L"";
	_result[1] = bLisp_AddToken;     input_data[1] = L"+";
	_result[2] = bLisp_SubToken;     input_data[2] = L"-";
	_result[3] = bLisp_MulToken;     input_data[3] = L"*";
	_result[4] = bLisp_DivToken;     input_data[4] = L"/";
	_result[5] = bLisp_IfToken;      input_data[5] = L"if";
	_result[6] = bLisp_DigitToken;   input_data[6] = L"5";
	_result[7] = bLisp_FloatToken;   input_data[7] = L"1.7";
	_result[8] = bLisp_StringToken;  input_data[8] = L"\"hello world\"";
	_result[9] = bLisp_NameToken;    input_data[9] = L"i";
	_result[10]= bLisp_BindToken;    input_data[10]= L"bind";
	_result[11]= bLisp_BeginToken;   input_data[11]= L"begin";
	_result[12]= bLisp_GetToken;     input_data[12]= L"get";
	_result[13]= bLisp_WhileToken;   input_data[13]= L"while";
	_result[14]= bLisp_CommentToken; input_data[14]= L"// comment";
	_result[15]= bLisp_SharpToken;   input_data[15]= L"#";
	_result[16]= bLisp_IncludeToken; input_data[16]= L"include";
	_result[17]= bLisp_CarToken;     input_data[17]= L"car";
	_result[18]= bLisp_CdrToken;     input_data[18]= L"cdr";
	_result[19]= bLisp_ConsToken;    input_data[19]= L"cons";
	_result[20]= bLisp_IncToken;     input_data[20]= L"++";
	_result[21]= bLisp_IncToken;     input_data[21]= L"+++";
	_result[22]= bLisp_DecToken;     input_data[22]= L"--";
	_result[23]= bLisp_DecToken;     input_data[23]= L"--//";
	_result[24]= bLisp_ListToken;    input_data[24]= L"list";
	_result[25]= bLisp_LetToken;     input_data[25]= L"let";
	_result[26]= bLisp_LambdaToken;  input_data[26]= L"lambda";
	_result[27]= bLisp_SpaceToken;   input_data[27]= L"\t";
	_result[28]= bLisp_LineToken;    input_data[28]= L"\n";
	_result[29]= bLisp_NEqualToken;  input_data[29]= L"!=";
	_result[30]= bLisp_NotToken;     input_data[30]= L"!5";
	_result[31]= bLisp_LBraceToken;  input_data[31]= L"(";
	_result[32]= bLisp_RBraceToken;  input_data[32]= L")";
	_result[33]= bLisp_ErrorToken;   input_data[33]= L"\"error str";
	_result[34]= bLisp_SetToken;     input_data[34]= L"set";
}