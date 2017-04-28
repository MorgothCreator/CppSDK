/*
 * lcd.cpp
 */

#include "lcd_def.h"
#include <include/global.h>
#include <lib/gfx/gfxdef.h>
//#######################################################################################
SysErr GI::Dev::Screen::setOrientation(LCD_ORIENTATION orientation)
{
	if(setOrientation_Ptr)
		setOrientation_Ptr(driverHandler_Ptr, orientation);
	else
		return SYS_ERR_NOT_IMPLEMENTED;
	return SYS_ERR_OK;
}
//#######################################################################################
SysErr GI::Dev::Screen::setOn()
{
	if(setOn_Ptr)
		setOn_Ptr(driverHandler_Ptr);
	else
		return SYS_ERR_NOT_IMPLEMENTED;
	return SYS_ERR_OK;
}
//#######################################################################################
SysErr GI::Dev::Screen::setOff()
{
	if(setOff_Ptr)
		setOff_Ptr(driverHandler_Ptr);
	else
		return SYS_ERR_NOT_IMPLEMENTED;
	return SYS_ERR_OK;
}
//#######################################################################################
SysErr GI::Dev::Screen::setBacklight(unsigned char value)
{
	if(setBacklight_Ptr)
		setBacklight_Ptr(driverHandler_Ptr, value);
	else
		return SYS_ERR_NOT_IMPLEMENTED;
	return SYS_ERR_OK;
}
//#######################################################################################
SysErr GI::Dev::Screen::copyScreen(void *pDisplayFrom, bool put_cursor,
			signed int X, signed int Y, unsigned int color)
{
	if(copyScreen_Ptr)
		copyScreen_Ptr(driverHandler_Ptr, pDisplayFrom, put_cursor, X, Y, color);
	else
		return SYS_ERR_NOT_IMPLEMENTED;
	return SYS_ERR_OK;
}
//#######################################################################################
SysErr GI::Dev::Screen::cacheClean(signed int x_start, signed int y_start,
			signed int x_len, signed int y_len)
{
	if(cacheClean_Ptr)
		cacheClean_Ptr(driverHandler_Ptr, x_start, y_start, x_len, y_len);
	else
		return SYS_ERR_NOT_IMPLEMENTED;
	return SYS_ERR_OK;
}
//#######################################################################################
SysErr GI::Dev::Screen::drawRectangle(signed int x_start, signed int y_start,
			signed int x_len, signed int y_len, bool fill, unsigned int color)
{
	if(drawRectangle_Ptr)
		drawRectangle_Ptr(driverHandler_Ptr, x_start, y_start, x_len, y_len, fill, color);
	else
		return SYS_ERR_NOT_IMPLEMENTED;
	return SYS_ERR_OK;
}
//#######################################################################################
SysErr GI::Dev::Screen::drawPixel(signed int X, signed int Y, unsigned int color)
{
	if(drawPixel_Ptr)
		drawPixel_Ptr(driverHandler_Ptr, X, Y, color);
	else
		return SYS_ERR_NOT_IMPLEMENTED;
	return SYS_ERR_OK;
}
//#######################################################################################
SysErr GI::Dev::Screen::copyRectangle16Bit(unsigned short *rgb_buffer,
			unsigned int x1, unsigned int y1, unsigned int width,
			unsigned int height)
{
	if(copyRectangle16Bit_Ptr)
		copyRectangle16Bit_Ptr(driverHandler_Ptr, rgb_buffer, x1, y1, width, height);
	else
		return SYS_ERR_NOT_IMPLEMENTED;
	return SYS_ERR_OK;
}
//#######################################################################################
SysErr GI::Dev::Screen::copyRectangle24Bit(unsigned char *rgb_buffer,
			unsigned long x1, unsigned long y1, unsigned long width,
			unsigned long height)
{
	if(copyRectangle24Bit_Ptr)
		copyRectangle24Bit_Ptr(driverHandler_Ptr, rgb_buffer, x1, y1, width, height);
	else
		return SYS_ERR_NOT_IMPLEMENTED;
	return SYS_ERR_OK;
}
//#######################################################################################
SysErr GI::Dev::Screen::copyRectangle32Bit(unsigned char *rgb_buffer,
			unsigned int x1, unsigned int y1, unsigned int width,
			unsigned int height)
{
	if(copyRectangle32Bit_Ptr)
		copyRectangle32Bit_Ptr(driverHandler_Ptr, rgb_buffer, x1, y1, width, height);
	else
		return SYS_ERR_NOT_IMPLEMENTED;
	return SYS_ERR_OK;
}
//#######################################################################################
SysErr GI::Dev::Screen::drawHLine(signed int X1, signed int X2,
			signed int Y, unsigned char width, unsigned int color)
{
	if(drawHLine_Ptr)
		drawHLine_Ptr(driverHandler_Ptr, X1, X2, Y, width, color);
	else
		return SYS_ERR_NOT_IMPLEMENTED;
	return SYS_ERR_OK;
}
//#######################################################################################
SysErr GI::Dev::Screen::drawVLine(signed int Y1, signed int Y2,
			signed int X, unsigned char width, unsigned int color)
{
	if(drawVLine_Ptr)
		drawVLine_Ptr(driverHandler_Ptr, Y1, Y2, X, width, color);
	else
		return SYS_ERR_NOT_IMPLEMENTED;
	return SYS_ERR_OK;
}
//#######################################################################################
SysErr GI::Dev::Screen::clear(unsigned int color)
{
	if(clear_Ptr)
		clear_Ptr(driverHandler_Ptr, color);
	else
		return SYS_ERR_NOT_IMPLEMENTED;
	return SYS_ERR_OK;
}
//#######################################################################################
SysErr GI::Dev::Screen::setLuminosity(unsigned char luminosity)
{
	if(setLuminosity_Ptr)
		setLuminosity_Ptr(driverHandler_Ptr, luminosity);
	else
		return SYS_ERR_NOT_IMPLEMENTED;
	return SYS_ERR_OK;
}
//#######################################################################################
SysErr GI::Dev::Screen::drawTouchPoint(signed int X, signed int Y, unsigned int color)
{
	if(drawTouchPoint_Ptr)
	{
		drawTouchPoint_Ptr(driverHandler_Ptr, X, Y, color);
		return SYS_ERR_OK;
	}
	drawHLine(X - 7, 6, Y, 1, color);
	drawHLine(X + 2, 6, Y, 1, color);
	drawVLine(Y - 7, 6, X, 1, color);
	drawVLine(Y + 2, 6, X, 1, color);
	return SYS_ERR_OK;
}
//#######################################################################################
SysErr GI::Dev::Screen::drawCircle(signed int x, signed int y, signed int _radius, unsigned char fill, unsigned int color)
{
	if(drawCircle_Ptr)
	{
		drawCircle_Ptr(driverHandler_Ptr, x, y, _radius, fill, color);
		return SYS_ERR_OK;
	}
	signed int radius = _radius;
	if (radius < 0)
		radius = ~radius;
	signed int a, b, P;

	a = 0;
	b = radius;
	P = 1 - radius;
	signed int Tmp1;
	signed int Tmp2;
	signed int Tmp3;
	signed int Tmp4;
	signed int Tmp5;
	signed int Tmp6;
	signed int Tmp7;
	signed int Tmp8;
	signed int _Tmp5 = 5, _Tmp7 = 0;

	do
	{
		Tmp1 = x + a;
		Tmp2 = x - a;
		Tmp3 = x + b;
		Tmp4 = x - b;
		Tmp5 = y + a;
		Tmp6 = y - a;
		Tmp7 = y + b;
		Tmp8 = y - b;
		if (fill)
		{

			if (_Tmp7 != Tmp7)
			{
				drawHLine(Tmp2, Tmp1 - Tmp2, Tmp7, 1, color);
				drawHLine(Tmp2, Tmp1 - Tmp2, Tmp8, 1, color);
			}
			if (_Tmp5 != Tmp5)
			{
				drawHLine(Tmp4, Tmp3 - Tmp4, Tmp5, 1, color);
				drawHLine(Tmp4, Tmp3 - Tmp4, Tmp6, 1, color);
			}
			_Tmp5 = Tmp5;
			_Tmp7 = Tmp7;
		}
		else
		{
			drawPixel(Tmp1, Tmp7, color);
			drawPixel(Tmp3, Tmp5, color);
			drawPixel(Tmp2, Tmp7, color);
			drawPixel(Tmp4, Tmp5, color);
			drawPixel(Tmp3, Tmp6, color);
			drawPixel(Tmp1, Tmp8, color);
			drawPixel(Tmp2, Tmp8, color);
			drawPixel(Tmp4, Tmp6, color);
		}

		if (P < 0)
			P += 3 + 2 * a++;
		else
			P += 5 + 2 * (a++ - b--);
	} while (a <= b);
	refresh();
	return SYS_ERR_OK;
}
//#######################################################################################
SysErr GI::Dev::Screen::drawLine(signed int X1, signed int Y1, signed int X2, signed int Y2, unsigned char width, unsigned int color)
{
	if(drawLine_Ptr)
	{
		drawLine_Ptr(driverHandler_Ptr, X1, Y1, X2, Y2, width, color);
		return SYS_ERR_OK;
	}
	/*if(X1 > X2)
	 {
	 int Tmp = X1;
	 X1 = X2;
	 X2 = Tmp;
	 }
	 if(Y1 > Y2)
	 {
	 int Tmp = Y1;
	 Y1 = Y2;
	 Y2 = Tmp;
	 }*/
	if (width == 1)
	{
		signed int CurrentX, CurrentY, Xinc, Yinc, Dx, Dy, TwoDx, TwoDy,
				TwoDxAccumulatedError, TwoDyAccumulatedError;
		Dx = (X2 - X1);
		Dy = (Y2 - Y1);
		TwoDx = Dx + Dx;
		TwoDy = Dy + Dy;
		CurrentX = X1;
		CurrentY = Y1;
		Xinc = 1;
		Yinc = 1;
		if (Dx < 0)
		{
			Xinc = -1;
			Dx = -Dx;
			TwoDx = -TwoDx;
		}
		if (Dy < 0)
		{
			Yinc = -1;
			Dy = -Dy;
			TwoDy = -TwoDy;
		}
		drawPixel(X1, Y1, color);
		if ((Dx != 0) || (Dy != 0))
		{
			if (Dy <= Dx)
			{
				TwoDxAccumulatedError = 0;
				do
				{
					CurrentX += Xinc;
					TwoDxAccumulatedError += TwoDy;
					if (TwoDxAccumulatedError > Dx)
					{
						CurrentY += Yinc;
						TwoDxAccumulatedError -= TwoDx;
					}
					drawPixel(CurrentX, CurrentY, color);
				} while (CurrentX != X2);
			}
			else
			{
				TwoDyAccumulatedError = 0;
				do
				{
					CurrentY += Yinc;
					TwoDyAccumulatedError += TwoDx;
					if (TwoDyAccumulatedError > Dy)
					{
						CurrentX += Xinc;
						TwoDyAccumulatedError -= TwoDy;
					}
					drawPixel(CurrentX, CurrentY, color);
				} while (CurrentY != Y2);
			}
		}
	}
	else
	{
		signed int half_width;
		signed int dy, dx;
		signed int addx = 1, addy = 1, j;
		signed int P, diff;
		signed int c1, c2;

		int i = 0;
		dx = X2 - X1;
		dy = Y2 - Y1;

		half_width = width / 2;
		c1 = -(dx * X1 + dy * Y1);
		c2 = -(dx * X2 + dy * Y2);

		if (X1 > X2)
		{
			signed int temp;
			temp = c1;
			c1 = c2;
			c2 = temp;
			addx = -1;
		}
		if (Y1 > Y2)
		{
			signed int temp;
			temp = c1;
			c1 = c2;
			c2 = temp;
			addy = -1;
		}

		if (dx >= dy)
		{
			P = 2 * dy - dx;
			diff = P - dx;

			for (i = 0; i <= dx; ++i)
			{
#ifdef Use_FastDrawBar
				lcd_func.screen_draw_vertical_line(pContext,Y1+ (-half_width), Y1+ (half_width+width%2), X1, 1);
#else
				for (j = -half_width; j < half_width + width % 2; ++j)
				{
					signed int temp;

					temp = dx * X1 + dy * (Y1 + j); // Use more RAM to increase speed
					if (temp + c1 >= 0 && temp + c2 <= 0)
						drawPixel(X1, Y1 + j, color);
				}
#endif
				if (P < 0)
				{
					P += 2 * dy;
					X1 += addx;
				}
				else
				{
					P += diff;
					X1 += addx;
					Y1 += addy;
				}
			}
		}
		else
		{
			P = 2 * dx - dy;
			diff = P - dy;

			for (i = 0; i <= dy; ++i)
			{
				if (P < 0)
				{
					P += 2 * dx;
					Y1 += addy;
				}
				else
				{
					P += diff;
					X1 += addx;
					Y1 += addy;
				}
#ifdef Use_FastDrawBar
				lcd_func.put_horizontal_line(pContext,(X1+(-half_width)), (X1+(half_width+width%2)), Y1, 1);
#else
				for (j = -half_width; j < half_width + width % 2; ++j)
				{
					signed int temp;

					temp = dx * X1 + dy * (Y1 + j); // Use more RAM to increase speed
					if (temp + c1 >= 0 && temp + c2 <= 0)
						drawPixel(X1 + j, Y1, color);
				}
#endif
			}
		}
	}
	refresh();
	return SYS_ERR_OK;
}

//#######################################################################################
static void elipseplot(GI::Dev::Screen *pDisplay, signed int xc, signed int yc,
		signed int x, signed int y, unsigned char Fill, unsigned int color)
{
	int Tmp1 = xc + x;
	int Tmp2 = xc - x;
	int Tmp3 = yc + y;
	int Tmp4 = yc - y;

	if (Fill)
	{
		pDisplay->drawHLine(Tmp2, Tmp1 - Tmp2, Tmp3, 1, color);
		pDisplay->drawHLine(Tmp2, Tmp1 - Tmp2, Tmp4, 1, color);
	}
	else
	{
		pDisplay->drawPixel((unsigned int) (Tmp1), (unsigned int) (Tmp3),
				color);
		pDisplay->drawPixel((unsigned int) (Tmp2), (unsigned int) (Tmp3),
				color);
		pDisplay->drawPixel((unsigned int) (Tmp1), (unsigned int) (Tmp4),
				color);
		pDisplay->drawPixel((unsigned int) (Tmp2), (unsigned int) (Tmp4),
				color);
	}
}
//----------------------------------------------------------------------------------------
SysErr GI::Dev::Screen::drawElipse(signed int xc,signed int yc,signed int _rx,signed int _ry, unsigned char Fill, unsigned int color)
{
	if(drawElipse_Ptr)
	{
		drawElipse_Ptr(driverHandler_Ptr, xc, yc, _rx, _ry, Fill, color);
		return SYS_ERR_OK;
	}
	signed int rx = _rx;
	if (rx < 0)
		rx = 0xFFFFFFFF - rx;
	signed int ry = _ry;
	if (ry < 0)
		ry = 0xFFFFFFFF - ry;
	int rx2 = rx * rx;
	int ry2 = ry * ry;
	int tory2 = 2 * ry2;
	int torx2 = 2 * rx2;
	int p;
	int x = 0;
	int y = ry;
	int py = torx2 * y;
	int px = 0;
	elipseplot(this, xc, yc, x, y, Fill, color);
	p = /*round(*/ry2 - (rx2 * ry) + (0.25 * rx2)/*)*/;
	while (px < py)
	{
		x++;
		px += tory2;
		if (p < 0)
			p += ry2 + px;
		else
		{
			y--;
			py -= torx2;
			p += ry2 + px - py;
		}
		elipseplot(this, xc, yc, x, y, Fill, color);
	}
	p = /*round(*/ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1)
			- rx2 * ry2/*)*/;
	while (y > 0)
	{
		y--;
		py -= torx2;
		if (p > 0)
			p += rx2 - py;
		else
		{
			x++;
			px += tory2;
			p += rx2 - py + px;
		}
		elipseplot(this, xc, yc, x, y, Fill, color);
	}
	refresh();
	return SYS_ERR_OK;
}

//#######################################################################################
/*
 *  the coordinates of vertices are (A.x,A.y), (B.x,B.y), (C.x,C.y); we assume that A.y<=B.y<=C.y (you should sort them first)
 * dx1,dx2,dx3 are deltas used in interpolation
 * horizline draws horizontal segment with coordinates (S.x,Y), (E.x,Y)
 * S.x, E.x are left and right x-coordinates of the segment we have to draw
 * S=A means that S.x=A.x; S.y=A.y;
 */

static void triangle_swap_nibble(signed int* a, signed int *b)
{
	signed int t = *a;
	*a = *b;
	*b = t;
}

SysErr GI::Dev::Screen::drawTriangle(signed int  Ax,signed int  Ay,signed int  Bx,signed int  By,signed int  Cx,signed int  Cy, unsigned char Fill, unsigned int color)
{
	if(drawTriangle_Ptr)
	{
		drawTriangle_Ptr(driverHandler_Ptr, Ax, Ay, Bx, By, Cx, Cy, Fill, color);
		return SYS_ERR_OK;
	}
	signed long dx1, dx2, dx3;
	signed long Sx, Ex;
	signed int Sy, Ey;

	if (Ay > By)
	{
		triangle_swap_nibble(&Ay, &By);
		triangle_swap_nibble(&Ax, &Bx);
	}
	if (Ay > Cy)
	{
		triangle_swap_nibble(&Ay, &Cy);
		triangle_swap_nibble(&Ax, &Cx);
	}
	if (By > Cy)
	{
		triangle_swap_nibble(&By, &Cy);
		triangle_swap_nibble(&Bx, &Cx);
	}
	if (By - Ay > 0)
		dx1 = ((signed long) (Bx - Ax) << 16) / (By - Ay);
	else
		dx1 = 0;
	if (Cy - Ay > 0)
		dx2 = ((signed long) (Cx - Ax) << 16) / (Cy - Ay);
	else
		dx2 = 0;
	if (Cy - By > 0)
		dx3 = ((signed long) (Cx - Bx) << 16) / (Cy - By);
	else
		dx3 = 0;

	Ex = Sx = (signed long) Ax << 16;
	Ey = Sy = Ay;

	if (dx1 > dx2)
	{
		while (Sy <= By)
		{
			drawLine(Sx >> 16, Sy++, Ex >> 16, Ey++, 1, color);
			Sx += dx2;
			Ex += dx1;
		}
		Ex = (signed long) Bx << 16;
		Ey = By;
		while (Sy <= Cy)
		{
			drawLine(Sx >> 16, Sy++, Ex >> 16, Ey++, 1, color);
			Sx += dx2;
			Ex += dx3;
		}
	}
	else
	{
		while (Sy <= By)
		{
			drawLine(Sx >> 16, Sy++, Ex >> 16, Ey++, 1, color);
			Sx += dx1;
			Ex += dx2;
		}
		Sx = (signed long) Bx << 16;
		Sy = By;
		while (Sy <= Cy)
		{
			drawLine(Sx >> 16, Sy++, Ex >> 16, Ey++, 1, color);
			Sx += dx3;
			Ex += dx2;
		}
	}
	refresh();
	return SYS_ERR_OK;
}
//#######################################################################################
#ifdef __AVR_XMEGA__
#include <avr/pgmspace.h>
extern const gfx_u8 CharTable6x8[];
#else
extern const gfx_u8 CharTable6x8[];
#endif
int GI::Dev::Screen::drawString(GI::String *string, signed int X, signed int Y, tRectangle *box, bool WordWrap, unsigned int foreColor, unsigned int color)
{
	drawString(string->buff, X, Y, box, WordWrap, foreColor, color);
}


int GI::Dev::Screen::drawString(char *string, signed int X, signed int Y, tRectangle *box, bool wordWrap, unsigned int foreColor, unsigned int color)
{
	tRectangle int_box;
	if(box)
	{
		int_box.sXMin = 0;
		int_box.sXMax = LcdTimings->X;
		int_box.sYMin = 0;
		int_box.sYMax = LcdTimings->Y;
		box = &int_box;
	}
	char *pcString = string;
	bool WordWrap = wordWrap;
	int lLength = -1;
	//gfx_s32 _SelStart = properties->_SelStart;
	//gfx_s32 _SelLen = properties->_SelLen;

	gfx_s8 chWidth = 0;
	gfx_s8 chHeight = 0;
	gfx_s32 CharPtr;
	gfx_s8 Tmp = 0;
	bool CompactWriting = true;
	gfx_s32 Cursor_X = X;
	gfx_s32 Cursor_Y = Y;
	bool ulVisible = true;
	gfx_s32 CharCnt = 0;
	bool ulOpaque = false;
#ifdef __AVR_XMEGA__
	chWidth = pgm_read_byte(&CharTable6x8[2]);
	chHeight = pgm_read_byte(&CharTable6x8[3]);
#else
	chWidth = CharTable6x8[2];
	chHeight = CharTable6x8[3];
#endif
	do
	{
		gfx_u8 Char = *pcString;
		if (lLength >= 0 && CharCnt > lLength)
		{
			refresh();
			return CharCnt - 1;
		}
		if (Char == 0)
		{
			refresh();
			return CharCnt - 1;
		}
#ifdef __AVR_XMEGA__
		CharPtr = ((Char - pgm_read_byte(&CharTable6x8[4])) * chWidth) + pgm_read_byte(&CharTable6x8[0]);
		if(Char < pgm_read_byte(&CharTable6x8[4]) || Char > pgm_read_byte(&CharTable6x8[5]))
#else
		CharPtr = ((Char - CharTable6x8[4]) * chWidth) + CharTable6x8[0];
		if (Char < CharTable6x8[4] || Char > CharTable6x8[5])
#endif
		{
			//chWidth_Tmp = chWidth;
			chWidth = 0;
		}
		else
		{
			gfx_u8 Temp;
			/* if CompactWriting is true search the character for free cols from right to left and clear them */
			if (CompactWriting)
			{
				for (Tmp = 1; Tmp < chWidth; Tmp++)
				{
#ifdef __AVR_XMEGA__
					Temp = pgm_read_byte(&CharTable6x8[Tmp + CharPtr]);
#else
					Temp = CharTable6x8[Tmp + CharPtr];
#endif
					if (Temp == 0)
						break;
				}
				Tmp++;
			}
			else
			{
				Tmp = chWidth;
			}
			if (Cursor_X + Tmp >= sClipRegion.sXMin
					&& Cursor_X < sClipRegion.sXMax + Tmp
					&& Cursor_Y + chHeight >= sClipRegion.sYMin
					&& Cursor_Y < sClipRegion.sYMax + chHeight)
			{
				if (ulVisible)
				{
					gfx_s32 XX = 0;
					gfx_s32 YY = 0;
					for (XX = 0; XX < Tmp; XX++)
					{
#ifdef __AVR_XMEGA__
						Temp = pgm_read_byte(&CharTable6x8[XX + CharPtr]);
#else
						Temp = CharTable6x8[XX + CharPtr];
#endif
						for (YY = 0; YY < chHeight; YY++)
						{
							if (Temp & 0x1)
							{
								drawPixel(XX + Cursor_X,
										YY + Cursor_Y, color);
							}
							else
							{
								if (ulOpaque)
									drawPixel(XX + Cursor_X,
											YY + Cursor_Y, foreColor);
							}
							Temp = Temp >> 1;
						}
					}
				}
			}
		}
		//if(Tmp < chWidth) Tmp++;
		switch (Char)
		{
		case '\r':
			Cursor_X = X;
			pcString++;
			break;
		case '\n':
			Cursor_Y += chHeight;
			pcString++;
			break;
			//case 9:
			//case 11:
			////signed short TabCursor = (ScreenStruct->Cursor_X/((chWidth_Tmp>>1)*4)*(chWidth_Tmp>>1))*4;
			//Tmp = (((Cursor_X/(chWidth_Tmp<<2))*(chWidth_Tmp<<2))-Cursor_X) + (chWidth_Tmp<<2);
		default:
			Cursor_X += Tmp;
			/*if((ScreenStruct->Cursor_X + chWidth > ScreenStruct->Width) && ScreenStruct->WorldWrap == True)
			 {
			 ScreenStruct->CharWidth = Tmp;
			 ScreenStruct->CharHeight = chHeight;
			 return EOF;
			 }*/
			if ((Cursor_X + chWidth > sClipRegion.sXMax)
					&& WordWrap == true)
			{
				Cursor_Y += chHeight;
				Cursor_X = X;
			}
			pcString++;
		}
		CharCnt++;
	} while (1);
	refresh();
}
//#######################################################################################
SysErr GI::Dev::Screen::refresh()
{
	if(refresh_Ptr)
		refresh_Ptr(driverHandler_Ptr);
	else
		return SYS_ERR_NOT_IMPLEMENTED;
	return SYS_ERR_OK;
}
//#######################################################################################
