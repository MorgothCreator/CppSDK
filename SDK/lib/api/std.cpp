/*
 * std.cpp
 *
 *  Created on: Dec 14, 2016
 *      Author: John Smith
 */

#include "std.h"
#include <api/init.h>
#include <lib/gfx/window.h>
#include <api/dev_request.h>

GI::Std::Std(char *devPath) :
IoDevices(STDIO_UART),
devHandle(NULL)
{
	if(!strncmp((char *)"uart-", devPath, sizeof("uart-") - 1))
	{
		GI::Dev::Uart *device;
		if(GI::Dev::DevRequest::request(devPath, &device) == SYS_ERR_OK)
		{
			devHandle = (void *)device;
			IoDevices = STDIO_UART;
		}
	}
	else if(!strncmp((char *)"SCREEN<", devPath, sizeof("SCREEN<") - 1))
	{
		char* ptr = strchr((const char *)(devPath + sizeof("SCREEN<") - 1), (int)'>');
		if(ptr)
		{
			char tmp_name[32];
			memset(tmp_name, 0, 32);
			strncpy(tmp_name, (const char *)(devPath + sizeof("SCREEN<") - 1), (unsigned int)(ptr - (devPath + sizeof("SCREEN<") - 1)));
			GI::Screen::Gfx::Window *terminalWindow = new GI::Screen::Gfx::Window(NULL, dev.SCREEN[0]);
			terminalWindow->AllowHScroll = false;
			terminalWindow->AllowVScroll = false;
			terminalWindow->HideHScroll = true;
			terminalWindow->HideVScroll = true;
			terminalWindow->MaxMinEnabled = false;
			terminalWindow->Internals.FullScreen = false;
			terminalWindow->SizeNormalScreen.X = 400;
			terminalWindow->SizeNormalScreen.Y = 700;
			terminalWindow->MaxMinVisible = true;
			terminalWindow->MinimizeButonEnabled = true;
			terminalWindow->CloseButonEnabled = true;
			terminalWindow->Caption->textAlign = alignLeft;
			terminalWindow->Caption->setText(tmp_name);

			newTextBox(terminalWindow, terminalScreen);
			terminalScreen->Position.X = 1;
			terminalScreen->Position.Y = 2;
			terminalScreen->Size.X = terminalWindow->Size.X - 6;
			terminalScreen->Size.Y = terminalWindow->Size.Y - terminalWindow->Internals.Header.Size.Y - 6;
			terminalScreen->Color.Enabled.TextBackGround = 0x00000000;
			//terminalScreen->Color.Enabled. = 0x00000000;
			terminalScreen->Color.Enabled.Ink.Pull = 0xFFFFFFFF;
			terminalScreen->Color.Enabled.Ink.Push = 0xFFFFFFFF;

			terminalWindow->idle();
			terminalWindow->idle();
			terminalWindow->idle();
			terminalWindow->idle();
			devHandle = (void *)terminalWindow;
			userData = (void *)terminalScreen;
			IoDevices = STDIO_SCREEN;
		}
	}
	/*switch((unsigned char)IoDevices)
	{
	case STDIO_UART:
		devHandle = (void *)dev.UART[uartNr];
		break;
	case STDIO_SCREEN:
		break;
	case STDIO_FS:
		break;
	}*/
}

GI::Std::~Std()
{
	switch((unsigned char)IoDevices)
	{
	case STDIO_UART:
		((GI::Dev::Uart *)devHandle)->~Uart();
		break;
	case STDIO_SCREEN:
		break;
	case STDIO_FS:
		break;
	}
}

void GI::Std::print(char Char)
{
	switch((unsigned char)IoDevices)
	{
	case STDIO_UART:
		((GI::Dev::Uart *)devHandle)->putChar(Char);
		break;
	case STDIO_SCREEN:
		//GI::Screen::Gfx::TextBox *terminalScreen = (GI::Screen::Gfx::TextBox *)userData;
		//terminalScreen->text->append(Char);
		break;
	case STDIO_FS:
		break;
	}
}

void GI::Std::print(char* string)
{
	switch((unsigned char)IoDevices)
	{
	case STDIO_UART:
		((GI::Dev::Uart *)devHandle)->print(string);
		break;
	case STDIO_SCREEN:
		//GI::Screen::Gfx::TextBox *terminalScreen = (GI::Screen::Gfx::TextBox *)userData;
		//terminalScreen->text->append(string);
		break;
	case STDIO_FS:
		break;
	}
}

void GI::Std::print(GI::Screen::String *string)
{
	switch((unsigned char)IoDevices)
	{
	case STDIO_UART:
		((GI::Dev::Uart *)devHandle)->print(string);
		break;
	case STDIO_SCREEN:
		//GI::Screen::Gfx::TextBox *terminalScreen = (GI::Screen::Gfx::TextBox *)userData;
		//terminalScreen->text->append(string);
		break;
	case STDIO_FS:
		break;
	}
}

void GI::Std::print(GI::String *string)
{
	switch((unsigned char)IoDevices)
	{
	case STDIO_UART:
		((GI::Dev::Uart *)devHandle)->print(string);
		break;
	case STDIO_SCREEN:
		//GI::Screen::Gfx::TextBox *terminalScreen = (GI::Screen::Gfx::TextBox *)userData;
		//terminalScreen->text->append(string);
		break;
	case STDIO_FS:
		break;
	}
}

void GI::Std::clear()
{
	switch((unsigned char)IoDevices)
	{
	case STDIO_UART:
		break;
	case STDIO_SCREEN:
		//GI::Screen::Gfx::TextBox *terminalScreen = (GI::Screen::Gfx::TextBox *)userData;
		//terminalScreen->text->clear();
		break;
	case STDIO_FS:
		break;
	}
}

/*#####################################################*/
/*
 * Copyright Patrick Powell 1995 & modified by Iulian Gheorghiu
 * This code is based on code written by Patrick Powell (papowell@astart.com)
 * It may be used for any purpose as long as this notice remains intact
 * on all source code distributions
 */

/**************************************************************
 * Original:
 * Patrick Powell Tue Apr 11 09:48:21 PDT 1995
 * A bombproof version of doprnt (dopr) included.
 * Sigh.  This sort of thing is always nasty do deal with.  Note that
 * the version here does not include floating point...
 *
 * snprintf() is used instead of sprintf() as it does limit checks
 * for string length.  This covers a nasty loophole.
 *
 * The other functions are there to prevent NULL pointers from
 * causing nast effects.
 *
 * More Recently:
 *  Brandon Long <blong@fiction.net> 9/15/96 for mutt 0.43
 *  This was ugly.  It is still ugly.  I opted out of floating point
 *  numbers, but the formatter understands just about everything
 *  from the normal C string format, at least as far as I can tell from
 *  the Solaris 2.5 printf(3S) man page.
 *
 *  Brandon Long <blong@fiction.net> 10/22/97 for mutt 0.87.1
 *    Ok, added some minimal floating point support, which means this
 *    probably requires libm on most operating systems.  Don't yet
 *    support the exponent (e,E) and sigfig (g,G).  Also, fmtint()
 *    was pretty badly broken, it just wasn't being exercised in ways
 *    which showed it, so that's been fixed.  Also, formated the code
 *    to mutt conventions, and removed dead code left over from the
 *    original.  Also, there is now a builtin-test, just compile with:
 *           gcc -DTEST_SNPRINTF -o snprintf snprintf.c -lm
 *    and run snprintf for results.
 *
 *  Thomas Roessler <roessler@guug.de> 01/27/98 for mutt 0.89i
 *    The PGP code was using unsigned hexadecimal formats.
 *    Unfortunately, unsigned formats simply didn't work.
 *
 *  Michael Elkins <me@cs.hmc.edu> 03/05/98 for mutt 0.90.8
 *    The original code assumed that both snprintf() and vsnprintf() were
 *    missing.  Some systems only have snprintf() but not vsnprintf(), so
 *    the code is now broken down under HAVE_SNPRINTF and HAVE_VSNPRINTF.
 *
 *  Andrew Tridgell (tridge@samba.org) Oct 1998
 *    fixed handling of %.0f
 *    added test for HAVE_LONG_DOUBLE
 *
 **************************************************************/

/* Version number of package */
#undef VERSION//#include "config.h"

#if !defined(HAVE_SNPRINTF) || !defined(HAVE_VSNPRINTF)

#include <string.h>
#include <ctype.h>
#ifndef USE_AVR_STUDIO
#include <sys/types.h>
#endif


/* Define this as a fall through, HAVE_STDARG_H is probably already set */

#define HAVE_VARARGS_H


/* varargs declarations: */
#define HAVE_STDARG_H

#if defined(HAVE_STDARG_H)
# include "stdarg.h"
# define HAVE_STDARGS    /* let's hope that works everywhere (mj) */
# define VA_LOCAL_DECL   va_list ap
# define VA_START(f)     va_start(ap, f)
# define VA_SHIFT(v,t)  ;   /* no-op for ANSI */
# define VA_END          va_end(ap)
#else
# if defined(HAVE_VARARGS_H)
#  include "varargs.h"
#  undef HAVE_STDARGS
#  define VA_LOCAL_DECL   va_list ap
#  define VA_START(f)     va_start(ap)      /* f is ignored! */
#  define VA_SHIFT(v,t) v = va_arg(ap,t)
#  define VA_END        va_end(ap)
# else
/*XX ** NO VARARGS ** XX*/
# endif
#endif

/*
 * dopr(): poor man's version of doprintf
 */

/* format read states */
#define DP_S_DEFAULT 0
#define DP_S_FLAGS   1
#define DP_S_MIN     2
#define DP_S_DOT     3
#define DP_S_MAX     4
#define DP_S_MOD     5
#define DP_S_CONV    6
#define DP_S_DONE    7

/* format flags - Bits */
#define DP_F_MINUS 	(1 << 0)
#define DP_F_PLUS  	(1 << 1)
#define DP_F_SPACE 	(1 << 2)
#define DP_F_NUM   	(1 << 3)
#define DP_F_ZERO  	(1 << 4)
#define DP_F_UP    	(1 << 5)
#define DP_F_UNSIGNED 	(1 << 6)

/* Conversion Flags */
#define DP_C_SHORT   1
#define DP_C_LONG    2
#define DP_C_LDOUBLE 3

#define char_to_int(p) (p - '0')
#define MAX(p,q) ((p >= q) ? p : q)

void GI::Std::dopr (char *buffer, size_t maxlen, const char *format, va_list args)
{
  int ch;
  long value;
  LDOUBLE fvalue;
  char *strvalue;
  int min;
  int max;
  int state;
  int flags;
  int cflags;
  size_t currlen;

  state = DP_S_DEFAULT;
  currlen = flags = cflags = min = 0;
  max = -1;
  ch = *format++;

  while (state != DP_S_DONE)
  {
    if ((ch == '\0') || (currlen >= maxlen))
      state = DP_S_DONE;

    switch(state)
    {
    case DP_S_DEFAULT:
      if (ch == '%')
	state = DP_S_FLAGS;
      else
	dopr_outch (buffer, &currlen, maxlen, ch);
      ch = *format++;
      break;
    case DP_S_FLAGS:
      switch (ch)
      {
      case '-':
	flags |= DP_F_MINUS;
        ch = *format++;
	break;
      case '+':
	flags |= DP_F_PLUS;
        ch = *format++;
	break;
      case ' ':
	flags |= DP_F_SPACE;
        ch = *format++;
	break;
      case '#':
	flags |= DP_F_NUM;
        ch = *format++;
	break;
      case '0':
	flags |= DP_F_ZERO;
        ch = *format++;
	break;
      default:
	state = DP_S_MIN;
	break;
      }
      break;
    case DP_S_MIN:
      if (isdigit(ch))
      {
	min = 10*min + char_to_int (ch);
	ch = *format++;
      }
      else if (ch == '*')
      {
	min = va_arg (args, int);
	ch = *format++;
	state = DP_S_DOT;
      }
      else
	state = DP_S_DOT;
      break;
    case DP_S_DOT:
      if (ch == '.')
      {
	state = DP_S_MAX;
	ch = *format++;
      }
      else
	state = DP_S_MOD;
      break;
    case DP_S_MAX:
      if (isdigit(ch))
      {
	if (max < 0)
	  max = 0;
	max = 10*max + char_to_int (ch);
	ch = *format++;
      }
      else if (ch == '*')
      {
	max = va_arg (args, int);
	ch = *format++;
	state = DP_S_MOD;
      }
      else
	state = DP_S_MOD;
      break;
    case DP_S_MOD:
      /* Currently, we don't support Long Long, bummer */
      switch (ch)
      {
      case 'h':
	cflags = DP_C_SHORT;
	ch = *format++;
	break;
      case 'l':
	cflags = DP_C_LONG;
	ch = *format++;
	break;
      case 'L':
	cflags = DP_C_LDOUBLE;
	ch = *format++;
	break;
      default:
	break;
      }
      state = DP_S_CONV;
      break;
    case DP_S_CONV:
      switch (ch)
      {
      case 'd':
      case 'i':
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, short);
	else if (cflags == DP_C_LONG)
	  value = va_arg (args, long);
	else
	  value = va_arg (args, int);
	fmtint (buffer, &currlen, maxlen, value, 10, min, max, flags);
	break;
      case 'o':
	flags |= DP_F_UNSIGNED;
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, unsigned short);
	else if (cflags == DP_C_LONG)
	  value = va_arg (args, unsigned long);
	else
	  value = va_arg (args, unsigned int);
	fmtint (buffer, &currlen, maxlen, value, 8, min, max, flags);
	break;
      case 'u':
	flags |= DP_F_UNSIGNED;
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, unsigned short);
	else if (cflags == DP_C_LONG)
	  value = va_arg (args, unsigned long);
	else
	  value = va_arg (args, unsigned int);
	fmtint (buffer, &currlen, maxlen, value, 10, min, max, flags);
	break;
      case 'X':
	flags |= DP_F_UP;
      case 'x':
	flags |= DP_F_UNSIGNED;
	if (cflags == DP_C_SHORT)
	  value = va_arg (args, unsigned short);
	else if (cflags == DP_C_LONG)
	  value = va_arg (args, unsigned long);
	else
	  value = va_arg (args, unsigned int);
	fmtint (buffer, &currlen, maxlen, value, 16, min, max, flags);
	break;
      case 'f':
	if (cflags == DP_C_LDOUBLE)
	  fvalue = va_arg (args, LDOUBLE);
	else
	  fvalue = va_arg (args, double);
	/* um, floating point? */
	fmtfp (buffer, &currlen, maxlen, fvalue, min, max, flags);
	break;
      case 'E':
	flags |= DP_F_UP;
      case 'e':
	if (cflags == DP_C_LDOUBLE)
	  fvalue = va_arg (args, LDOUBLE);
	else
	  fvalue = va_arg (args, double);
	break;
      case 'G':
	flags |= DP_F_UP;
      case 'g':
	if (cflags == DP_C_LDOUBLE)
	  fvalue = va_arg (args, LDOUBLE);
	else
	  fvalue = va_arg (args, double);
	break;
      case 'c':
	dopr_outch (buffer, &currlen, maxlen, va_arg (args, int));
	break;
      case 's':
	strvalue = va_arg (args, char *);
	if (max < 0)
	  max = maxlen; /* ie, no max */
	fmtstr (buffer, &currlen, maxlen, strvalue, flags, min, max);
	break;
      case 'p':
	strvalue = (char *)va_arg (args, void *);
	fmtint (buffer, &currlen, maxlen, (int)strvalue, 16, min, max, flags);
	break;
      case 'n':
	if (cflags == DP_C_SHORT)
	{
	  short int *num;
	  num = va_arg (args, short *);
	  *num = currlen;
        }
	else if (cflags == DP_C_LONG)
	{
	  long int *num;
	  num = va_arg (args, long *);
	  *num = currlen;
        }
	else
	{
	  int *num;
	  num = va_arg (args, int *);
	  *num = currlen;
        }
	break;
      case '%':
	dopr_outch (buffer, &currlen, maxlen, ch);
	break;
      case 'w':
	/* not supported yet, treat as next char */
	ch = *format++;
	break;
      default:
	/* Unknown, skip */
	break;
      }
      ch = *format++;
      state = DP_S_DEFAULT;
      flags = cflags = min = 0;
      max = -1;
      break;
    case DP_S_DONE:
      break;
    default:
      /* hmm? */
      break; /* some picky compilers need this */
    }
  }
  if (currlen < maxlen - 1)
    buffer[currlen] = '\0';
  else
    buffer[maxlen - 1] = '\0';
}

void GI::Std::fmtstr (char *buffer, size_t *currlen, size_t maxlen,
		    char *value, int flags, int min, int max)
{
  int padlen, strln;     /* amount to pad */
  int cnt = 0;

  if (value == 0)
  {
    value = "<NULL>";
  }

  for (strln = 0; value[strln]; ++strln); /* strlen */
  padlen = min - strln;
  if (padlen < 0)
    padlen = 0;
  if (flags & DP_F_MINUS)
    padlen = -padlen; /* Left Justify */

  while ((padlen > 0) && (cnt < max))
  {
    dopr_outch (buffer, currlen, maxlen, ' ');
    --padlen;
    ++cnt;
  }
  while (*value && (cnt < max))
  {
    dopr_outch (buffer, currlen, maxlen, *value++);
    ++cnt;
  }
  while ((padlen < 0) && (cnt < max))
  {
    dopr_outch (buffer, currlen, maxlen, ' ');
    ++padlen;
    ++cnt;
  }
}

/* Have to handle DP_F_NUM (ie 0x and 0 alternates) */

void GI::Std::fmtint (char *buffer, size_t *currlen, size_t maxlen,
		    long value, int base, int min, int max, int flags)
{
  int signvalue = 0;
  unsigned long uvalue;
  char convert[20];
  int place = 0;
  int spadlen = 0; /* amount to space pad */
  int zpadlen = 0; /* amount to zero pad */
  int caps = 0;

  if (max < 0)
    max = 0;

  uvalue = value;

  if(!(flags & DP_F_UNSIGNED))
  {
    if( value < 0 ) {
      signvalue = '-';
      uvalue = -value;
    }
    else
      if (flags & DP_F_PLUS)  /* Do a sign (+/i) */
	signvalue = '+';
    else
      if (flags & DP_F_SPACE)
	signvalue = ' ';
  }

  if (flags & DP_F_UP) caps = 1; /* Should characters be upper case? */

  do {
    convert[place++] =
      (caps? "0123456789ABCDEF":"0123456789abcdef")
      [uvalue % (unsigned)base  ];
    uvalue = (uvalue / (unsigned)base );
  } while(uvalue && (place < 20));
  if (place == 20) place--;
  convert[place] = 0;

  zpadlen = max - place;
  spadlen = min - MAX (max, place) - (signvalue ? 1 : 0);
  if (zpadlen < 0) zpadlen = 0;
  if (spadlen < 0) spadlen = 0;
  if (flags & DP_F_ZERO)
  {
    zpadlen = MAX(zpadlen, spadlen);
    spadlen = 0;
  }
  if (flags & DP_F_MINUS)
    spadlen = -spadlen; /* Left Justifty */

#ifdef DEBUG_SNPRINTF
  dprint (1, (debugfile, "zpad: %d, spad: %d, min: %d, max: %d, place: %d\n",
      zpadlen, spadlen, min, max, place));
#endif

  /* Spaces */
  while (spadlen > 0)
  {
    dopr_outch (buffer, currlen, maxlen, ' ');
    --spadlen;
  }

  /* Sign */
  if (signvalue)
    dopr_outch (buffer, currlen, maxlen, signvalue);

  /* Zeros */
  if (zpadlen > 0)
  {
    while (zpadlen > 0)
    {
      dopr_outch (buffer, currlen, maxlen, '0');
      --zpadlen;
    }
  }

  /* Digits */
  while (place > 0)
    dopr_outch (buffer, currlen, maxlen, convert[--place]);

  /* Left Justified spaces */
  while (spadlen < 0) {
    dopr_outch (buffer, currlen, maxlen, ' ');
    ++spadlen;
  }
}

static LDOUBLE abs_val (LDOUBLE value)
{
  LDOUBLE result = value;

  if (value < 0)
    result = -value;

  return result;
}

static LDOUBLE _pow10_ (int exp)
{
  LDOUBLE result = 1;

  while (exp)
  {
    result *= 10;
    exp--;
  }

  return result;
}

static long _round_ (LDOUBLE value)
{
  long intpart;

  intpart = value;
  value = value - intpart;
  if (value >= 0.5)
    intpart++;

  return intpart;
}

void GI::Std::fmtfp (char *buffer, size_t *currlen, size_t maxlen,
		   LDOUBLE fvalue, int min, int max, int flags)
{
  int signvalue = 0;
  LDOUBLE ufvalue;
  char iconvert[20];
  char fconvert[20];
  int iplace = 0;
  int fplace = 0;
  int padlen = 0; /* amount to pad */
  int zpadlen = 0;
  int caps = 0;
  long intpart;
  long fracpart;

  /*
   * AIX manpage says the default is 0, but Solaris says the default
   * is 6, and sprintf on AIX defaults to 6
   */
  if (max < 0)
    max = 6;

  ufvalue = abs_val (fvalue);

  if (fvalue < 0)
    signvalue = '-';
  else
    if (flags & DP_F_PLUS)  /* Do a sign (+/i) */
      signvalue = '+';
    else
      if (flags & DP_F_SPACE)
	signvalue = ' ';

#if 0
  if (flags & DP_F_UP) caps = 1; /* Should characters be upper case? */
#endif

  intpart = ufvalue;

  /*
   * Sorry, we only support 9 digits past the decimal because of our
   * conversion method
   */
  if (max > 9)
    max = 9;

  /* We "cheat" by converting the fractional part to integer by
   * multiplying by a factor of 10
   */
  fracpart = _round_ ((_pow10_ (max)) * (ufvalue - intpart));

  if (fracpart >= _pow10_ (max))
  {
    intpart++;
    fracpart -= _pow10_ (max);
  }

#ifdef DEBUG_SNPRINTF
  dprint (1, (debugfile, "fmtfp: %f =? %d.%d\n", fvalue, intpart, fracpart));
#endif

  /* Convert integer part */
  do {
    iconvert[iplace++] =
      (caps? "0123456789ABCDEF":"0123456789abcdef")[intpart % 10];
    intpart = (intpart / 10);
  } while(intpart && (iplace < 20));
  if (iplace == 20) iplace--;
  iconvert[iplace] = 0;

  /* Convert fractional part */
  do {
    fconvert[fplace++] =
      (caps? "0123456789ABCDEF":"0123456789abcdef")[fracpart % 10];
    fracpart = (fracpart / 10);
  } while(fracpart && (fplace < 20));
  if (fplace == 20) fplace--;
  fconvert[fplace] = 0;

  /* -1 for decimal point, another -1 if we are printing a sign */
  padlen = min - iplace - max - 1 - ((signvalue) ? 1 : 0);
  zpadlen = max - fplace;
  if (zpadlen < 0)
    zpadlen = 0;
  if (padlen < 0)
    padlen = 0;
  if (flags & DP_F_MINUS)
    padlen = -padlen; /* Left Justifty */

  if ((flags & DP_F_ZERO) && (padlen > 0))
  {
    if (signvalue)
    {
      dopr_outch (buffer, currlen, maxlen, signvalue);
      --padlen;
      signvalue = 0;
    }
    while (padlen > 0)
    {
      dopr_outch (buffer, currlen, maxlen, '0');
      --padlen;
    }
  }
  while (padlen > 0)
  {
    dopr_outch (buffer, currlen, maxlen, ' ');
    --padlen;
  }
  if (signvalue)
    dopr_outch (buffer, currlen, maxlen, signvalue);

  while (iplace > 0)
    dopr_outch (buffer, currlen, maxlen, iconvert[--iplace]);

  /*
   * Decimal point.  This should probably use locale to find the correct
   * char to print out.
   */
  if (max > 0)
  {
    dopr_outch (buffer, currlen, maxlen, '.');

    while (fplace > 0)
      dopr_outch (buffer, currlen, maxlen, fconvert[--fplace]);
  }

  while (zpadlen > 0)
  {
    dopr_outch (buffer, currlen, maxlen, '0');
    --zpadlen;
  }

  while (padlen < 0)
  {
    dopr_outch (buffer, currlen, maxlen, ' ');
    ++padlen;
  }
}

void GI::Std::dopr_outch (char *buffer, size_t *currlen, size_t maxlen, char c)
{
  /*if (*currlen < maxlen)
    buffer[(*currlen)++] = c;*/
	print((unsigned char)c);
}

int GI::Std::vsnprintf (char *str, size_t count, const char *fmt, va_list args)
{
  str[0] = 0;
  dopr(str, count, fmt, args);
  return(strlen(str));
}
/* VARARGS3 */
/*
 * Source of this description is : http://www.cplusplus.com/reference/cstdio/printf/
 * specifier	output
 *
 * d or i		Signed decimal integer																392
 * u			Unsigned decimal integer															7235
 * o			Unsigned octal																		610
 * x			Unsigned hexadecimal integer														7fa
 * X			Unsigned hexadecimal integer (uppercase)											7FA
 * f			Decimal floating point, lowercase													392.65
 * F			Decimal floating point, uppercase													392.65
 * e			Scientific notation (mantissa/exponent), lowercase									3.9265e+2
 * E			Scientific notation (mantissa/exponent), uppercase									3.9265E+2
 * g			Use the shortest representation: %e or %f											392.65
 * G			Use the shortest representation: %E or %F											392.65
 * a			Hexadecimal floating point, lowercase												-0xc.90fep-2
 * A			Hexadecimal floating point, uppercase												-0XC.90FEP-2
 * c			Character																			a
 * s			String of characters																sample
 * p			Pointer address																		b8000000
 * n			Nothing printed.
 * 					The corresponding argument must be a pointer to a signed int.
 * 					The number of characters written so far is stored in the pointed location.
 * %			A % followed by another % character will write a single % to the stream.			%
 *
 *
 *
 *
 *The format specifier can also contain sub-specifiers: flags, width, .precision and modifiers (in that order), which are optional and follow these specifications:

 * flags		description
 * -			Left-justify within the given field width; Right justification is the default (see width sub-specifier).
 * +			Forces to preceed the result with a plus or minus sign (+ or -) even for positive numbers. By default, only negative numbers are preceded with a - sign.
 * (space)		If no sign is going to be written, a blank space is inserted before the value.
 * #			Used with o, x or X specifiers the value is preceeded with 0, 0x or 0X respectively for values different than zero.
 * 					Used with a, A, e, E, f, F, g or G it forces the written output to contain a decimal point even if no more digits follow. By default, if no digits follow, no decimal point is written.
 * 0			Left-pads the number with zeroes (0) instead of spaces when padding is specified (see width sub-specifier).
 *
 * width		description
 * (number)		Minimum number of characters to be printed. If the value to be printed is shorter than this number, the result is padded with blank spaces. The value is not truncated even if the result is larger.
 * *			The width is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted.
 *
 * .precision	description
 * .number		For integer specifiers (d, i, o, u, x, X): precision specifies the minimum number of digits to be written. If the value to be written is shorter than this number, the result is padded with leading zeros. The value is not truncated even if the result is longer. A precision of 0 means that no character is written for the value 0.
 * 					For a, A, e, E, f and F specifiers: this is the number of digits to be printed after the decimal point (by default, this is 6).
 * 					For g and G specifiers: This is the maximum number of significant digits to be printed.
 * 					For s: this is the maximum number of characters to be printed. By default all characters are printed until the ending null character is encountered.
 * 					If the period is specified without an explicit value for precision, 0 is assumed.
 * .*			The precision is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted.
 *
 *
 *
 *
 * The length sub-specifier modifies the length of the data type. This is a chart showing the types used to interpret the corresponding arguments with and without length specifier (if a different type is used, the proper type promotion or conversion is performed, if allowed):
 * 											specifiers
 * length		d i				u o x X						f F e E g G a A				c			s			p			n
 * (none)		int				unsigned int				double						int			char*		void*		int*
 * hh			signed char		unsigned char																				signed char*
 * h			short int		unsigned short int																			short int*
 * l			long int		unsigned long int										wint_t		wchar_t*				long int*
 * ll			long long int	unsigned long long int																		long long int*
 * j			intmax_t		uintmax_t																					intmax_t*
 * z			size_t			size_t																						size_t*
 * t			ptrdiff_t		ptrdiff_t																					ptrdiff_t*
 * L														long double
 *
 *
 * Note regarding the c specifier: it takes an int (or wint_t) as argument, but performs the proper conversion to a char value (or a wchar_t) before formatting it for output.
 *
 * Example
 *
 *  uart_printf example
 *    uart_printf ("Characters: %c %c \n", 'a', 65);
 *    uart_printf ("Decimals: %d %ld\n", 1977, 650000L);
 *    uart_printf ("Preceding with blanks: %10d \n", 1977);
 *    uart_printf ("Preceding with zeros: %010d \n", 1977);
 *    uart_printf ("Some different radices: %d %x %o %#x %#o \n", 100, 100, 100, 100, 100);
 *    uart_printf ("floats: %4.2f %+.0e %E \n", 3.1416, 3.1416, 3.1416);
 *    uart_printf ("Width trick: %*d \n", 5, 10);
 *    uart_printf ("%s \n", "A string");
 *
 * Output:
 *
 *
 * Characters: a A
 * Decimals: 1977 650000
 * Preceding with blanks:       1977
 * Preceding with zeros: 0000001977
 * Some different radices: 100 64 144 0x64 0144
 * floats: 3.14 +3e+000 3.141600E+000
 * Width trick:    10
 * A string
 *
 */
void GI::Std::printF(const char *pcString, ...)
//int snprintf (Uart_t* char *str,size_t count,const char *fmt,...)
{
	VA_LOCAL_DECL;
	VA_START (pcString);
	VA_SHIFT (str, char *);
	VA_SHIFT (count, size_t );
	VA_SHIFT (fmt, char *);
	char buff[1];
	vsnprintf(buff, 65535, pcString, ap);
	VA_END;
}

void GI::Std::printF(GI::String *string, ...)
//int snprintf (Uart_t* char *str,size_t count,const char *fmt,...)
{
	const char *pcString = string->buff;
	VA_LOCAL_DECL;
	VA_START (pcString);
	VA_SHIFT (str, char *);
	VA_SHIFT (count, size_t );
	VA_SHIFT (fmt, char *);
	char buff[1];
	vsnprintf(buff, 65535, (const char *)string->buff, ap);
	VA_END;
}

void GI::Std::printF(GI::Screen::String *string, ...)
//int snprintf (Uart_t* char *str,size_t count,const char *fmt,...)
{
	const char *pcString = string->buff;
	VA_LOCAL_DECL;
	VA_START (pcString);
	VA_SHIFT (str, char *);
	VA_SHIFT (count, size_t );
	VA_SHIFT (fmt, char *);
	char buff[1];
	vsnprintf(buff, 65535, (const char *)string->buff, ap);
	VA_END;
}
#endif /* !HAVE_SNPRINTF */
