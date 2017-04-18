/*
 *  lib/gfx/gfx_util.h
 */

#ifndef __Gfx_Utility_H__
#define __Gfx_Utility_H__
//#######################################################
#define RGB_TO_UINT(R,G,B) ((R)<<16) | ((G)<<8) | ((B))
#define BGR_TO_UINT(R,G,B) ((R)) | ((G)<<8) | ((B<<16))
//#######################################################
// Utility to divide or multiply the base color
//gfx_u32 gfx_modify_color(gfx_u32 color, s8 modify);
//#######################################################
#ifdef USE_VISUAL_STUDIO
#include "gfx_util.cpp"
#endif
//#######################################################
#endif
