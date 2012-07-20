/*------------------------------------------------------------------------------------------------*/
/*                                        6y6yka Engine                                           */
/*------------------------------------------------------------------------------------------------*/
#include "b_math.h"
/*------------------------------------------------------------------------------------------------*/
bfloat bSin_Table[361] = {
+0.000000f, +0.017452f, +0.034899f, +0.052336f, +0.069756f, +0.087156f, +0.104528f, +0.121869f,
+0.139173f, +0.156434f, +0.173648f, +0.190809f, +0.207912f, +0.224951f, +0.241922f, +0.258819f,
+0.275637f, +0.292372f, +0.309017f, +0.325568f, +0.342020f, +0.358368f, +0.374607f, +0.390731f,
+0.406737f, +0.422618f, +0.438371f, +0.453990f, +0.469472f, +0.484810f, +0.500000f, +0.515038f,
+0.529919f, +0.544639f, +0.559193f, +0.573576f, +0.587785f, +0.601815f, +0.615662f, +0.629320f,
+0.642788f, +0.656059f, +0.669131f, +0.681998f, +0.694658f, +0.707107f, +0.719340f, +0.731354f,
+0.743145f, +0.754710f, +0.766044f, +0.777146f, +0.788011f, +0.798635f, +0.809017f, +0.819152f,
+0.829038f, +0.838671f, +0.848048f, +0.857167f, +0.866025f, +0.874620f, +0.882948f, +0.891007f,
+0.898794f, +0.906308f, +0.913545f, +0.920505f, +0.927184f, +0.933580f, +0.939693f, +0.945519f,
+0.951057f, +0.956305f, +0.961262f, +0.965926f, +0.970296f, +0.974370f, +0.978148f, +0.981627f,
+0.984808f, +0.987688f, +0.990268f, +0.992546f, +0.994522f, +0.996195f, +0.997564f, +0.998630f,
+0.999391f, +0.999848f, +1.000000f, +0.999848f, +0.999391f, +0.998630f, +0.997564f, +0.996195f,
+0.994522f, +0.992546f, +0.990268f, +0.987688f, +0.984808f, +0.981627f, +0.978148f, +0.974370f,
+0.970296f, +0.965926f, +0.961262f, +0.956305f, +0.951057f, +0.945519f, +0.939693f, +0.933580f,
+0.927184f, +0.920505f, +0.913545f, +0.906308f, +0.898794f, +0.891007f, +0.882948f, +0.874620f,
+0.866025f, +0.857167f, +0.848048f, +0.838671f, +0.829038f, +0.819152f, +0.809017f, +0.798635f,
+0.788011f, +0.777146f, +0.766044f, +0.754710f, +0.743145f, +0.731354f, +0.719340f, +0.707107f,
+0.694658f, +0.681998f, +0.669131f, +0.656059f, +0.642788f, +0.629320f, +0.615662f, +0.601815f,
+0.587785f, +0.573576f, +0.559193f, +0.544639f, +0.529919f, +0.515038f, +0.500000f, +0.484810f,
+0.469472f, +0.453990f, +0.438371f, +0.422618f, +0.406737f, +0.390731f, +0.374607f, +0.358368f,
+0.342020f, +0.325568f, +0.309017f, +0.292372f, +0.275637f, +0.258819f, +0.241922f, +0.224951f,
+0.207912f, +0.190809f, +0.173648f, +0.156434f, +0.139173f, +0.121869f, +0.104528f, +0.087156f,
+0.069756f, +0.052336f, +0.034899f, +0.017452f, +0.000000f, -0.017452f, -0.034899f, -0.052336f,
-0.069756f, -0.087156f, -0.104528f, -0.121869f, -0.139173f, -0.156434f, -0.173648f, -0.190809f,
-0.207912f, -0.224951f, -0.241922f, -0.258819f, -0.275637f, -0.292372f, -0.309017f, -0.325568f,
-0.342020f, -0.358368f, -0.374607f, -0.390731f, -0.406737f, -0.422618f, -0.438371f, -0.453990f,
-0.469472f, -0.484810f, -0.500000f, -0.515038f, -0.529919f, -0.544639f, -0.559193f, -0.573576f,
-0.587785f, -0.601815f, -0.615662f, -0.629320f, -0.642788f, -0.656059f, -0.669131f, -0.681998f,
-0.694658f, -0.707107f, -0.719340f, -0.731354f, -0.743145f, -0.754710f, -0.766044f, -0.777146f,
-0.788011f, -0.798635f, -0.809017f, -0.819152f, -0.829038f, -0.838671f, -0.848048f, -0.857167f,
-0.866025f, -0.874620f, -0.882948f, -0.891007f, -0.898794f, -0.906308f, -0.913545f, -0.920505f,
-0.927184f, -0.933580f, -0.939693f, -0.945519f, -0.951057f, -0.956305f, -0.961262f, -0.965926f,
-0.970296f, -0.974370f, -0.978148f, -0.981627f, -0.984808f, -0.987688f, -0.990268f, -0.992546f,
-0.994522f, -0.996195f, -0.997564f, -0.998630f, -0.999391f, -0.999848f, -1.000000f, -0.999848f,
-0.999391f, -0.998630f, -0.997564f, -0.996195f, -0.994522f, -0.992546f, -0.990268f, -0.987688f,
-0.984808f, -0.981627f, -0.978148f, -0.974370f, -0.970296f, -0.965926f, -0.961262f, -0.956305f,
-0.951057f, -0.945519f, -0.939693f, -0.933580f, -0.927184f, -0.920505f, -0.913545f, -0.906308f,
-0.898794f, -0.891007f, -0.882948f, -0.874620f, -0.866025f, -0.857167f, -0.848048f, -0.838671f,
-0.829038f, -0.819152f, -0.809017f, -0.798635f, -0.788011f, -0.777146f, -0.766044f, -0.754710f,
-0.743145f, -0.731354f, -0.719340f, -0.707107f, -0.694658f, -0.681998f, -0.669131f, -0.656059f,
-0.642788f, -0.629320f, -0.615662f, -0.601815f, -0.587785f, -0.573576f, -0.559193f, -0.544639f,
-0.529919f, -0.515038f, -0.500000f, -0.484810f, -0.469472f, -0.453990f, -0.438371f, -0.422618f,
-0.406737f, -0.390731f, -0.374607f, -0.358368f, -0.342020f, -0.325568f, -0.309017f, -0.292372f,
-0.275637f, -0.258819f, -0.241922f, -0.224951f, -0.207912f, -0.190809f, -0.173648f, -0.156434f,
-0.139173f, -0.121869f, -0.104528f, -0.087156f, -0.069756f, -0.052336f, -0.034899f, -0.017452f,
-0.000000f};
/*------------------------------------------------------------------------------------------------*/
bfloat bCos_Table[361] = {
+1.000000f, +0.999848f, +0.999391f, +0.998630f, +0.997564f, +0.996195f, +0.994522f, +0.992546f,
+0.990268f, +0.987688f, +0.984808f, +0.981627f, +0.978148f, +0.974370f, +0.970296f, +0.965926f,
+0.961262f, +0.956305f, +0.951057f, +0.945519f, +0.939693f, +0.933580f, +0.927184f, +0.920505f,
+0.913545f, +0.906308f, +0.898794f, +0.891007f, +0.882948f, +0.874620f, +0.866025f, +0.857167f,
+0.848048f, +0.838671f, +0.829038f, +0.819152f, +0.809017f, +0.798635f, +0.788011f, +0.777146f,
+0.766044f, +0.754710f, +0.743145f, +0.731354f, +0.719340f, +0.707107f, +0.694658f, +0.681998f,
+0.669131f, +0.656059f, +0.642788f, +0.629320f, +0.615662f, +0.601815f, +0.587785f, +0.573576f,
+0.559193f, +0.544639f, +0.529919f, +0.515038f, +0.500000f, +0.484810f, +0.469472f, +0.453990f,
+0.438371f, +0.422618f, +0.406737f, +0.390731f, +0.374607f, +0.358368f, +0.342020f, +0.325568f,
+0.309017f, +0.292372f, +0.275637f, +0.258819f, +0.241922f, +0.224951f, +0.207912f, +0.190809f,
+0.173648f, +0.156434f, +0.139173f, +0.121869f, +0.104528f, +0.087156f, +0.069756f, +0.052336f,
+0.034899f, +0.017452f, +0.000000f, -0.017452f, -0.034899f, -0.052336f, -0.069756f, -0.087156f,
-0.104528f, -0.121869f, -0.139173f, -0.156434f, -0.173648f, -0.190809f, -0.207912f, -0.224951f,
-0.241922f, -0.258819f, -0.275637f, -0.292372f, -0.309017f, -0.325568f, -0.342020f, -0.358368f,
-0.374607f, -0.390731f, -0.406737f, -0.422618f, -0.438371f, -0.453990f, -0.469472f, -0.484810f,
-0.500000f, -0.515038f, -0.529919f, -0.544639f, -0.559193f, -0.573576f, -0.587785f, -0.601815f,
-0.615662f, -0.629320f, -0.642788f, -0.656059f, -0.669131f, -0.681998f, -0.694658f, -0.707107f,
-0.719340f, -0.731354f, -0.743145f, -0.754710f, -0.766044f, -0.777146f, -0.788011f, -0.798635f,
-0.809017f, -0.819152f, -0.829038f, -0.838671f, -0.848048f, -0.857167f, -0.866025f, -0.874620f,
-0.882948f, -0.891007f, -0.898794f, -0.906308f, -0.913545f, -0.920505f, -0.927184f, -0.933580f,
-0.939693f, -0.945519f, -0.951057f, -0.956305f, -0.961262f, -0.965926f, -0.970296f, -0.974370f,
-0.978148f, -0.981627f, -0.984808f, -0.987688f, -0.990268f, -0.992546f, -0.994522f, -0.996195f,
-0.997564f, -0.998630f, -0.999391f, -0.999848f, -1.000000f, -0.999848f, -0.999391f, -0.998630f,
-0.997564f, -0.996195f, -0.994522f, -0.992546f, -0.990268f, -0.987688f, -0.984808f, -0.981627f,
-0.978148f, -0.974370f, -0.970296f, -0.965926f, -0.961262f, -0.956305f, -0.951057f, -0.945519f,
-0.939693f, -0.933580f, -0.927184f, -0.920505f, -0.913545f, -0.906308f, -0.898794f, -0.891007f,
-0.882948f, -0.874620f, -0.866025f, -0.857167f, -0.848048f, -0.838671f, -0.829038f, -0.819152f,
-0.809017f, -0.798635f, -0.788011f, -0.777146f, -0.766044f, -0.754710f, -0.743145f, -0.731354f,
-0.719340f, -0.707107f, -0.694658f, -0.681998f, -0.669131f, -0.656059f, -0.642788f, -0.629320f,
-0.615662f, -0.601815f, -0.587785f, -0.573576f, -0.559193f, -0.544639f, -0.529919f, -0.515038f,
-0.500000f, -0.484810f, -0.469472f, -0.453990f, -0.438371f, -0.422618f, -0.406737f, -0.390731f,
-0.374607f, -0.358368f, -0.342020f, -0.325568f, -0.309017f, -0.292372f, -0.275637f, -0.258819f,
-0.241922f, -0.224951f, -0.207912f, -0.190809f, -0.173648f, -0.156434f, -0.139173f, -0.121869f,
-0.104528f, -0.087156f, -0.069756f, -0.052336f, -0.034899f, -0.017452f, -0.000000f, +0.017452f,
+0.034899f, +0.052336f, +0.069756f, +0.087156f, +0.104528f, +0.121869f, +0.139173f, +0.156434f,
+0.173648f, +0.190809f, +0.207912f, +0.224951f, +0.241922f, +0.258819f, +0.275637f, +0.292372f,
+0.309017f, +0.325568f, +0.342020f, +0.358368f, +0.374607f, +0.390731f, +0.406737f, +0.422618f,
+0.438371f, +0.453990f, +0.469472f, +0.484810f, +0.500000f, +0.515038f, +0.529919f, +0.544639f,
+0.559193f, +0.573576f, +0.587785f, +0.601815f, +0.615662f, +0.629320f, +0.642788f, +0.656059f,
+0.669131f, +0.681998f, +0.694658f, +0.707107f, +0.719340f, +0.731354f, +0.743145f, +0.754710f,
+0.766044f, +0.777146f, +0.788011f, +0.798635f, +0.809017f, +0.819152f, +0.829038f, +0.838671f,
+0.848048f, +0.857167f, +0.866025f, +0.874620f, +0.882948f, +0.891007f, +0.898794f, +0.906308f,
+0.913545f, +0.920505f, +0.927184f, +0.933580f, +0.939693f, +0.945519f, +0.951057f, +0.956305f,
+0.961262f, +0.965926f, +0.970296f, +0.974370f, +0.978148f, +0.981627f, +0.984808f, +0.987688f,
+0.990268f, +0.992546f, +0.994522f, +0.996195f, +0.997564f, +0.998630f, +0.999391f, +0.999848f,
+1.000000f};
/*------------------------------------------------------------------------------------------------*/
/* bSin - fast sine. work in degrees.                                                             */
/*------------------------------------------------------------------------------------------------*/
bfloat bSin(bfloat _angle)
{
	bint   angle_full;
	bfloat angle_part;

	_angle = fmodf(_angle, 360);

	if(_angle < 0) _angle += 360.0f;

	angle_full = (bint)_angle;
	angle_part = _angle - angle_full;

	return bSin_Table[angle_full] + angle_part * 
		  (bSin_Table[angle_full + 1] - bSin_Table[angle_full]);
}
/*------------------------------------------------------------------------------------------------*/
/* bCos - fast cosine. work in degrees.                                                           */
/*------------------------------------------------------------------------------------------------*/
bfloat bCos(bfloat _angle)
{
	bint   angle_full;
	bfloat angle_part;

	_angle = fmodf(_angle, 360);

	if(_angle < 0) _angle += 360.0f;

	angle_full = (bint)_angle;
	angle_part = _angle - angle_full;

	return bCos_Table[angle_full] + angle_part * 
		  (bCos_Table[angle_full + 1] - bCos_Table[angle_full]);
}