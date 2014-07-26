/*!
 * \file ps_string.hxx
 * \date 2014/05/02 23:01
 *
 * \author Mattias Jonsson (jonssonm)
 * Contact: jonsson.mattias@siemens.com
 *
 * \brief Various string functions
 *
*/

#pragma once

namespace ps
{
	//! Check whether \a str is either null or empty
	bool null_or_empty(const char *str);
	//! Remove whitespace characters from string \a str
	void trim(string &str);
	//! Remove starting whitespace characters from string \a str
	void trim_left(string &str);
	//! Remove ending whitespace characters from string \a str
	void trim_right(string &str);
	//! Return to lower version of string
	void to_lower(string &str);
	//! Return to upper version of string
	void to_upper(string &str);
	
	bool match_string(const char *find_str, const char *str);
	bool match_string(const char *find_str, string &str);
	bool match_string(string &find_str, const char *str);
	bool match_string(string &find_str, string &str);
	bool match_string(const char *find_str, const char *str, bool reg_ex);
	bool match_string(string &find_str, const char *str, bool reg_ex);

	//! Match string either exact or by regex
	bool match_string(const char *find_str, string &str, bool reg_ex);
	//! Match string either exact or by regex
	bool match_string(string &find_str, string &str, bool reg_ex);
	//! Split string \a split_str on delimiters \a delims to vector \a str_vector. Optionally, whitespace can be removed with flag \a remove_whitespace
	void split_string(string &split_str, const char *delims, bool remove_whitespace, vector<string> &str_vector);
	//! Split C-style string \a split_str on delimiters \a delims to vector \a str_vector. Optionally, whitespace can be removed with flag \a remove_whitespace
	void split_string(const char *split_str, const char *delims, bool remove_whitespace, vector<string> &str_vector);
	//! Concatenate vector \a str_vec on delimiter \a delim and return string type. Optionally, whitespace can be removed with flag \a remove_whitespace
	string concat_string(vector<string> &str_vec, const char delim, bool remove_whitespace);
	//! Find C-style string \a find_str in \a full_str (delimiter = ",", remove_whitespace = false, reg_ex = false)
	bool find_string(const char *find_str, const char *full_str);
	//! Find C-style string \a find_str in \a full_str (delimiter = ",", remove_whitespace = false, reg_ex = false)
	bool find_string(const char *find_str, string &full_str);
	//! Find string \a find_str in \a full_str (delimiter = ",", remove_whitespace = false, reg_ex = false)
	bool find_string(string &find_str, string &full_str);
	//! Find string \a find_str in string \a full_str with delimiters \a delims. Whitespace can be removed with flag \a remove_whitespace. Regular expression search with flag \a reg_ex.
	bool find_string(string &find_str, string &full_str, const char *delims, bool remove_whitespace, bool reg_ex);
	//! Find C-style string \a find_str in string \a full_str with delimiters \a delims. Whitespace can be removed with flag \a remove_whitespace. Regular expression search with flag \a reg_ex.
	bool find_string(const char *find_str, string &full_str, const char *delims, bool remove_whitespace, bool reg_ex);
	//! Find C-style string \a find_str in C-style string \a full_str with delimiters \a delims. Whitespace can be removed with flag \a remove_whitespace. Regular expression search with flag \a reg_ex.
	bool find_string(const char *find_str, const char *full_str, const char *delims, bool remove_whitespace, bool reg_ex);
	//! Find C-style string \a find_str in \a full_str (reg_ex = false)
	bool find_string(const char *find_str, vector<string> &str_vec);
	//! Find string \a find_str in \a full_str (reg_ex = false)
	bool find_string(string &find_str, vector<string> &str_vec);
	//! Find string \a find_str in vector \a str_vec. Regular expression search with flag \a reg_ex.
	bool find_string(string &find_str, vector<string> &str_vec, bool reg_ex);
	//! Find C-style string \a find_str in vector \a str_vec. Regular expression search with flag \a reg_ex.
	bool find_string(const char *find_str, vector<string> &str_vec, bool reg_ex);
	//! Convert vector<string> to char**
	vector<const char*> vec_string_to_char(vector<string> &str_vec);
}