/*------------------------------------------------------------------------------------------------*/
/*                                       6y6yka Engine test                                       */
/*------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include "b_pool.c"
#include "b_stack.c"
#include "b_lisp.c"
/*------------------------------------------------------------------------------------------------*/
#define num_tests 1
/*------------------------------------------------------------------------------------------------*/
void build_data(bLisp_Token *_result, bLisp_Source _data[num_tests]);
/*------------------------------------------------------------------------------------------------*/
int test1(); int test2(); int test3(); int test4(); int test5();
/*------------------------------------------------------------------------------------------------*/
void build_source(wchar_t *_code, bLisp_Source *_source);
/*------------------------------------------------------------------------------------------------*/
int main()
{
	bLisp_Token      right_result[num_tests];
	bLisp_Source     input_data[num_tests];
    bLisp_Token      bad_result;
	int i, result =  0;

	build_data(right_result, input_data);
	
	for(i = 0; i < num_tests; i++)
	{
		bad_result = bLisp_GetNextToken(&input_data[i]);

		if(bad_result.type != right_result[i].type)
		{
			result = 1;
			break;
		}
	}

	if(test1()) return 1;
	if(test2()) return 1;
	if(test3()) return 1;
	if(test4()) return 1;

	return result;
}
/*------------------------------------------------------------------------------------------------*/
int test1()
{
	bLisp_Token  result, bad_result;
	bLisp_Source data;

	result.type = bLisp_NumberToken; result.value.number = 5;
	build_source(L"5", &data);

	bad_result = bLisp_GetNextToken(&data);

	if(result.type != bad_result.type)
		return 1;
	if(result.value.number != bad_result.value.number)
		return 1;

	return 0;
}
/*------------------------------------------------------------------------------------------------*/
int test2()
{
	bLisp_Token  result, bad_result;
	bLisp_Source data;

	result.type = bLisp_NumberToken; result.value.number = 5.7f;
	build_source(L"5.7", &data);

	bad_result = bLisp_GetNextToken(&data);

	if(result.type != bad_result.type)
		return 1;
	if(result.value.number != bad_result.value.number)
		return 1;

	return 0;
}
/*------------------------------------------------------------------------------------------------*/
int test3()
{
	bLisp_Token  result, bad_result;
	bLisp_Source data;

	result.type = bLisp_StringToken; result.value.string = L"hello world!";
	build_source(L"\"hello world!\"", &data);

	bad_result = bLisp_GetNextToken(&data);

	if(result.type != bad_result.type)
		return 1;
	if(wcscmp(result.value.string, bad_result.value.string) != 0)
		return 1;

	return 0;
}
/*------------------------------------------------------------------------------------------------*/
int test4()
{
	bLisp_Token  result, bad_result;
	bLisp_Source data;

	result.type = bLisp_NameToken; result.value.name = L"hello";
	build_source(L"hello world!", &data);

	bad_result = bLisp_GetNextToken(&data);

	if(result.type != bad_result.type)
		return 1;
	if(wcscmp(result.value.name, bad_result.value.name) != 0)
		return 1;

	return 0;
}
/*------------------------------------------------------------------------------------------------*/
void build_data(bLisp_Token *_result, bLisp_Source input_data[num_tests])
{
	_result[0].type = bLisp_AddToken; build_source(L"+", &input_data[0]);
	
}
/*------------------------------------------------------------------------------------------------*/
void build_source(wchar_t *_code, bLisp_Source *_source)
{
	_source->include           = malloc(sizeof(bLisp_Include));
	_source->current_include   = _source->include;
	_source->include->source   = _code;
	_source->include->complete = bfalse;
	_source->include->pos      = _code;
	_source->include->line     = 1;
	_source->include->file     = L"hello.c";
}