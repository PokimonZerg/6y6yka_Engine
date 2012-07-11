/*----------------------------------------------------------------------------*/
/*                              6y6yka Engine                                 */
/*----------------------------------------------------------------------------*/
#ifndef _B_BOX_H_
#define _B_BOX_H_
/*----------------------------------------------------------------------------*/
#include "b_type.h"
/*----------------------------------------------------------------------------*/
/* bBox                                                                       */
/*                                                                            */
/* Box for items with equal size.                                             */
/*----------------------------------------------------------------------------*/
struct bBox
{
	bbyte *data;
	buint  item_size;
	buint  item_num;
	buint  item_num_max;
};

typedef struct bBox bBox;
/*----------------------------------------------------------------------------*/
/* bBox_Create - create new box structure.                                    */
/*                                                                            */
/* arguments:                                                                 */
/*     _size - single item size. Must be greater then zero.                   */
/*     _num  - items num. Must be greater then zero.                          */
/* return:                                                                    */
/*     pointer to bBox structure.                                             */
/*----------------------------------------------------------------------------*/
bvoid *bBox_Create(buint _size, buint _num);
/*----------------------------------------------------------------------------*/
/* bBox_Add - add item in box. Also resize box.                               */
/*                                                                            */
/* arguments:                                                                 */
/*     _box  - pointer to valid bbox structure.                               */
/*     _item - pointer to new item.                                           */
/* return:                                                                    */
/*     pointer to copy of item in box.                                        */
/*----------------------------------------------------------------------------*/
bvoid *bBox_Add(bvoid *_box, bvoid *_item);
/*----------------------------------------------------------------------------*/
/* bBox_Delete - delete box and all his data->                                 */
/*                                                                            */
/* arguments:                                                                 */
/*     _box - pointer to valid bbox structure.                                */
/*----------------------------------------------------------------------------*/
bvoid bBox_Delete(bvoid *_box);
/*----------------------------------------------------------------------------*/
/* bBox_GetLast - return last added item.                                     */
/*                                                                            */
/* arguments:                                                                 */
/*     _box - pointer to valid bbox structure.                                */
/*----------------------------------------------------------------------------*/
bvoid *bBox_GetLast(bvoid *_box);
/*----------------------------------------------------------------------------*/
#endif /* _B_BOX_H_ */