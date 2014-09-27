#include "ps_global.hxx"
#include <regex>


using namespace ps;

bool ps::null_or_empty(const char *str)
{
	if (str == NULL) return true;
	else if (strlen(str) == 0) return true;
	else return false;
}

void ps::trim_left(string &str)
{
	string::iterator i = str.begin();

	while (i != str.end())
	{
		if (isspace(*i))
		{
			i = str.erase(i);
		}
		else
			break;
	}
}

void ps::trim_right(string &str)
{
	string::iterator i = --str.end();

	while (i != str.end())
	{
		if (isspace(*i))
		{
			i = --str.erase(i);
		}
		else
			break;
	}
}

void ps::trim_left(string &str, char ch)
{
	string::iterator i = str.begin();

	while (i != str.end())
	{
		if (*i == ch)
		{
			i = str.erase(i);
		}
		else
			break;
	}
}

void ps::trim_right(string &str, char ch)
{
	string::iterator i = --str.end();

	while (i != str.end())
	{
		if (*i == ch)
		{
			i = --str.erase(i);
		}
		else
			break;
	}
}

void ps::trim(string &str)
{
	trim_left(str);
	trim_right(str);
}

void ps::to_lower(string &str)
{
	for (string::iterator i = str.begin(); i < str.end(); ++i)
	{
		*i = tolower(*i);
	}
}

void ps::to_upper(string &str)
{
	for (string::iterator i = str.begin(); i < str.end(); ++i)
	{
		*i = toupper(*i);
	}
}

void ps::split_string(string &split_str, const char *delims, bool remove_whitespace, vector<string> &str_vector)
{
	char			*ptr = NULL;

	if (split_str.empty() || null_or_empty(delims))
		return;

	split_string(split_str.c_str(), delims, remove_whitespace, str_vector);
}

void ps::split_string(const char *split_str, const char *delims, bool remove_whitespace, vector<string> &str_vector)
{
	char			*ptr = NULL;

	if (null_or_empty(split_str) || null_or_empty(delims))
		return;

	ptr = strtok((char*) split_str, delims);
	while (ptr != NULL)
	{
		string		splitted_str(ptr);

		if (remove_whitespace)
			trim(splitted_str);

		str_vector.push_back(splitted_str);

		ptr = strtok(NULL, delims);
	}
}

string ps::concat_string(vector<string> &str_vec, const char delim, bool remove_whitespace)
{
	string concStr;

	for (vector<string>::iterator i = str_vec.begin(); i != str_vec.end(); ++i)
	{
		string str = *i;

		if (remove_whitespace)
			trim(str);

		concStr += str;

		if (i != --str_vec.end()) concStr += delim;
	}

	return concStr;
}

// Match string without reg_ex
bool ps::match_string(const char *find_str, string &str)
{
	return match_string(find_str, str.c_str());
}

bool ps::match_string(string &find_str, const char *str)
{
	return match_string(find_str.c_str(), str);
}

bool ps::match_string(string &find_str, string &str)
{
	return match_string(find_str.c_str(), str.c_str());
}

bool ps::match_string(const char *find_str, const char *str)
{
		if (tc_strcmp(find_str, str) == 0)
			return true;
		else
			return false;
}

// Match string with reg_ex
bool ps::match_string_rx(const char *find_str, const char *str)
{
	return match_string_rx(string(find_str), string(str));
}

bool ps::match_string_rx(const char *find_str, string &str)
{
	return match_string_rx(string(find_str), str);
}

bool ps::match_string_rx(string &find_str, const char *str)
{
	return match_string_rx(find_str, string(str));
}

bool ps::match_string_rx(string &find_str, string &str)
{
		regex rx(find_str);
		
		return regex_match(str.begin(), str.end(), rx);
}

// Find string in string without regex
bool ps::find_string(const char *find_str, const char *full_str, bool remove_whitespace)
{
	return find_string(find_str, full_str, ",", remove_whitespace);
}

bool ps::find_string(const char *find_str, string &full_str, bool remove_whitespace)
{
	return find_string(find_str, full_str.c_str(), ",", remove_whitespace);
}

bool ps::find_string(string &find_str, string &full_str, bool remove_whitespace)
{
	return find_string(find_str.c_str(), full_str.c_str(), ",", remove_whitespace);
}

bool ps::find_string(const char *find_str, const char *full_str, const char *delims, bool remove_whitespace)
{
	vector<string>		strVector;

	split_string(full_str, delims, remove_whitespace, strVector);

	return find_string(find_str, strVector);
}

// Find string without reg_ex in vector
bool ps::find_string(string &find_str, vector<string> &str_vec)
{
	return find_string(find_str.c_str(), str_vec);
}

bool ps::find_string(const char *find_str, vector<string> &str_vec)
{
	for (vector<string>::iterator i = str_vec.begin(); i != str_vec.end(); ++i)
	{
		if (match_string(find_str, *i))
			return true;
	}

	return false;
}

// Find string in string with reg_ex
bool ps::find_string_rx(const char *find_str, const char *full_str, const char *delims, bool remove_whitespace)
{
	return find_string_rx(string(find_str), full_str, delims, remove_whitespace);
}

bool ps::find_string_rx(string &find_str, string &full_str, const char *delims, bool remove_whitespace)
{
	return find_string_rx(find_str, full_str.c_str(), delims, remove_whitespace);
}

bool ps::find_string_rx(const char *find_str, string &full_str, const char *delims, bool remove_whitespace)
{
	return find_string_rx(string(find_str), full_str.c_str(), delims, remove_whitespace);
}

bool ps::find_string_rx(string &find_str, const char *full_str, const char *delims, bool remove_whitespace)
{
	vector<string>		strVector;

	split_string(full_str, delims, remove_whitespace, strVector);

	return find_string_rx(find_str, strVector);
}

// Find string with reg_ex in vector
bool ps::find_string_rx(const char *find_str, vector<string> &str_vec)
{
	return find_string_rx(string(find_str), str_vec);
}

bool ps::find_string_rx(string &find_str, vector<string> &str_vec)
{
	for (vector<string>::iterator i = str_vec.begin(); i != str_vec.end(); ++i)
	{
		if (match_string_rx(find_str, *i))
			return true;
	}

	return false;
}

vector<const char*> ps::vec_string_to_char(vector<string> &str_vec)
{
	vector<const char*>	char_vec;

	for (vector<string>::iterator it = str_vec.begin(); it != str_vec.end(); ++it)
	{
		char_vec.push_back(it->c_str());
	}

	return char_vec;
}