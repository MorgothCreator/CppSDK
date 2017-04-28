/*
 * lib/gfx/string.h
 */

#ifndef LIB_LIB_GFX_STRING_H_
#define LIB_LIB_GFX_STRING_H_

#include <avr/pgmspace.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <api/lcd_def.h>
#include "resource/fonts.h"
#include <lib/string.h>
#include "gfxdef.h"

#define FONT_FMT_UNCOMPRESSED   0x00
#define ABSENT_CHAR_REPLACEMENT '.'

/*#####################################################*/
#define RGB_TO_UINT(R,G,B) ((R)<<16) | ((G)<<8) | ((B))

/*#####################################################*/
#ifdef HAVE_LONG_DOUBLE
#define LDOUBLE long double
#else
#define LDOUBLE double
#endif
#ifdef __AVR_XMEGA__
extern const gfx_u8 CharTable6x8[] PROGMEM;
#else
extern const gfx_u8 CharTable6x8[];
#endif


#if (__SIZEOF_INT__ == 4)
//#if 4 == sizeof(gfx_u32)
#define ReturnCommand_gp  (unsigned)(29)
#define ReturnCommand_gm  (unsigned)(0x07 << ReturnCommand_gp)
#define ReturnCommand_NoCommand  (unsigned)(0x00<<ReturnCommand_gp)
#define ReturnCommand_GoUp  (unsigned)(0x01<<ReturnCommand_gp)
#define ReturnCommand_GoDn  (unsigned)(0x02<<ReturnCommand_gp)
#define ReturnCommand_GoLeft  (unsigned)(0x03<<ReturnCommand_gp)
#define ReturnCommand_GoRight  (unsigned)(0x04<<ReturnCommand_gp)
#define ReturnCommand_MaxValue  (unsigned)(0x08)
#else
#define ReturnCommand_gp  (unsigned)(13)
#define ReturnCommand_gm  (unsigned)(0x07 << ReturnCommand_gp)
#define ReturnCommand_NoCommand  (unsigned)(0x00<<ReturnCommand_gp)
#define ReturnCommand_GoUp  (unsigned)(0x01<<ReturnCommand_gp)
#define ReturnCommand_GoDn  (unsigned)(0x02<<ReturnCommand_gp)
#define ReturnCommand_GoLeft  (unsigned)(0x03<<ReturnCommand_gp)
#define ReturnCommand_GoRight  (unsigned)(0x04<<ReturnCommand_gp)
#define ReturnCommand_MaxValue  (unsigned)(0x08)
#endif
namespace GI
{
namespace Screen
{
	class String
	{
	public:
		String(GI::Dev::Screen *pDisplay);
		String(GI::Dev::Screen *pDisplay, char *String);
		~String();
		bool equal(GI::Screen::String *string);
		bool equal(char *string);
		void toUper();
		void toLower();
		void removeNewLine(GI::Screen::String *string);
		void setText(char* string);
		void setText(GI::Screen::String *string);
		void setText(GI::String *string);
		void setTextF(const char *pcString, ...);
		void setTextF(GI::String *string, ...);
		void setTextF(GI::Screen::String *string, ...);
		void append(char* string);
		void append(GI::Screen::String *string);
		void append(GI::String *string);
		void append(char Char);
		void appendF(const char *pcString, ...);
		void appendF(GI::String *string, ...);
		void appendF(GI::Screen::String *string, ...);
		void insert(GI::String* string, gfx_u32 location);
		void insert(GI::Screen::String* string, gfx_u32 location);
		void insert(char *string, gfx_u32 location);
		void clone(GI::Screen::String* string);
		void clear();
		char *subString(gfx_u32 position, gfx_u32 len);

		//#######################################################################################
		gfx_u8 getFontHeight()
		{
			return read_data_byte(pFont->ucHeight);
		}
		void getStrSelect(gfx_s32 *Start, gfx_s32 *SelStartReturn, gfx_s32 *SelLenReturn, gfx_s32 _XPush, gfx_s32 _YPush, gfx_s32 _XPull, gfx_s32 _YPull, gfx_s32 lX, gfx_s32 lY, gfx_u32 *return_command);
		gfx_s32 getStrWidth();
		gfx_s32 getStrRowsNr();
		StringProperties_t getStrProp();
		gfx_s32 drawString();
		gfx_s32 drawStringTiny();
		gfx_s32 getStrPropTiny();



		char *buff;

		tRectangle Capsule;
		GI::Dev::Screen *pDisplay;
		tFont *pFont;
		gfx_s32 length;
		gfx_u32 foreground_color;
		gfx_u32 background_color;
		bool ulOpaque;
		bool ulVisible;
		bool wordWrap;
		gfx_s32 lX;
		gfx_s32 lY;
		gfx_s32 _SelStart;
		gfx_s32 _SelLen;
		allign textAlign;
		/*
		 * Private variables.
		 */
		gfx_u32 modifyed;
		int error;
		//bool initialized;
	private:
		/*int vsnprintf (char *str, size_t count, const char *fmt, va_list args);
		void dopr (char *buffer, size_t maxlen, const char *format, va_list args);
		void fmtstr (char *buffer, size_t *currlen, size_t maxlen,
				    char *value, int flags, int min, int max);
		void fmtint (char *buffer, size_t *currlen, size_t maxlen,
				    long value, int base, int min, int max, int flags);
		void fmtfp (char *buffer, size_t *currlen, size_t maxlen,
				   LDOUBLE fvalue, int min, int max, int flags);
		void dopr_outch (char *buffer, size_t *currlen, size_t maxlen, char c );*/

	};
}
}

/*#####################################################*/
#ifdef USE_VISUAL_STUDIO
#include "string.cpp"
#endif
/*#####################################################*/
#endif /* LIB_LIB_GFX_STRING_H_ */
