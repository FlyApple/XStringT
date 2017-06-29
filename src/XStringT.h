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
typedef		unsigned short	utf16;  // removed typedef to prevent usage, as utf16 is not supported (yet)
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


	/**************************
		Iterator Classes
	***************************/
    //! regular iterator for String.
    class iterator : public std::iterator<std::random_access_iterator_tag, utf32>
    {
    public:
        iterator() : d_ptr(0) {}
        explicit iterator(utf32* const ptr) : d_ptr(ptr) {}

        utf32& operator*() const
        {
            return *d_ptr;
        }

        utf32* operator->() const
        {
            return &**this;
        }

        String::iterator& operator++()
        {
            ++d_ptr; return *this;
        }

        String::iterator operator++(int)
        {
            String::iterator temp = *this; ++*this;
            return temp;
        }

        String::iterator& operator--()
        {
            --d_ptr; return *this;
        }

        String::iterator operator--(int)
        {
            String::iterator temp = *this; --*this;
            return temp;
        }

        String::iterator& operator+=(difference_type offset)
        {
            d_ptr += offset; return *this;
        }

        String::iterator operator+(difference_type offset) const
        {
            String::iterator temp = *this;
            return temp += offset;
        }

        String::iterator& operator-=(difference_type offset)
        {
            return *this += -offset;
        }

        String::iterator operator-(difference_type offset) const
        {
            String::iterator temp = *this;
            return temp -= offset;
        }

        utf32& operator[](difference_type offset) const
        {
            return *(*this + offset);
        }

        friend difference_type operator-(const String::iterator& lhs,
                                         const String::iterator& rhs)
        { return lhs.d_ptr - rhs.d_ptr; }

        friend String::iterator operator+(difference_type offset, const String::iterator& iter)
		{ return iter + offset; }

        friend bool operator==(const String::iterator& lhs,
                               const String::iterator& rhs)
		{ return lhs.d_ptr == rhs.d_ptr; }

        friend bool operator!=(const String::iterator& lhs,
                               const String::iterator& rhs)
        { return lhs.d_ptr != rhs.d_ptr; }

        friend bool operator<(const String::iterator& lhs,
                              const String::iterator& rhs)
        { return lhs.d_ptr < rhs.d_ptr; }

        friend bool operator>(const String::iterator& lhs,
                              const String::iterator& rhs)
        { return lhs.d_ptr > rhs.d_ptr; }

        friend bool operator<=(const String::iterator& lhs,
                               const String::iterator& rhs)
        { return lhs.d_ptr <= rhs.d_ptr; }

        friend bool operator>=(const String::iterator& lhs,
                               const String::iterator& rhs)
        { return lhs.d_ptr >= rhs.d_ptr; }

        utf32* d_ptr;
    };

    //! const iterator for String.
    class const_iterator : public std::iterator<std::random_access_iterator_tag, const utf32>
    {
    public:
        const_iterator() : d_ptr(0) {}
        explicit const_iterator(const utf32* const ptr) : d_ptr(ptr) {}
        const_iterator(const String::iterator& iter) : d_ptr(iter.d_ptr) {}

        const utf32& operator*() const
        {
            return *d_ptr;
        }

        const utf32* operator->() const
        {
            return &**this;
        }

        String::const_iterator& operator++()
        {
            ++d_ptr; return *this;
        }

        String::const_iterator operator++(int)
        {
            String::const_iterator temp = *this; ++*this;
            return temp;
        }

        String::const_iterator& operator--()
        {
            --d_ptr;
            return *this;
        }

        String::const_iterator operator--(int)
        {
            String::const_iterator temp = *this; --*this;
            return temp;
        }

        String::const_iterator& operator+=(difference_type offset)
        {
            d_ptr += offset;
            return *this;
        }

        String::const_iterator operator+(difference_type offset) const
        {
            String::const_iterator temp = *this;
            return temp += offset;
        }

        String::const_iterator& operator-=(difference_type offset)
        {
            return *this += -offset;
        }

        String::const_iterator operator-(difference_type offset) const
        {
            String::const_iterator temp = *this;
            return temp -= offset;
        }

        const utf32& operator[](difference_type offset) const
        {
            return *(*this + offset);
        }

        String::const_iterator& operator=(const String::iterator& iter)
        {
            d_ptr = iter.d_ptr;
            return *this;
        }

        friend String::const_iterator operator+(difference_type offset, const String::const_iterator& iter)
        { return iter + offset; }

        friend difference_type operator-(const String::const_iterator& lhs,
                                         const String::const_iterator& rhs)
        { return lhs.d_ptr - rhs.d_ptr; }

        friend bool operator==(const String::const_iterator& lhs,
                               const String::const_iterator& rhs)
        { return lhs.d_ptr == rhs.d_ptr; }

        friend bool operator!=(const String::const_iterator& lhs,
                               const String::const_iterator& rhs)
        { return lhs.d_ptr != rhs.d_ptr; }

        friend bool operator<(const String::const_iterator& lhs,
                              const String::const_iterator& rhs)
        { return lhs.d_ptr < rhs.d_ptr; }

        friend bool operator>(const String::const_iterator& lhs,
                              const String::const_iterator& rhs)
        { return lhs.d_ptr > rhs.d_ptr; }

        friend bool operator<=(const String::const_iterator& lhs,
                               const String::const_iterator& rhs)
        { return lhs.d_ptr <= rhs.d_ptr; }

        friend bool operator>=(const String::const_iterator& lhs,
                               const String::const_iterator& rhs)
        { return lhs.d_ptr >= rhs.d_ptr; }

        const utf32* d_ptr;
    };

	/*!
	\brief
		Constant reverse iterator class for String objects
		(Requires version Visual C++ 2008 or later)
	*/
#if defined(_MSC_VER) && (_MSC_VER <= 1500)
	#error "Does not support Visual C++ 2008 previous versions."
#else
	typedef	std::reverse_iterator<const_iterator>	const_reverse_iterator;
#endif

	/*!
	\brief
		Reverse iterator class for String objects
		(Requires version Visual C++ 2008 or later)
	*/
#if defined(_MSC_VER) && (_MSC_VER < 1500)
	#error "Does not support Visual C++ 2008 previous versions."
#else
	typedef std::reverse_iterator<iterator>			reverse_iterator;
#endif

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
		Assign the value of String \a str to this String

	\param str
		String object containing the string value to be assigned.

	\return
		This String after the assignment has happened
	*/
	String&	operator=(const String& str)
	{
		return assign(str);
	}

	/*!
	\brief
		Assign the value of std::string \a std_str to this String

	\note
		The characters of \a std_str are taken to be unencoded data which represent Unicode code points 0x00..0xFF.  No translation of
		the provided data will occur.

	\param std_str
		std::string object containing the string value to be assigned.

	\return
		This String after the assignment has happened

	\exception std::length_error	Thrown if the resulting String would have been too large.
	*/
	String&	operator=(const std::string& std_str)
	{
		return assign(std_str);
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
	String&	operator=(const utf8* utf8_str)
	{
		return assign(utf8_str, utf_length(utf8_str));
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
	String&	operator=(const char* cstr)
	{
		return assign(cstr, strlen(cstr));
	}

	/*!
	\brief
		Appends the String \a str

	\param str
		String object that is to be appended

	\return
		This String after the append operation

	\exception std::length_error	Thrown if resulting String would be too large.
	*/
	String&	operator+=(const String& str)
	{
		return append(str);
	}

	/*!
	\brief
		Appends the std::string \a std_str

	\param std_str
		std::string object that is to be appended

	\note
		The characters of \a std_str are taken to be unencoded data which represent Unicode code points 0x00..0xFF.  No translation of
		the provided data will occur.

	\return
		This String after the append operation

	\exception std::length_error	Thrown if resulting String would be too large.
	*/
	String&	operator+=(const std::string& std_str)
	{
		return append(std_str);
	}

	/*!
	\brief
		Appends to the String the null-terminated utf8 encoded data in the buffer utf8_str.

	\param utf8_str
		buffer holding the null-terminated utf8 encoded data that is to be appended

	\note
		A basic string literal (cast to utf8*) can be passed to this function, provided that the string is
		comprised only of code points 0x00..0x7f.  The use of extended ASCII characters (with values >0x7f)
		would result in incorrect behaviour as the String will attempt to 'decode' the data, with unpredictable
		results.

	\return
		This String after the append operation

	\exception std::length_error	Thrown if resulting String would be too large.
	*/
	String&	operator+=(const utf8* utf8_str)
	{
		return append(utf8_str, utf_length(utf8_str));
	}

	/*!
	\brief
		Appends to the String the given c-string.

	\param c_str
		c-string that is to be appended.

	\return
		This String after the append operation

	\exception std::length_error	Thrown if resulting String would be too large.
	*/
	String&	operator+=(const char* cstr)
	{
		return append(cstr, strlen(cstr));
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

		/*!
	\brief
		Returns the code point at the given index.

	\param idx
		Zero based index of the code point to be returned.

	\return
		The utf32 code point at the given index within the String.

	\exception std::out_of_range	Thrown if \a idx is >= length().
	*/
	reference	at(size_type idx)
	{
		if (d_cplength <= idx)
			XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String"));

		return ptr()[idx];
	}

	/*!
	\brief
		Returns the code point at the given index.

	\param idx
		Zero based index of the code point to be returned.

	\return
		The utf32 code point at the given index within the String.

	\exception std::out_of_range	Thrown if \a idx is >= length().
	*/
	const_reference	at(size_type idx) const
	{
		if (d_cplength <= idx)
			XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String"));

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

	// reserve internal memory for at-least 'num' code-points (characters).  if num is 0, request is shrink-to-fit.
	/*!
	\brief
		Specifies the amount of reserve capacity to allocate.

	\param num
		The number of code points to allocate space for.  If \a num is larger that the current reserve, then a re-allocation will occur.  If
		\a num is smaller than the current reserve (but not 0) the buffer may be shrunk to the larger of the specified number, or the current
		String size (operation is currently not implemented).  If \a num is 0, then the buffer is re-allocated to fit the current String size.

	\return
		Nothing

	\exception std::length_error	Thrown if resulting String object would be too big.
	*/
	void	reserve(size_type num = 0)
	{
		if (num == 0)
			trim();
		else
			grow(num);
	}


	//////////////////////////////////////////////////////////////////////////
	// C-Strings and arrays
	//////////////////////////////////////////////////////////////////////////
	/*!
	\brief
		Returns contents of the String as a null terminated string of utf8 encoded data.

	\return
		Pointer to a char buffer containing the contents of the String encoded as null-terminated utf8 data.

	\note
		The buffer returned from this function is owned by the String object.

	\note
		Any function that modifies the String data will invalidate the buffer returned by this call.
	*/
	const char* c_str(void) const
	{
		return (const char*)build_utf8_buff();
	}

	/*!
	\brief
		Returns contents of the String as utf8 encoded data.

	\return
		Pointer to a buffer containing the contents of the String encoded utf8 data.

	\note
		The buffer returned from this function is owned by the String object.

	\note
		Any function that modifies the String data will invalidate the buffer returned by this call.
	*/
	const utf8* data(void) const
	{
		return build_utf8_buff();
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

	/*!
	\brief
		Swaps the value of this String with the given String \a str

	\param str
		String object whos value is to be swapped with this String.

	\return
		Nothing
	*/
	void	swap(String& str)
	{
		size_type	temp_len	= d_cplength;
		d_cplength = str.d_cplength;
		str.d_cplength = temp_len;

		size_type	temp_res	= d_reserve;
		d_reserve = str.d_reserve;
		str.d_reserve = temp_res;

		utf32*		temp_buf	= d_buffer;
		d_buffer = str.d_buffer;
		str.d_buffer = temp_buf;

		// see if we need to swap 'quick buffer' data
		if (temp_res <= XSTRINGT_STRING_QUICKBUFF_SIZE)
		{
			utf32		temp_qbf[XSTRINGT_STRING_QUICKBUFF_SIZE];

			memcpy(temp_qbf, d_quickbuff, XSTRINGT_STRING_QUICKBUFF_SIZE* sizeof(utf32));
			memcpy(d_quickbuff, str.d_quickbuff, XSTRINGT_STRING_QUICKBUFF_SIZE * sizeof(utf32));
			memcpy(str.d_quickbuff, temp_qbf, XSTRINGT_STRING_QUICKBUFF_SIZE * sizeof(utf32));
		}

	}

	//////////////////////////////////////////////////////////////////////////
	// Iterator creation
	//////////////////////////////////////////////////////////////////////////
	/*!
	\brief
		Return a forwards iterator that describes the beginning of the String

	\return
		iterator object that describes the beginning of the String.
	*/
	iterator		begin(void)
	{
		return iterator(ptr());
	}

	/*!
	\brief
		Return a constant forwards iterator that describes the beginning of the String

	\return
		const_iterator object that describes the beginning of the String.
	*/
	const_iterator	begin(void) const
	{
		return const_iterator(ptr());
	}

	/*!
	\brief
		Return a forwards iterator that describes the end of the String

	\return
		iterator object that describes the end of the String.
	*/
	iterator		end(void)
	{
		return iterator(&ptr()[d_cplength]);
	}

	/*!
	\brief
		Return a constant forwards iterator that describes the end of the String

	\return
		const_iterator object that describes the end of the String.
	*/
	const_iterator	end(void) const
	{
		return const_iterator(&ptr()[d_cplength]);
	}

	/*!
	\brief
		Return a reverse iterator that describes the beginning of the String

	\return
		reverse_iterator object that describes the beginning of the String (so is actually at the end)
	*/
	reverse_iterator		rbegin(void)
	{
		return reverse_iterator(end());
	}

	/*!
	\brief
		Return a constant reverse iterator that describes the beginning of the String

	\return
		const_reverse_iterator object that describes the beginning of the String (so is actually at the end)
	*/
	const_reverse_iterator	rbegin(void) const
	{
		return const_reverse_iterator(end());
	}

	/*!
	\brief
		Return a reverse iterator that describes the end of the String

	\return
		reverse_iterator object that describes the end of the String (so is actually at the beginning)
	*/
	reverse_iterator		rend(void)
	{
		return reverse_iterator(begin());
	}

	/*!
	\brief
		Return a constant reverse iterator that describes the end of the String

	\return
		const_reverse_iterator object that describes the end of the String (so is actually at the beginning)
	*/
	const_reverse_iterator	rend(void) const
	{
		return const_reverse_iterator(begin());
	}


	//////////////////////////////////////////////////////////////////////////
	// Appending Functions
	//////////////////////////////////////////////////////////////////////////
	/*!
	\brief
		Appends a sub-string of the String \a str

	\param str
		String object containing data to be appended

	\param str_idx
		Index of the first code point to be appended

	\param str_num
		Maximum number of code points to be appended

	\return
		This String after the append operation

	\exception std::out_of_range	Thrown if \a str_idx is invalid for \a str.
	\exception std::length_error	Thrown if resulting String would be too large.
	*/
	String& append(const String& str, size_type str_idx = 0, size_type str_num = npos)
	{
		if (str.d_cplength < str_idx)
			XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String"));

		if ((str_num == npos) || (str_num > str.d_cplength - str_idx))
			str_num = str.d_cplength - str_idx;

		grow(d_cplength + str_num);
		memcpy(&ptr()[d_cplength], &str.ptr()[str_idx], str_num * sizeof(utf32));
		setlen(d_cplength + str_num);
		return *this;
	}

	/*!
	\brief
		Appends a sub-string of the std::string \a std_str

	\param std_str
		std::string object containing data to be appended

	\note
		The characters of \a std_str are taken to be unencoded data which represent Unicode code points 0x00..0xFF.  No translation of
		the provided data will occur.

	\param str_idx
		Index of the first character to be appended

	\param str_num
		Maximum number of characters to be appended

	\return
		This String after the append operation

	\exception std::out_of_range	Thrown if \a str_idx is invalid for \a std_str.
	\exception std::length_error	Thrown if resulting String would be too large.
	*/
	String& append(const std::string& std_str, size_type str_idx = 0, size_type str_num = npos)
	{
		if (std_str.size() < str_idx)
			XSTRINGT_THROW(std::out_of_range("Index is out of range for std::string"));

		if ((str_num == npos) || (str_num > (size_type)std_str.size() - str_idx))
			str_num = (size_type)std_str.size() - str_idx;

		size_type newsze = d_cplength + str_num;

		grow(newsze);
		utf32* pt = &ptr()[newsze-1];

		while(str_num--)
			*pt-- = static_cast<utf32>(static_cast<unsigned char>(std_str[str_num]));

		setlen(newsze);
		return *this;
	}

	/*!
	\brief
		Appends to the String the null-terminated utf8 encoded data in the buffer utf8_str.

	\param utf8_str
		Buffer holding the null-terminated utf8 encoded data that is to be appended

	\note
		A basic string literal (cast to utf8*) can be passed to this function, provided that the string is
		comprised only of code points 0x00..0x7f.  The use of extended ASCII characters (with values >0x7f)
		would result in incorrect behaviour as the String will attempt to 'decode' the data, with unpredictable
		results.

	\return
		This String after the append operation

	\exception std::length_error	Thrown if resulting String would be too large.
	*/
	String& append(const utf8* utf8_str)
	{
		return append(utf8_str, utf_length(utf8_str));
	}

	/*!
	\brief
		Appends to the String the utf8 encoded data in the buffer utf8_str.

	\param utf8_str
		Buffer holding the utf8 encoded data that is to be appended

	\note
		A basic string literal (cast to utf8*) can be passed to this function, provided that the string is
		comprised only of code points 0x00..0x7f.  The use of extended ASCII characters (with values >0x7f)
		would result in incorrect behaviour as the String will attempt to 'decode' the data, with unpredictable
		results.

	\param len
		Number of code units (not code points) in the buffer to be appended

	\return
		This String after the append operation

	\exception std::length_error	Thrown if resulting String would be too large, or if \a len was 'npos'
	*/
	String& append(const utf8* utf8_str, size_type len)
	{
		if (len == npos)
			XSTRINGT_THROW(std::length_error("Length for utf8 encoded string can not be 'npos'"));

		size_type encsz = encoded_size(utf8_str, len);
		size_type newsz = d_cplength + encsz;

		grow(newsz);
		encode(utf8_str, &ptr()[d_cplength], encsz, len);
		setlen(newsz);

		return *this;
	}

	/*!
	\brief
		Appends the code points in the reange [beg, end)

	\param beg
		Iterator describing the start of the range to be appended

	\param end
		Iterator describing the (exclusive) end of the range to be appended.

	\return
		This String after the append operation

	\exception std::length_error	Thrown if the resulting string would be too large.
	*/
	String&	append(const_iterator iter_beg, const_iterator iter_end)
	{
		return replace(end(), end(), iter_beg, iter_end);
	}

	/*!
	\brief
		Appends to the String the given c-string.

	\param c_str
		c-string that is to be appended.

	\return
		This String after the append operation

	\exception std::length_error	Thrown if resulting String would be too large.
	*/
	String& append(const char* cstr)
	{
		return append(cstr, strlen(cstr));
	}


	/*!
	\brief
		Appends to the String chars from the given char array.

	\param chars
		char array holding the chars that are to be appended

	\param chars_len
		Number of chars to be appended

	\return
		This String after the append operation

	\exception std::length_error	Thrown if resulting String would be too large, or if \a chars_len was 'npos'
	*/
	String& append(const char* chars, size_type chars_len)
	{
		if (chars_len == npos)
			XSTRINGT_THROW(std::length_error("Length for char array can not be 'npos'"));

		size_type newsz = d_cplength + chars_len;

		grow(newsz);

		utf32* pt = &ptr()[newsz-1];

		while(chars_len--)
			*pt-- = static_cast<utf32>(static_cast<unsigned char>(chars[chars_len]));

		setlen(newsz);

		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	// Insertion Functions
	//////////////////////////////////////////////////////////////////////////
	/*!
	\brief
		Inserts the given String object at the specified position.

	\param idx
		Index where the string is to be inserted.

	\param str
		String object that is to be inserted.

	\return
		This String after the insert.

	\exception std::out_of_range	Thrown if \a idx is invalid for this String.
	\exception std::length_error	Thrown if resulting String would be too large.
	*/
	String&	insert(size_type idx, const String& str)
	{
		return insert(idx, str, 0, npos);
	}

	/*!
	\brief
		Inserts a sub-string of the given String object at the specified position.

	\param idx
		Index where the string is to be inserted.

	\param str
		String object containing data to be inserted.

	\param str_idx
		Index of the first code point from \a str to be inserted.

	\param str_num
		Maximum number of code points from \a str to be inserted.

	\return
		This String after the insert.

	\exception std::out_of_range	Thrown if \a idx or \a str_idx are out of range.
	\exception std::length_error	Thrown if resulting String would be too large.
	*/
	String& insert(size_type idx, const String& str, size_type str_idx, size_type str_num)
	{
		if ((d_cplength < idx) || (str.d_cplength < str_idx))
			XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String"));

		if ((str_num == npos) || (str_num > str.d_cplength - str_idx))
			str_num = str.d_cplength - str_idx;

		size_type newsz = d_cplength + str_num;
		grow(newsz);
		memmove(&ptr()[idx + str_num], &ptr()[idx], (d_cplength - idx) * sizeof(utf32));
		memcpy(&ptr()[idx], &str.ptr()[str_idx], str_num * sizeof(utf32));
		setlen(newsz);

		return *this;
	}
	/*!
	\brief
		Inserts the given std::string object at the specified position.

	\param idx
		Index where the std::string is to be inserted.

	\param std_str
		std::string object that is to be inserted.

	\note
		The characters of \a std_str are taken to be unencoded data which represent Unicode code points 0x00..0xFF.  No translation of
		the provided data will occur.

	\return
		This String after the insert.

	\exception std::out_of_range	Thrown if \a idx is invalid for this String.
	\exception std::length_error	Thrown if resulting String would be too large.
	*/
	String&	insert(size_type idx, const std::string& std_str)
	{
		return insert(idx, std_str, 0, npos);
	}

	/*!
	\brief
		Inserts a sub-string of the given std::string object at the specified position.

	\param idx
		Index where the string is to be inserted.

	\param std_str
		std::string object containing data to be inserted.

	\note
		The characters of \a std_str are taken to be unencoded data which represent Unicode code points 0x00..0xFF.  No translation of
		the provided data will occur.

	\param str_idx
		Index of the first character from \a std_str to be inserted.

	\param str_num
		Maximum number of characters from \a str to be inserted.

	\return
		This String after the insert.

	\exception std::out_of_range	Thrown if \a idx or \a str_idx are out of range.
	\exception std::length_error	Thrown if resulting String would be too large.
	*/
	String& insert(size_type idx, const std::string& std_str, size_type str_idx, size_type str_num)
	{
		if (d_cplength < idx)
			XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String"));

		if (std_str.size() < str_idx)
			XSTRINGT_THROW(std::out_of_range("Index is out of range for std::string"));

		if ((str_num == npos) || (str_num > (size_type)std_str.size() - str_idx))
			str_num = (size_type)std_str.size() - str_idx;

		size_type newsz = d_cplength + str_num;
		grow(newsz);

		memmove(&ptr()[idx + str_num], &ptr()[idx], (d_cplength - idx) * sizeof(utf32));

		utf32* pt = &ptr()[idx + str_num - 1];

		while(str_num--)
			*pt-- = static_cast<utf32>(static_cast<unsigned char>(std_str[str_idx + str_num]));

		setlen(newsz);

		return *this;
	}

	/*!
	\brief
		Inserts the given null-terminated utf8 encoded data at the specified position.

	\param idx
		Index where the data is to be inserted.

	\param utf8_str
		Buffer containing the null-terminated utf8 encoded data that is to be inserted.

	\note
		A basic string literal (cast to utf8*) can be passed to this function, provided that the string is
		comprised only of code points 0x00..0x7f.  The use of extended ASCII characters (with values >0x7f)
		would result in incorrect behaviour as the String will attempt to 'decode' the data, with unpredictable
		results.

	\return
		This String after the insert.

	\exception std::out_of_range	Thrown if \a idx is invalid for this String.
	\exception std::length_error	Thrown if resulting String would be too large.
	*/
	String&	insert(size_type idx, const utf8* utf8_str)
	{
		return insert(idx, utf8_str, utf_length(utf8_str));
	}

	/*!
	\brief
		Inserts the given utf8 encoded data at the specified position.

	\param idx
		Index where the data is to be inserted.

	\param utf8_str
		Buffer containing the utf8 encoded data that is to be inserted.

	\note
		A basic string literal (cast to utf8*) can be passed to this function, provided that the string is
		comprised only of code points 0x00..0x7f.  The use of extended ASCII characters (with values >0x7f)
		would result in incorrect behaviour as the String will attempt to 'decode' the data, with unpredictable
		results.

	\param len
		Length of the data to be inserted in uf8 code units (not code points)

	\return
		This String after the insert.

	\exception std::out_of_range	Thrown if \a idx is invalid for this String.
	\exception std::length_error	Thrown if resulting String would be too large, or if \a len is 'npos'
	*/
	String& insert(size_type idx, const utf8* utf8_str, size_type len)
	{
		if (d_cplength < idx)
			XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String"));

		if (len == npos)
			XSTRINGT_THROW(std::length_error("Length of utf8 encoded string can not be 'npos'"));

		size_type encsz = encoded_size(utf8_str, len);
		size_type newsz = d_cplength + encsz;

		grow(newsz);
		memmove(&ptr()[idx + encsz], &ptr()[idx], (d_cplength - idx) * sizeof(utf32));
		encode(utf8_str, &ptr()[idx], encsz, len);
		setlen(newsz);

		return *this;
	}

	/*!
	\brief
		Inserts code points specified by the range [beg, end).

	\param pos
		Iterator describing the position where the data is to be inserted

	\param beg
		Iterator describing the begining of the range to be inserted

	\param end
		Iterator describing the (exclusive) end of the range to be inserted.

	\return
		Nothing.

	\exception std::length_error	Thrown if resulting String would be too large.
	*/
	void	insert(iterator iter_pos, const_iterator iter_beg, const_iterator iter_end)
	{
		replace(iter_pos, iter_pos, iter_beg, iter_end);
	}


	/*!
	\brief
		Inserts the given c-string at the specified position.

	\param idx
		Index where the c-string is to be inserted.

	\param c_str
		c-string that is to be inserted.

	\return
		This String after the insert.

	\exception std::out_of_range	Thrown if \a idx is invalid for this String.
	\exception std::length_error	Thrown if resulting String would be too large.
	*/
	String&	insert(size_type idx, const char* cstr)
	{
		return insert(idx, cstr, strlen(cstr));
	}


	/*!
	\brief
		Inserts chars from the given char array at the specified position.

	\param idx
		Index where the data is to be inserted.

	\param chars
		char array containing the chars that are to be inserted.

	\param chars_len
		Length of the char array to be inserted.

	\return
		This String after the insert.

	\exception std::out_of_range	Thrown if \a idx is invalid for this String.
	\exception std::length_error	Thrown if resulting String would be too large, or if \a chars_len is 'npos'
	*/
	String& insert(size_type idx, const char* chars, size_type chars_len)
	{
		if (d_cplength < idx)
			XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String"));

		if (chars_len == npos)
			XSTRINGT_THROW(std::length_error("Length of char array can not be 'npos'"));

		size_type newsz = d_cplength + chars_len;

		grow(newsz);
		memmove(&ptr()[idx + chars_len], &ptr()[idx], (d_cplength - idx) * sizeof(utf32));

		utf32* pt = &ptr()[idx + chars_len - 1];

		while(chars_len--)
			*pt-- = static_cast<utf32>(static_cast<unsigned char>(chars[chars_len]));

		setlen(newsz);

		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	// Erasing characters
	//////////////////////////////////////////////////////////////////////////
	/*!
	\brief
		Removes all data from the String

	\return
		Nothing
	*/
	void	clear(void)
	{
		setlen(0);
		trim();
	}

	/*!
	\brief
		Removes all data from the String

	\return
		The empty String (*this)
	*/
	String& erase(void)
	{
		clear();
		return *this;
	}

	/*!
	\brief
		Erase a single code point from the string

	\param idx
		The index of the code point to be removed.

	\return
		This String after the erase operation

	\exception std::out_of_range	Thrown if \a idx is invalid for this String.
	*/
	String&	erase(size_type idx)
	{
		return erase(idx, 1);
	}

	/*!
	\brief
		Erase a range of code points

	\param idx
		Index of the first code point to be removed.

	\param len
		Maximum number of code points to be removed.

	\return
		This String after the erase operation.

	\exception std::out_of_range	Thrown if \a idx is invalid for this String.
	*/
	String& erase(size_type idx, size_type len)
	{
        // cover the no-op case.
        if (len == 0)
            return *this;

		if (d_cplength <= idx)
			XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String"));

		if (len == npos)
			len = d_cplength - idx;

		size_type newsz = d_cplength - len;

		memmove(&ptr()[idx], &ptr()[idx + len], (d_cplength - idx - len) * sizeof(utf32));
		setlen(newsz);
		return	*this;
	}

	/*!
	\brief
		Erase the code point described by the given iterator

	\param pos
		Iterator describing the code point to be erased

	\return
		This String after the erase operation.
	*/
	String& erase(iterator pos)
	{
		return erase(safe_iter_dif(pos, begin()), 1);
	}

	/*!
	\brief
		Erase a range of code points described by the iterators [beg, end).

	\param beg
		Iterator describing the postion of the beginning of the range to erase

	\param end
		Iterator describing the postion of the (exclusive) end of the range to erase

	\return
		This String after the erase operation.
	*/
	String& erase(iterator iter_beg, iterator iter_end)
	{
		return erase(safe_iter_dif(iter_beg, begin()), safe_iter_dif(iter_end, iter_beg));
	}

	//////////////////////////////////////////////////////////////////////////
	// Replacing Characters
	//////////////////////////////////////////////////////////////////////////
	/*!
	\brief
		Replace code points in the String with the specified String object

	\param idx
		Index of the first code point to be replaced

	\param len
		Maximum number of code points to be replaced (if this is 0, operation is an insert at position \a idx)

	\param str
		The String object that is to replace the specified code points

	\return
		This String after the replace operation

	\exception std::out_of_range	Thrown if \a idx is invalid for this String
	\exception std::length_error	Thrown if the resulting String would be too large.
	*/
	String& replace(size_type idx, size_type len, const String& str)
	{
		return replace(idx, len, str, 0, npos);
	}

	/*!
	\brief
		Replace the code points in the range [beg, end) with the specified String object

	\note
		If \a beg == \a end, the operation is a insert at iterator position \a beg

	\param beg
		Iterator describing the start of the range to be replaced

	\param end
		Iterator describing the (exclusive) end of the range to be replaced.

	\param str
		The String object that is to replace the specified range of code points

	\return
		This String after the replace operation

	\exception std::length_error	Thrown if the resulting String would be too large.
	*/
	String& replace(iterator iter_beg, iterator iter_end, const String& str)
	{
		return replace(safe_iter_dif(iter_beg, begin()), safe_iter_dif(iter_end, iter_beg), str, 0, npos);
	}

	/*!
	\brief
		Replace code points in the String with a specified sub-string of a given String object.

	\param idx
		Index of the first code point to be replaced

	\param len
		Maximum number of code points to be replaced.  If this is 0, the operation is an insert at position \a idx.

	\param str
		String object containing the data that will replace the specified range of code points

	\param str_idx
		Index of the first code point of \a str that is to replace the specified code point range

	\param str_num
		Maximum number of code points of \a str that are to replace the specified code point range

	\return
		This String after the replace operation

	\exception std::out_of_range	Thrown if either \a idx, or \a str_idx are invalid
	\exception std::length_error	Thrown if the resulting String would have been too large.
	*/
	String& replace(size_type idx, size_type len, const String& str, size_type str_idx, size_type str_num)
	{
		if ((d_cplength < idx) || (str.d_cplength < str_idx))
			XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String"));

		if (((str_idx + str_num) > str.d_cplength) || (str_num == npos))
			str_num = str.d_cplength - str_idx;

		if (((len + idx) > d_cplength) || (len == npos))
			len = d_cplength - idx;

		size_type newsz = d_cplength + str_num - len;

		grow(newsz);

		if ((idx + len) < d_cplength)
			memmove(&ptr()[idx + str_num], &ptr()[len + idx], (d_cplength - idx - len) * sizeof(utf32));

		memcpy(&ptr()[idx], &str.ptr()[str_idx], str_num * sizeof(utf32));
		setlen(newsz);

		return *this;
	}


	/*!
	\brief
		Replace code points in the String with the specified std::string object

	\param idx
		Index of the first code point to be replaced

	\param len
		Maximum number of code points to be replaced (if this is 0, operation is an insert at position \a idx)

	\param std_str
		The std::string object that is to replace the specified code points

	\note
		Characters from \a std_str are considered to represent Unicode code points in the range 0x00..0xFF.  No translation of
		the encountered data is performed.

	\return
		This String after the replace operation

	\exception std::out_of_range	Thrown if \a idx is invalid for this String
	\exception std::length_error	Thrown if the resulting String would be too large.
	*/
	String& replace(size_type idx, size_type len, const std::string& std_str)
	{
		return replace(idx, len, std_str, 0, npos);
	}

	/*!
	\brief
		Replace the code points in the range [beg, end) with the specified std::string object

	\note
		If \a beg == \a end, the operation is a insert at iterator position \a beg

	\param beg
		Iterator describing the start of the range to be replaced

	\param end
		Iterator describing the (exclusive) end of the range to be replaced.

	\param std_str
		The std::string object that is to replace the specified range of code points

	\note
		Characters from \a std_str are considered to represent Unicode code points in the range 0x00..0xFF.  No translation of
		the encountered data is performed.

	\return
		This String after the replace operation

	\exception std::length_error	Thrown if the resulting String would be too large.
	*/
	String& replace(iterator iter_beg, iterator iter_end, const std::string& std_str)
	{
		return replace(safe_iter_dif(iter_beg, begin()), safe_iter_dif(iter_end, iter_beg), std_str, 0, npos);
	}

	/*!
	\brief
		Replace code points in the String with a specified sub-string of a given std::string object.

	\param idx
		Index of the first code point to be replaced

	\param len
		Maximum number of code points to be replaced.  If this is 0, the operation is an insert at position \a idx.

	\param std_str
		std::string object containing the data that will replace the specified range of code points

	\note
		Characters from \a std_str are considered to represent Unicode code points in the range 0x00..0xFF.  No translation of
		the encountered data is performed.

	\param str_idx
		Index of the first code point of \a std_str that is to replace the specified code point range

	\param str_num
		Maximum number of code points of \a std_str that are to replace the specified code point range

	\return
		This String after the replace operation

	\exception std::out_of_range	Thrown if either \a idx, or \a str_idx are invalid
	\exception std::length_error	Thrown if the resulting String would have been too large.
	*/
	String& replace(size_type idx, size_type len, const std::string& std_str, size_type str_idx, size_type str_num)
	{
		if (d_cplength < idx)
			XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String"));

		if (std_str.size() < str_idx)
			XSTRINGT_THROW(std::out_of_range("Index is out of range for std::string"));

		if (((str_idx + str_num) > std_str.size()) || (str_num == npos))
			str_num = (size_type)std_str.size() - str_idx;

		if (((len + idx) > d_cplength) || (len == npos))
			len = d_cplength - idx;

		size_type newsz = d_cplength + str_num - len;

		grow(newsz);

		if ((idx + len) < d_cplength)
			memmove(&ptr()[idx + str_num], &ptr()[len + idx], (d_cplength - idx - len) * sizeof(utf32));

		utf32* pt = &ptr()[idx + str_num - 1];

		while (str_num--)
			*pt-- = static_cast<utf32>(static_cast<unsigned char>(std_str[str_idx + str_num]));

		setlen(newsz);

		return *this;
	}


	/*!
	\brief
		Replace code points in the String with the specified null-terminated utf8 encoded data.

	\param idx
		Index of the first code point to be replaced

	\param len
		Maximum number of code points to be replaced (if this is 0, operation is an insert at position \a idx)

	\param utf8_str
		Buffer containing the null-terminated utf8 encoded data that is to replace the specified code points

	\note
		A basic string literal (cast to utf8*) can be passed to this function, provided that the string is
		comprised only of code points 0x00..0x7f.  The use of extended ASCII characters (with values >0x7f)
		would result in incorrect behaviour as the String will attempt to 'decode' the data, with unpredictable
		results.

	\return
		This String after the replace operation

	\exception std::out_of_range	Thrown if \a idx is invalid for this String
	\exception std::length_error	Thrown if the resulting String would be too large.
	*/
	String& replace(size_type idx, size_type len, const utf8* utf8_str)
	{
		return replace(idx, len, utf8_str, utf_length(utf8_str));
	}

	/*!
	\brief
		Replace the code points in the range [beg, end) with the specified null-terminated utf8 encoded data.

	\note
		If \a beg == \a end, the operation is a insert at iterator position \a beg

	\param beg
		Iterator describing the start of the range to be replaced

	\param end
		Iterator describing the (exclusive) end of the range to be replaced.

	\param utf8_str
		Buffer containing the null-terminated utf8 encoded data that is to replace the specified range of code points

	\note
		A basic string literal (cast to utf8*) can be passed to this function, provided that the string is
		comprised only of code points 0x00..0x7f.  The use of extended ASCII characters (with values >0x7f)
		would result in incorrect behaviour as the String will attempt to 'decode' the data, with unpredictable
		results.

	\return
		This String after the replace operation

	\exception std::length_error	Thrown if the resulting String would be too large.
	*/
	String& replace(iterator iter_beg, iterator iter_end, const utf8* utf8_str)
	{
		return replace(iter_beg, iter_end, utf8_str, utf_length(utf8_str));
	}

	/*!
	\brief
		Replace code points in the String with the specified utf8 encoded data.

	\param idx
		Index of the first code point to be replaced

	\param len
		Maximum number of code points to be replaced (if this is 0, operation is an insert at position \a idx)

	\param utf8_str
		Buffer containing the null-terminated utf8 encoded data that is to replace the specified code points

	\note
		A basic string literal (cast to utf8*) can be passed to this function, provided that the string is
		comprised only of code points 0x00..0x7f.  The use of extended ASCII characters (with values >0x7f)
		would result in incorrect behaviour as the String will attempt to 'decode' the data, with unpredictable
		results.

	\param str_len
		Length of the utf8 encoded data in utf8 code units (not code points).

	\return
		This String after the replace operation

	\exception std::out_of_range	Thrown if \a idx is invalid for this String
	\exception std::length_error	Thrown if the resulting String would be too large, or if \a str_len was 'npos'.
	*/
	String& replace(size_type idx, size_type len, const utf8* utf8_str, size_type str_len)
	{
		if (d_cplength < idx)
			XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String"));

		if (str_len == npos)
			XSTRINGT_THROW(std::length_error("Length for utf8 encoded string can not be 'npos'"));

		if (((len + idx) > d_cplength) || (len == npos))
			len = d_cplength - idx;

		size_type encsz = encoded_size(utf8_str, str_len);
		size_type newsz = d_cplength + encsz - len;

		grow(newsz);

		if ((idx + len) < d_cplength)
			memmove(&ptr()[idx + encsz], &ptr()[len + idx], (d_cplength - idx - len) * sizeof(utf32));

		encode(utf8_str, &ptr()[idx], encsz, str_len);

		setlen(newsz);
		return *this;
	}

	/*!
	\brief
		Replace the code points in the range [beg, end) with the specified null-terminated utf8 encoded data.

	\note
		If \a beg == \a end, the operation is a insert at iterator position \a beg

	\param beg
		Iterator describing the start of the range to be replaced

	\param end
		Iterator describing the (exclusive) end of the range to be replaced.

	\param utf8_str
		Buffer containing the null-terminated utf8 encoded data that is to replace the specified range of code points

	\note
		A basic string literal (cast to utf8*) can be passed to this function, provided that the string is
		comprised only of code points 0x00..0x7f.  The use of extended ASCII characters (with values >0x7f)
		would result in incorrect behaviour as the String will attempt to 'decode' the data, with unpredictable
		results.

	\param str_len
		Length of the utf8 encoded data in utf8 code units (not code points).

	\return
		This String after the replace operation

		\exception std::length_error	Thrown if the resulting String would be too large, or if \a str_len was 'npos'.
	*/
	String& replace(iterator iter_beg, iterator iter_end, const utf8* utf8_str, size_type str_len)
	{
		return replace(safe_iter_dif(iter_beg, begin()), safe_iter_dif(iter_end, iter_beg), utf8_str, str_len);
	}

	/*!
	\brief
		Replace the code points in the range [beg, end) with code points from the range [newBeg, newEnd).

	\note
		If \a beg == \a end, the operation is an insert at iterator position \a beg

	\param beg
		Iterator describing the start of the range to be replaced

	\param end
		Iterator describing the (exclusive) end of the range to be replaced.

	\param newBeg
		Iterator describing the beginning of the range to insert.

	\param newEnd
		Iterator describing the (exclusive) end of the range to insert.

	\return
		This String after the insert operation.

	\exception std::length_error	Thrown if the resulting string would be too long.
	*/
	String& replace(iterator iter_beg, iterator iter_end, const_iterator iter_newBeg, const_iterator iter_newEnd)
	{
		if (iter_newBeg == iter_newEnd)
		{
			erase(safe_iter_dif(iter_beg, begin()), safe_iter_dif(iter_end, iter_beg));
		}
		else
		{
			size_type str_len = safe_iter_dif(iter_newEnd, iter_newBeg);
			size_type idx = safe_iter_dif(iter_beg, begin());
			size_type len = safe_iter_dif(iter_end, iter_beg);

			if ((len + idx) > d_cplength)
				len = d_cplength - idx;

			size_type newsz = d_cplength + str_len - len;

			grow(newsz);

			if ((idx + len) < d_cplength)
				memmove(&ptr()[idx + str_len], &ptr()[len + idx], (d_cplength - idx - len) * sizeof(utf32));

			memcpy(&ptr()[idx], iter_newBeg.d_ptr, str_len * sizeof(utf32));
			setlen(newsz);
		}

		return *this;
	}


	/*!
	\brief
		Replace code points in the String with the specified c-string.

	\param idx
		Index of the first code point to be replaced

	\param len
		Maximum number of code points to be replaced (if this is 0, operation is an insert at position \a idx)

	\param c_str
		c-string that is to replace the specified code points

	\return
		This String after the replace operation

	\exception std::out_of_range	Thrown if \a idx is invalid for this String
	\exception std::length_error	Thrown if the resulting String would be too large.
	*/
	String& replace(size_type idx, size_type len, const char* cstr)
	{
		return replace(idx, len, cstr, strlen(cstr));
	}


	/*!
	\brief
		Replace the code points in the range [beg, end) with the specified c-string.

	\note
		If \a beg == \a end, the operation is a insert at iterator position \a beg

	\param beg
		Iterator describing the start of the range to be replaced

	\param end
		Iterator describing the (exclusive) end of the range to be replaced.

	\param c_str
		c-string that is to replace the specified range of code points

	\return
		This String after the replace operation

	\exception std::length_error	Thrown if the resulting String would be too large.
	*/
	String& replace(iterator iter_beg, iterator iter_end, const char* cstr)
	{
		return replace(iter_beg, iter_end, cstr, strlen(cstr));
	}


	/*!
	\brief
		Replace code points in the String with chars from the given char array.

	\param idx
		Index of the first code point to be replaced

	\param len
		Maximum number of code points to be replaced (if this is 0, operation is an insert at position \a idx)

	\param chars
		char array containing the cars that are to replace the specified code points

	\param chars_len
		Number of chars in the char array.

	\return
		This String after the replace operation

	\exception std::out_of_range	Thrown if \a idx is invalid for this String
	\exception std::length_error	Thrown if the resulting String would be too large, or if \a chars_len was 'npos'.
	*/
	String& replace(size_type idx, size_type len, const char* chars, size_type chars_len)
	{
		if (d_cplength < idx)
			XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String"));

		if (chars_len == npos)
			XSTRINGT_THROW(std::length_error("Length for the char array can not be 'npos'"));

		if (((len + idx) > d_cplength) || (len == npos))
			len = d_cplength - idx;

		size_type newsz = d_cplength + chars_len - len;

		grow(newsz);

		if ((idx + len) < d_cplength)
			memmove(&ptr()[idx + chars_len], &ptr()[len + idx], (d_cplength - idx - len) * sizeof(utf32));

		utf32* pt = &ptr()[idx + chars_len - 1];

		while (chars_len--)
			*pt-- = static_cast<utf32>(static_cast<unsigned char>(chars[chars_len]));

		setlen(newsz);
		return *this;
	}


	/*!
	\brief
		Replace the code points in the range [beg, end) with chars from the given char array.

	\note
		If \a beg == \a end, the operation is a insert at iterator position \a beg

	\param beg
		Iterator describing the start of the range to be replaced

	\param end
		Iterator describing the (exclusive) end of the range to be replaced.

	\param chars
		char array containing the chars that are to replace the specified range of code points

	\param chars_len
		Number of chars in the char array.

	\return
		This String after the replace operation

	\exception std::length_error	Thrown if the resulting String would be too large, or if \a chars_len was 'npos'.
	*/
	String& replace(iterator iter_beg, iterator iter_end, const char* chars, size_type chars_len)
	{
		return replace(safe_iter_dif(iter_beg, begin()), safe_iter_dif(iter_end, iter_beg), chars, chars_len);
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
	// perform re-allocation to remove wasted space.
    void	trim(void);
	// encoding functions
	// for all:
	//	src_len is in code units, or 0 for null terminated string.
	//	dest_len is in code units.
	//	returns number of code units put into dest buffer.
	size_type encode(const utf32* src, utf8* dest, size_type dest_len, size_type src_len = 0) const;
	size_type encode(const utf8* src, utf32* dest, size_type dest_len, size_type src_len = 0) const;
	// return the number of utf8 code units required to encode the given utf32 code point
	size_type encoded_size(utf32 code_point) const;
	// return number of code units required to re-encode given null-terminated utf32 data as utf8.  return does not include terminating null.
	size_type encoded_size(const utf32* buf) const;
	// return number of code units required to re-encode given utf32 data as utf8.   len is number of code units in 'buf'.
	size_type encoded_size(const utf32* buf, size_type len) const;
	// return number of utf32 code units required to re-encode given utf8 data as utf32.  return does not include terminating null.
	size_type encoded_size(const utf8* buf) const;
	// return number of utf32 code units required to re-encode given utf8 data as utf32.  len is number of code units in 'buf'.
	size_type encoded_size(const utf8* buf, size_type len) const;
	
	// build an internal buffer with the string encoded as utf8 (remains valid until string is modified).
    utf8* build_utf8_buff(void) const;

	// return number of code units in a null terminated string
	size_type utf_length(const utf8* utf8_str) const
	{
		size_type cnt = 0;
		while (*utf8_str++){ cnt++; }
		return cnt;
	}

	// return number of code units in a null terminated string
	size_type utf_length(const utf32* utf32_str) const
	{
		size_type cnt = 0;
		while (*utf32_str++){ cnt++; }
		return cnt;
	}

	// compute distance between two iterators, returning a 'safe' value
	size_type safe_iter_dif(const const_iterator& iter1, const const_iterator& iter2) const
	{
		return (iter1.d_ptr == 0) ? 0 : (iter1 - iter2);
	}



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

//////////////////////////////////////////////////////////////////////////
// Concatenation operator functions
//////////////////////////////////////////////////////////////////////////
/*!
\brief
	Return String object that is the concatenation of the given inputs

\param str1
	String object describing first part of the new string

\param str2
	String object describing the second part of the new string

\return
	A String object that is the concatenation of \a str1 and \a str2

\exception std::length_error	Thrown if the resulting String would be too large.
*/
String	operator+(const String& str1, const String& str2);

/*!
\brief
	Return String object that is the concatenation of the given inputs

\param str
	String object describing first part of the new string

\param std_str
	std::string object describing the second part of the new string

\return
	A String object that is the concatenation of \a str and \a std_str

\exception std::length_error	Thrown if the resulting String would be too large.
*/
String operator+(const String& str, const std::string& std_str);

/*!
\brief
	Return String object that is the concatenation of the given inputs

\param std_str
	std::string object describing the first part of the new string

\param str
	String object describing the second part of the new string

\return
	A String object that is the concatenation of \a std_str and \a str

\exception std::length_error	Thrown if the resulting String would be too large.
*/
String operator+(const std::string& std_str, const String& str);

/*!
\brief
	Return String object that is the concatenation of the given inputs

\param str
	String object describing first part of the new string

\param utf8_str
	Buffer containing null-terminated utf8 encoded data describing the second part of the new string

\return
	A String object that is the concatenation of \a str and \a utf8_str

\exception std::length_error	Thrown if the resulting String would be too large.
*/
String operator+(const String& str, const utf8* utf8_str);

/*!
\brief
	Return String object that is the concatenation of the given inputs

\param utf8_str
	Buffer containing null-terminated utf8 encoded data describing the first part of the new string

\param str
	String object describing the second part of the new string

\return
	A String object that is the concatenation of \a str and \a utf8_str

\exception std::length_error	Thrown if the resulting String would be too large.
*/
String operator+(const utf8* utf8_str, const String& str);

/*!
\brief
	Return String object that is the concatenation of the given inputs

\param str
	String object describing first part of the new string

\param c_str
	c-string describing the second part of the new string

\return
	A String object that is the concatenation of \a str and \a c_str

\exception std::length_error	Thrown if the resulting String would be too large.
*/
String operator+(const String& str, const char* c_str);

/*!
\brief
	Return String object that is the concatenation of the given inputs

\param c_str
	c-string describing the first part of the new string

\param str
	String object describing the second part of the new string

\return
	A String object that is the concatenation of \a c_str and \a str

\exception std::length_error	Thrown if the resulting String would be too large.
*/
String operator+(const char* c_str, const String& str);

};//namespace XStringT

#endif /* __XSTRINGT_H__ */
