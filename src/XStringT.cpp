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
// definition of 'no position' value
const String::size_type String::npos = (String::size_type)(-1);

String::String()
{
	// TODO Auto-generated constructor stub
	init();
}

String::~String()
{
	// TODO Auto-generated destructor stub
	if (d_reserve > XSTRINGT_STRING_QUICKBUFF_SIZE)
	{
		delete[] d_buffer;
	}
		if (d_encodedbufflen > 0)
	{
		delete[] d_encodedbuff;
	}
}

bool String::grow(size_type new_size)
{
    // check for too big
    if (max_size() <= new_size)
    {
    	XSTRINGT_THROW(std::length_error("Resulting String::grow() would be too big"));
    }

    // increase, as we always null-terminate the buffer.
    ++new_size;

    if (new_size > d_reserve)
    {
        utf32* temp = XSTRINGT_NEW_ARRAY_PT(utf32, new_size);

        if (d_reserve > XSTRINGT_STRING_QUICKBUFF_SIZE)
        {
            memcpy(temp, d_buffer, (d_cplength + 1) * sizeof(utf32));
            XSTRINGT_DELETE_ARRAY_PT(d_buffer, utf32, d_reserve);
        }
        else
        {
            memcpy(temp, d_quickbuff, (d_cplength + 1) * sizeof(utf32));
        }

        d_buffer = temp;
        d_reserve = new_size;

        return true;
    }

    return false;
}

// perform re-allocation to remove wasted space.
void String::trim(void)
{
    size_type min_size = d_cplength + 1;

    // only re-allocate when not using quick-buffer, and when size can be trimmed
    if ((d_reserve > XSTRINGT_STRING_QUICKBUFF_SIZE) && (d_reserve > min_size))
    {
            // see if we can trim to quick-buffer
        if (min_size <= XSTRINGT_STRING_QUICKBUFF_SIZE)
        {
            memcpy(d_quickbuff, d_buffer, min_size * sizeof(utf32));
            XSTRINGT_DELETE_ARRAY_PT(d_buffer, utf32, d_reserve);
            d_reserve = XSTRINGT_STRING_QUICKBUFF_SIZE;
        }
        // re-allocate buffer
        else
        {
            utf32* temp = XSTRINGT_NEW_ARRAY_PT(utf32, min_size);
            memcpy(temp, d_buffer, min_size * sizeof(utf32));
            XSTRINGT_DELETE_ARRAY_PT(d_buffer, utf32, d_reserve);
            d_buffer = temp;
            d_reserve = min_size;
        }

    }

}

String::size_type String::encode(const utf32* src, utf8* dest, size_type dest_len, size_type src_len) const
{
	// count length for null terminated source...
	if (src_len == 0)
	{
		src_len = utf_length(src);
	}

	size_type destCapacity = dest_len;

	// while there is data in the source buffer,
	for (unsigned int idx = 0; idx < src_len; ++idx)
	{
		utf32	cp = src[idx];

		// check there is enough destination buffer to receive this encoded unit (exit loop & return if not)
		if (destCapacity < encoded_size(cp))
		{
			break;
		}

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

String::size_type String::encode(const utf8* src, utf32* dest, size_type dest_len, size_type src_len) const
{
	// count length for null terminated source...
	if (src_len == 0)
	{
		src_len = utf_length(src);
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

String::size_type String::encoded_size(utf32 code_point) const
{
	if (code_point < 0x80)
		return 1;
	else if (code_point < 0x0800)
		return 2;
	else if (code_point < 0x10000)
		return 3;
	else
		return 4;
}

String::size_type String::encoded_size(const utf32* buf) const
{
	return encoded_size(buf, utf_length(buf));
}

String::size_type String::encoded_size(const utf32* buf, size_type len) const
{
	size_type count = 0;

	while (len--)
	{
		count += encoded_size(*buf++);
	}

	return count;
}

// return number of utf32 code units required to re-encode given utf8 data as utf32.  return does not include terminating null.
String::size_type String::encoded_size(const utf8* buf) const
{
	return encoded_size(buf, utf_length(buf));
}

// return number of utf32 code units required to re-encode given utf8 data as utf32.  len is number of code units in 'buf'.
String::size_type String::encoded_size(const utf8* buf, size_type len) const
{
	utf8 tcp;
	size_type count = 0;

	while (len--)
	{
		tcp = *buf++;
		++count;
		size_type size = 0;

		if (tcp < 0x80)
		{
		}
		else if (tcp < 0xE0)
		{
			size = 1;
			++buf;
		}
		else if (tcp < 0xF0)
		{
			size = 2;
			buf += 2;
		}
		else
		{
			size = 3;
			buf += 3;
		}

		if (len >= size)
			len -= size;
		else 
			break;
	}

	return count;
}

// build an internal buffer with the string encoded as utf8 (remains valid until string is modified).
utf8* String::build_utf8_buff(void) const
{
    size_type buffsize = encoded_size(ptr(), d_cplength) + 1;

    if (buffsize > d_encodedbufflen) {

        if (d_encodedbufflen > 0)
        {
            XSTRINGT_DELETE_ARRAY_PT(d_encodedbuff, utf8, d_encodedbufflen);
        }

        d_encodedbuff = XSTRINGT_NEW_ARRAY_PT(utf8, buffsize);
        d_encodedbufflen = buffsize;
    }

    encode(ptr(), d_encodedbuff, buffsize, d_cplength);

    // always add a null at end
    d_encodedbuff[buffsize-1] = ((utf8)0);
    d_encodeddatlen = buffsize;

    return d_encodedbuff;
}

//////////////////////////////////////////////////////////////////////////
// Concatenation operator functions
//////////////////////////////////////////////////////////////////////////
String	operator+(const String& str1, const String& str2)
{
	String temp(str1);
	temp.append(str2);
	return temp;
}

String	operator+(const String& str, const utf8* utf8_str)
{
	String temp(str);
	temp.append(utf8_str);
	return temp;
}

String	operator+(const utf8* utf8_str, const String& str)
{
	String temp(utf8_str);
	temp.append(str);
	return temp;
}

String operator+(const String& str, const char* c_str)
{
	String tmp(str);
	tmp.append(c_str);
	return tmp;
}

String operator+(const char* c_str, const String& str)
{
	String tmp(c_str);
	tmp.append(str);
	return tmp;
}

}
