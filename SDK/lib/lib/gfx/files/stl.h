/*
 *  lib/files/stl.h
 */

#ifndef STL_H_
#define STL_H_

#include "../3d.h"

typedef struct
{
	unsigned char Header[80];
	unsigned long TriangleNr;
}stl_header;

typedef struct
{
	struct
	{
		float x;
		float y;
		float z;
	}NormalVector;
	struct
	{
		float x;
		float y;
		float z;
	}Vertex1;
	struct
	{
		float x;
		float y;
		float z;
	}Vertex2;
	struct
	{
		float x;
		float y;
		float z;
	}Vertex3;
	unsigned short Attribute;
}stl_solid_t;

void stl_get_triangle(_3d_points *triangle_bounds, unsigned char *file, unsigned long triangle_nr);
unsigned long stl_get_nr_of_triangles(unsigned char *file);

#ifdef USE_VISUAL_STUDIO
#include "stl.cpp"
#endif

#endif /* STL_H_ */
