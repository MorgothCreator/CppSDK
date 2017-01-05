/*
 * drawtext_impl.c
 *
 *  Created on: Dec 27, 2016
 *      Author: John Smith
 */

struct dtx_font *dtx_font;
int dtx_font_sz;
int dtx_buf_mode;	/* DTX_NBF is 0 */
float dtx_cur_color[4];
int dtx_cur_color_int[4];
float dtx_cur_offset[2];

const char *(*dtx_drawchar)(const char*, float*, float*, int*);
void (*dtx_drawflush)(void);



