/*----------------------------------------------------------------------------*/
/*                              6y6yka Engine                                 */
/*----------------------------------------------------------------------------*/
#ifndef _B_POOL_H_
#define _B_POOL_H_
/*----------------------------------------------------------------------------*/
#include "b_type.h"
/*----------------------------------------------------------------------------*/
/* bPool                                                                      */
/*                                                                            */
/* Pool for items with variant size.                                          */
/*----------------------------------------------------------------------------*/
struct bPool
{
	bbyte *data;
	buint  max_size;
	buint  size;
};

typedef struct bPool bPool;
/*----------------------------------------------------------------------------*/
/* bPool_Create - create new pool structure.                                  */
/*                                                                            */
/* arguments:                                                                 */
/*     _size - initial buffer size in bytes.                                  */
/* return:                                                                    */
/*     pointer to bPool structure.                                            */
/*----------------------------------------------------------------------------*/
bvoid *bPool_Create(buint _size);
/*----------------------------------------------------------------------------*/
/* bPool_Add - add new item in pool.                                          */
/*                                                                            */
/* arguments:                                                                 */
/*     _pool - pointer to valid pool.                                         */
/*     _item - pointer to new item;                                           */
/*     _size - item size in bytes.                                            */
/* return:                                                                    */
/*     pointer to item in pool.                                               */
/*----------------------------------------------------------------------------*/
bvoid *bPool_Add(bvoid *_pool, bvoid *_item, buint _size);
/*----------------------------------------------------------------------------*/
/* bPool_Delete - delete pool ad all his data->                                */
/*                                                                            */
/* arguments:                                                                 */
/*     _pool - pointer to valid pool.                                         */
/*----------------------------------------------------------------------------*/
bvoid bPool_Delete(bvoid *_pool);
/*----------------------------------------------------------------------------*/
/* bPool_GetNext - return pointer to next item in pool.                       */
/*                                                                            */
/* arguments:                                                                 */
/*     _pool - pointer to valid pool.                                         */
/*     _pos  - position for new search.                                       */
/* return:                                                                    */
/*     return pointer to next item. If _pos == pool->data it return secon     */
/*     item. Return value contain dword item size, then item data->            */
/*     Function return 0 for last item.                                       */
/*----------------------------------------------------------------------------*/
bvoid *bPool_GetNext(bvoid *_pool, bvoid *_pos);
/*----------------------------------------------------------------------------*/
#endif /* _B_POOL_H_ */