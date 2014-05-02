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