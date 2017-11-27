/*
 * MXStringT.h
 *
 *  Created on: 2017年6月29日
 *      Author: silly
 */

#ifndef __MXSTRINGT_H__
#define __MXSTRINGT_H__

#include <algorithm>
#include <cstring>

#include "MXString.h"


//
namespace MXString
{

//
#define __MXSTRING_UNICODE_X(X)			(L##X)
#define __MXSTRING_UTF8_S(X)			(MXString::StringU8((const MXString::utf8*)X))
#define __MXSTRING_UTF8_SL(X,V)			(MXString::StringU8((const MXString::utf8*)X, V))
#define __MXSTRING_STRING_X(X)			(MXString::StringX(__MXSTRING_UNICODE_X(X)))

//
class IconvStringTranscoder;

/*-----------------------------------------------------
StringUX Class
	: Base utf32 data format
	: Extand utf8 data format
-------------------------------------------------------*/

//
class StringX : public String
{
	//设置为静态,共用转换函数.
	static IconvStringTranscoder*	d_transcoder;
public:
	StringX();
	virtual ~StringX();

	//////////////////////////////////////////////////////////////////////////
	// Construction via XStringT::StringU8
	//////////////////////////////////////////////////////////////////////////
	StringX(const String& str)
	{
		assign(str);
	}

	StringX(const String& str, size_type str_idx, size_type str_num = String::npos)
	{
		assign(str, str_idx, str_num);
	}

	StringX(const std::string& str)
	{
		assignUA(str);
	}

	StringX(const std::string& str, size_type str_idx, size_type str_num = String::npos)
	{
		assignUA(str, str_idx, str_num);
	}

	StringX(const std::wstring& str)
	{
		assignUA(str);
	}

	StringX(const std::wstring& str, size_type str_idx, size_type str_num = String::npos)
	{
		assignUA(str, str_idx, str_num);
	}

	StringX(const char* chars)
	{
		assignUA(chars);
	}

	StringX(const char* chars, size_type chars_len)
	{
		assignUA(chars, 0, chars_len);
	}

	StringX(const wchar_t* chars)
	{
		assignUA(chars);
	}

	StringX(const wchar_t* chars, size_type chars_len)
	{
		assignUA(chars, 0, chars_len);
	}

	__inline const char* c_str_ua()
	{
		this->build_ansi_data_ua();
		return (const char*)d_stringUA.c_str();
	}

	__inline const wchar_t* c_str_uw()
	{
		this->build_wide_data_ua();
		return (const wchar_t*)d_stringUW.c_str();
	}

	__inline const StringUA&	stringUA()
	{
		this->build_ansi_data_ua();
		return d_stringUA; 
	}

#if defined(_MSC_VER)
	__inline const StringU16&	stringUW()
#else
	__inline const StringU32&	stringUW()
#endif
	{
		this->build_wide_data_ua();
		return d_stringUW;
	}


public:
	//
	__inline StringX&	operator=(const StringUA& str)
	{
		return assignUA(str);
	}

	__inline StringX&	operator=(const std::string& str)
	{
		return assignUA(str);
	}

	__inline StringX&	operator=(const std::wstring& str)
	{
		return assignUA(str);
	}

	__inline StringX&	operator=(const char* chars)
	{
		return assignUA(chars, 0, strlen(chars));
	}

	__inline StringX&	operator=(const wchar_t* chars)
	{
		return assignUA(chars, 0, wcslen(chars));
	}

	//
	__inline StringX&	operator+=(const StringUA& str)
	{
		return appendUA(str);
	}

	__inline StringX&	operator+=(const std::string& str)
	{
		return appendUA(str);
	}

	__inline StringX&	operator+=(const std::wstring& str)
	{
		return appendUA(str);
	}

	__inline StringX&	operator+=(const char* chars)
	{
		return appendUA(chars, 0, strlen(chars));
	}

	__inline StringX&	operator+=(const wchar_t* chars)
	{
		return appendUA(chars, 0, wcslen(chars));
	}

	//
	__inline StringX	operator+(const StringUA& str)
	{
		return appendUA(str);
	}

	__inline StringX	operator+(const std::string& str)
	{
		return appendUA(str);
	}

	__inline StringX	operator+(const std::wstring& str)
	{
		return appendUA(str);
	}

	__inline StringX	operator+(const char* chars)
	{
		return appendUA(chars);
	}

	__inline StringX	operator+(const wchar_t* chars)
	{
		return appendUA(chars);
	}

public:
	//////////////////////////////////////////////////////////////////////////
	// Assignment Functions
	//////////////////////////////////////////////////////////////////////////
	StringX&		assignUA(const StringUA& str, size_type str_idx = 0, size_type str_num = StringX::npos)
	{
		d_stringUA.assign(str, str_idx, str_num != StringX::npos ? str_num : str.length());
		this->build_utf32_data_ua();
		return *this;
	}
		
	StringX&		assignUA(const std::string& str, size_type str_idx = 0, size_type str_num = StringX::npos)
	{
		d_stringUA.assign(str.data(), str_idx, str_num != StringX::npos ? str_num : str.length());
		this->build_utf32_data_ua();
		return *this;
	}

	StringX&		assignUA(const char* str, size_type str_idx = 0, size_type str_num = StringX::npos)
	{
		d_stringUA.assign(str, str_idx, str_num != String::npos ? str_num : strlen(str));
		this->build_utf32_data_ua();
		return *this;
	}

	StringX&		assignUA(const wchar_t* chars, size_type chars_idx = 0, size_type chars_num = StringX::npos);
	StringX&		assignUA(const std::wstring& str, size_type str_idx = 0, size_type str_num = StringX::npos);

	//////////////////////////////////////////////////////////////////////////
	// Appending Functions
	//////////////////////////////////////////////////////////////////////////
	StringX&		appendUA(const StringUA& str, size_type str_idx = 0, size_type str_num = StringX::npos)
	{
		d_stringUA.append(str, str_idx, str_num);
		this->build_utf32_data_ua();
		return *this;
	}

	StringX&		appendUA(const std::string& str, size_type str_idx = 0, size_type str_num = StringX::npos)
	{
		d_stringUA.append(str.data(), str_idx, str_num);
		this->build_utf32_data_ua();
		return *this;
	}

	StringX&		appendUA(const char* chars, size_type chars_idx = 0, size_type chars_len = StringX::npos)
	{
		d_stringUA.append(chars, chars_idx, chars_len);
		this->build_utf32_data_ua();
		return *this;
	}

	StringX&		appendUA(const wchar_t* chars, size_type chars_idx = 0, size_type chars_num = StringX::npos);
	StringX&		appendUA(const std::wstring& str, size_type str_idx = 0, size_type str_num = StringX::npos);

protected:
	//
	void			build_ansi_data_ua();
	void			build_wide_data_ua();
	void			build_utf32_data_ua();

private:
	StringUA		d_stringUA;
#if defined(_MSC_VER)
	StringU16		d_stringUW;
#else
	StringU32		d_stringUW;
#endif
};



};//namespace MXString

#endif /* __MXSTRINGT_H__ */
