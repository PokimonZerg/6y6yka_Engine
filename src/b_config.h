/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#ifndef _B_CONFIG_H_
#define _B_CONFIG_H_
/*------------------------------------------------------------------------------------------------*/
#include "b_type.h"
/*------------------------------------------------------------------------------------------------*/
typedef struct bConfig bConfig;
/*------------------------------------------------------------------------------------------------*/
bConfig *bConfig_Load(const bchar *_name);
/*------------------------------------------------------------------------------------------------*/
bvoid bConfig_Unload(bchar *_name, bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_Save(bchar *_name, bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_GetString(bchar **_value, const bchar *_key, const bConfig *_config);
bbool bConfig_GetInt   (bint   *_value, const bchar *_key, const bConfig *_config);
bbool bConfig_GetFloat (bfloat *_value, const bchar *_key, const bConfig *_config);
bbool bConfig_GetBool  (bbool  *_value, const bchar *_key, const bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
bbool bConfig_SetString(bchar *_value, const bchar *_key, const bConfig *_config);
bbool bConfig_SetInt   (bint   _value, const bchar *_key, const bConfig *_config);
bbool bConfig_SetFloat (bfloat _value, const bchar *_key, const bConfig *_config);
bbool bConfig_SetBool  (bbool  _value, const bchar *_key, const bConfig *_config);
/*------------------------------------------------------------------------------------------------*/
#endif /* _B_CONFIG_H_ */