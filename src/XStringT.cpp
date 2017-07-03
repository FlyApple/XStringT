/*
 * XStringT.cpp
 *
 *  Created on: 2017年6月29日
 *      Author: silly
 */

#include "XStringT.h"


//
namespace XStringT
{

/*-----------------------------------------------------
StringAU8 Class
-------------------------------------------------------*/
//////////////////////////////////////////////////////////////////////////
// Concatenation operator functions
//////////////////////////////////////////////////////////////////////////
StringAU8	operator+(const StringAU8& str1, const StringAU8& str2)
{
	StringAU8 temp(str1);
	temp.append(str2);
	return temp;
}

StringAU8	operator+(const StringAU8& str, const utf8* utf8_str)
{
	StringAU8 temp(str);
	temp.append(utf8_str);
	return temp;
}

StringAU8	operator+(const utf8* utf8_str, const StringAU8& str)
{
	StringAU8 temp(utf8_str);
	temp.append(str);
	return temp;
}

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////
StringAU8::StringAU8()
{
	// TODO Auto-generated constructor stub
}

StringAU8::~StringAU8()
{
	// TODO Auto-generated destructor stub
}

// return number of utf32 code units required to re-encode given utf8 data as utf32.  len is number of code units in 'buf'.
StringAU8::size_type StringAU8::encoded_size(const utf8* buf, size_type len) const
{
	utf8 tcp;
	size_type count = 0;

	while (len--)
	{
		tcp = *buf++; ++count;

		size_type size = 0;
		if (tcp < 0x80){ }
		else if (tcp < 0xE0){ size = 1; buf += 1; }
		else if (tcp < 0xF0){ size = 2; buf += 2; }
		else{ size = 3; buf += 3; }

		if (len >= size){ len -= size; }
		else{ break; }
	}

	return count;
}

StringAU8::size_type StringAU8::encode(const utf8* src, utf32* dest, size_type dest_len, size_type src_len) const
{
	// count length for null terminated source...
	if (src_len == 0)
	{
		src_len = total_length(src);
	}

	size_type destCapacity = dest_len;

	// while there is data in the source buffer, and space in the dest buffer
	for (unsigned int idx = 0; ((idx < src_len) && (destCapacity > 0));)
	{
		utf32	cp;
		utf8	cu = src[idx++];

		if (cu < 0x80)
		{
			cp = (utf32)(cu);
		}
		else if (cu < 0xE0)
		{
			cp = ((cu & 0x1F) << 6);
			cp |= (src[idx++] & 0x3F);
		}
		else if (cu < 0xF0)
		{
			cp = ((cu & 0x0F) << 12);
			cp |= ((src[idx++] & 0x3F) << 6);
			cp |= (src[idx++] & 0x3F);
		}
		else
		{
			cp = ((cu & 0x07) << 18);
			cp |= ((src[idx++] & 0x3F) << 12);
			cp |= ((src[idx++] & 0x3F) << 6);
			cp |= (src[idx++] & 0x3F);
		}

		*dest++ = cp;
		--destCapacity;
	}

	return dest_len - destCapacity;
}

}
