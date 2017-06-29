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

    /*!
    \brief
        Transcode the given string to a UTF-16 encoded buffer.

    \param input
        String object with the text to be transcoded.

    \return
        Pointer to an array of utf16 values.  This buffer should be deleted by
        calling the deleteUTF16Buffer function.
    */
    virtual utf16* stringToUTF16(const String& input) const = 0;

    /*!
    \brief
        Transcode the given string to a std::wstring object.

    \param input
        String object with the text to be transcoded.

    \return
        std::wstring holding the transcoded data in some appropriate encoding.

    \note
        What is represented by std::wstring and how it should be interpreted is
        implementation specific.  This means that the content of the returned
        std::wstring may vary according to the operating system and compiler
        used - although what is returned should be consistent with other
        std::wstring data running on the same implementation.  This largely
        means that on Microsoft Windows you will have UTF-16 and on *nix type
        environments you will have UTF-32.
    */
    virtual std::wstring stringToStringW(const String& input) const = 0;

    /*
    \brief
        Constructs a String object from the given null terminated UTF-16 encoded
        buffer.

    \param input
        Pointer to a null terminated array of uint16 values representing a
        string encoded using UTF-16.

    \return
        String object holding the transcoded data.
    */
    virtual String stringFromUTF16(const utf16* input) const = 0;

    /*
    \brief
        Constructs a String object from the given std::wstring.

    \param input
        reference to a std::wstring object holding the string data to be
        transcoded.

    \return
        String object holding the transcoded data.

    \note
        What is represented by std::wstring and how it should be interpreted is
        implementation specific.  This means that the content of the
        std::wstring you pass must be consistent with what is expected given the
        implementation.  This largely means that on Microsoft Windows you will
        use UTF-16 and on *nix type environments you will use UTF-32.  Do NOT
        assume that blithely passing what you think is UTF-16 data in a
        std::wstring will work everywhere - because it wont.
    */
    virtual String stringFromStringW(const std::wstring& input) const = 0;

    //! deletes a buffer returned from the stringToUTF16 function.
    virtual void deleteUTF16Buffer(utf16* input) const = 0;    
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
	char*			stringToANSI(const String& input) const;
    utf16*			stringToUTF16(const String& input) const;
    std::wstring	stringToStringW(const String& input) const;
    String			stringFromUTF16(const utf16* input) const;
    String			stringFromStringW(const std::wstring& input) const;
    void			deleteUTF16Buffer(utf16* input) const;

private:
	
	//
#if defined(_MSC_VER)
	unsigned long	d_nLanguage;
	char			d_szLanguage[LOCALE_NAME_MAX_LENGTH+1];
	__inline char* ANSIPE(void)		const 
	{
		switch (d_nLanguage)
		{
		case 0x34303830:{ return "GBK"; }				//0804 "CHS"
		case 0x34303430:{ return "BIG5"; }				//0404 "CHT"
		case 0x31313030:{ return "SHIFT-JIS"; }			//0011 "JPN"
		case 0x32313030:{ return "KSC_5601"; }			//0012 "KOR"
		}
		return "MS-ANSI";
	}
#else
	__inline char* ANSIPE(void)		const { return "UTF-8"; }
#endif

	__inline char* UTF16PE(void)	const { return is_big_endian() ? "UTF-16BE" : "UTF-16LE"; };
	__inline char* UTF32PE(void)	const { return is_big_endian() ? "UTF-32BE" : "UTF-32LE"; };

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
