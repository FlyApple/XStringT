/*
 * MXStringTranscoderT.h
 *
 *  Created on: 2017年6月29日
 *      Author: silly
 *
 * Updated to 2017-11-27: 
 * Base encode UTF32(LE/BE)
 * Windows default : ANSI
 * Windows process : 
 *					ANSI->UTF16->UTF8->UTF32 (not use iconv library, need add utf8)
 *					UTF32->UTF16->ANSI (use iconv library weak convert, only UTF32->ANSI)
 * Linux/Unix default : UTF8
 * Linux/Unix process :
 *					UTF8->UTF32
 *					UTF32->UTF8
 */

#ifndef __MXSTRING_TRANSCODER_H__
#define __MXSTRING_TRANSCODER_H__

#include "MXStringT.h"

//
namespace MXString
{

/*-----------------------------------------------------
StringTranscoder Class
-------------------------------------------------------*/
class StringTranscoder
{
public:
	StringTranscoder(){}
	virtual ~StringTranscoder() {}

    //! deletes a buffer returned from the stringToXXX function.
    virtual void		deleteANSIBuffer(const char* input) const		= 0;
    virtual void		deleteUTF16Buffer(const utf16* input) const		= 0;
};

/*-----------------------------------------------------
IconvStringTranscoder Class
-------------------------------------------------------*/
//! Implementation of StringTranscoder that uses iconv
class IconvStringTranscoder : public StringTranscoder
{
public:
    IconvStringTranscoder();

    // implement abstract interface
	char*			stringToANSI(StringX& input) const;
    utf16*			stringToUTF16(StringX& input) const;

	StringX			stringFromANSI(const char* input, StringX::size_type len = StringX::npos) const;
    StringX			stringFromUTF16(const utf16* input, StringX::size_type len = StringX::npos) const;

    void			deleteANSIBuffer(const char* input) const;
    void			deleteUTF16Buffer(const utf16* input) const;
private:

	//
	__inline const char* UTF16PE(void)	const { return is_big_endian() ? "UTF-16BE" : "UTF-16LE"; };
	__inline const char* UTF32PE(void)	const { return is_big_endian() ? "UTF-32BE" : "UTF-32LE"; };

	// Helper to detect the platform endianess at run time.
	__inline bool is_big_endian(void) const
	{
		union
		{
			unsigned int	i;
			unsigned char	c[4];
		} bint = {0x01020304};

		return bint.c[0] == 0x01;
	}
};

};//namespace MXString

#endif /* __MXSTRING_TRANSCODER_H__ */
