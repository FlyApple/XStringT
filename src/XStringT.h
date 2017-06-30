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

#include "XStringBaseT.h"

//
namespace XStringT
{

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
	String(const String& str)
	{
		init();
		assign(str);
	}

	String(const String& str, size_type str_idx, size_type str_num = npos)
	{
		init();
		assign(str, str_idx, str_num);
	}
		
	String(const utf8* utf8_str)
	{
		init();
		assign(utf8_str);
	}

	String(const utf8* utf8_str, size_type chars_len)
	{
		init();
		assign(utf8_str, chars_len);
	}

	String(const_iterator iter_beg, const_iterator iter_end)
	{
		init();
		append(iter_beg, iter_end);
	}

public:

	String&	operator=(const String& str)
	{
		return assign(str);
	}

	String&	operator=(const utf8* utf8_str)
	{
		return assign(utf8_str, total_utf_length(utf8_str));
	}

	String&	operator+=(const String& str)
	{
		return append(str);
	}

	String&	operator+=(const utf8* utf8_str)
	{
		return append(utf8_str, total_utf_length(utf8_str));
	}

	reference	operator[](size_type idx)
	{
		return (ptr()[idx]);
	}

	value_type	operator[](size_type idx) const
	{
		return ptr()[idx];
	}

	reference	at(size_type idx)
	{
		if (d_cplength <= idx)
		{ XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String::at().")); }

		return ptr()[idx];
	}

	const_reference	at(size_type idx) const
	{
		if (d_cplength <= idx)
		{ XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String::at().")); }

		return ptr()[idx];
	}


public:
	//////////////////////////////////////////////////////////////////////////
	// Size operations
	//////////////////////////////////////////////////////////////////////////
	size_type	size(void) const
	{
		return d_cplength;
	}

	size_type	length(void) const
	{
		return d_cplength;
	}

	size_type max_size(void) const
	{
		return (((size_type)-1) / sizeof(utf32));
	}

	bool	empty(void) const
	{
		return	(d_cplength == 0);
	}

	//////////////////////////////////////////////////////////////////////////
	// Capacity Operations
	//////////////////////////////////////////////////////////////////////////
	// return the number of code points the string could hold without re-allocation
	// (due to internal encoding this will always report the figure for worst-case encoding, and could even be < size()!)

	size_type capacity(void) const
	{
		return d_reserve - 1;
	}

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
	const utf8* data(void) const
	{
		return build_utf8_buffer();
	}

	utf32*	ptr(void)
	{
		return (d_reserve > XSTRINGT_STRING_QUICKBUFF_SIZE) ? d_buffer : d_quickbuffer;
	}

	const utf32*	ptr(void) const
	{
		return (d_reserve > XSTRINGT_STRING_QUICKBUFF_SIZE) ? d_buffer : d_quickbuffer;
	}

	size_type	copy(utf8* buf, size_type len = npos, size_type idx = 0) const
	{
		if (d_cplength < idx)
		{ XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String::copy().")); }

		if (len == npos)
		{ len = d_cplength; }

		return encode(&ptr()[idx], buf, npos, len);
	}

	//////////////////////////////////////////////////////////////////////////
	// Assignment Functions
	//////////////////////////////////////////////////////////////////////////
	String&	assign(const String& str, size_type str_idx = 0, size_type str_num = npos)
	{
		if (str.d_cplength < str_idx)
		{
			XSTRINGT_THROW(std::out_of_range("Index was out of range for XStringT::String::assign() object."));
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

	String&	assign(const utf8* utf8_str)
	{
		return assign(utf8_str, total_utf_length(utf8_str));
	}

	String&	assign(const utf8* utf8_str, size_type str_num)
	{
		if (str_num == npos)
		{ XSTRINGT_THROW(std::length_error("Length for utf8 encoded string can not be 'npos'.")); }

		size_type enc_sze = encoded_size(utf8_str, str_num);

		grow(enc_sze);
		encode(utf8_str, ptr(), d_reserve, str_num);
		setlen(enc_sze);
		return *this;
	}


	//
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

			memcpy(temp_qbf, d_quickbuffer, XSTRINGT_STRING_QUICKBUFF_SIZE* sizeof(utf32));
			memcpy(d_quickbuffer, str.d_quickbuffer, XSTRINGT_STRING_QUICKBUFF_SIZE * sizeof(utf32));
			memcpy(str.d_quickbuffer, temp_qbf, XSTRINGT_STRING_QUICKBUFF_SIZE * sizeof(utf32));
		}

	}

	//////////////////////////////////////////////////////////////////////////
	// Iterator creation
	//////////////////////////////////////////////////////////////////////////
	iterator		begin(void)
	{
		return iterator(ptr());
	}

	const_iterator	begin(void) const
	{
		return const_iterator(ptr());
	}

	iterator		end(void)
	{
		return iterator(&ptr()[d_cplength]);
	}

	const_iterator	end(void) const
	{
		return const_iterator(&ptr()[d_cplength]);
	}

	reverse_iterator		rbegin(void)
	{
		return reverse_iterator(end());
	}

	const_reverse_iterator	rbegin(void) const
	{
		return const_reverse_iterator(end());
	}

	reverse_iterator		rend(void)
	{
		return reverse_iterator(begin());
	}

	const_reverse_iterator	rend(void) const
	{
		return const_reverse_iterator(begin());
	}


	//////////////////////////////////////////////////////////////////////////
	// Appending Functions
	//////////////////////////////////////////////////////////////////////////
	String& append(const String& str, size_type str_idx = 0, size_type str_num = npos)
	{
		if (str.d_cplength < str_idx)
		{ XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String::append().")); }

		if ((str_num == npos) || (str_num > str.d_cplength - str_idx))
		{ str_num = str.d_cplength - str_idx; }

		grow(d_cplength + str_num);
		memcpy(&ptr()[d_cplength], &str.ptr()[str_idx], str_num * sizeof(utf32));
		setlen(d_cplength + str_num);
		return *this;
	}

	String& append(const utf8* utf8_str)
	{
		return append(utf8_str, total_utf_length(utf8_str));
	}

	String& append(const utf8* utf8_str, size_type len)
	{
		if (len == npos)
		{ XSTRINGT_THROW(std::length_error("Length for utf8 encoded string can not be 'npos'.")); }

		size_type encsz = encoded_size(utf8_str, len);
		size_type newsz = d_cplength + encsz;

		grow(newsz);
		encode(utf8_str, &ptr()[d_cplength], encsz, len);
		setlen(newsz);

		return *this;
	}

	String&	append(const_iterator iter_beg, const_iterator iter_end)
	{
		return replace(end(), end(), iter_beg, iter_end);
	}

	//////////////////////////////////////////////////////////////////////////
	// Insertion Functions
	//////////////////////////////////////////////////////////////////////////
	String&	insert(size_type idx, const String& str)
	{
		return insert(idx, str, 0, npos);
	}

	String& insert(size_type idx, const String& str, size_type str_idx, size_type str_num)
	{
		if ((d_cplength < idx) || (str.d_cplength < str_idx))
		{ XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String::insert().")); }

		if ((str_num == npos) || (str_num > str.d_cplength - str_idx))
		{ str_num = str.d_cplength - str_idx; }

		size_type newsz = d_cplength + str_num;
		grow(newsz);
		memmove(&ptr()[idx + str_num], &ptr()[idx], (d_cplength - idx) * sizeof(utf32));
		memcpy(&ptr()[idx], &str.ptr()[str_idx], str_num * sizeof(utf32));
		setlen(newsz);

		return *this;
	}

	String&	insert(size_type idx, const utf8* utf8_str)
	{
		return insert(idx, utf8_str, total_utf_length(utf8_str));
	}

	String& insert(size_type idx, const utf8* utf8_str, size_type len)
	{
		if (d_cplength < idx)
		{ XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String::insert().")); }

		if (len == npos)
		{ XSTRINGT_THROW(std::length_error("Length of utf8 encoded string can not be 'npos'.")); }

		size_type encsz = encoded_size(utf8_str, len);
		size_type newsz = d_cplength + encsz;

		grow(newsz);
		memmove(&ptr()[idx + encsz], &ptr()[idx], (d_cplength - idx) * sizeof(utf32));
		encode(utf8_str, &ptr()[idx], encsz, len);
		setlen(newsz);

		return *this;
	}

	void	insert(iterator iter_pos, const_iterator iter_beg, const_iterator iter_end)
	{
		replace(iter_pos, iter_pos, iter_beg, iter_end);
	}

	//////////////////////////////////////////////////////////////////////////
	// Erasing characters
	//////////////////////////////////////////////////////////////////////////
	void	clear(void)
	{
		setlen(0);
		trim();
	}

	String& erase(void)
	{
		clear();
		return *this;
	}

	String&	erase(size_type idx)
	{
		return erase(idx, 1);
	}

	String& erase(size_type idx, size_type len)
	{
        // cover the no-op case.
        if (len == 0)
		{ return *this; }

		if (d_cplength <= idx)
		{ XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String::erase().")); }

		if (len == npos)
		{ len = d_cplength - idx; }

		size_type newsz = d_cplength - len;

		memmove(&ptr()[idx], &ptr()[idx + len], (d_cplength - idx - len) * sizeof(utf32));
		setlen(newsz);
		return	*this;
	}

	String& erase(iterator pos)
	{
		return erase(safe_iter_dif(pos, begin()), 1);
	}

	String& erase(iterator iter_beg, iterator iter_end)
	{
		return erase(safe_iter_dif(iter_beg, begin()), safe_iter_dif(iter_end, iter_beg));
	}

	//////////////////////////////////////////////////////////////////////////
	// Replacing Characters
	//////////////////////////////////////////////////////////////////////////
	String& replace(size_type idx, size_type len, const String& str)
	{
		return replace(idx, len, str, 0, npos);
	}

	String& replace(iterator iter_beg, iterator iter_end, const String& str)
	{
		return replace(safe_iter_dif(iter_beg, begin()), safe_iter_dif(iter_end, iter_beg), str, 0, npos);
	}

	String& replace(size_type idx, size_type len, const String& str, size_type str_idx, size_type str_num)
	{
		if ((d_cplength < idx) || (str.d_cplength < str_idx))
		{ XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String::replace().")); }

		if (((str_idx + str_num) > str.d_cplength) || (str_num == npos))
		{ str_num = str.d_cplength - str_idx; }

		if (((len + idx) > d_cplength) || (len == npos))
		{ len = d_cplength - idx; }

		size_type newsz = d_cplength + str_num - len;

		grow(newsz);

		if ((idx + len) < d_cplength)
		{ memmove(&ptr()[idx + str_num], &ptr()[len + idx], (d_cplength - idx - len) * sizeof(utf32)); }

		memcpy(&ptr()[idx], &str.ptr()[str_idx], str_num * sizeof(utf32));
		setlen(newsz);

		return *this;
	}

	String& replace(size_type idx, size_type len, const utf8* utf8_str)
	{
		return replace(idx, len, utf8_str, total_utf_length(utf8_str));
	}

	String& replace(iterator iter_beg, iterator iter_end, const utf8* utf8_str)
	{
		return replace(iter_beg, iter_end, utf8_str, total_utf_length(utf8_str));
	}

	String& replace(size_type idx, size_type len, const utf8* utf8_str, size_type str_len)
	{
		if (d_cplength < idx)
		{ XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String::replace().")); }

		if (str_len == npos)
		{ XSTRINGT_THROW(std::length_error("Length for utf8 encoded string can not be 'npos'.")); }

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

	String& replace(iterator iter_beg, iterator iter_end, const utf8* utf8_str, size_type str_len)
	{
		return replace(safe_iter_dif(iter_beg, begin()), safe_iter_dif(iter_end, iter_beg), utf8_str, str_len);
	}

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
		
	//////////////////////////////////////////////////////////////////////////
	// Substring
	//////////////////////////////////////////////////////////////////////////
	String	substr(size_type idx = 0, size_type len = npos) const
	{
		if (d_cplength < idx)
		{ XSTRINGT_THROW(std::out_of_range("Index is out of range for this CEGUI::String::substr().")); }

		return String(*this, idx, len);
	}

protected:
	// initialise string object
	virtual void	init(void)
	{
		d_reserve			= XSTRINGT_STRING_QUICKBUFF_SIZE;
		d_encodedbuff		= 0;
		d_encodedbufflen	= 0;
		d_encodeddatlen		= 0;
        d_buffer            = 0;
		setlen(0);
	}

	// set the length of the string, and terminate it, according to the given value (will not re-allocate, use grow() first).
	virtual void	setlen(size_type len)
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
    utf8* build_utf8_buffer(void) const;

	// return number of code units in a null terminated string
	__inline size_type total_utf_length(const utf8* utf8_str) const
	{
		size_type cnt = 0; while (*utf8_str++){ cnt++; }
		return cnt;
	}

	// return number of code units in a null terminated string
	__inline size_type total_utf_length(const utf32* utf32_str) const
	{
		size_type cnt = 0; while (*utf32_str++){ cnt++; }
		return cnt;
	}

	// compute distance between two iterators, returning a 'safe' value
	__inline size_type safe_iter_dif(const const_iterator& iter1, const const_iterator& iter2) const
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

	utf32				d_quickbuffer[XSTRINGT_STRING_QUICKBUFF_SIZE]; //!< This is a integrated 'quick' buffer to save allocations for smallish strings
	utf32*				d_buffer;							//!< Pointer the the main buffer memory.  This is only valid when quick-buffer is not being used

};

//////////////////////////////////////////////////////////////////////////
// Concatenation operator functions
//////////////////////////////////////////////////////////////////////////
String	operator+(const String& str1, const String& str2);
String	operator+(const String& str, const utf8* utf8_str);
String	operator+(const utf8* utf8_str, const String& str);

/*-----------------------------------------------------
StringU32 Class
-------------------------------------------------------*/
class StringU32 : public StringBaseU32
{
public:
	friend class StringU8;

public:
	StringU32();
	virtual ~StringU32();

public:
	StringU32&	assign(const utf8* utf8_str)
	{
		return assign(utf8_str, total_length(utf8_str));
	}

	StringU32&	assign(const utf8* utf8_str, size_type str_num)
	{
		if (str_num == npos)
		{ XSTRINGT_THROW(std::length_error("Length for utf8 encoded string can not be 'npos'.")); }

		size_type enc_sze = encoded_size(utf8_str, str_num);

		grow(enc_sze);
		encode(utf8_str, ptr(), d_reserve, str_num);
		setlen(enc_sze);
		return *this;
	}

protected:
	// return number of utf32 code units required to re-encode given utf8 data as utf32.  return does not include terminating null.
	size_type encoded_size(const utf8* buf) const;
	// return number of utf32 code units required to re-encode given utf8 data as utf32.  len is number of code units in 'buf'.
	size_type encoded_size(const utf8* buf, size_type len) const;
	// encoding functions
	// for all:
	//	src_len is in code units, or 0 for null terminated string.
	//	dest_len is in code units.
	//	returns number of code units put into dest buffer.
	size_type encode(const utf8* src, utf32* dest, size_type dest_len, size_type src_len = 0) const;

	// return number of code units in a null terminated string
	__inline size_type total_length(const utf8* utf8_str) const
	{
		size_type cnt = 0; while (*utf8_str++){ cnt++; }
		return cnt;
	}
};

/*-----------------------------------------------------
StringU8 Class
-------------------------------------------------------*/
class StringU8 : public StringBaseU8
{
public:
	StringU8();
	virtual ~StringU8();

	//////////////////////////////////////////////////////////////////////////
	// Construction via XStringT::StringU8
	//////////////////////////////////////////////////////////////////////////
	StringU8(const StringU8& str)
	{
		init();
		assign(str);
	}

	StringU8(const StringU8& str, size_type str_idx, size_type str_num = npos)
	{
		init();
		assign(str, str_idx, str_num);
	}

	StringU8(const utf8* utf8_str)
	{
		init();
		assign(utf8_str);
	}

	StringU8(const utf8* utf8_str, size_type chars_len)
	{
		init();
		assign(utf8_str, chars_len);
	}

public:
	StringU8&	assign(const StringU8& str, size_type str_idx = 0, size_type str_num = npos)
	{
		StringBaseU8::assign(str, str_idx, str_num);

		d_stringU32.assign(this->ptr());
		return *this;
	}

	StringU8&	assign(const utf8* utf8_str)
	{
		return assign(utf8_str, total_length(utf8_str));
	}

	StringU8&	assign(const utf8* utf8_str, size_type str_num)
	{
		StringBaseU8::assign(utf8_str, str_num);

		d_stringU32.assign(this->ptr());
		return *this;
	}
protected:
	virtual void	init(void)
	{
		d_stringU32.init();

		StringBaseU8::init();
	}
protected:
	StringU32		d_stringU32;
};

};//namespace XStringT

#endif /* __XSTRINGT_H__ */
