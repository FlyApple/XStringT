/*
 * XStringT.h
 *
 *  Created on: 2017年6月29日
 *      Author: silly
 */

#ifndef __XSTRINGT_H__
#define __XSTRINGT_H__

#include <cstddef>
#include <stdexcept>
#include <iostream>

//
namespace XStringT
{
/*-----------------------------------------------------
Basic Types
-------------------------------------------------------*/
typedef		unsigned char	utf8;
//typedef		unsigned short	utf16;  // removed typedef to prevent usage, as utf16 is not supported (yet)
typedef		unsigned int	utf32;

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

//
#define XSTRINGT_STRING_QUICKBUFF_SIZE 		(64)

/*-----------------------------------------------------
String Class
-------------------------------------------------------*/
class String
{
public:
	/*************************
		Integral Types
	**************************/
	typedef		utf32			value_type;					//!< Basic 'code point' type used for String (utf32)
	typedef		size_t			size_type;					//!< Unsigned type used for size values and indices
	typedef		std::ptrdiff_t	difference_type;			//!< Signed type used for differences
	typedef		utf32&			reference;					//!< Type used for utf32 code point references
	typedef		const utf32&	const_reference;			//!< Type used for constant utf32 code point references
	typedef		utf32*			pointer;					//!< Type used for utf32 code point pointers
	typedef		const utf32*	const_pointer;				//!< Type used for constant utf32 code point pointers

	static const size_type		npos;						//!< Value used to represent 'not found' conditions and 'all code points' etc.

public:
	String();
	virtual ~String();

	//////////////////////////////////////////////////////////////////////////
	// Construction via CEGUI::String
	//////////////////////////////////////////////////////////////////////////
	/*!
	\brief
		Copy constructor - Creates a new string with the same value as \a str

	\param str
		String object used to initialise the newly created string

	\return
		Nothing
	*/
	String(const String& str)
	{
		init();
		assign(str);
	}

	/*!
	\brief
		Constructs a new string initialised with code points from another String object.

	\param str
		String object used to initialise the newly created string

	\param str_idx
		Starting code-point of \a str to be used when initialising the new String

	\param str_num
		Maximum number of code points from \a str that are to be assigned to the new String

	\return
		Nothing
	*/
	String(const String& str, size_type str_idx, size_type str_num = npos)
	{
		init();
		assign(str, str_idx, str_num);
	}

	//////////////////////////////////////////////////////////////////////////
	// Construction via std::string
	//////////////////////////////////////////////////////////////////////////
	/*!
	\brief
		Constructs a new string and initialises it using the std::string std_str

	\param std_str
		The std::string object that is to be used to initialise the new String object.

	\note
		The characters of \a std_str are taken to be unencoded data which represent Unicode code points 0x00..0xFF.  No translation of
		the provided data will occur.

	\return
		Nothing

	\exception std::length_error	Thrown if resulting String object would be too big.
	*/
	String(const std::string& std_str)
	{
		init();
		assign(std_str);
	}
	/*!
	\brief
		Constructs a new string initialised with characters from the given std::string object.

	\param std_str
		std::string object used to initialise the newly created string

	\param str_idx
		Starting character of \a std_str to be used when initialising the new String

	\note
		The characters of \a std_str are taken to be unencoded data which represent Unicode code points 0x00..0xFF.  No translation of
		the provided data will occur.

	\param str_num
		Maximum number of characters from \a std_str that are to be assigned to the new String

	\return
		Nothing

	\exception std::length_error	Thrown if resulting String object would be too big.
	*/
	String(const std::string& std_str, size_type str_idx, size_type str_num = npos)
	{
		init();
		assign(std_str, str_idx, str_num);
	}

	//////////////////////////////////////////////////////////////////////////
	// Construction via UTF-8 stream (for straight ASCII use, only codes 0x00 - 0x7f are valid)
	//////////////////////////////////////////////////////////////////////////
	/*!
	\brief
		Constructs a new String object and initialise it using the provided utf8 encoded string buffer.

	\param utf8_str
		Pointer to a buffer containing a null-terminated Unicode string encoded as utf8 data.

	\note
		A basic string literal (cast to utf8*) can be passed to this function, provided that the string is
		comprised only of code points 0x00..0x7f.  The use of extended ASCII characters (with values >0x7f)
		would result in incorrect behaviour as the String will attempt to 'decode' the data, with unpredictable
		results.

	\return
		Nothing

	\exception std::length_error	Thrown if resulting String object would be too big.
	*/
	String(const utf8* utf8_str)
	{
		init();
		assign(utf8_str);
	}

	/*!
	\brief
		Constructs a new String object and initialise it using the provided utf8 encoded string buffer.

		A basic string literal (cast to utf8*) can be passed to this function, provided that the string is
		comprised only of code points 0x00..0x7f.  The use of extended ASCII characters (with values >0x7f)
		would result in incorrect behaviour as the String will attempt to 'decode' the data, with unpredictable
		results.

	\param utf8_str
		Pointer to a buffer containing Unicode string data encoded as utf8.

	\note
		A basic string literal (cast to utf8*) can be passed to this function, provided that the string is
		comprised only of code points 0x00..0x7f.  The use of extended ASCII characters (with values >0x7f)
		would result in incorrect behaviour as the String will attempt to 'decode' the data, with unpredictable
		results.

	\param chars_len
		Length of the provided utf8 string in code units (not code-points).

	\return
		Nothing

	\exception std::length_error	Thrown if resulting String object would be too big.
	*/
	String(const utf8* utf8_str, size_type chars_len)
	{
		init();
		assign(utf8_str, chars_len);
	}

	//////////////////////////////////////////////////////////////////////////
	// Construction via c-string
	//////////////////////////////////////////////////////////////////////////
	/*!
	\brief
		Constructs a new String object and initialise it using the provided c-string.

	\param c_str
		Pointer to a c-string.

	\return
		Nothing

	\exception std::length_error	Thrown if resulting String object would be too big.
	*/
	String(const char* cstr)
	{
		init();
		assign(cstr);
	}

	/*!
	\brief
		Constructs a new String object and initialise it using characters from the provided char array.

	\param chars
		char array.

	\param chars_len
		Number of chars from the array to be used.

	\return
		Nothing

	\exception std::length_error	Thrown if resulting String object would be too big.
	*/
	String(const char* chars, size_type chars_len)
	{
		init();
		assign(chars, chars_len);
	}

public:
	/*!
	\brief
		Returns the code point at the given index.

	\param idx
		Zero based index of the code point to be returned.

	\note
		- For constant strings length()/size() provide a valid index and will access the default utf32 value.
		- For non-constant strings length()/size() is an invalid index, and acceesing (especially writing) this index could cause string corruption.

	\return
		The utf32 code point at the given index within the String.
	*/
	reference	operator[](size_type idx)
	{
		return (ptr()[idx]);
	}

	/*!
	\brief
		Returns the code point at the given index.

	\param idx
		Zero based index of the code point to be returned.

	\note
		- For constant strings length()/size() provide a valid index and will access the default utf32 value.
		- For non-constant strings length()/size() is an invalid index, and acceesing (especially writing) this index could cause string corruption.

	\return
		The utf32 code point at the given index within the String.
	*/
	value_type	operator[](size_type idx) const
	{
		return ptr()[idx];
	}

public:
	//////////////////////////////////////////////////////////////////////////
	// Size operations
	//////////////////////////////////////////////////////////////////////////
	/*!
	\brief
		Returns the size of the String in code points

	\return
		Number of code points currently in the String
	*/
	size_type	size(void) const
	{
		return d_cplength;
	}

	/*!
	\brief
		Returns the size of the String in code points

	\return
		Number of code points currently in the String
	*/
	size_type	length(void) const
	{
		return d_cplength;
	}

	/*!
	\brief
		Returns the maximum size of a String.

		Any operation that would result in a String that is larger than this value will throw the std::length_error exception.

	\return
		The maximum number of code points that a string can contain
	*/
	size_type max_size(void) const
	{
		return (((size_type)-1) / sizeof(utf32));
	}

	/*!
	\brief
		Returns true if the String is empty

	\return
		true if the String is empty, else false.
	*/
	bool	empty(void) const
	{
		return	(d_cplength == 0);
	}

	//////////////////////////////////////////////////////////////////////////
	// Capacity Operations
	//////////////////////////////////////////////////////////////////////////
	// return the number of code points the string could hold without re-allocation
	// (due to internal encoding this will always report the figure for worst-case encoding, and could even be < size()!)
	/*!
	\brief
		Return the number of code points that the String could hold before a re-allocation would be required.

	\return
		Size of the current reserve buffer.  This is the maximum number of code points the String could hold before a buffer
		re-allocation would be required
	*/
	size_type capacity(void) const
	{
		return d_reserve - 1;
	}

	/*!
	\brief
		Returns a pointer to the buffer in use.
	*/
	utf32*	ptr(void)
	{
		return (d_reserve > XSTRINGT_STRING_QUICKBUFF_SIZE) ? d_buffer : d_quickbuff;
	}

	/*!
	\brief
		Returns a pointer to the buffer in use. (const version)
	*/
	const utf32*	ptr(void) const
	{
		return (d_reserve > XSTRINGT_STRING_QUICKBUFF_SIZE) ? d_buffer : d_quickbuff;
	}

	/*!
	\brief
		Assign a sub-string of String \a str to this String

	\param str
		String object containing the string data to be assigned.

	\param str_idx
		Index of the first code point in \a str that is to be assigned

	\param str_num
		Maximum number of code points from \a str that are be be assigned

	\return
		This String after the assignment has happened

	\exception std::out_of_range	Thrown if str_idx is invalid for \a str
	*/
	String&	assign(const String& str, size_type str_idx = 0, size_type str_num = npos)
	{
		if (str.d_cplength < str_idx)
		{
			XSTRINGT_THROW(std::out_of_range("Index was out of range for XStringT::String object"));
		}

		if ((str_num == npos) || (str_num > str.d_cplength - str_idx))
		{
			str_num = str.d_cplength - str_idx;
		}

		grow(str_num);
		setlen(str_num);
		memcpy(ptr(), &str.ptr()[str_idx], str_num * sizeof(utf32));

		return *this;
	}

	/*!
	\brief
		Assign a sub-string of std::string \a std_str to this String

	\note
		The characters of \a std_str are taken to be unencoded data which represent Unicode code points 0x00..0xFF.  No translation of
		the provided data will occur.

	\param std_str
		std::string object containing the string value to be assigned.

	\param str_idx
		Index of the first character of \a std_str to be assigned

	\param str_num
		Maximum number of characters from \a std_str to be assigned

	\return
		This String after the assignment has happened

	\exception std::out_of_range	Thrown if \a str_idx is invalid for \a std_str
	\exception std::length_error	Thrown if the resulting String would have been too large.
	*/
	String&	assign(const std::string& std_str, size_type str_idx = 0, size_type str_num = npos)
	{
		if (std_str.size() < str_idx)
		{
			XSTRINGT_THROW(std::out_of_range("Index was out of range for std::string object"));
		}

		if ((str_num == npos) || (str_num > (size_type)std_str.size() - str_idx))
		{
			str_num = (size_type)std_str.size() - str_idx;
		}

		grow(str_num);
		setlen(str_num);

		while(str_num--)
		{
			((*this)[str_num]) = static_cast<utf32>(static_cast<unsigned char>(std_str[str_num + str_idx]));
		}

		return *this;
	}

	/*!
	\brief
		Assign to this String the string value represented by the given null-terminated utf8 encoded data

	\note
		A basic string literal (cast to utf8*) can be passed to this function, provided that the string is
		comprised only of code points 0x00..0x7f.  The use of extended ASCII characters (with values >0x7f)
		would result in incorrect behaviour as the String will attempt to 'decode' the data, with unpredictable
		results.

	\param utf8_str
		Buffer containing valid null-terminated utf8 encoded data

	\return
		This String after the assignment has happened

	\exception std::length_error	Thrown if the resulting String would have been too large.
	*/
	String&	assign(const utf8* utf8_str)
	{
		return assign(utf8_str, utf_length(utf8_str));
	}

	/*!
	\brief
		Assign to this String the string value represented by the given utf8 encoded data

	\note
		A basic string literal (cast to utf8*) can be passed to this function, provided that the string is
		comprised only of code points 0x00..0x7f.  The use of extended ASCII characters (with values >0x7f)
		would result in incorrect behaviour as the String will attempt to 'decode' the data, with unpredictable
		results.

	\param utf8_str
		Buffer containing valid utf8 encoded data

	\param str_num
		Number of code units (not code points) in the buffer pointed to by \a utf8_str

	\return
		This String after the assignment has happened

	\exception std::length_error	Thrown if the resulting String would have been too large, or if str_num is 'npos'.
	*/
	String&	assign(const utf8* utf8_str, size_type str_num)
	{
		if (str_num == npos)
		{
			XSTRINGT_THROW(std::length_error("Length for utf8 encoded string can not be 'npos'"));
		}

		size_type enc_sze = encoded_size(utf8_str, str_num);

		grow(enc_sze);
		encode(utf8_str, ptr(), d_reserve, str_num);
		setlen(enc_sze);
		return *this;
	}

	/*!
	\brief
		Assign to this String the given C-string.

	\param c_str
		Pointer to a valid C style string.

	\return
		This String after the assignment has happened

	\exception std::length_error	Thrown if the resulting String would have been too large.
	*/
	String&	assign(const char* cstr)
	{
		return assign(cstr, strlen(cstr));
	}

	/*!
	\brief
		Assign to this String a number of chars from a char array.

	\param chars
		char array.

	\param chars_len
		Number of chars to be assigned.

	\return
		This String after the assignment has happened

	\exception std::length_error	Thrown if the resulting String would have been too large.
	*/
	String&	assign(const char* chars, size_type chars_len)
	{
		grow(chars_len);

		utf32* pt = ptr();

		for (size_type i = 0; i < chars_len; ++i)
		{
			*pt++ = static_cast<utf32>(static_cast<unsigned char>(*chars++));
		}

		setlen(chars_len);
		return *this;
	}

private:
	// initialise string object
	void	init(void)
	{
		d_reserve			= XSTRINGT_STRING_QUICKBUFF_SIZE;
		d_encodedbuff		= 0;
		d_encodedbufflen	= 0;
		d_encodeddatlen		= 0;
        d_buffer            = 0;
		setlen(0);
	}

	// set the length of the string, and terminate it, according to the given value (will not re-allocate, use grow() first).
	void	setlen(size_type len)
	{
		d_cplength = len;
		ptr()[len] = (utf32)(0);
	}

	// return true if the given pointer is inside the string data
	bool	inside(utf32* inptr)
	{
		if (inptr < ptr() || ptr() + d_cplength <= inptr)
		{ return false; }
		return true;
	}

	// change size of allocated buffer so it is at least 'new_size'.
	// May or may not cause re-allocation and copy of buffer if size is larger
	// will never re-allocate to make size smaller.  (see trim())
    bool	grow(size_type new_size);

private:
	/**************************
		Implementation data
	***************************/
	size_type			d_cplength;			//!< holds length of string in code points (not including null termination)
	size_type			d_reserve;			//!< code point reserve size (currently allocated buffer size in code points).

	mutable utf8*		d_encodedbuff;		//!< holds string data encoded as utf8 (generated only by calls to c_str() and data())
	mutable size_type	d_encodeddatlen;	//!< holds length of encoded data (in case it's smaller than buffer).
	mutable size_type	d_encodedbufflen;	//!< length of above buffer (since buffer can be bigger then the data it holds to save re-allocations).

	utf32				d_quickbuff[XSTRINGT_STRING_QUICKBUFF_SIZE]; //!< This is a integrated 'quick' buffer to save allocations for smallish strings
	utf32*				d_buffer;							//!< Pointer the the main buffer memory.  This is only valid when quick-buffer is not being used

};

};//namespace XStringT

#endif /* __XSTRINGT_H__ */
