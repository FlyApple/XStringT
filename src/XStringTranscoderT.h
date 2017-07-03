/*
 * XStringTranscoderT.h
 *
 *  Created on: 2017年6月29日
 *      Author: silly
 */

#ifndef __XSTRINGT_TRANSCODER_H__
#define __XSTRINGT_TRANSCODER_H__

#include "XStringT.h"

//
namespace XStringT
{

/*-----------------------------------------------------
StringTranscoder Class
-------------------------------------------------------*/
class StringTranscoder
{
public:
	StringTranscoder(){}
	virtual ~StringTranscoder() {}

	//
    virtual utf16*			stringToUTF16(const StringAU8& input) const = 0;
    virtual std::wstring	stringToStringW(const StringAU8& input) const = 0;
    virtual StringAU8		stringFromUTF16(const utf16* input) const = 0;
    virtual StringAU8		stringFromStringW(const std::wstring& input) const = 0;

    //! deletes a buffer returned from the stringToUTF16 function.
    virtual void deleteUTF16Buffer(const utf16* input) const = 0;    
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
	char*			stringToANSI(const StringAU8& input) const;
    utf16*			stringToUTF16(const StringAU8& input) const;
    std::wstring	stringToStringW(const StringAU8& input) const;
	StringAU8		stringFromANSI(const char* input) const;
    StringAU8		stringFromUTF16(const utf16* input) const;
    StringAU8		stringFromStringW(const std::wstring& input) const;
    void			deleteANSIBuffer(const char* input) const;
    void			deleteUTF16Buffer(const utf16* input) const;

private:
	
	//
#if defined(_MSC_VER)
	unsigned long	d_nLanguage;
	char			d_szLanguage[LOCALE_NAME_MAX_LENGTH+1];
#endif

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

};//namespace XStringT

#endif /* __XSTRINGT_TRANSCODER_H__ */
