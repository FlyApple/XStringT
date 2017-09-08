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

    //! deletes a buffer returned from the stringToXXX function.
    virtual void		deleteANSIBuffer(const char* input) const		= 0;
    virtual void		deleteUTF16Buffer(const utf16* input) const		= 0;
    virtual void		deleteUTF32Buffer(const utf32* input) const		= 0; 
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
	char*			stringToANSI(StringAU8& input) const;
    utf16*			stringToUTF16(StringAU8& input) const;
    utf32*			stringToUTF32(StringAU8& input) const;

	StringAU8		stringFromANSI(const char* input, StringBase::size_type len = StringBase::npos) const;
    StringAU8		stringFromUTF16(const utf16* input, StringBase::size_type len = StringBase::npos) const;
    StringAU8		stringFromUTF32(const utf32* input, StringBase::size_type len = StringBase::npos) const;

    void			deleteANSIBuffer(const char* input) const;
    void			deleteUTF16Buffer(const utf16* input) const;
    void			deleteUTF32Buffer(const utf32* input) const;

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
