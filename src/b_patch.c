/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <assert.h>
#include "b_patch.h"

bPatch *bPatch_Circle_Create(buint _size)
{
	bPatch *begin, *current;

	assert(_size);

	current = begin = malloc(sizeof(bPatch));

	while(--_size)
	{
		current->empty = btrue;
		current->next  = malloc(sizeof(bPatch));
		current        = current->next;
	}

	current->empty = btrue;
	current->next  = begin;

	return begin;
}

bvoid bPatch_Circle_Delete(bPatch *_patch)
{
	bPatch *temp, *end = _patch;

	while(_patch->next != end)
	{
		temp = _patch;
		_patch = _patch->next;
		free(temp);
	}

	free(_patch);
}

bvoid bPatch_Circle_Insert(bPatch *_patch)
{
	bPatch *next, *prev, *new;

	prev = _patch;
	next = _patch->next;

	new = malloc(sizeof(bPatch));

	prev->next = new;
	new->next  = next;
	new->empty = btrue;
}