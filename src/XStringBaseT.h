/*
 * XStringBaseT.h
 *
 *  Created on: 2017年6月29日
 *      Author: silly
 */

#ifndef __XSTRINGBASET_H__
#define __XSTRINGBASET_H__

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
typedef		unsigned short	utf16; 
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
StringBase Class
-------------------------------------------------------*/
class StringBase
{
public:
	typedef			size_t			size_type;					//!< Unsigned type used for size values and indices
	typedef			std::ptrdiff_t	difference_type;			//!< Signed type used for differences
	static const 	size_type		npos;						//!< Value used to represent 'not found' conditions and 'all code points' etc.

public:
	StringBase(){ }
	virtual ~StringBase(){ }
};

template <typename _TU>
class StringBaseT : public StringBase
{
public:
	//*************************
	//	Integral Types
	//*************************
	typedef		_TU				value_type;					//!< Basic 'code point' type used for String (utf32)
	typedef		_TU&			reference;					//!< Type used for utf32 code point references
	typedef		const _TU&		const_reference;			//!< Type used for constant utf32 code point references
	typedef		_TU*			pointer;					//!< Type used for utf32 code point pointers
	typedef		const _TU*		const_pointer;				//!< Type used for constant utf32 code point pointers

	typedef		StringBase::size_type				size_type;					//!< Unsigned type used for size values and indices
	typedef		StringBase::difference_type			difference_type;			//!< Signed type used for differences

	//**************************
	//	Iterator Classes
	//**************************
    //! regular iterator for String.
    class iterator : public std::iterator<std::random_access_iterator_tag, value_type>
    {
    public:
        iterator() : d_ptr(0) {}
        explicit iterator(value_type* const ptr) : d_ptr(ptr) {}

        value_type& operator*() const { return *d_ptr; }
        value_type* operator->() const { return &**this; }

        iterator& operator++(){ ++d_ptr; return *this; }
        iterator operator++(int){ iterator temp = *this; ++*this; return temp; }

        iterator& operator--(){ --d_ptr; return *this; }
        iterator operator--(int){ iterator temp = *this; --*this; return temp; }

        iterator& operator+=(difference_type offset){ d_ptr += offset; return *this; }
        iterator operator+(difference_type offset) const
        {
            iterator temp = *this;
            return temp += offset;
        }

        iterator& operator-=(difference_type offset){ return *this += -offset; }
        iterator operator-(difference_type offset) const
        {
            iterator temp = *this;
            return temp -= offset;
        }

        value_type& operator[](difference_type offset) const
        {
            return *(*this + offset);
        }

        friend difference_type operator-(const iterator& lhs, const iterator& rhs)
        { return lhs.d_ptr - rhs.d_ptr; }

        friend iterator operator+(difference_type offset, const iterator& iter)
		{ return iter + offset; }

        friend bool operator==(const iterator& lhs, const iterator& rhs)
		{ return lhs.d_ptr == rhs.d_ptr; }

        friend bool operator!=(const iterator& lhs, const iterator& rhs)
        { return lhs.d_ptr != rhs.d_ptr; }

        friend bool operator<(const iterator& lhs, const iterator& rhs)
        { return lhs.d_ptr < rhs.d_ptr; }

        friend bool operator>(const iterator& lhs, const iterator& rhs)
        { return lhs.d_ptr > rhs.d_ptr; }

        friend bool operator<=(const iterator& lhs, const iterator& rhs)
        { return lhs.d_ptr <= rhs.d_ptr; }

        friend bool operator>=(const iterator& lhs, const iterator& rhs)
        { return lhs.d_ptr >= rhs.d_ptr; }

        value_type* d_ptr;
    };

    //! const iterator for String.
    class const_iterator : public std::iterator<std::random_access_iterator_tag, const value_type>
    {
    public:
        const_iterator() : d_ptr(0) {}
        explicit const_iterator(const value_type* const ptr) : d_ptr(ptr) {}
        const_iterator(const iterator& iter) : d_ptr(iter.d_ptr) {}

        const value_type& operator*() const { return *d_ptr; }
        const value_type* operator->() const { return &**this; }

        const_iterator& operator++(){ ++d_ptr; return *this; }
        const_iterator operator++(int){ const_iterator temp = *this; ++*this; return temp; }

        const_iterator& operator--(){ --d_ptr; return *this; }
        const_iterator operator--(int){ const_iterator temp = *this; --*this; return temp; }

        const_iterator& operator+=(difference_type offset){ d_ptr += offset; return *this; }
        const_iterator operator+(difference_type offset) const
        {
        	const_iterator temp = *this;
            return temp += offset;
        }

        const_iterator& operator-=(difference_type offset){ return *this += -offset; }
        const_iterator operator-(difference_type offset) const
        {
            const_iterator temp = *this;
            return temp -= offset;
        }

        const value_type& operator[](difference_type offset) const
        {
        	return *(*this + offset);
        }

        const_iterator& operator=(const iterator& iter)
        {
            d_ptr = iter.d_ptr;
            return *this;
        }

        friend const_iterator operator+(difference_type offset, const const_iterator& iter)
        { return iter + offset; }

        friend difference_type operator-(const const_iterator& lhs,
                                         const const_iterator& rhs)
        { return lhs.d_ptr - rhs.d_ptr; }

        friend bool operator==(const const_iterator& lhs,
                               const const_iterator& rhs)
        { return lhs.d_ptr == rhs.d_ptr; }

        friend bool operator!=(const const_iterator& lhs,
                               const const_iterator& rhs)
        { return lhs.d_ptr != rhs.d_ptr; }

        friend bool operator<(const const_iterator& lhs,
                              const const_iterator& rhs)
        { return lhs.d_ptr < rhs.d_ptr; }

        friend bool operator>(const const_iterator& lhs,
                              const const_iterator& rhs)
        { return lhs.d_ptr > rhs.d_ptr; }

        friend bool operator<=(const const_iterator& lhs,
                               const const_iterator& rhs)
        { return lhs.d_ptr <= rhs.d_ptr; }

        friend bool operator>=(const const_iterator& lhs,
                               const const_iterator& rhs)
        { return lhs.d_ptr >= rhs.d_ptr; }

        const value_type* d_ptr;
    };

	//
	//brief
	//	Constant reverse iterator class for String objects
	//	(Requires version Visual C++ 2008 or later)
	//
#if defined(_MSC_VER) && (_MSC_VER <= 1500)
	#error "Does not support Visual C++ 2008 previous versions."
#else
	typedef	std::reverse_iterator<const_iterator>	const_reverse_iterator;
#endif

	//
	//brief
	//	Reverse iterator class for String objects
	//	(Requires version Visual C++ 2008 or later)
	//
#if defined(_MSC_VER) && (_MSC_VER < 1500)
	#error "Does not support Visual C++ 2008 previous versions."
#else
	typedef std::reverse_iterator<iterator>			reverse_iterator;
#endif

public:
	StringBaseT(){ init(); }
	virtual ~StringBaseT()
	{
		if (d_reserve > XSTRINGT_STRING_QUICKBUFF_SIZE)
		{ delete[] d_buffer; d_buffer = NULL; }
	}

	value_type*			ptr(void){ return (d_reserve > XSTRINGT_STRING_QUICKBUFF_SIZE) ? d_buffer : d_quickbuffer; }
	const value_type*	ptr(void) const{ return (d_reserve > XSTRINGT_STRING_QUICKBUFF_SIZE) ? d_buffer : d_quickbuffer; }

public:
	//////////////////////////////////////////////////////////////////////////
	// Size operations
	//////////////////////////////////////////////////////////////////////////
	size_type	size(void) const { return d_cplength; }
	size_type 	max_size(void) const { return (((size_type)-1) / sizeof(_TU)); }
	bool		empty(void) const { return (d_cplength == 0); }

	//////////////////////////////////////////////////////////////////////////
	// Capacity Operations
	//////////////////////////////////////////////////////////////////////////
	// return the number of code points the string could hold without re-allocation
	// (due to internal encoding this will always report the figure for worst-case encoding, and could even be < size()!)
	size_type 	capacity(void) const { return d_reserve - 1; }

	void		reserve(size_type num = 0)
	{
		if (num == 0)
		{ trim(); return; }

		grow(num);
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
	// Erasing characters
	//////////////////////////////////////////////////////////////////////////
	void	clear(void)
	{
		setlen(0);
		trim();
	}

	StringBaseT& 	erase(void)
	{
		clear();
		return *this;
	}

	StringBaseT&	erase(size_type idx)
	{
		return erase(idx, 1);
	}

	StringBaseT& 	erase(size_type idx, size_type len)
	{
        // cover the no-op case.
        if (len == 0)
		{ return *this; }

		if (d_cplength <= idx)
		{ XSTRINGT_THROW(std::out_of_range("Index is out of range for XStringT::String::erase().")); }

		if (len == npos)
		{ len = d_cplength - idx; }

		size_type newsz = d_cplength - len;

		memmove(&ptr()[idx], &ptr()[idx + len], (d_cplength - idx - len) * sizeof(value_type));
		setlen(newsz);
		return	*this;
	}

	StringBaseT& erase(iterator pos)
	{
		return erase(safe_iter_dif(pos, begin()), 1);
	}

	StringBaseT& erase(iterator iter_beg, iterator iter_end)
	{
		return erase(safe_iter_dif(iter_beg, begin()), safe_iter_dif(iter_end, iter_beg));
	}

public:
	//////////////////////////////////////////////////////////////////////////
	// Assignment Functions
	//////////////////////////////////////////////////////////////////////////
	StringBaseT&	assign(const StringBaseT& str, size_type str_idx = 0, size_type str_num = npos)
	{
		if (str.d_cplength < str_idx)
		{
			XSTRINGT_THROW(std::out_of_range("Index was out of range for XStringT::String::assign() object."));
		}

		if ((str_num == npos) || (str_num > str.d_cplength - str_idx))
		{ str_num = str.d_cplength - str_idx; }

		grow(str_num);
		setlen(str_num);
		memcpy(ptr(), &str.ptr()[str_idx], str_num * sizeof(value_type));
		return *this;
	}

	StringBaseT&	assign(const value_type* str)
	{
		return assign(str, total_length(str));
	}

	StringBaseT&	assign(const value_type* str, size_type num)
	{
		if (num == npos)
		{ XSTRINGT_THROW(std::length_error("Length for string can not be 'npos'.")); }

		grow(num);
		setlen(num);
		memcpy(ptr(), str, num * sizeof(value_type));
		return *this;
	}
protected:
	virtual void	init(void)
	{
		d_reserve			= XSTRINGT_STRING_QUICKBUFF_SIZE;
        d_buffer            = NULL;
		setlen(0);
	}

	// set the length of the string, and terminate it, according to the given value (will not re-allocate, use grow() first).
	virtual void	setlen(size_type len)
	{
		d_cplength = len;
		ptr()[len] = (value_type)(0);
	}

	// change size of allocated buffer so it is at least 'new_size'.
	// May or may not cause re-allocation and copy of buffer if size is larger
	// will never re-allocate to make size smaller.  (see trim())
    bool	grow(size_type new_size);
	// perform re-allocation to remove wasted space.
    void	trim(void);

	// compute distance between two iterators, returning a 'safe' value
	__inline size_type safe_iter_dif(const const_iterator& iter1, const const_iterator& iter2) const
	{
		return (iter1.d_ptr == NULL) ? 0 : (iter1 - iter2);
	}
	// return number of code units in a null terminated string
	__inline size_type total_length(const value_type* str) const
	{
		size_type cnt = 0; while (*str++){ cnt++; }
		return cnt;
	}
protected:
	size_type			d_cplength;			//!< holds length of string in code points (not including null termination)
	size_type			d_reserve;			//!< code point reserve size (currently allocated buffer size in code points).
	value_type			d_quickbuffer[XSTRINGT_STRING_QUICKBUFF_SIZE]; //!< This is a integrated 'quick' buffer to save allocations for smallish strings
	value_type*			d_buffer;							//!< Pointer the the main buffer memory.  This is only valid when quick-buffer is not being used
};

//
template <typename _TU>
__inline bool StringBaseT<_TU>::grow(size_type new_size)
{
    // check for too big
    if (max_size() <= new_size)
    {
    	XSTRINGT_THROW(std::length_error("Resulting String::grow() would be too big"));
    }

    // increase, as we always null-terminate the buffer.
    ++new_size;

    if (new_size > d_reserve)
    {
    	value_type* temp = XSTRINGT_NEW_ARRAY_PT(value_type, new_size);
        if (d_reserve > XSTRINGT_STRING_QUICKBUFF_SIZE)
        {
            memcpy(temp, d_buffer, (d_cplength + 1) * sizeof(value_type));
            XSTRINGT_DELETE_ARRAY_PT(d_buffer, value_type, d_reserve);
        }
        else
        {
            memcpy(temp, d_quickbuffer, (d_cplength + 1) * sizeof(value_type));
        }

        d_buffer = temp;
        d_reserve = new_size;

        return true;
    }
    return false;
}

// perform re-allocation to remove wasted space.
template <typename _TU>
__inline void StringBaseT<_TU>::trim(void)
{
    size_type min_size = d_cplength + 1;

    // only re-allocate when not using quick-buffer, and when size can be trimmed
    if ((d_reserve > XSTRINGT_STRING_QUICKBUFF_SIZE) && (d_reserve > min_size))
    {
            // see if we can trim to quick-buffer
        if (min_size <= XSTRINGT_STRING_QUICKBUFF_SIZE)
        {
            memcpy(d_quickbuffer, d_buffer, min_size * sizeof(value_type));
            XSTRINGT_DELETE_ARRAY_PT(d_buffer, value_type, d_reserve);
            d_reserve = XSTRINGT_STRING_QUICKBUFF_SIZE;
        }
        // re-allocate buffer
        else
        {
        	value_type* temp = XSTRINGT_NEW_ARRAY_PT(value_type, min_size);
            memcpy(temp, d_buffer, min_size * sizeof(value_type));
            XSTRINGT_DELETE_ARRAY_PT(d_buffer, value_type, d_reserve);
            d_buffer = temp;
            d_reserve = min_size;
        }

    }
}


/*-----------------------------------------------------
StringBaseU32 Class
-------------------------------------------------------*/
class StringBaseU32 : public StringBaseT<utf32>
{
public:
	StringBaseU32(){ }
	virtual ~StringBaseU32(){ }
};

/*-----------------------------------------------------
StringBaseU16 Class
-------------------------------------------------------*/
class StringBaseU16 : public StringBaseT<utf16>
{
public:
	StringBaseU16(){ }
	virtual ~StringBaseU16(){ }
};

/*-----------------------------------------------------
StringBaseU8 Class
-------------------------------------------------------*/
class StringBaseU8 : public StringBaseT<utf8>
{
public:
	StringBaseU8(){ }
	virtual ~StringBaseU8(){ }
};

};//namespace XStringT

#endif /* __XSTRINGBASET_H__ */
