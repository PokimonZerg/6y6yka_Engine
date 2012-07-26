/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <assert.h>
#include "b_patch.h"
/*------------------------------------------------------------------------------------------------*/
/* bPatch_Add                                                                                     */
/*------------------------------------------------------------------------------------------------*/
bPatch *bPatch_Add(bPatch *_list, bPatch *_patch)
{
	_patch->next = _list;
	_patch->prev = 0;

	if(_list)
		_list->prev  = _patch;

	return _patch;
}
/*------------------------------------------------------------------------------------------------*/
/* bPatch_Del                                                                                     */
/*------------------------------------------------------------------------------------------------*/
bPatch *bPatch_Del(bPatch *_list)
{
	bPatch *next;

	switch(_list->type)
	{
	default:
		break;
	}

	next = _list->prev;

	free(_list);

	return next;
}
/*------------------------------------------------------------------------------------------------*/
/* bPatch_Reverse                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bPatch *bPatch_Reverse(bPatch *_list)
{
	while(_list->next)
	{
		_list = _list->next;
	}

	return _list;
}

bPatch_Segment *bPatch_Circle_Create(buint _size)
{
	bPatch_Segment *begin, *current;

	assert(_size);

	current = begin = malloc(sizeof(bPatch_Segment));

	while(--_size)
	{
		current->empty = btrue;
		current->next  = malloc(sizeof(bPatch_Segment));
		current        = current->next;
	}

	current->empty = btrue;
	current->next  = begin;

	return begin;
}

bvoid bPatch_Circle_Delete(bPatch_Segment *_patch)
{
	bPatch_Segment *temp, *end = _patch;

	while(_patch->next != end)
	{
		temp = _patch;
		_patch = _patch->next;
		free(temp);
	}

	free(_patch);
}

bvoid bPatch_Circle_Insert(bPatch_Segment *_patch)
{
	bPatch_Segment *next, *prev, *new;

	prev = _patch;
	next = _patch->next;

	new = malloc(sizeof(bPatch_Segment));

	prev->next = new;
	new->next  = next;
	new->empty = btrue;
}