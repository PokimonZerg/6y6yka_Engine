/*----------------------------------------------------------------------------*/
/*                              6y6yka Engine                                 */
/*----------------------------------------------------------------------------*/
#include <stdlib.h>
#include "b_stack.h"
/*----------------------------------------------------------------------------*/
/* bStack_Create                                                              */
/*----------------------------------------------------------------------------*/
bvoid *bStack_Create(buint _size, buint _num)
{
	bStack *stack;
	buint   size;

	size = _size * _num;

	stack         = malloc(sizeof(bStack));
	stack->data   = malloc(size);
	stack->bottom = stack->data;
	stack->top    = stack->data + size;

	return stack;
}
/*----------------------------------------------------------------------------*/
/* bStack_Delete                                                              */
/*----------------------------------------------------------------------------*/
bvoid bStack_Delete(bvoid *_stack)
{
	bStack *stack = _stack;

	free(stack->bottom);
	free(stack);
}