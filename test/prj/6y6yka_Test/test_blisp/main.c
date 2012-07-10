/*----------------------------------------------------------------------------*/
/*                           6y6yka Engine test                               */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include "b_lisp.h"
#include "b_box.c"
#include "b_pool.c"
#include "b_lisp.c"
/*----------------------------------------------------------------------------*/
#define tests_num 5
/*----------------------------------------------------------------------------*/
bchar *right_result[tests_num];
bchar *input_data[tests_num];
/*----------------------------------------------------------------------------*/
bvoid build_data();
/*----------------------------------------------------------------------------*/
int main()
{
	bLisp_Script *s[tests_num];
	bchar *result;
	buint i;

	build_data();

	for(i = 0; i < tests_num; i++)
	{
		s[i] = bLisp_OpenScript(input_data[i]);

		if(!s[i])
		{
			return 1;
		}

		if(!bLisp_Run(s[i], &result))
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
/*----------------------------------------------------------------------------*/
bvoid build_data()
{
	right_result[0] = L"5"; input_data[0] = L"(+ 2 3)";
	right_result[1] = L"4"; input_data[1] = L"(bind a 4)";
	right_result[2] = L"3"; input_data[2] = L" 2 3";
	right_result[3] = L"2"; input_data[3] = L"(bind b 2) b";
	right_result[4] = L"1"; input_data[4] = L"(bind g 7) (set g 1) g";
}
