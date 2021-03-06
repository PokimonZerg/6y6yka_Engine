/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#ifndef _B_CONFIG_H_
#define _B_CONFIG_H_
/*------------------------------------------------------------------------------------------------*/
#include "b_type.h"
/*------------------------------------------------------------------------------------------------*/
/* bConfig                                                                                        */
/*------------------------------------------------------------------------------------------------*/
typedef struct bConfig bConfig;
/*------------------------------------------------------------------------------------------------*/
/* bConfig_Create                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bConfig *bConfig_Create(bchar *_name);
/*------------------------------------------------------------------------------------------------*/
/* bConfig_Delete                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bvoid bConfig_Delete(bchar *_name, bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_Save(bchar *_name, bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_GetString(bchar **_value, bchar *_key, bConfig *_config);
bbool bConfig_GetInt   (bint   *_value, bchar *_key, bConfig *_config);
bbool bConfig_GetFloat (bfloat *_value, bchar *_key, bConfig *_config);
bbool bConfig_GetBool  (bbool  *_value, bchar *_key, bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_SetString(bchar *_value, bchar *_key, bConfig *_config);
bbool bConfig_SetInt   (bint   _value, bchar *_key, bConfig *_config);
bbool bConfig_SetFloat (bfloat _value, bchar *_key, bConfig *_config);
bbool bConfig_SetBool  (bbool  _value, bchar *_key, bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
#endif /* _B_CONFIG_H_ */