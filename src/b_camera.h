/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#ifndef _B_CAMERA_H_
#define _B_CAMERA_H_
/*------------------------------------------------------------------------------------------------*/
#include "b_math.h"
/*------------------------------------------------------------------------------------------------*/
/* bCamera                                                                                        */
/*------------------------------------------------------------------------------------------------*/
typedef struct bCamera bCamera;
/*------------------------------------------------------------------------------------------------*/
/* bCamera_Create                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bCamera *bCamera_Create();
/*------------------------------------------------------------------------------------------------*/
/* bCamera_Delete                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_Delete(bCamera *_camera);
/*------------------------------------------------------------------------------------------------*/
/* bCamera_SetPos                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_SetPos(const bVector3D *_pos, bCamera *_camera);
/*------------------------------------------------------------------------------------------------*/
/* bCamera_SetRotation                                                                            */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_SetRotation(const bQuaternion *_rotation, bCamera *_camera);
/*------------------------------------------------------------------------------------------------*/
/* bCamera_MoveForward                                                                            */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_MoveForward(bfloat _distance, bCamera *_camera);
/*------------------------------------------------------------------------------------------------*/
/* bCamera_Strafe                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_Strafe(bfloat _distance, bCamera *_camera);
/*------------------------------------------------------------------------------------------------*/
/* bCamera_RotateHorizontal                                                                       */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_RotateHorizontal(bfloat _angle, bCamera *_camera);
/*------------------------------------------------------------------------------------------------*/
/* bCamera_RotateVertical                                                                         */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_RotateVertical(bfloat _angle, bCamera *_camera);
/*------------------------------------------------------------------------------------------------*/
/* bCamera_HeadOverHeels                                                                          */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_HeadOverHeels(bfloat _angle, bCamera *_camera);
/*------------------------------------------------------------------------------------------------*/
/* bCamera_GetMatrix                                                                              */
/*------------------------------------------------------------------------------------------------*/
bvoid bCamera_GetMatrix(bMatrix4X4 _r, bCamera *_camera);
/*------------------------------------------------------------------------------------------------*/
#endif /* _B_CAMERA_H_ */