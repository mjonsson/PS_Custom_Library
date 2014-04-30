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
		//! Constructor that also sets the initial array size to size of \a chunk
		c_ptr(int chunk)
		{
			construct();
			m_alloc_chunk = chunk;
			m_alloc_size = m_alloc_chunk;
			alloc();
		}
		//! Constructor that sets the pointer to point to \a ptr
		c_ptr(const T* ptr)
		{
			construct();
			m_ptr = ptr;
		}
		//! Default destructor
		~c_ptr()
		{
			if (m_free) dealloc();
		}
		//! Allocates \a m_alloc_size bytes
		void alloc()
		{
			if ((m_ptr = (T*)MEM_alloc(m_alloc_size * sizeof(T))) == NULL)
				throw psexception("Memory allocation error.");
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
			if ((m_ptr = (T*)MEM_realloc(m_ptr, m_alloc_size * sizeof(T))) == NULL)
				throw psexception("Memory allocation error.");
		}
		//! Reallocates \a size bytes
		void realloc(const int size)
		{
			m_alloc_size = size;
			realloc();
		}
		//! Appends \a obj of type \ T to the array
		void append(T obj)
		{
			if (m_size >= m_alloc_size)
			{
				m_alloc_size = m_size + m_alloc_chunk;
				realloc(m_alloc_size);
			}
			m_ptr[m_size] = obj;
			m_size++;
		}
		//! Deallocates allocated memory
		void dealloc()
		{
			if (m_ptr != NULL)
			{
				MEM_free(m_ptr);
				m_ptr = NULL;
			}
		}
		//! Get element \a i from array
		T get(const int i) { return m_ptr[i]; }
		//! Get pointer to array
		T* get() { return m_ptr; }
		//! Get pointer to element \a i in array
		T* get_ptr(const int i) { return &(m_ptr[i]); }
		//! Set array element \a i to value of \a T
		void set(const int i, const T value) { m_ptr[i] = value; }
		//! Set pointer to value of pointer of type \a T
		void set(const T* value) { m_ptr = value; }
		//! Get pointer address to pointer of first element in array
		T** get_ptr() { return &m_ptr; }
		//! Get size of data in array
		int get_len() { return m_size; }
		//! Set size of data in array to \a value
		void set_len(int value) { m_size = value; }
		//! Get pointer to size of data in array
		int* get_len_ptr() { return &m_size; }
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
		//! Constructor that also sets the initial array size
		c_pptr(int chunk)
		{
			construct();
			m_alloc_chunk = chunk;
			m_alloc_size = m_alloc_chunk;
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
			if ((m_ptr = (T**)MEM_alloc(m_alloc_size * sizeof(T*))) == NULL)
				throw psexception("Memory allocation error.");
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
			if ((m_ptr[pos] = (T*)MEM_alloc(size * sizeof(T))) == NULL)
				throw psexception("Memory allocation error.");
		}
		//! Reallocates \a m_alloc_size bytes
		void realloc()
		{
			if ((m_ptr = (T**)MEM_realloc(m_ptr, m_alloc_size * sizeof(T*))) == NULL)
				throw psexception("Memory allocation error.");
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
			if ((m_ptr = (T*)MEM_realloc(m_ptr, size * sizeof(T))) == NULL)
				throw psexception("Memory allocation error.");
		}
		//! Appends \a obj of type const char * to array
		void append(const char *obj)
		{
			if (m_size >= m_alloc_size)
			{
				m_alloc_size = m_size + m_alloc_chunk;
				realloc();
			}
			alloc(m_size, sizeof(char) * (strlen(obj) + 1));
			tc_strcpy(m_ptr[m_size], obj);
			m_size++;
		}
		//! Appends \a obj of type \a T to array
		void append(T *obj)
		{
			if (m_size >= m_alloc_size)
			{
				m_alloc_size = m_size + m_alloc_chunk;
				realloc();
			}
			m_ptr[m_size] = obj;
			m_size++;
		}
		//! Appends a copy of \a obj of type \a T to array
		/*!
		 *  \note Make sure to input proper size of \a obj
		 */
		void append(const T* obj, int size)
		{
			if (m_size >= m_alloc_size)
			{
				m_alloc_size = m_size + m_alloc_chunk;
				realloc(m_alloc_size);
			}
			alloc(m_size, sizeof(T) * size);
			memcpy(m_ptr[m_size], obj, sizeof(T) * size);
			m_size++;
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
				MEM_free(m_ptr);
				m_ptr = NULL;
			}
		}
		//! Deallocate memory of element \a pos
		void dealloc(const int pos)
		{
			if (m_ptr[pos] != NULL)
			{
				MEM_free(m_ptr[pos]);
				m_ptr[pos] = NULL;
			}
		}
		//! Get value of type \a T of element \a x and \a y
		T get(const int y, const int x) { return m_ptr[y][x]; }
		//! Set value of type \a T of element \a x and \a y
		void set(const T value, const int y, const int x) { m_ptr[y][x] = value; }
		//! Get pointer of type \a T at element \a i
		T* get(const int i) { return m_ptr[i]; }
		//! Set pointer of type \a T at position \a y
		void set(const T* value, const int y) { m_ptr[y] = value; }
		//! Get pointer to first element of array
		T** get_ptr() { return m_ptr; }
		//! Get pointer to element \a i of array
		T** get_ptr(const int i) { return &(m_ptr[i]); }
		//! Set pointer of type \a T
		void set(const T** value) { m_ptr = value; }
		//! Get pointer to pointer of array
		T*** get_pptr() { return &m_ptr; }
		//! Get length of array
		int get_len() { return m_size; }
		//! Set length of array to \a value
		void set_len(int value) { m_size = value; }
		//! Get pointer to length value
		int* get_len_ptr() { return &m_size; }
		//! Whether to free memory upon destruction
		void set_free(const bool value) { m_free = value; }
		//! Whether to free only first level of array upon destruction
		void set_free_container_only(const bool value) { m_free_container_only = value; }
	};
}