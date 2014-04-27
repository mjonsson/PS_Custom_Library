#pragma once

class str_format
{
private:
	char	*m_ptr;

public:
	void construct()
	{
		m_ptr = NULL;
	}
	str_format()
	{
		construct();
	}
	str_format(const char *fmt, ...)
	{
		va_list args;

		construct();

		alloc(strlen(fmt) + 1024);

		va_start(args, fmt);
		vsprintf(m_ptr, fmt, args);
		va_end(args);
	}
	~str_format()
	{
		dealloc();
	}
	void alloc(const int size)
	{
		if ((m_ptr = (char*) MEM_alloc(size * sizeof(char))) == NULL)
			throw psexception("Memory allocation error.");
	}
	void dealloc()
	{
		if (m_ptr != NULL)
		{
			MEM_free(m_ptr);
			m_ptr = NULL;
		}
	}
	const char* get() { return m_ptr; }
};

bool ps_null_or_empty(const char *str);
vector<string> ps_split_str(string &split_str, const char *delims, bool remove_whitespace);
vector<string> ps_split_str(const char *split_str, const char *delims, bool remove_whitespace);
const char *ps_concat_str(vector<string> &str_vec, const char delim, bool remove_whitespace);
bool ps_find_str(string &find_str, string &full_str, const char *delims, bool remove_whitespace);
bool ps_find_str(const char *find_str, string &full_str, const char *delims, bool remove_whitespace);
bool ps_find_str(const char *find_str, const char *full_str, const char *delims, bool remove_whitespace);
bool ps_find_str(string &find_str, vector<string> &str_vec);
bool ps_find_str(const char *find_str, vector<string> &str_vec);