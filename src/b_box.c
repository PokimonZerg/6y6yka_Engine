/*----------------------------------------------------------------------------*/
/*                              6y6yka Engine                                 */
/*----------------------------------------------------------------------------*/
#include <stdlib.h>
#include "b_assert.h"
#include "b_box.h"
/*----------------------------------------------------------------------------*/
/* bBox_Create                                                                */
/*----------------------------------------------------------------------------*/
bvoid *bBox_Create(buint _size, buint _num)
{
	bBox *box;

	bAssert(_size);
	bAssert(_num);

	box = malloc(sizeof(bBox));

	box->item_size    = _size;
	box->item_num     = 0;
	box->item_num_max = _num;
	box->data         = malloc(box->item_size * box->item_num_max);

	return box;
}
/*----------------------------------------------------------------------------*/
/* bBox_Delete                                                                */
/*----------------------------------------------------------------------------*/
bvoid bBox_Delete(bvoid *_box)
{
	bBox *ptr = _box;

	free(ptr->data);
	free(ptr);
}
/*----------------------------------------------------------------------------*/
/* bBox_GetLast                                                               */
/*----------------------------------------------------------------------------*/
bvoid *bBox_GetLast(bvoid *_box)
{
	bBox *box = _box;

	return box->data + box->item_size * (box->item_num - 1);
}
/*----------------------------------------------------------------------------*/
/* bBox_Add                                                                   */
/*----------------------------------------------------------------------------*/
bvoid *bBox_Add(bvoid *_box, bvoid *_item)
{
	bbyte *new_data;
	bBox  *box = _box;

	if(box->item_num == box->item_num_max)
	{
		new_data = malloc((box->item_num_max * box->item_size) * 2);

		memcpy(new_data, box->data, box->item_num_max * box->item_size);

		free(box->data);

		box->data = new_data;
		box->item_num_max *= 2;
	}

	memcpy(box->data + box->item_num * box->item_size, _item, box->item_size);

	return  box->data + box->item_num++ * box->item_size;
}