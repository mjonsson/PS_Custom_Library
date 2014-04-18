#pragma once

#include "ps_macro.hxx"
#include "ps_write_syslog.hxx"

/*******************************************************************************
Class Name  		: c_ptr
Description			: auto_ptr class for C types
Author				: Mattias Jonsson
Change History

  Who  | Date       | Description
=======|============|=========================================================
  MJ   | 22/01/2012 | Initial Creation
******************************************************************************/

template<typename T>
class c_ptr
{
private:
	T*	m_ptr;
	int m_size;
	bool m_free;

public:
	c_ptr()
	{
		m_ptr = NULL;
		m_size = NULL;
		m_free = true;
	}
	c_ptr(const T* ptr)
	{
		c_ptr();
		m_ptr = ptr;
	}
	~c_ptr()
	{
		if (m_free) dealloc();
	}
	void alloc()
	{
		if ((m_ptr = (T*)MEM_alloc(m_size * sizeof(T))) == NULL)
			throw psexception("Memory allocation error.");
	}
	void alloc(const int size)
	{
		m_size = size;
		alloc();
	}
	void realloc()
	{
		if ((m_ptr = (T*)MEM_realloc(m_ptr, m_size * sizeof(T))) == NULL)
			throw psexception("Memory allocation error.");
	}
	void realloc(const int size)
	{
		m_size = size;
		realloc();
	}
	void dealloc()
	{
		if (m_ptr != NULL)
		{
			//ps_write_debug("Clear ptr @ %p", m_ptr);
			MEM_free(m_ptr);
			m_ptr = NULL;
		}
	}
	T get(const int i) { return m_ptr[i]; }
	T* get() { return m_ptr; }
	void set(const int i, const T value) { m_ptr[i] = value; }
	void set(const T* value) { m_ptr = value; }
	T** get_ptr() { return &m_ptr; }
	int get_len() { return m_size; }
	void set_len(int value) { m_size = value; }
	int* get_len_ptr() { return &m_size; }
	void set_free(const bool value) { m_free = value; }
};

/*******************************************************************************
Class Name  		: c_pptr
Description			: auto_ptr class for C types
Author				: Mattias Jonsson
Change History

  Who  | Date       | Description
=======|============|=========================================================
  MJ   | 22/01/2012 | Initial Creation
******************************************************************************/

template<typename T>
class c_pptr
{
private:
	T**	m_ptr;
	int m_size;
	bool m_free;
	bool m_free_container_only;

public:
	c_pptr()
	{
		m_ptr = NULL;
		m_size = NULL;
		m_free = true;
		m_free_container_only = false;
	}
	c_pptr(const T** ptr)
	{
		c_pptr();
		m_ptr = ptr;
	}
	~c_pptr()
	{
		if (m_free) dealloc();
	}
	void alloc()
	{
		if ((m_ptr = (T**)MEM_alloc(m_size * sizeof(T*))) == NULL)
			throw psexception("Memory allocation error.");
		for (int i = 0; i < m_size; i++) m_ptr[i] = NULL;
	}
	void alloc(const int size)
	{
		m_size = size;
		alloc();
	}
	void alloc(const int pos, const int size)
	{
		if ((m_ptr[pos] = (T*)MEM_alloc(size * sizeof(T))) == NULL)
			throw psexception("Memory allocation error.");
	}
	void realloc()
	{
		if ((m_ptr = (T**)MEM_realloc(m_ptr, m_size * sizeof(T*))) == NULL)
			throw psexception("Memory allocation error.");
	}
	void realloc(const int size)
	{
		m_size = size;
		realloc();
	}
	void realloc(const int pos, const int size)
	{
		if ((m_ptr = (T*)MEM_realloc(m_ptr, size * sizeof(T))) == NULL)
			throw psexception("Memory allocation error.");
	}
	void dealloc()
	{
		if (m_ptr != NULL)
		{
			if (!m_free_container_only)
			{
				for (int i = 0; i < m_size; i++)
				{
					dealloc(i);
				}
			}
			//ps_write_debug("Clear pptr @ %p", m_ptr);
			MEM_free(m_ptr);
			m_ptr = NULL;
		}
	}

	void dealloc(const int pos)
	{
		if (m_ptr[pos] != NULL)
		{
			MEM_free(m_ptr[pos]);
			m_ptr[pos] = NULL;
		}
	}
	T get(const int y, const int x) { return m_ptr[y][x]; }
	void set(const T value, const int y, const int x) { m_ptr[y][x] = value; }
	T* get(const int i) { return m_ptr[i]; }
	T** get_ptr(const int i) { return &(m_ptr[i]); }
	void set(const T* value, const int y) { m_ptr[y] = value; }
	T** get() { return m_ptr; }
	void set(const T** value) { m_ptr = value; }
	T*** get_pptr() { return &m_ptr; }
	int get_len() { return m_size; }
	void set_len(int value) { m_size = value; }
	int* get_len_ptr() { return &m_size; }
	void set_free(const bool value) { m_free = value; }
	void set_free_container_only(const bool value) { m_free_container_only = value; }
};
