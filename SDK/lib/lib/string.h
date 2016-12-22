/*
 * string.h
 *
 *  Created on: Dec 13, 2016
 *      Author: John Smith
 */

#ifndef LIB_LIB_STRING_H_
#define LIB_LIB_STRING_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <include/global.h>

namespace GI
{
	class String
	{
	public:
		String();
		String(char *String);
		~String();
		bool equal(GI::String *string);
		void toUper();
		void toLower();
		void removeNewLine(GI::String *string);
		void set(char* string);
		void set(GI::String *string);
		void append(char* string);
		void append(char Char);
		void insert(GI::String* string, unsigned int location);
		void insert(char *string, unsigned int location);
		void clone(GI::String* string);
		void clear();
		char *subString(unsigned int position, unsigned int len);

		char *buff;
		signed int length;
		/*
		 * Private variables.
		 */
		unsigned int modifyed;
		int error;
		//bool initialized;
	};
}



#endif /* LIB_LIB_STRING_H_ */
