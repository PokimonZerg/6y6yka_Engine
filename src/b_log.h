/*----------------------------------------------------------------------------*/
/*                              6y6yka Engine                                 */
/*----------------------------------------------------------------------------*/
#ifndef _B_LOG_H_
#define _B_LOG_H_
/*----------------------------------------------------------------------------*/
#include "b_type.h"
/*----------------------------------------------------------------------------*/
typedef enum
{
	bLog_Normal,
	bLog_Warning,
	bLog_Error
}   bLog_Level;
/*----------------------------------------------------------------------------*/
bvoid bLog_Start(bchar *_name);
/*----------------------------------------------------------------------------*/
bvoid bLog_Stop();
/*----------------------------------------------------------------------------*/
bvoid bLog_Print(bchar *_text, bLog_Level _level);
/*----------------------------------------------------------------------------*/
#endif /* _B_LOG_H_ */