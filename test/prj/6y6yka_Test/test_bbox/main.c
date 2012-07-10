/*----------------------------------------------------------------------------*/
/*                           6y6yka Engine test                               */
/*----------------------------------------------------------------------------*/
#include <windows.h>
#include "b_box.h"
#include "b_box.c"
/*----------------------------------------------------------------------------*/
typedef struct
{
	bint  *data;
	buint  item_size;
	buint  item_num;
	buint  item_num_max;
} bLisp_int_Box;
/*----------------------------------------------------------------------------*/
int main()
{
	bLisp_int_Box *box;
	int *result;
	int data[5] = {1, 2, 3, 4, 5};

	box = bBox_Create(sizeof(int), 2);

	bBox_Add(box, data);
	bBox_Add(box, data + 1);
	bBox_Add(box, data + 2);
	bBox_Add(box, data + 3);
	result = bBox_Add(box, data + 4);

	if(*result != 5)
		return 1;

	bBox_Delete(box);

	return 0;
}