/*----------------------------------------------------------------------------*/
/*                              6y6yka Engine                                 */
/*----------------------------------------------------------------------------*/
#include <stdlib.h>
#include "b_assert.h"
#include "b_pool.h"
/*----------------------------------------------------------------------------*/
/* bPool_Create                                                               */
/*----------------------------------------------------------------------------*/
bvoid *bPool_Create(buint _size)
{
	bPool *pool;

	bAssert(_size > sizeof(buint));

	pool = malloc(sizeof(bPool));

	pool->data     = malloc(_size);
	pool->max_size = _size;
	pool->size     = 0;

	return pool;
}
/*----------------------------------------------------------------------------*/
/* bPool_Delete                                                               */
/*----------------------------------------------------------------------------*/
bvoid bPool_Delete(void *_pool)
{
	bPool *pool = _pool;

	free(pool->data);
	free(pool);
}
/*----------------------------------------------------------------------------*/
/* bPool_Add                                                                  */
/*----------------------------------------------------------------------------*/
bvoid *bPool_Add(bvoid *_pool, bvoid *_item, buint _size)
{
	buint  free_space;
	buint  new_size;
	bbyte *new_data;
	bbyte *cur_pos;
	bPool *pool;
	
	pool = _pool;

	free_space = pool->max_size - pool->size;

	if(free_space < _size + sizeof(buint))
	{
		new_size = (pool->max_size + _size) * 2;
		new_data = malloc(new_size);

		memcpy(new_data, pool->data, pool->size);

		free(pool->data);

		pool->data = new_data;
		pool->max_size = new_size;
	}

	cur_pos = pool->data + pool->size;

	*(buint *)cur_pos = _size;
	cur_pos += sizeof(buint);

	memcpy(cur_pos, _item, _size);
	pool->size += _size + sizeof(buint);

	return cur_pos;
}
/*----------------------------------------------------------------------------*/
/* bPool_GetNext                                                              */
/*----------------------------------------------------------------------------*/
bvoid *bPool_GetNext(bvoid *_pool, bvoid *_pos)
{
	bPool *pool = _pool;

	if((bbyte *)_pos >= pool->data + pool->size)
		return 0;

	return (bbyte *)_pos + *(buint *)_pos;
}