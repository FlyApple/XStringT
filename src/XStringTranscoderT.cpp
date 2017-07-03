/*
 * XStringTranscoderT.cpp
 *
 *  Created on: 2017年6月29日
 *      Author: silly
 */

#if defined(_MSC_VER)
#include <windows.h>
#endif 

#include "XStringTranscoderT.h"

#include <vector>
#include <string>
#include <iconv/iconv.h>


//
namespace XStringT
{

//----------------------------------------------------------------------------//
class IconvHelper
{
public:
    //------------------------------------------------------------------------//
    IconvHelper(const std::string& tocode, const std::string& fromcode) :
        d_fromCode(fromcode),
        d_toCode(tocode),
        d_cd(iconv_open(d_toCode.c_str(), d_fromCode.c_str()))
    {
        if (d_cd == reinterpret_cast<iconv_t>(-1))
		{
            XSTRINGT_THROW("Failed to create conversion descriptor from \"" + d_fromCode + "\" to \"" + d_toCode + "\".");
		}
    }

	virtual ~IconvHelper()
    {
        iconv_close(d_cd);
    }

    //------------------------------------------------------------------------//
    size_t iconv(const char** inbuf, size_t* inbytesleft,
                 char** outbuf, size_t* outbytesleft)
    {
        return ::iconv(d_cd, const_cast<char**>(inbuf), inbytesleft, outbuf, outbytesleft);
    }

    //------------------------------------------------------------------------//
    void throwErrorException(int err)
    {
        std::string reason;

		if (err == EINVAL){ reason = "Incomplete " + d_fromCode + " sequence."; }
		else if (err == EILSEQ){ reason = "Invalid " + d_fromCode + " sequence."; }
		else { reason = "Unknown error."; }

        XSTRINGT_THROW("Failed to convert from \"" + d_fromCode + "\" to \"" + d_toCode + "\": " + reason);
    }

private:
	std::string d_fromCode;
    std::string d_toCode;
    iconv_t		d_cd;
};

//----------------------------------------------------------------------------//
// Helper to use iconv to perform some transcode operation.
template<typename T>
static T* iconvTranscode(IconvHelper& ich, const utf8* in_buf, size_t in_len)
{
    // Handle empty strings
    if (in_len == 0)
    {
        T* ret_buff = XSTRINGT_NEW_ARRAY_PT(T, 1);
        ret_buff[0] = 0;
        return ret_buff;
    }

    std::vector<T> out_vec;
    out_vec.resize(in_len);
    size_t out_count = 0;

    while (true)
    {
        char* out_buf = reinterpret_cast<char*>(&out_vec[out_count]);
        const size_t start_out_bytes_left = (out_vec.size() - out_count) * sizeof(T);
        size_t out_bytes_left = start_out_bytes_left;

        const size_t result = ich.iconv((const char**)&in_buf, &in_len, &out_buf, &out_bytes_left);

        out_count += (start_out_bytes_left - out_bytes_left) / sizeof(T);

        if (result != static_cast<size_t>(-1))
        {
            T* ret_buff = XSTRINGT_NEW_ARRAY_PT(T, out_count + 1);
            memcpy(ret_buff, &out_vec[0], out_count * sizeof(T));
            ret_buff[out_count] = 0;
            return ret_buff;
        }

        if (errno != E2BIG)
            break;

        out_vec.resize(out_vec.size() + 8); // this is some arbitrary number
    }

#if defined(DEBUG) || defined(_DEBUG)
    ich.throwErrorException(errno);
#endif

    // this is there mostly to silence compiler warnings, this code should
    // never be executed
    return 0;
}

//----------------------------------------------------------------------------//
// Helper tp return length of zero terminated string of Ts
template<typename T>
static size_t getStringLength(const T* buffer)
{
    const T* b = buffer;
    while (*b++);

    return b - buffer - 1;
}

//----------------------------------------------------------------------------//
// Helper to correctly delete a buffer returned from iconvTranscode
template<typename T>
static void deleteTranscodeBuffer(T* buffer)
{
    XSTRINGT_DELETE_ARRAY_PT(buffer, T, getStringLength(buffer) + 1);
}

//----------------------------------------------------------------------------//
// Helper to transcode a buffer and return a string class built from it.
template<typename String_T, typename CodeUnit_T>
static String_T iconvTranscode(IconvHelper& ich, const utf8* in_buf, size_t in_len)
{
    CodeUnit_T* tmp = iconvTranscode<CodeUnit_T>(ich, in_buf, in_len);
    String_T result(tmp);
    deleteTranscodeBuffer(tmp);
    return result;
}


//----------------------------------------------------------------------------//
IconvStringTranscoder::IconvStringTranscoder()
{
#if defined(_MSC_VER)
	GetLocaleInfoA(LOCALE_SYSTEM_DEFAULT, LOCALE_ILANGUAGE, (LPSTR)&d_nLanguage, sizeof(d_nLanguage)/sizeof(CHAR));
	GetLocaleInfoA(LOCALE_SYSTEM_DEFAULT, LOCALE_SENGLANGUAGE, (LPSTR)d_szLanguage, sizeof(d_szLanguage)/sizeof(CHAR) - 1);
#endif
}

//----------------------------------------------------------------------------//
char* IconvStringTranscoder::stringToANSI(const StringAU8& input) const
{
	const char* data = input.c_str();
	int length = strlen((const char*)data);

#if defined(_MSC_VER)
	int lengthW = MultiByteToWideChar(CP_UTF8, 0, (const char*)data, length, NULL, 0);
	utf16* dataW = XSTRINGT_NEW_ARRAY_PT(utf16, lengthW + 1);
	MultiByteToWideChar(CP_UTF8, 0, (const char*)data, length, (wchar_t*)dataW, lengthW);
	int lengthA = WideCharToMultiByte(CP_ACP, 0, (wchar_t*)dataW, lengthW, NULL, 0, NULL, NULL);
	char* result = XSTRINGT_NEW_ARRAY_PT(char, lengthA + 1);
	WideCharToMultiByte(CP_ACP, 0, (wchar_t*)dataW, lengthW, result, lengthA, NULL, NULL);
	result[lengthA] = (char)0;
	XSTRINGT_DELETE_ARRAY_PT(dataW, utf16, lengthW+1);
#else
	utf8* result = XSTRINGT_NEW_ARRAY_PT(utf8, length + 1);
	input.copy(result);
	result[length] = (utf8)(0);
#endif
	return (char*)result;
}

utf16* IconvStringTranscoder::stringToUTF16(const StringAU8& input) const
{
    IconvHelper ich(UTF16PE(), "UTF-8");
	return iconvTranscode<utf16>(ich, (const utf8*)input.c_str(), getStringLength(input.c_str()));
}

//----------------------------------------------------------------------------//
std::wstring IconvStringTranscoder::stringToStringW(const StringAU8& input) const
{
#if defined(_MSC_VER)
	IconvHelper ich(UTF16PE(), "UTF-8");
#else
    IconvHelper ich("WCHAR_T", "UTF-8");
#endif
	return iconvTranscode<std::wstring, wchar_t>(ich, (const utf8*)input.c_str(), getStringLength(input.c_str()));
}

//----------------------------------------------------------------------------//
StringAU8		IconvStringTranscoder::stringFromANSI(const char* input) const
{
	const char* data = input;
	int length = strlen((const char*)data);

#if defined(_MSC_VER)

	int lengthW = MultiByteToWideChar(CP_ACP, 0, (const char*)data, length, NULL, 0);
	utf16* dataW = XSTRINGT_NEW_ARRAY_PT(utf16, lengthW + 1);
	MultiByteToWideChar(CP_ACP, 0, (const char*)data, length, (wchar_t*)dataW, lengthW);
	int lengthA = WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)dataW, lengthW, NULL, 0, NULL, NULL);
	char* resultA = XSTRINGT_NEW_ARRAY_PT(char, lengthA + 1);
	WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)dataW, lengthW, resultA, lengthA, NULL, NULL);
	resultA[lengthA] = (char)0;
	StringAU8 result((utf8*)resultA);
	XSTRINGT_DELETE_ARRAY_PT(dataW, utf16, lengthW+1);
	XSTRINGT_DELETE_ARRAY_PT(resultA, utf8, lengthA+1);

#else
	StringAU8 result(input);
#endif
	return result;
}

//----------------------------------------------------------------------------//
StringAU8 IconvStringTranscoder::stringFromUTF16(const utf16* input) const
{
#if defined(_MSC_VER)
    IconvHelper ich("UTF-8", UTF16PE());
    return iconvTranscode<StringAU8, utf8>(ich, reinterpret_cast<const utf8*>(input), getStringLength(input)*sizeof(utf16));
#else
    IconvHelper ich("WCHAR_T", UTF16PE());
    return stringFromStringW(iconvTranscode<std::wstring, wchar_t>(ich, reinterpret_cast<const utf8*>(input), getStringLength(input)));
#endif
}

//----------------------------------------------------------------------------//
StringAU8 IconvStringTranscoder::stringFromStringW(const std::wstring& input) const
{
#if defined(_MSC_VER)
    IconvHelper ich("UTF-8", UTF16PE());
    return iconvTranscode<StringAU8, utf8>(ich, reinterpret_cast<const utf8*>(input.c_str()), input.length() * sizeof(wchar_t));
#else
    IconvHelper ich("WCHAR_T", UTF16PE());
    return iconvTranscode<StringAU8, utf8>(ich, reinterpret_cast<const utf8*>(input.c_str()), input.length() * sizeof(wchar_t));
#endif
}

//----------------------------------------------------------------------------//
void IconvStringTranscoder::deleteANSIBuffer(const char* input) const
{
	deleteTranscodeBuffer(input);
}

void IconvStringTranscoder::deleteUTF16Buffer(const utf16* input) const
{
    deleteTranscodeBuffer(input);
}


}
