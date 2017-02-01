/*
 * util.cpp
 *
 *  Created on: Jan 29, 2017
 *      Author: John Smith
 */

#include "util.h"

static const char splitChard[] = {' ', ','};

SysErr Sys::Util::strToCmd(GI::String *inStr, GI::StringArray **result)
{
	GI::StringArray *tmp_cmt_splitted = new GI::StringArray();
	unsigned int char_count = 0;
	GI::String *tmp_in_str = new GI::String(inStr->buff);
	while(tmp_in_str->length)
	{
		if(tmp_in_str->buff[char_count] == splitChard[0] || tmp_in_str->buff[char_count] == splitChard[1] || tmp_in_str->buff[char_count] == '\0')
		{
			if(tmp_in_str->buff[0] == splitChard[0] || tmp_in_str->buff[0] == splitChard[1] || tmp_in_str->buff[0] == '\0')
			{
				char *tmp_str;
				tmp_in_str->set((tmp_str = tmp_in_str->subString(1)));
				if(tmp_str)
					free(tmp_str);
			}
			else
			{
				tmp_in_str->buff[char_count] = '\0';
				tmp_cmt_splitted->add(tmp_in_str->buff);
				char *tmp_str;
				if(char_count >= tmp_in_str->length)
					tmp_in_str->clear();
				else
				{
					tmp_in_str->set((tmp_str = tmp_in_str->subString(char_count + 1)));
					if(tmp_str)
						free(tmp_str);
				}
			}
			char_count = 0;
			continue;
		}
		char_count++;
	}
	*result = tmp_cmt_splitted;
	return SYS_ERR_OK;
}


