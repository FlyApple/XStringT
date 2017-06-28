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

}
