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

	ptr = strtok((char*) split_str.c_str(), delims);
	while (ptr != NULL)
	{
		string		splitted_str(ptr);

		if (remove_whitespace)
			trim(splitted_str);

		str_vector.push_back(splitted_str);

		ptr = strtok(NULL, delims);
	}
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

bool ps::match_string(const char *find_str, const char *str)
{
	return match_string(find_str, str, false);
}

bool ps::match_string(const char *find_str, string &str)
{
	return match_string(find_str, str, false);
}

bool ps::match_string(string &find_str, string &str)
{
	return match_string(find_str, str, false);
}

bool ps::match_string(string &find_str, const char *str)
{
	return match_string(find_str, str, false);
}

bool ps::match_string(const char *find_str, const char *str, bool reg_ex)
{
	if (!reg_ex)
	{
		if (tc_strcmp(find_str, str) == 0)
			return true;
		else
			return false;
	}
	else
	{
		string fi_str(find_str);
		string fu_str(str);
	
		return match_string(fi_str, fu_str, reg_ex);
	}
}

bool ps::match_string(const char *find_str, string &str, bool reg_ex)
{
	if (!reg_ex)
	{
		if (tc_strcmp(find_str, str.c_str()) == 0)
			return true;
		else
			return false;
	}
	else
	{
		string f_str(find_str);
	
		return match_string(f_str, str, reg_ex);
	}
}

bool ps::match_string(string &find_str, const char *str, bool reg_ex)
{
	if (!reg_ex)
	{
		if (tc_strcmp(find_str.c_str(), str) == 0)
			return true;
		else
			return false;
	}
	else
	{
		string fu_str(str);
	
		return match_string(find_str, fu_str, reg_ex);
	}
}

bool ps::match_string(string &find_str, string &str, bool reg_ex)
{
	if (reg_ex)
	{
		regex rx(find_str);
		return regex_match(str.begin(), str.end(), rx);
	}
	else
	{
		if (find_str == str)
			return true;
		else
			return false;
	}
}

bool ps::find_string(const char *find_str, const char *full_str)
{
	return find_string(find_str, full_str, ",", false, false);
}

bool ps::find_string(const char *find_str, string &full_str)
{
	return find_string(find_str, full_str, ",", false, false);
}

bool ps::find_string(string &find_str, string &full_str)
{
	return find_string(find_str, full_str, ",", false, false);
}

bool ps::find_string(const char *find_str, const char *full_str, const char *delims, bool remove_whitespace, bool reg_ex)
{
	string fi_str(find_str);
	string fu_str(full_str);

	return find_string(fi_str, fu_str, delims, remove_whitespace, reg_ex);
}

bool ps::find_string(const char *find_str, string &full_str, const char *delims, bool remove_whitespace, bool reg_ex)
{
	string f_str(find_str);

	return find_string(f_str, full_str, delims, remove_whitespace, reg_ex);
}

bool ps::find_string(string &find_str, string &full_str, const char *delims, bool remove_whitespace, bool reg_ex)
{
	vector<string>		strVector;
	regex ex;

	split_string(full_str, delims, remove_whitespace, strVector);

	for (vector<string>::iterator i = strVector.begin(); i != strVector.end(); ++i)
	{
		if (match_string(find_str, *i, reg_ex))
			return true;
	}

	return false;
}

bool ps::find_string(const char *find_str, vector<string> &str_vec)
{
	string f_str(find_str);

	return find_string(f_str, str_vec);
}

bool ps::find_string(string &find_str, vector<string> &str_vec)
{
	return find_string(find_str, str_vec, false);
}

bool ps::find_string(const char *find_str, vector<string> &str_vec, bool reg_ex)
{
	string f_str(find_str);

	return find_string(f_str, str_vec, reg_ex);
}

bool ps::find_string(string &find_str, vector<string> &str_vec, bool reg_ex)
{
	for (vector<string>::iterator i = str_vec.begin(); i != str_vec.end(); ++i)
	{
		if (match_string(find_str, *i, reg_ex))
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