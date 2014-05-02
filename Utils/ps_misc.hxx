/*!
 * \file ps_misc.hxx
 * \date 2014/04/30 22:11
 *
 * \author Mattias Jonsson (jonssonm)
 * Contact: jonsson.mattias@siemens.com
 *
 * \brief Various utility functions.
 *
*/

#pragma once

namespace ps
{
	typedef map<string, vector<string>> ArgsMap;
	typedef pair<string, vector<string>> ArgPair;

	//! Class used for constructing formatted, parametrized C-style strings
	class str_format
	{
	private:
		char	*m_ptr;

		//! Called by all constructors
		void construct()
		{
			m_ptr = NULL;
		}

		//! Allocate \a size bytes memory
		void alloc(const int size)
		{
			if ((m_ptr = (char*) MEM_alloc(size)) == NULL)
				throw psexception("Memory allocation error.");
		}
		//! Reallocate \a size bytes memory
		void realloc(const int size)
		{
			if ((m_ptr = (char*) MEM_realloc(m_ptr, size)) == NULL)
				throw psexception("Memory allocation error.");
		}
		//! Deallocate memory
		void dealloc()
		{
			if (m_ptr != NULL)
			{
				MEM_free(m_ptr);
				m_ptr = NULL;
			}
		}
		//! Default constructor
		str_format()
		{
			construct();
		}
	public:
		//! Construct and initialize a formatted, parametrized C-style string
		str_format(const char *fmt, ...)
		{
			va_list args;

			construct();

			int bufsiz = strlen(fmt) + 512;
			alloc(bufsiz);
			
			va_start(args, fmt);
			int need = vsnprintf(m_ptr, bufsiz, fmt, args) + 1;
			if (need > bufsiz)
			{
				realloc(need);
				vsnprintf(m_ptr, need, fmt, args);
			}
			va_end(args);
		}
		//! Destructor method
		~str_format()
		{
			dealloc();
		}
		//! Return value of C-style string
		const char* get() { return m_ptr; }
	};

	class h_args
	{
	private:
		ArgsMap argsMap;

	public:
		//! Construct the ArgsMap map
		h_args(TC_argument_list_t *args);
		//! Default destructor
		~h_args() { };
		//! Get and return a string by value
		string getStr(string flag);
		//! Get and return a logical by value
		bool getFlag(string flag);
		//! Get and return a string by reference
		bool getStr(string flag, string &value);
		//! Get and return a vector of string by reference
		bool getVec(string flag, vector<string> &value);
		//! Get and return a logical by reference
		bool getFlag(string flag, logical &value);
		//! Get and return a logical by reference
		bool getInt(string flag, int &value);
		//! Return size of ArgsMap
		int size();
	};


	//! Check whether \a str is either null or empty
	bool null_or_empty(const char *str);
	//! Remove whitespace characters from string \a str
	void trim(string &str);
	//! Remove starting whitespace characters from string \a str
	void trim_left(string &str);
	//! Remove ending whitespace characters from string \a str
	void trim_right(string &str);
	//! Split string \a split_str on delimiters \a delims to vector \a str_vector. Optionally, whitespace can be removed with flag \a remove_whitespace
	void split_str(string &split_str, const char *delims, bool remove_whitespace, vector<string> &str_vector);
	//! Split C-style string \a split_str on delimiters \a delims to vector \a str_vector. Optionally, whitespace can be removed with flag \a remove_whitespace
	void split_str(const char *split_str, const char *delims, bool remove_whitespace, vector<string> &str_vector);
	//! Concatenate vector \a str_vec on delimiter \a delim and return string type. Optionally, whitespace can be removed with flag \a remove_whitespace
	string concat_str(vector<string> &str_vec, const char delim, bool remove_whitespace);
	//! Find string \a find_str in string \a full_str with delimiters \a delims. Optionally, whitespace can be removed with flag \a remove_whitespace
	bool find_str(string &find_str, string &full_str, const char *delims, bool remove_whitespace);
	//! Find C-style string \a find_str in string \a full_str with delimiters \a delims. Optionally, whitespace can be removed with flag \a remove_whitespace
	bool find_str(const char *find_str, string &full_str, const char *delims, bool remove_whitespace);
	//! Find C-style string \a find_str in C-style string \a full_str with delimiters \a delims. Optionally, whitespace can be removed with flag \a remove_whitespace
	bool find_str(const char *find_str, const char *full_str, const char *delims, bool remove_whitespace);
	//! Find string \a find_str in vector \a str_vec
	bool find_str(string &find_str, vector<string> &str_vec);
	//! Find C-style string \a find_str in vector \a str_vec
	bool find_str(const char *find_str, vector<string> &str_vec);
	//! Convert vector<string> to char**
	vector<const char*> vec_string_to_char(vector<string> &str_vec);
}