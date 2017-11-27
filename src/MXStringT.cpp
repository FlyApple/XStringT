/*
 * MXStringT.cpp
 *
 *  Created on: 2017年6月29日
 *      Author: silly
 */

#include "MXStringT.h"
#include "MXStringTranscoder.h"

//
namespace MXString
{

	//
	IconvStringTranscoder*	StringX::d_transcoder = new IconvStringTranscoder();

	//
	StringX::StringX()
	{

	}
	StringX::~StringX()
	{

	}

	StringX&	StringX::assignUA(const wchar_t* chars, size_type chars_idx, size_type chars_num)
	{
		chars_num = chars_num != StringX::npos ? chars_num : wcslen(chars);

		std::wstring	str(chars, chars_idx, chars_num);
		return this->assignUA(str);
	}

	StringX&	StringX::assignUA(const std::wstring& str, size_type str_idx, size_type str_num)
	{
		str_num = str_num != StringX::npos ? str_num : str.length();

#if defined(_MSC_VER)
		StringX temp = d_transcoder->stringFromUTF16((utf16*)str.substr(str_idx, str_num).data());
#else
		StringX temp((utf32*)chars);
#endif
		this->assign(temp);
		return *this;
	}

	StringX&	StringX::appendUA(const wchar_t* chars, size_type chars_idx, size_type chars_num)
	{
		chars_num = chars_num != StringX::npos ? chars_num : wcslen(chars);

		std::wstring	str(chars, chars_idx, chars_num);
		return this->appendUA(str);
	}

	StringX&	StringX::appendUA(const std::wstring& str, size_type str_idx, size_type str_num)
	{
		str_num = str_num != StringX::npos ? str_num : str.length();

#if defined(_MSC_VER)
		StringX temp = d_transcoder->stringFromUTF16((utf16*)str.substr(str_idx, str_num).data());
#else
		StringX temp((utf32*)chars);
#endif
		this->append(temp);
		return *this;
	}

	void		StringX::build_ansi_data_ua()
	{
		const char* v = d_transcoder->stringToANSI(*this);
		d_stringUA.assign(v);
		d_transcoder->deleteANSIBuffer(v);
	}

	void		StringX::build_wide_data_ua()
	{
#if defined(_MSC_VER)
		const utf16* v = d_transcoder->stringToUTF16(*this);
#else
		const utf32* v = this->data();
#endif

		d_stringUW.assign(v);

#if defined(_MSC_VER)
		d_transcoder->deleteUTF16Buffer(v);
#endif
	}

	void		StringX::build_utf32_data_ua()
	{
		StringX temp = d_transcoder->stringFromANSI(d_stringUA.data());
		this->assign(temp);
	}
}
