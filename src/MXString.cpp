/*
 * MXString.cpp
 *
 *  Created on: 2017年6月29日
 *      Author: silly
 */

#include "MXString.h"


//
namespace MXString
{
	//
	String::size_type			String::encoded_size(utf32 code_point) const
	{
		if (code_point < 0x80){ return 1; }
		else if (code_point < 0x0800){ return 2; }
		else if (code_point < 0x10000){ return 3; }
		return 4;
	}

	String::size_type			String::encoded_size(const utf32* data, String::size_type length) const
	{
		size_type count = 0;
		while (length--) { count += encoded_size(*data++); }
		return count;
	}

	String::size_type			String::encoded_size(const utf8* data, String::size_type length) const
	{
		utf8 tcp;
		size_type count = 0;

		while (length--)
		{
			tcp = *data++; ++count;

			size_type size = 0;
			if (tcp < 0x80){ }
			else if (tcp < 0xE0){ size = 1; data += 1; }
			else if (tcp < 0xF0){ size = 2; data += 2; }
			else{ size = 3; data += 3; }

			if (length >= size){ length -= size; }
			else{ break; }
		}
		return count;
	}

	//
	String::size_type			String::encode(const utf32* src, utf8* dest, size_type dest_len, size_type src_len) const
	{
		// count length for null terminated source...
		if (src_len == 0)
		{ src_len = total_length(src); }

		size_type destCapacity = dest_len;

		// while there is data in the source buffer,
		for (size_type idx = 0; idx < src_len; ++idx)
		{
			utf32	cp = src[idx];

			// check there is enough destination buffer to receive this encoded unit (exit loop & return if not)
			if (destCapacity < encoded_size(cp))
			{ break; }

			if (cp < 0x80)
			{
				*dest++ = (utf8)cp;
				--destCapacity;
			}
			else if (cp < 0x0800)
			{
				*dest++ = (utf8)((cp >> 6) | 0xC0);
				*dest++ = (utf8)((cp & 0x3F) | 0x80);
				destCapacity -= 2;
			}
			else if (cp < 0x10000)
			{
				*dest++ = (utf8)((cp >> 12) | 0xE0);
				*dest++ = (utf8)(((cp >> 6) & 0x3F) | 0x80);
				*dest++ = (utf8)((cp & 0x3F) | 0x80);
				destCapacity -= 3;
			}
			else
			{
				*dest++ = (utf8)((cp >> 18) | 0xF0);
				*dest++ = (utf8)(((cp >> 12) & 0x3F) | 0x80);
				*dest++ = (utf8)(((cp >> 6) & 0x3F) | 0x80);
				*dest++ = (utf8)((cp & 0x3F) | 0x80);
				destCapacity -= 4;
			}
		}

		return dest_len - destCapacity;
	}

	String::size_type			String::encode(const utf8* src, utf32* dest, size_type dest_len, size_type src_len) const
	{
		// count length for null terminated source...
		if (src_len == 0)
		{ src_len = total_length(src); }

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
