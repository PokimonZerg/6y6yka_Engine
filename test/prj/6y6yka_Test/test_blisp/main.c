/*------------------------------------------------------------------------------------------------*/
/*                                       6y6yka Engine test                                       */
/*------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include "b_define.h"
#include "b_align.c"
#include "b_log.c"
#include "b_error.c"
#include "b_unicode.c"
#include "b_file.h"
#include "b_file.c"
#include "b_lisp.h"
#include "b_lisp.c"
/*------------------------------------------------------------------------------------------------*/
#define tests_num 42
/*------------------------------------------------------------------------------------------------*/
bchar *right_result[tests_num];
bchar *input_data[tests_num];
/*------------------------------------------------------------------------------------------------*/
bvoid build_data();
bint hope(bint _i);
void reg_hope(bvoid *_stack);
/*------------------------------------------------------------------------------------------------*/
int main()
{
	bLisp_Script *s[tests_num];
	bchar *result;
	buint i;

	build_data();

	for(i = 0; i < tests_num; i++)
	{
		s[i] = bLisp_OpenScript(input_data[i], 0);

		if(!s[i])
		{
			return 1;
		}

		if(i == 25)
		{
			bLisp_RegisterFunction(s[i], L"hope", reg_hope);
		}

		if(i == 37)
		{
			bLisp_Save(L"main.bcode", s[i]);
			bLisp_Close(s[i]);
			s[i] = bLisp_OpenCode(L"main.bcode");
		}

		if(!bLisp_Run(s[i], &result, 0))
		{
			return 1;
		}

		if(wcscmp(result, right_result[i]) != 0)
		{
			return 1;
		}
	}

	return 0;
}
/*------------------------------------------------------------------------------------------------*/
bvoid build_data()
{
	right_result[0]  = L"5"; input_data[0]  = L"(+ 2 3)";
	right_result[1]  = L"4"; input_data[1]  = L"(bind a 4)";
	right_result[2]  = L"3"; input_data[2]  = L" 2 3";
	right_result[3]  = L"2"; input_data[3]  = L"(bind b 2) b";
	right_result[4]  = L"1"; input_data[4]  = L"(bind g 7) (set g 1) g";
	right_result[5]  = L"7"; input_data[5]  = L"(bind f (lambda (x) x)) 7";
	right_result[6]  = L"8"; input_data[6]  = L"(bind f (lambda (x) x)) (f 8)";
	right_result[7]  = L"9"; input_data[7]  = L"(bind f (lambda (y) (+ y 4))) (f 5)";
	right_result[8]  = L"3"; input_data[8]  = L"((lambda (x) x) 3)";
	right_result[9]  = L"1"; input_data[9]  = L"((lambda (x y) x) 1 2)";
	right_result[10] = L"2"; input_data[10] = L"((lambda (x y) y) 1 2)";
	right_result[11] = L"3"; input_data[11] = L"(bind f (lambda (y) y)) (bind g (lambda (x) x)) (g 3)";
	right_result[12] = L"3"; input_data[12] = L"((lambda (x) ((lambda (y) y) x)) 3)";
	right_result[13] = L"5"; input_data[13] = L"(bind a 7) (set a 5)";
	right_result[14] = L"2"; input_data[14] = L"(if 0 1 2)";
	right_result[15] = L"1"; input_data[15] = L"(if 1 1 2)";
	right_result[16] = L"3"; input_data[16] = L"(if 1 3)";
	right_result[17] = L"0"; input_data[17] = L"(if 0 1)";
	right_result[18] = L"5"; input_data[18] = L"((lambda (x) (if x 5 3)) 1)";
	right_result[19] = L"0"; input_data[19] = L"((lambda (x) (if x 5)) 0)";
	right_result[20] = L"5"; input_data[20] = L"((lambda (x) (if x 5)) 7)";
	right_result[21] = L"5"; input_data[21] = L"((lambda (x) x 1 2 3 4 (+ 2 3)) 0)";
	right_result[22] = L"0"; input_data[22] = L"(bind a 1) (while a (set a 0))";
	right_result[23] = L"3"; input_data[23] = L"(bind a -3) (while a (set a (+ a 1))) 3";
	right_result[24] = L"2"; input_data[24] = L"#native \"hello\" 2";
	right_result[25] = L"3"; input_data[25] = L"#native \"hope\" (hope 1)";
	right_result[26] = L"5"; input_data[26] = L"#include \"test.blisp\" #include \"test.blisp\"";
	right_result[27] = L"7"; input_data[27] = L"((lambda (x) (set x 7) x) 5)";
	right_result[28] = L"2"; input_data[28] = L"(- 100 97 1)";
	right_result[29] = L"4"; input_data[29] = L"(* 2 2)";
	right_result[30] = L"2"; input_data[30] = L"(/ 10 5)";
	right_result[31] = L"20";input_data[31] = L"(+ (- 3 2) (* 8 2) (/ 9 3))";
	right_result[32] = L"0"; input_data[32] = L"(= 2 5)";
	right_result[33] = L"1"; input_data[33] = L"(< 2 5)";
	right_result[34] = L"0"; input_data[34] = L"(> 2 5)";
	right_result[35] = L"1"; input_data[35] = L"(!= 2 5)";
	right_result[36] = L"C"; input_data[36] = L"\"C\"";
	right_result[37] = L"C"; input_data[37] = L"(+ 2 3) \"C\"";
	right_result[38] = L"0"; input_data[38] = L"(get (array 5) 1)";
	right_result[39] = L"7"; input_data[39] = L"(bind a (array 5)) (set (get a 1) 7) (get a 1)";
	right_result[40] = L"120"; input_data[40] = 
		L"(bind fact (lambda (x) (if (< x 3) x (* (fact (- x 1)) x)))) (fact 5)";
	right_result[41] = L"C"; input_data[41] = L"((lambda () (begin 1 2 3 4 5 \"C\")) )";
}
/*------------------------------------------------------------------------------------------------*/
bint hope(bint _i)
{
	return _i + 2;
}
/*------------------------------------------------------------------------------------------------*/
void reg_hope(bvoid *_stack)
{
	bLisp_ReturnNumber(_stack, (bfloat)hope((bint)bLisp_GetNumberArg(_stack, 0)));
}
