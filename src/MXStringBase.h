/*
 * MXStringBase.h
 *
 *  Created on: 2017年6月29日
 *      Author: silly
 */

#ifndef __MXSTRINGBASE_H__
#define __MXSTRINGBASE_H__

#include <algorithm>
#include <string>
#include <stdexcept>
#include <iostream>

//
namespace MXString
{
/*-----------------------------------------------------
Basic Types
-------------------------------------------------------*/
typedef		unsigned char	utf8;
typedef		unsigned short	utf16; 
typedef		unsigned int	utf32;

//
#ifndef MXSTRING_MAX
#define MXSTRING_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MXSTRING_MIN
#define MXSTRING_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

//
#ifndef MXSTRING_NEW_PT
#   define MXSTRING_NEW_PT(T) 						new T
#endif
#ifndef MXSTRING_DELETE_PT
#   define MXSTRING_DELETE_PT(ptr, T) 				delete ptr
#endif
#ifndef MXSTRING_NEW_ARRAY_PT
#   define MXSTRING_NEW_ARRAY_PT(T, count) 			new T[count]
#endif
#ifndef MXSTRING_DELETE_ARRAY_PT
#   define MXSTRING_DELETE_ARRAY_PT(ptr, T, count) 	delete [] ptr
#endif

#ifndef MXSTRING_THROW
#   define MXSTRING_THROW(e) throw e
#endif
#ifndef MXSTRING_RETHROW
#   define MXSTRING_RETHROW throw
#endif

/*-----------------------------------------------------
StringBase Class
-------------------------------------------------------*/

//
class StringBase
{
public:
	StringBase(){ }
	virtual ~StringBase(){ }
};

//
template <typename _TU>
class StringBaseT : public StringBase, public std::basic_string<_TU>
{
public:
	StringBaseT(){ }
	virtual ~StringBaseT(){ }
};

//ANSI
class StringUA : public StringBaseT<char>
{
};

//UTF-8
class StringU8 : public StringBaseT<utf8>
{
};

//UTF-16
class StringU16 : public StringBaseT<utf16>
{
};

//UTF-32
class StringU32 : public StringBaseT<utf32>
{
};

};//namespace MXString

#endif /* __MXSTRINGBASE_H__ */
