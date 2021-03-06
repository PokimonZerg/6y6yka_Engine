/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include "b_matrix4x4.h"
#include "b_camera.h"
/*------------------------------------------------------------------------------------------------*/
/* bCamera                                                                                        */
/*------------------------------------------------------------------------------------------------*/
struct bCamera
{
	bQuaternion rotation;
	bVector3D   position;
};
/*------------------------------------------------------------------------------------------------*/
/* bCamera_Create                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bCamera *bCamera_Create()
{
	bCamera *camera;

	camera = malloc(sizeof(bCamera));

	camera->position.x = 0.0f;
	camera->position.y = 0.0f;
	camera->position.z = 0.0f;

	camera->rotation.x = 0.0f;
	camera->rotation.y = 0.0f;
	camera->rotation.z = 0.0f;
	camera->rotation.w = 1.0f;

	return camera;
}
/*------------------------------------------------------------------------------------------------*/
/* bCamera_Delete                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_Delete(bCamera *_camera)
{
	free(_camera);
}
/*------------------------------------------------------------------------------------------------*/
/* bCamera_SetPos                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_SetPos(bVector3D *_pos, bCamera *_camera)
{
	_camera->position = *_pos;
}
/*------------------------------------------------------------------------------------------------*/
/* bCamera_SetRotation                                                                            */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_SetRotation(bQuaternion *_rotation, bCamera *_camera)
{
	_camera->rotation = *_rotation;
}
/*------------------------------------------------------------------------------------------------*/
/* bCamera_MoveForward                                                                            */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_MoveForward(bfloat _distance, bCamera *_camera)
{
	bVector3D v;

	v.x = 0.0f;
	v.y = 0.0f;
	v.z = _distance;

	bVector3D_Mul_Q(&v, &_camera->rotation);

	bVector3D_Add(&_camera->position, &v);
}
/*------------------------------------------------------------------------------------------------*/
/* bCamera_Strafe                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_Strafe(bfloat _distance, bCamera *_camera)
{
	bVector3D v;

	v.x = _distance;
	v.y = 0.0f;
	v.z = 0.0f;

	bVector3D_Mul_Q(&v, &_camera->rotation);

	bVector3D_Add(&_camera->position, &v);
}
/*------------------------------------------------------------------------------------------------*/
/* bCamera_RotateHorizontal                                                                       */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_RotateHorizontal(bfloat _angle, bCamera *_camera)
{
	bQuaternion r, c = _camera->rotation;
	bVector3D      v = {1.0f, 0.0f, 0.0f};

	bQuaternion_Build(-_angle, &v, &r);

	bQuaternion_Mul(&c, &r, &_camera->rotation);
}
/*------------------------------------------------------------------------------------------------*/
/* bCamera_RotateVertical                                                                         */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_RotateVertical(bfloat _angle, bCamera *_camera)
{
	bQuaternion r, c = _camera->rotation;
	bVector3D      v = {0.0f, 1.0f, 0.0f};

	bQuaternion_Build(-_angle, &v, &r);

	bQuaternion_Mul(&r, &c, &_camera->rotation);
}
/*------------------------------------------------------------------------------------------------*/
/* bCamera_HeadOverHeels                                                                          */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_HeadOverHeels(bfloat _angle, bCamera *_camera)
{
	bQuaternion r, c = _camera->rotation;
	bVector3D      v = {0.0f, 0.0f, 1.0f};

	bQuaternion_Build(_angle, &v, &r);

	bQuaternion_Mul(&r, &c, &_camera->rotation);
}
/*------------------------------------------------------------------------------------------------*/
/* bCamera_GetMatrix                                                                              */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_GetMatrix(bMatrix4X4 _r, bCamera *_camera)
{
	bMatrix4X4 T, Q;

	bQuaternion_Normalize(&_camera->rotation);

	bMatrix4X4_Translate(&_camera->position, T);
	bMatrix4X4_RotateQ(&_camera->rotation, Q);
	bMatrix4X4_Mul(_r, T, Q);

	//bMatrix4X4_Translate(&_camera->position, _r);
}