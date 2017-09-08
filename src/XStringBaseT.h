/*
 * XStringBaseT.h
 *
 *  Created on: 2017年6月29日
 *      Author: silly
 */

#ifndef __XSTRINGBASET_H__
#define __XSTRINGBASET_H__

#include <algorithm>
#include <string>
#include <stdexcept>
#include <iostream>

//
namespace XStringT
{
/*-----------------------------------------------------
Basic Types
-------------------------------------------------------*/
typedef		unsigned char	utf8;
typedef		unsigned short	utf16; 
typedef		unsigned int	utf32;

//
#ifndef XSTRINGT_MAX
#define XSTRINGT_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef XSTRINGT_MIN
#define XSTRINGT_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

//
#ifndef XSTRINGT_NEW_PT
#   define XSTRINGT_NEW_PT(T) 						new T
#endif
#ifndef XSTRINGT_DELETE_PT
#   define XSTRINGT_DELETE_PT(ptr, T) 				delete ptr
#endif
#ifndef XSTRINGT_NEW_ARRAY_PT
#   define XSTRINGT_NEW_ARRAY_PT(T, count) 			new T[count]
#endif
#ifndef XSTRINGT_DELETE_ARRAY_PT
#   define XSTRINGT_DELETE_ARRAY_PT(ptr, T, count) 	delete [] ptr
#endif

#ifndef XSTRINGT_THROW
#   define XSTRINGT_THROW(e) throw e
#endif
#ifndef XSTRINGT_RETHROW
#   define XSTRINGT_RETHROW throw
#endif

/*-----------------------------------------------------
StringBase Class
-------------------------------------------------------*/
template <typename _TU>
class StringBase : public std::basic_string<_TU>
{
public:
	StringBase(){ }
	virtual ~StringBase(){ }
};

template <class _TC, typename _TU>
class StringBaseT : public StringBase<_TU>
{
public:
	friend _TC;
};

};//namespace XStringT

#endif /* __XSTRINGBASET_H__ */
