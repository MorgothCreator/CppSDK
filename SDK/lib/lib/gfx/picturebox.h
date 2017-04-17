#ifndef PICTUREBOX_H_
#define PICTUREBOX_H_
//#######################################################################################
#include <stdbool.h>
#include "3d.h"
#include "controls_definition.h"
#include "string.h"
#include "api/timer.h"
#include "gfxdef.h"
//#include "window_def.h"
//#######################################################################################
namespace GI
{
namespace Screen
{
namespace Gfx
{
class Picturebox {
public:
	Picturebox(void *parentWindowHandler);
	~Picturebox();
	void idle(tControlCommandData* control_comand);
	void idle();
	void clear();
	void copyRectangle(gfx_u32 *src_buff, gfx_u32 src_buff_data_offset, tRectangle *_dest_rectangle, tRectangle *_src_rectangle, gfx_s32 src_width, gfx_s32 src_height);
	void drawPixel(gfx_s32 X, gfx_s32 Y, gfx_u32 color);
	void drawHLine(gfx_s32 X1, gfx_s32 X2, gfx_s32 Y, gfx_u8 width, gfx_u32 color);
	void drawVLine(gfx_s32 Y1, gfx_s32 Y2, gfx_s32 X, gfx_u8 width, gfx_u32 color);
	void drawCircle(gfx_s32 x, gfx_s32 y, gfx_s32 radius, gfx_u8 fill, gfx_u32 color);
	void drawLine(gfx_s32 X1, gfx_s32 Y1, gfx_s32 X2, gfx_s32 Y2, gfx_u8 width, gfx_u32 color);
	void drawElipse(gfx_s32 xc,gfx_s32 yc,gfx_s32 rx,gfx_s32 ry, gfx_u8 fill, gfx_u32 color);
	void drawTriangle(gfx_s32  Ax,gfx_s32  Ay,gfx_s32  Bx,gfx_s32  By,gfx_s32  Cx,gfx_s32  Cy, gfx_u8 fill, gfx_u32 color);
	void drawString(GI::Screen::String *string);
	void draw3dTriangle(_3d_points *Points, gfx_s32 X_offset, gfx_s32 Y_offset, double X_Angle, double Y_Angle, double Z_Angle, gfx_u32 Color);
	void draw3dCube(_3d_points *Points, gfx_s32 X_offset, gfx_s32 Y_offset, double X_Angle, double Y_Angle, double Z_Angle, gfx_u32 Color);


	struct
	{
		gfx_s32 X;
		gfx_s32 Y;
	}Position;
	struct
	{
		gfx_s32 X;
		gfx_s32 Y;
	}Size;
	controls_color_struct_t Color;

	struct {
		struct {
			Color::ARGB border;
			Color::ARGB back;
		}disabled;
		struct {
			Color::ARGB border;
			Color::ARGB back;
		}enabled;
	}color;

	struct
	{
		struct
		{
			void* CallbackReturnData;
			void*(*CallBack)(GI::Screen::Gfx::Picturebox *, tControlCommandData *);
		}OnMove;
		struct
		{
			void* CallbackReturnData;
			void*(*CallBack)(GI::Screen::Gfx::Picturebox *, tControlCommandData *);
		}OnUp;
		struct
		{
			void* CallbackReturnData;
			void*(*CallBack)(GI::Screen::Gfx::Picturebox *, tControlCommandData *);
		}OnDown;
		struct
		{
			void* CallbackReturnData;
			void*(*CallBack)(GI::Screen::Gfx::Picturebox *, tControlCommandData *);
		}Refresh;
		bool CursorUp;
		bool CursorDown;
		bool CursorMove;
	}Events;
	struct
	{
		struct
		{
			bool Initiated;
		}Control;
		struct
		{
			gfx_s32 X;
			gfx_s32 Y;
		}Position;
		struct
		{
			gfx_s32 X;
			gfx_s32 Y;
		}PositionOffset;
		struct
		{
			gfx_s32 X;
			gfx_s32 Y;
		}Size;
		bool OldStateVisible;
		bool OldStateEnabled;
		bool NeedEntireRefresh;
		bool NeedEntireRepaint;
		bool CursorDownInsideBox;
		bool IsChildren;
		bool NoPaintBackGround;
		//CursorState OldStateCursor;
		GI::Sys::Timer ContinuouslyPushTimer;
		GI::Dev::Screen *pDisplay;
		tRectangle PictureWindowLimits;
		void *parentWindowHandler;
		bool parentWindowStateEnabled;
	}Internals;
	bool PaintBackground;
	gfx_u32 BackgroundColor;
	bool Visible;
	bool Enabled;

private:
	void paint(void *pDisplay, gfx_s32 x_start, gfx_s32 y_start, gfx_s32 x_len, gfx_s32 y_len, tControlCommandData* control_comand);

};
}
}
}
#if _USE_BITMAP_LIBRARY == 1
bool picturebox_put_bitmap(tPictureBox* settings, gfx_u8 *file, gfx_s32 X, gfx_s32 Y, bool use_transparency, bool scale);
bool picturebox_put_fbitmap(tPictureBox* settings, char *path, gfx_s32 X, gfx_s32 Y, bool use_transparency, bool scale);
#endif
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "picturebox.c"
#endif
//#######################################################################################
#endif /* BUTON_H_ */
