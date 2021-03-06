/*------------------------------------------------------------------------------------------------*/
/*                              6y6yka Engine                                                     */
/*------------------------------------------------------------------------------------------------*/
#ifndef _B_FILE_H_
#define _B_FILE_H_
/*------------------------------------------------------------------------------------------------*/
#include <windows.h>
#include "b_type.h"
/*------------------------------------------------------------------------------------------------*/
/* bFile                                                                                          */
/*------------------------------------------------------------------------------------------------*/
typedef HANDLE bFile;
/*------------------------------------------------------------------------------------------------*/
/* bFile_Pos - file cursir position.                                                              */
/*------------------------------------------------------------------------------------------------*/
typedef enum
{
	bFile_Begin,
	bFile_Current,
	bFile_End
} 
bFile_Pos;
/*------------------------------------------------------------------------------------------------*/
/* bFile_Mode - read/write mode.                                                                  */
/*------------------------------------------------------------------------------------------------*/
typedef enum
{
	bFile_ReadMode,
	bFile_WriteMode,
	bFile_ReadWriteMode
}
bFile_Mode;
/*------------------------------------------------------------------------------------------------*/
/* bFile_Open                                                                                     */
/*------------------------------------------------------------------------------------------------*/
bFile bFile_Open(bchar *_name, bFile_Mode _mode);
/*------------------------------------------------------------------------------------------------*/
/* bFile_Create                                                                                   */
/*------------------------------------------------------------------------------------------------*/
bFile bFile_Create(bchar *_name, bbool _override, bFile_Mode _mode);
/*------------------------------------------------------------------------------------------------*/
/* bFile_Read                                                                                     */
/*------------------------------------------------------------------------------------------------*/
bbool bFile_Read(bvoid *_buf, buint _size, buint *_rsize, bFile _file);
/*------------------------------------------------------------------------------------------------*/
/* bFile_Write                                                                                    */
/*------------------------------------------------------------------------------------------------*/
bbool bFile_Write(bvoid *_buf, buint _size, buint *_wsize, bFile _file);
/*------------------------------------------------------------------------------------------------*/
/* bFile_ReadText                                                                                 */
/*------------------------------------------------------------------------------------------------*/
bchar *bFile_ReadText(bchar *_name);
/*------------------------------------------------------------------------------------------------*/
/* bFile_WriteText                                                                                */
/*------------------------------------------------------------------------------------------------*/
bbool bFile_WriteText(bchar *_name, bchar *_text);
/*------------------------------------------------------------------------------------------------*/
/* bFile_SetPos                                                                                   */
/*------------------------------------------------------------------------------------------------*/
bbool bFile_SetPos(buint _offset, bFile_Pos _origin, bFile _file);
/*------------------------------------------------------------------------------------------------*/
/* bFile_Close                                                                                    */
/*------------------------------------------------------------------------------------------------*/
bbool bFile_Close(bFile _file);
/*------------------------------------------------------------------------------------------------*/
/* bFile_GetSize                                                                                  */
/*------------------------------------------------------------------------------------------------*/
buint bFile_GetSize(bFile _file);
/*------------------------------------------------------------------------------------------------*/
#endif /* _B_FILE_H_ */