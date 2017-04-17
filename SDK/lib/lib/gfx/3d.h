/*
 *  lib/gfx/3d.h
 *
 *  Copyright (C) 2013  Iulian Gheorghiu <morgoth.creator@gmail.com>
 *
 *  This file is part of Multiplatform SDK.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _3D_H_
#define _3D_H_

//#include <math.h>
#include <stdlib.h>
#include <api/lcd_def.h>
#include "gfxdef.h"
//#define focalLength  300
#define _depth 1

#define PI (3.14159265358979323846264338327950288)
#define PI2 (3.14159265358979323846264338327950288/2.0)



//typedef struct
//{
//	s32 _x;
//	s32 _y;
//}theScene;

typedef struct
{
	double *x;
	double *y;
	double *z;
	double *depth;
	double *scaleFactor;
	double focalLength;
	double depthScale;
	u32 length;
}point;

typedef struct
{
	double x[8];
	double y[8];
	double z[8];
	double depth[8];
	double scaleFactor[8];
	double focalLength;
	double depthScale;
	u32 length;
}_3d_points;

typedef struct
{
	double x;
	double y;
	double z;
}axisRotations;

typedef struct
{
	point points[8];
}Transformed_Points_Array;

void Transform3DPointsTo2DPoints(_3d_points *screenPoints, _3d_points *Points, axisRotations AxisRotations);
void put_3d_triangle(GI::Dev::Screen *pDisplay, _3d_points *Points, gfx_s32 X_offset, gfx_s32 Y_offset, double X_Angle, double Y_Angle, double Z_Angle, gfx_u32 Color);
void put_3d_rectangle(GI::Dev::Screen *pDisplay, _3d_points *Points, gfx_s32 X_offset, gfx_s32 Y_offset, double X_Angle, double Y_Angle, double Z_Angle, gfx_u32 Color);

//#######################################################################################
#ifdef USE_VISUAL_STUDIO
#include "3d.cpp"
#endif
#endif /* 3D_H_ */
