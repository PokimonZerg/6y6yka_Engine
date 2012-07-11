/*----------------------------------------------------------------------------*/
/*                              6y6yka Engine                                 */
/*----------------------------------------------------------------------------*/
#ifndef _B_STACK_H_
#define _B_STACK_H_
/*----------------------------------------------------------------------------*/
#include "b_type.h"
#include "b_assert.h"
/*----------------------------------------------------------------------------*/
/* bStack - classic stack with static size.                                   */
/*----------------------------------------------------------------------------*/
struct bStack
{
	bbyte *data;
	bbyte *bottom;
	bbyte *top;
};

typedef struct bStack bStack;
/*----------------------------------------------------------------------------*/
/* bStack_Create - create new stack structure.                                */
/*                                                                            */
/* arguments:                                                                 */
/*     _size - single item size.                                              */
/*     _num  - items num.                                                     */
/* return:                                                                    */
/*     pointer to bStack structure.                                           */
/*----------------------------------------------------------------------------*/
bvoid *bStack_Create(buint _size, buint _num);
/*----------------------------------------------------------------------------*/
/* bStack_Delete - delete stack and all his data->                             */
/*                                                                            */
/* arguments:                                                                 */
/*     _stack - pointer to valid stack.                                       */
/*----------------------------------------------------------------------------*/
bvoid bStack_Delete(bvoid *_stack);
/*----------------------------------------------------------------------------*/
/* bStack_Push - push item in stack.                                          */
/*                                                                            */
/* arguments:                                                                 */
/*     _stack - pointer to valid stack.                                       */
/*     _item  - pointer to new item.                                          */
/*----------------------------------------------------------------------------*/
#define bStack_Push(_stack, _item) bAssert(_stack->top > _stack->data); \
	                               *_stack->data++ = *_item
/*----------------------------------------------------------------------------*/
/* bStack_Pop - pop item on stack.                                            */
/*                                                                            */
/* arguments:                                                                 */
/*     _stack - pointer to valid stack.                                       */
/*----------------------------------------------------------------------------*/
#define bStack_Pop(_stack) --_stack->data; \
						   bAssert(_stack->bottom <= _stack->data)
/*----------------------------------------------------------------------------*/
/* bStack_Top - return top item.                                              */
/*                                                                            */
/* arguments:                                                                 */
/*     _stack - pointer to valid stack.                                       */
/*----------------------------------------------------------------------------*/
#define bStack_Top(_stack) (_stack->data - 1); \
						   bAssert(_stack->bottom < _stack->data)
/*----------------------------------------------------------------------------*/
#endif /* _B_STACK_H_ */