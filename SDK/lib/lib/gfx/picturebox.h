#ifndef PICTUREBOX_H_
#define PICTUREBOX_H_
//#######################################################################################
#include <stdbool.h>
#include "3d.h"
#include "controls_definition.h"
#include "string.h"
#include "api/timer_api.h"
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
	void copyRectangle(unsigned int *src_buff, unsigned int src_buff_data_offset, tRectangle *_dest_rectangle, tRectangle *_src_rectangle, signed int src_width, signed int src_height);
	void drawPixel(signed int X, signed int Y, unsigned int color);
	void drawHLine(signed int X1, signed int X2, signed int Y, unsigned char width, unsigned int color);
	void drawVLine(signed int Y1, signed int Y2, signed int X, unsigned char width, unsigned int color);
	void drawCircle(signed int x, signed int y, signed int radius, unsigned char fill, unsigned int color);
	void drawLine(signed int X1, signed int Y1, signed int X2, signed int Y2, unsigned char width, unsigned int color);
	void drawElipse(signed int xc,signed int yc,signed int rx,signed int ry, unsigned char fill, unsigned int color);
	void drawTriangle(signed int  Ax,signed int  Ay,signed int  Bx,signed int  By,signed int  Cx,signed int  Cy, unsigned char fill, unsigned int color);
	void drawString(GI::Screen::String *string);
	void draw3dTriangle(_3d_points *Points, signed int X_offset, signed int Y_offset, double X_Angle, double Y_Angle, double Z_Angle, unsigned int Color);
	void draw3dCube(_3d_points *Points, signed int X_offset, signed int Y_offset, double X_Angle, double Y_Angle, double Z_Angle, unsigned int Color);


	struct
	{
		signed int X;
		signed int Y;
	}Position;
	struct
	{
		signed int X;
		signed int Y;
	}Size;
	controls_color_struct_t Color;

	struct {
		struct {
			colors border;
			colors back;
		}disabled;
		struct {
			colors border;
			colors back;
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
			signed int X;
			signed int Y;
		}Position;
		struct
		{
			signed int X;
			signed int Y;
		}PositionOffset;
		struct
		{
			signed int X;
			signed int Y;
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
	unsigned int BackgroundColor;
	bool Visible;
	bool Enabled;

private:
	void paint(void *pDisplay, signed int x_start, signed int y_start, signed int x_len, signed int y_len, tControlCommandData* control_comand);

};
}
}
}
#if _USE_BITMAP_LIBRARY == 1
bool picturebox_put_bitmap(tPictureBox* settings, unsigned char *file, signed int X, signed int Y, bool use_transparency, bool scale);
bool picturebox_put_fbitmap(tPictureBox* settings, char *path, signed int X, signed int Y, bool use_transparency, bool scale);
#endif
//#######################################################################################
#ifdef HEADER_INCLUDE_C_FILES
#include "picturebox.c"
#endif
//#######################################################################################
#endif /* BUTON_H_ */
