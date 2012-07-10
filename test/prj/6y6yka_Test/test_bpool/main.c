/*----------------------------------------------------------------------------*/
/*                           6y6yka Engine test                               */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include "b_pool.h"
#include "b_pool.c"
/*----------------------------------------------------------------------------*/
typedef struct Test_StringPool
{
	bchar *data;
	buint  max_size;
	buint  size;
} Test_StringPool;
/*----------------------------------------------------------------------------*/
int main()
{
	Test_StringPool *pool;
	bchar *result;

	pool = bPool_Create(10);

	result = bPool_Add(pool, L"Hello world", sizeof(L"Hello world"));
	
	bPool_Add(pool, L"Hello world", sizeof(L"Hello world"));

	if(wcscmp(L"Hello world", pool->data + 2) != 0)
		return 1;
	if(wcscmp(L"Hello world", result) != 0)
		return 1;

	bPool_Delete(pool);

	return 0;
}