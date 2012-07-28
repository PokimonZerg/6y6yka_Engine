/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <math.h>
#include "b_vector3d.h"
#include "b_math.h"
#include "b_quaternion.h"
/*------------------------------------------------------------------------------------------------*/
/* bQuaternion_Build                                                                              */
/*------------------------------------------------------------------------------------------------*/
bQuaternion *bQuaternion_Build(bfloat _angle, bVector3D *_axis, bQuaternion *_r)
{
	bfloat s   = bSin(_angle / 2);
	bfloat c   = bCos(_angle / 2);
	bVector3D v = *_axis;

	bVector3D_Normalize(&v);

	_r->x = v.x * s;
	_r->y = v.y * s;
	_r->z = v.z * s;
	_r->w = c;

	return _r;
}
/*------------------------------------------------------------------------------------------------*/
/* bQuaternion_Div_S                                                                              */
/*------------------------------------------------------------------------------------------------*/
bQuaternion *bQuaternion_Div_S(bQuaternion *_q, bfloat _s)
{
	_q->w /= _s;
	_q->x /= _s;
	_q->y /= _s;
	_q->z /= _s;

	return _q;
}
/*------------------------------------------------------------------------------------------------*/
/* bQuaternion_Mul                                                                                */
/*------------------------------------------------------------------------------------------------*/
bQuaternion *bQuaternion_Mul(bQuaternion *_q1, bQuaternion *_q2, bQuaternion *_r)
{
	_r->w = _q1->w * _q2->w - _q1->x * _q2->x - _q1->y * _q2->y - _q1->z * _q2->z;
	_r->x = _q1->w * _q2->x + _q1->x * _q2->w + _q1->y * _q2->z - _q1->z * _q2->y;
	_r->y = _q1->w * _q2->y + _q1->y * _q2->w + _q1->z * _q2->x - _q1->x * _q2->z;
	_r->z = _q1->w * _q2->z + _q1->z * _q2->w + _q1->x * _q2->y - _q1->y * _q2->x;

	return _r;
}
/*------------------------------------------------------------------------------------------------*/
/* bQuaternion_Length                                                                             */
/*------------------------------------------------------------------------------------------------*/
bfloat bQuaternion_Length(bQuaternion *_q)
{
	return sqrtf(_q->w * _q->w + _q->x * _q->x + _q->y * _q->y + _q->z * _q->z);
}
/*------------------------------------------------------------------------------------------------*/
/* bQuaternion_Normalize                                                                          */
/*------------------------------------------------------------------------------------------------*/
bQuaternion *bQuaternion_Normalize(bQuaternion *_q)
{
	return bQuaternion_Div_S(_q, bQuaternion_Length(_q));
}