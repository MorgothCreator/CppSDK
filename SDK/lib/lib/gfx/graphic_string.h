/*
 *  lib/gfx/graphic_string.h
 */

#ifndef GRAPHIC_STRING_H_
#define GRAPHIC_STRING_H_
/*#####################################################*/
//#include "api/lcd_def.h"
#include "controls_definition.h"
#include "string.h"
/*#####################################################*/
//char *gfx_change_str(String_t *dest, String_t *src);
//bool put_roll_string(GScreen::Window *pDisplay, graphic_strings_t *StringsStruct);
#ifdef __AVR_XMEGA___
bool put_roll_string_P(tDisplay *pDisplay, graphic_strings_t *StringsStruct);
#endif
/*#####################################################*/
#define new_roll_str strings_t
#ifndef new_
#define new_(structure) (structure*)calloc(1, sizeof(structure));
#endif
#define free_roll_str(address) free(address);
/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "graphic_string.cpp"
#endif
/*#####################################################*/
#endif /* GRAPHIC_STRING_H_ */
/*#####################################################*/

