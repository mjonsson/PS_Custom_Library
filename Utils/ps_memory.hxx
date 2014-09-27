/*!
 * \file ps_memory.hxx
 * \date 2014/04/30 21:06
 *
 * \author Mattias Jonsson (jonssonm)
 * Contact: jonsson.mattias@siemens.com
 *
 * \brief A class that contains, allocates and deallocates c-style pointers.
 *
 * \warning Remember to declare c_ptr or c_pptr inside iterative loops to avoid memory leaks.
*/

#pragma once

#include "ps_macro.hxx"
#include "ps_write_syslog.hxx"

namespace ps
{
	//! Template class that contains an array of any c-style type \a T.
	template<typename T>
	class c_ptr
	{
	private:
		T*	m_ptr;			//!< Pointer to type contained
		int m_size;			//!< Size of data contained in array
		int m_alloc_chunk;	//!< Block size to use for memory allocation
		int m_alloc_size;	//!< Total size of array
		bool m_free;		//!< Whether destructor shall free memory or not

		//! Called from the different constructor methods
		void construct()
		{
			m_ptr = NULL;
			m_size = 0;
			m_free = true;
			m_alloc_chunk = 10;
			m_alloc_size = 0;
		}

	public:
		//! Default constructor
		c_ptr()
		{
			construct();
		}
		//! Constructor that sets the initial array size and chunk size
		c_ptr(int initial)
		{
			construct();
			m_alloc_size = initial;
			alloc();
		}
		//! Constructor that sets the initial array size and chunk size
		c_ptr(int initial, int chunk)
		{
			construct();
			m_alloc_chunk = chunk;
			m_alloc_size = initial;
			alloc();
		}
		//! Constructor that sets the pointer to point to \a ptr
		c_ptr(const T* ptr)
		{
			construct();
			m_ptr = ptr;
		}
		//! Format a string at construction
		c_ptr(const char *fmt, ...)
		{
			construct();

			va_list args;

			int bufsiz = strlen(fmt) + 512;
			alloc(bufsiz);

			// Make sure we have enough memory to store parameters
			va_start(args, fmt);
			int need = vsnprintf(m_ptr, bufsiz, fmt, args) + 1;
			if (need > bufsiz)
			{
				realloc(need);
				vsnprintf(m_ptr, need, fmt, args);
			}
			va_end(args);
		}
		//! Format a string
		void format(const char *fmt, ...)
		{
			construct();

			va_list args;

			int bufsiz = strlen(fmt) + 512;
			alloc(bufsiz);

			// Make sure we have enough memory to store parameters
			va_start(args, fmt);
			int need = vsnprintf(m_ptr, bufsiz, fmt, args) + 1;
			if (need > bufsiz)
			{
				realloc(need);
				vsnprintf(m_ptr, need, fmt, args);
			}
			va_end(args);
		}
		//! Default destructor
		~c_ptr()
		{
			if (m_free) dealloc();
		}
		//! Allocates \a m_alloc_size bytes
		void alloc()
		{
			sm_alloc(m_ptr, T, m_alloc_size);
			//if ((m_ptr = (T*)MEM_alloc(m_alloc_size * sizeof(T))) == NULL)
			//	throw psexception("Memory allocation error.");
		}
		//! Allocates \a size bytes
		void alloc(const int size)
		{
			m_alloc_size = size;
			alloc();
		}
		//! Reallocates \a m_alloc_size bytes
		void realloc()
		{
			sm_realloc(m_ptr, T, m_alloc_size);
			//if ((m_ptr = (T*)MEM_realloc(m_ptr, m_alloc_size * sizeof(T))) == NULL)
			//	throw psexception("Memory allocation error.");
		}
		//! Reallocates \a size bytes
		void realloc(const int size)
		{
			m_alloc_size = size;
			realloc();
		}
		//! Deallocates allocated memory
		void dealloc()
		{
			sm_free(m_ptr);
			//if (m_ptr != NULL)
			//{
			//	MEM_free(m_ptr);
			//	m_ptr = NULL;
			//}
		}
		//! Add am object to this array
		void add(T anObject)
		{
			if (m_size == m_alloc_size - 1)
			{
				realloc(m_alloc_size + m_alloc_chunk);
			}
			m_ptr[m_size] = anObject;
			m_size++;
		}
		//! Append an array to this array
		void add(int size, T *anArray)
		{
			for (int i = 0; i < size; i++)
			{
				add(anArray[i]);
			}
		}
		//! Get object in array
		T val(const int i) { return m_ptr[i]; }
		//! Get pointer to array
		T* ptr() { return m_ptr; }
		//! Get pointer address to pointer of \a i element in array
		T* ptr(const int i) { return &m_ptr[i]; }
		//! Get pointer address to pointer of first element in array
		T** pptr() { return &m_ptr; }
		//! Get size of data in array
		int len() { return m_size; }
		//! Get pointer to size of data in array
		int* plen() { return &m_size; }
		//! Whether to free memory upon destruction
		void set_free(const bool value) { m_free = value; }
	};

	//! Template class that contains a two-dimension array of any c-style type \a T.
	template<typename T>
	class c_pptr
	{
	private:
		T**	m_ptr;				//!< Pointer to type contained
		int m_size;				//!< Size of data contained in array
		int m_alloc_chunk;		//!< Block size to use for memory allocation
		int m_alloc_size;		//!< Total size of array
		bool m_free;			//!< Whether destructor shall free memory or not
		bool m_free_container_only;	//!< Whether to free only the first-level array

		//! Called from the different constructor methods
		void construct()
		{
			m_ptr = NULL;
			m_size = 0;
			m_alloc_chunk = 10;
			m_alloc_size = 0;
			m_free = true;
			m_free_container_only = false;
		}

	public:
		//! Default constructor
		c_pptr()
		{
			construct();
		}
		//! Constructor that sets the initial array size and chunk size
		c_pptr(int initial)
		{
			construct();
			m_alloc_size = initial;
			alloc();
		}
		//! Constructor that sets the initial array size and chunk size
		c_pptr(int initial, int chunk)
		{
			construct();
			m_alloc_chunk = chunk;
			m_alloc_size = initial;
			alloc();
		}
		//! Constructor that sets the pointer to point to \a ptr
		c_pptr(const T** ptr)
		{
			construct();
			m_ptr = ptr;
		}
		//! Default destructor
		~c_pptr()
		{
			if (m_free) dealloc();
		}
		//! Allocates \a m_alloc_size bytes
		void alloc()
		{
			sm_alloc(m_ptr, T*, m_alloc_size);
			//if ((m_ptr = (T**)MEM_alloc(m_alloc_size * sizeof(T*))) == NULL)
			//	throw psexception("Memory allocation error.");
			for (int i = 0; i < m_alloc_size; i++) m_ptr[i] = NULL;
		}
		//! Allocates \a size bytes
		void alloc(const int size)
		{
			m_alloc_size = size;
			alloc();
		}
		//! Allocates \a size bytes at position \a pos in array
		void alloc(const int pos, const int size)
		{
			sm_alloc(m_ptr[pos], T, size);
			//if ((m_ptr[pos] = (T*)MEM_alloc(size * sizeof(T))) == NULL)
			//	throw psexception("Memory allocation error.");
		}
		//! Reallocates \a m_alloc_size bytes
		void realloc()
		{
			sm_realloc(m_ptr, T*, m_alloc_size);
			//if ((m_ptr = (T**)MEM_realloc(m_ptr, m_alloc_size * sizeof(T*))) == NULL)
			//	throw psexception("Memory allocation error.");
			for (int i = m_size; i < m_alloc_size; i++) m_ptr[i] = NULL;
		}
		//! Reallocates \a size bytes
		void realloc(const int size)
		{
			m_alloc_size += size;
			realloc();
		}
		//! Reallocates \a size bytes at position \a pos of array
		void realloc(const int pos, const int size)
		{
			sm_realloc(m_ptr[pos], T, size);
			//if ((m_ptr = (T*)MEM_realloc(m_ptr, size * sizeof(T))) == NULL)
			//	throw psexception("Memory allocation error.");
		}
		//! Deallocate memory of all elements
		void dealloc()
		{
			if (m_size > m_alloc_size)
				m_alloc_size = m_size;

			if (m_ptr != NULL)
			{
				if (!m_free_container_only)
				{
					for (int i = 0; i < m_alloc_size; i++)
					{
						dealloc(i);
					}
				}
				sm_free(m_ptr);
			}
		}
		//! Deallocate memory of element \a pos
		void dealloc(const int pos)
		{
			sm_free(m_ptr[pos]);
			//if (m_ptr[pos] != NULL)
			//{
			//	MEM_free(m_ptr[pos]);
			//	m_ptr[pos] = NULL;
			//}
		}
		//! Get object in array
		T val(const int i, const int j) { return m_ptr[i][j]; }
		//! Get objects in array
		T* val(const int i) { return m_ptr[i]; }
		//! Get pointer to first element of array
		T** ptr() { return m_ptr; }
		//! Get pointer to pointer of array
		T*** pptr() { return &m_ptr; }
		//! Get length of array
		int len() { return m_size; }
		//! Get pointer to length value
		int* plen() { return &m_size; }
		//! Whether to free memory upon destruction
		void set_free(const bool value) { m_free = value; }
		//! Whether to free only first level of array upon destruction
		void set_free_container_only(const bool value) { m_free_container_only = value; }
	};
}