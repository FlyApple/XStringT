/*
 * MXString.h
 *
 *  Created on: 2017年6月29日
 *      Author: silly
 */

#ifndef __MXSTRING_H__
#define __MXSTRING_H__

#include <cstddef>
#include <stdexcept>
#include <iostream>

#include "MXStringBase.h"

//
namespace MXString
{
/*-----------------------------------------------------
String Class
	: Base utf32 data format
	: Extand utf8 data format
-------------------------------------------------------*/

//
class String : public StringU32
{
public:
	String(){ }
	virtual ~String(){ }

	//////////////////////////////////////////////////////////////////////////
	// Construction via XStringT::StringU8
	//////////////////////////////////////////////////////////////////////////
	String(const StringU8& str)
	{
		assignU8(str);
	}

	String(const StringU8& str, size_type str_idx, size_type str_num = String::npos)
	{
		assignU8(str, str_idx, str_num);
	}

	String(const utf8* utf8_str)
	{
		assignU8(utf8_str);
	}

	String(const utf8* utf8_str, size_type chars_len)
	{
		assignU8(utf8_str, 0, chars_len);
	}

	String(const StringU32& str)
	{
		assign(str);
	}

	String(const StringU32& str, size_type str_idx, size_type str_num = String::npos)
	{
		assign(str, str_idx, str_num);
	}

	String(const utf32* utf32_str)
	{
		assign(utf32_str);
	}

	String(const utf32* utf32_str, size_type chars_len)
	{
		assign(utf32_str, 0, chars_len);
	}

	__inline const StringU8::value_type* c_str_u8()
	{
		this->build_utf8_data();
		return d_stringU8.c_str();
	}

	__inline const StringU8&	stringU8()
	{
		this->build_utf8_data();
		return d_stringU8; 
	}

public:
	//
	__inline String&	operator=(const String& str)
	{
		return (String&)assign(str);
	}

	__inline String&	operator=(const StringU8& str)
	{
		return assignU8(str);
	}

	__inline String&	operator=(const utf8* utf8_str)
	{
		return assignU8(utf8_str, 0, total_length(utf8_str));
	}

	__inline String&	operator=(const StringU32& str)
	{
		return (String&)assign(str);
	}

	__inline String&	operator=(const utf32* utf32_str)
	{
		return (String&)assign(utf32_str, 0, total_length(utf32_str));
	}

	//
	__inline String&	operator+=(const String& str)
	{
		return (String&)append(str); //强制转换
	}

	__inline String&	operator+=(const StringU8& str)
	{
		return appendU8(str);
	}

	__inline String&	operator+=(const utf8* utf8_str)
	{
		return appendU8(utf8_str, 0, total_length(utf8_str));
	}

	__inline String&	operator+=(const StringU32& str)
	{
		return (String&)append(str);
	}

	__inline String&	operator+=(const utf32* utf32_str)
	{
		return (String&)append(utf32_str, 0, total_length(utf32_str));
	}

	//
	__inline String	operator+(const String& str)
	{
		return (String&)append(str); //强制转换
	}

	__inline String	operator+(const StringU8& str)
	{
		return appendU8(str);
	}

	__inline String	operator+(const utf8* utf8_str)
	{
		return appendU8(utf8_str);
	}

	__inline String	operator+(const StringU32& str)
	{
		return (String&)append(str);
	}

	__inline String	operator+(const utf32* utf32_str)
	{
		return (String&)append(utf32_str);
	}

public:
	//////////////////////////////////////////////////////////////////////////
	// Assignment Functions
	//////////////////////////////////////////////////////////////////////////
	String&		assignU8(const StringU8& str, size_type str_idx = 0, size_type str_num = String::npos)
	{
		d_stringU8.assign(str, str_idx, str_num != String::npos ? str_num : str.length());
		this->build_utf32_data();
		return *this;
	}
		
	String&		assignU8(const utf8* utf8_str, size_type str_idx = 0, size_type str_num = String::npos)
	{
		d_stringU8.assign(utf8_str, str_idx, str_num != String::npos ? str_num : this->total_length(utf8_str));
		this->build_utf32_data();
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	// Appending Functions
	//////////////////////////////////////////////////////////////////////////
	String&		appendU8(const StringU8& str, size_type str_idx = 0, size_type str_num = String::npos)
	{
		d_stringU8.append(str, str_idx, str_num);
		this->build_utf32_data();
		return *this;
	}

	String&		appendU8(const utf8* utf8_str, size_type utf8_str_idx = 0, size_type utf8_str_len = String::npos)
	{
		d_stringU8.append(utf8_str, utf8_str_idx, utf8_str_len);
		this->build_utf32_data();
		return *this;
	}

protected:
	//
	__inline void		build_utf8_data()
	{
		size_type encode_size = encoded_size(this->data(), this->length());
		this->d_stringU8.resize(encode_size);
		encode(this->data(), (utf8*)d_stringU8.data(), encode_size, this->length());
	}
	__inline void		build_utf32_data()
	{
		size_type encode_size = encoded_size(this->d_stringU8.data(), this->d_stringU8.length());
		this->resize(encode_size);
		encode(this->d_stringU8.data(), (utf32*)this->data(), encode_size, this->d_stringU8.length());	
	}

	__inline size_type	total_length(const utf8* utf8_str) const
	{
		size_type cnt = 0; while (*utf8_str++){ cnt++; }
		return cnt;
	}
	__inline size_type	total_length(const utf32* utf32_str) const
	{
		size_type cnt = 0; while (*utf32_str++){ cnt++; }
		return cnt;
	}

	size_type			encoded_size(utf32 code_point) const;
	size_type			encoded_size(const utf32* data, size_type length) const;
	size_type			encoded_size(const utf8* data, size_type length) const;

	size_type			encode(const utf32* src, utf8* dest, size_type dest_len, size_type src_len = 0) const;
	size_type			encode(const utf8* src, utf32* dest, size_type dest_len, size_type src_len = 0) const;

private:
	StringU8	d_stringU8;
};


};//namespace MXString

#endif /* __MXSTRING_H__ */
