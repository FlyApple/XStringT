/*
 * XStringSwapperT.h
 *
 *  Created on: 2017年6月29日
 *      Author: silly
 */

#ifndef __XSTRINGT_SWAPPER_H__
#define __XSTRINGT_SWAPPER_H__

#include <algorithm>
#include <cstring>

#include "XStringT.h"
#include "XStringTranscoderT.h"

//
namespace XStringT
{

//
#define __XSTRINGT_STRING_UTFX(X)			(L##X)
#define __XSTRINGT_STRING_UTF8A(X)			(XStringT::StringSwapperT<std::string>(__XSTRINGT_STRING_UTFX(X)))
#define __XSTRINGT_STRING_UTF8W(X)			(XStringT::StringSwapperT<std::wstring>(__XSTRINGT_STRING_UTFX(X)))

/*-----------------------------------------------------
StringSwapper Class
-------------------------------------------------------*/
class StringSwapper
{
public:
	StringSwapper(){}
	virtual ~StringSwapper() {}    
};

/*-----------------------------------------------------
StringSwapperT Class
-------------------------------------------------------*/
//! 
template<typename _XTD>
class StringSwapperT : public StringSwapper
{
public:
	StringSwapperT(){ };
	virtual ~StringSwapperT() { }; 

	StringSwapperT(const StringSwapperT& sst) 
	{ 
		this->assign(sst);
	};

	StringSwapperT(const StringAU8& str) 
	{ 
		this->assign(str);
	};

	StringSwapperT(const char* str, StringBase::size_type len = StringBase::npos) 
	{ 
		assign(str, len);
	};
	StringSwapperT(const wchar_t* str, StringBase::size_type len = StringBase::npos) 
	{ 
		assign(str, len);
	};

public:
	StringSwapperT&	operator=(const StringSwapperT& sst)
	{
		return this->assign(sst);
	}

	StringSwapperT&	operator=(const StringAU8& str)
	{
		return this->assign(str);
	}

	StringSwapperT&	operator=(const char* str)
	{
		return this->assign(str);
	}

	StringSwapperT&	operator=(const wchar_t* str)
	{
		return this->assign(str);
	}

	StringSwapperT&	operator+=(const StringSwapperT& sst)
	{
		return append(sst);
	}
	
	StringSwapperT&	operator+=(const StringAU8& str)
	{
		return append(str);
	}

	StringSwapperT&	operator+=(const char* str)
	{
		return append(str);
	}

	StringSwapperT&	operator+=(const wchar_t* str)
	{
		return append(str);
	}

public:
	__inline const char*	c_str()		const{ return d_stringU8.c_str(); }
	__inline std::string	astr()		const
	{ 
#if defined(_MSC_VER)
		const char* v = d_transcoder.stringToANSI(d_stringU8);
		std::string		r(v); d_transcoder.deleteANSIBuffer(v);
#else
		std::string		r(d_stringU8.c_str());
#endif
		return r;
	}
	__inline std::wstring	wstr()		const
	{ 
#if defined(_MSC_VER)
		const utf16* v = d_transcoder.stringToUTF16(d_stringU8);
		std::wstring	r((const wchar_t*)v); d_transcoder.deleteUTF16Buffer(v);
#else
		const utf32* v = d_transcoder.stringToUTF32(d_stringU8);
		std::wstring	r((const wchar_t*)v); d_transcoder.deleteUTF32Buffer(v);
#endif
		return r;
	}

public:
    //////////////////////////////////////////////////////////////////////////
	// Assignment Functions
	//////////////////////////////////////////////////////////////////////////
	StringSwapperT&	assign(const StringSwapperT& sst)
	{
		d_stringU8 = sst.d_stringU8;
		return *this;
	}
	
	StringSwapperT&	assign(const StringAU8& str, StringBase::size_type str_idx = 0, StringBase::size_type str_num = StringBase::npos)
	{
		d_stringU8.assign(str, str_idx, str_num);
		return *this;
	}

	StringSwapperT&	assign(const char* str, StringBase::size_type len = StringBase::npos)
	{
		if(len == StringBase::npos){ len = strlen(str); }
		d_stringU8 = d_transcoder.stringFromANSI(str, len);
		return *this;
	}
	StringSwapperT&	assign(const wchar_t* str, StringBase::size_type len = StringBase::npos)
	{
		if(len == StringBase::npos){ len = wcslen(str); }
#if defined(_MSC_VER)
		d_stringU8 = d_transcoder.stringFromUTF16((const utf16*)str, len);
#else
		d_stringU8 = d_transcoder.stringFromUTF32((const utf32*)str, len);
#endif
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	// Appending Functions
	//////////////////////////////////////////////////////////////////////////
	StringSwapperT&	append(const StringSwapperT& sst)
	{
		d_stringU8.append(sst.d_stringU8);
		return *this;
	}

	StringSwapperT&	append(const StringAU8& str, StringBase::size_type str_idx = 0, StringBase::size_type str_num = StringBase::npos)
	{
		d_stringU8.append(str, str_idx, str_num);
		return *this;
	}

	StringSwapperT& append(const char* str, StringBase::size_type len = StringBase::npos)
	{
		if(len == StringBase::npos){ len = strlen(str); }
		d_stringU8 += d_transcoder.stringFromANSI(str, len);	
		return *this;
	}

	StringSwapperT& append(const wchar_t* str, StringBase::size_type len = StringBase::npos)
	{
		if(len == StringBase::npos){ len = wcslen(str); }
#if defined(_MSC_VER)
		d_stringU8 += d_transcoder.stringFromUTF16((const utf16*)str, len);
#else
		d_stringU8 += d_transcoder.stringFromUTF32((const utf32*)str, len);
#endif
		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	// Comparisons
	//////////////////////////////////////////////////////////////////////////
	int		compare(const StringSwapperT& sst) const
	{
		return d_stringU8.compare(sst.d_stringU8);
	}
	
	int		compare(const StringAU8& str) const
	{
		return d_stringU8.compare(str);
	}

private:
	IconvStringTranscoder		d_transcoder;
	StringAU8					d_stringU8;
};

//
template<typename _XTD>
__inline StringSwapperT<_XTD>	operator+(const StringSwapperT<_XTD>& sst1, const StringSwapperT<_XTD>& sst2)
{
	StringSwapperT<_XTD>	tmp(sst1); tmp += sst2;
	return tmp;
}

template<typename _XTD>
__inline StringSwapperT<_XTD>	operator+(const StringSwapperT<_XTD>& sst, const char* str)
{
	StringSwapperT<_XTD>	tmp(sst); tmp += str;
	return tmp;
}
template<typename _XTD>
__inline StringSwapperT<_XTD>	operator+(const char* str, const StringSwapperT<_XTD>& sst)
{
	StringSwapperT<_XTD>	tmp(str); tmp += sst;
	return tmp;
}

template<typename _XTD>
__inline StringSwapperT<_XTD>	operator+(const StringSwapperT<_XTD>& sst, const wchar_t* str)
{
	StringSwapperT<_XTD>	tmp(sst); tmp += str;
	return tmp;
}
template<typename _XTD>
__inline StringSwapperT<_XTD>	operator+(const wchar_t* str, const StringSwapperT<_XTD>& sst)
{
	StringSwapperT<_XTD>	tmp(str); tmp += sst;
	return tmp;
}

};//namespace XStringT

#endif /* __XSTRINGT_SWAPPER_H__ */
