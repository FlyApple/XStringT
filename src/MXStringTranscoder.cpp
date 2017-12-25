/*
 * XStringTranscoderT.cpp
 *
 *  Created on: 2017年6月29日
 *      Author: silly
 */

#if defined(_MSC_VER)
#include <windows.h>
#endif 

#include "MXStringTranscoder.h"

#include <vector>
#include <string>
#include <iconv/iconv.h>


//
namespace MXString
{

//----------------------------------------------------------------------------//
class IconvHelper
{
public:
    //------------------------------------------------------------------------//
    IconvHelper(const std::string& tocode, const std::string& fromcode) :
        d_fromCode(fromcode),
        d_toCode(tocode)
    {
    	d_cd = iconv_open(d_toCode.c_str(), d_fromCode.c_str());
        if (d_cd == reinterpret_cast<iconv_t>(-1))
		{
            MXSTRING_THROW("Failed to create conversion descriptor from \"" + d_fromCode + "\" to \"" + d_toCode + "\".");
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

        MXSTRING_THROW("Failed to convert from \"" + d_fromCode + "\" to \"" + d_toCode + "\": " + reason);
    }

private:
	std::string d_fromCode;
    std::string d_toCode;
    iconv_t		d_cd;
};

//----------------------------------------------------------------------------//
// Helper to use iconv to perform some transcode operation.
template<typename T1, typename T2>
static T1* iconvTranscode(IconvHelper& ich, const T2* in_buf, size_t in_len)
{
    // Handle empty strings
    if (in_len == 0)
    {
        T1* ret_buff = MXSTRING_NEW_ARRAY_PT(T1, 1);
        ret_buff[0] = 0;
        return ret_buff;
    }

    std::vector<T1> out_vec;
    out_vec.resize(in_len);
    
	in_len = in_len * sizeof(T2);
	size_t out_count = 0;

    while (true)
    {
        char* out_buf = reinterpret_cast<char*>(&out_vec[out_count]);
        const size_t start_out_bytes_left = (out_vec.size() - out_count) * sizeof(T1);
        size_t out_bytes_left = start_out_bytes_left;

        const size_t result = ich.iconv((const char**)&in_buf, &in_len, &out_buf, &out_bytes_left);

        out_count += (start_out_bytes_left - out_bytes_left) / sizeof(T1);

        if (result != static_cast<size_t>(-1))
        {
            T1* ret_buff = MXSTRING_NEW_ARRAY_PT(T1, out_count + 1);
            memcpy(ret_buff, &out_vec[0], out_count * sizeof(T1));
            ret_buff[out_count] = 0;
            return ret_buff;
        }

        if (errno != E2BIG)
        { 
			break;
		}

        out_vec.resize(out_vec.size() + 8); // this is some arbitrary number
    }

#if defined(DEBUG) || defined(_DEBUG)
    ich.throwErrorException(errno);
#endif

    // this is there mostly to silence compiler warnings, this code should
    // never be executed
    return NULL;
}

//----------------------------------------------------------------------------//
// Helper tp return length of zero terminated string of Ts
template<typename Ty>
static size_t getStringLength(const Ty* buffer)
{
    const Ty* b = buffer;
    while (*b++);

    return b - buffer - 1;
}

//----------------------------------------------------------------------------//
// Helper to correctly delete a buffer returned from iconvTranscode
template<typename T>
static void deleteTranscodeBuffer(T* buffer)
{
    MXSTRING_DELETE_ARRAY_PT(buffer, T, getStringLength(buffer) + 1);
}

//----------------------------------------------------------------------------//
// Helper to transcode a buffer and return a string class built from it.
template<class String_T, typename T1, typename T2>
static String_T iconvTranscodeT(IconvHelper& ich, const T2* in_buf, size_t in_len)
{
    T1* tmp = iconvTranscode<T1, T2>(ich, in_buf, in_len);
    String_T result(tmp);
    deleteTranscodeBuffer(tmp);
    return result;
}

IconvStringTranscoder::IconvStringTranscoder()
{
}

//----------------------------------------------------------------------------//
//UTF8->UTF16->ANSI
char* IconvStringTranscoder::stringToANSI(StringX& input) const
{
	const utf8* data	= input.stringU8().data();
	int			length	= input.stringU8().length();

#if defined(_MSC_VER)
	int lengthW = MultiByteToWideChar(CP_UTF8, 0, (const char*)data, length, NULL, 0);
	utf16* dataW = MXSTRING_NEW_ARRAY_PT(utf16, lengthW + 1);
	MultiByteToWideChar(CP_UTF8, 0, (const char*)data, length, (wchar_t*)dataW, lengthW);
	int lengthA = WideCharToMultiByte(CP_ACP, 0, (wchar_t*)dataW, lengthW, NULL, 0, NULL, NULL);
	char* result = MXSTRING_NEW_ARRAY_PT(char, lengthA + 1);
	WideCharToMultiByte(CP_ACP, 0, (wchar_t*)dataW, lengthW, result, lengthA, NULL, NULL);
	result[lengthA] = (char)0;
	MXSTRING_DELETE_ARRAY_PT(dataW, utf16, lengthW+1);
#else
	utf8* result = MXSTRING_NEW_ARRAY_PT(utf8, length + 1);
	input.stringU8().copy(result, length);
	result[length] = (utf8)(0);
#endif
	return (char*)result;
}

//UTF32->UTF16
utf16* IconvStringTranscoder::stringToUTF16(StringX& input) const
{
	IconvHelper ich(UTF16PE(), UTF32PE());
	return iconvTranscode<utf16, utf32>(ich, input.data(), input.length());
}


//----------------------------------------------------------------------------//
//ANSI->UTF16->UTF8->UTF32
StringX		IconvStringTranscoder::stringFromANSI(const char* input, StringX::size_type len) const
{
	const char* data = input;
	int length = len != StringX::npos ? len : getStringLength(data);

#if defined(_MSC_VER)

	int lengthW = MultiByteToWideChar(CP_ACP, 0, (const char*)data, length, NULL, 0);
	utf16* dataW = MXSTRING_NEW_ARRAY_PT(utf16, lengthW + 1);
	MultiByteToWideChar(CP_ACP, 0, (const char*)data, length, (wchar_t*)dataW, lengthW);
	int lengthA = WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)dataW, lengthW, NULL, 0, NULL, NULL);
	char* resultA = MXSTRING_NEW_ARRAY_PT(char, lengthA + 1);
	WideCharToMultiByte(CP_UTF8, 0, (wchar_t*)dataW, lengthW, resultA, lengthA, NULL, NULL);
	resultA[lengthA] = (char)0;
	String result((utf8*)resultA);
	MXSTRING_DELETE_ARRAY_PT(dataW, utf16, lengthW+1);
	MXSTRING_DELETE_ARRAY_PT(resultA, utf8, lengthA+1);

#else
	String result((const utf8*)data, length);
#endif
	return result;
}

//
StringX IconvStringTranscoder::stringFromUTF16(const utf16* input, StringX::size_type len) const
{
	int length = len != StringX::npos ? len : getStringLength(input);

#if defined(_MSC_VER)
	IconvHelper ich(UTF32PE(), UTF16PE());
	return iconvTranscodeT<String, utf32, utf16>(ich, input, length);
#else
	IconvHelper ich(UTF32PE(), UTF16PE());
    return iconvTranscodeT<String, utf32, utf16>(ich, input, length);
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
