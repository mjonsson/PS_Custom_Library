#include "ps_global.hxx"

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

void ps::split_str(string &split_str, const char *delims, bool remove_whitespace, vector<string> &str_vector)
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

void ps::split_str(const char *split_str, const char *delims, bool remove_whitespace, vector<string> &str_vector)
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

string ps::concat_str(vector<string> &str_vec, const char delim, bool remove_whitespace)
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

bool ps::find_str(string &find_str, string &full_str, const char *delims, bool remove_whitespace)
{
	vector<string>		strVector;

	split_str(full_str, delims, remove_whitespace, strVector);

	for (vector<string>::iterator i = strVector.begin(); i != strVector.end(); ++i)
	{
		if (*i == find_str)
			return true;
	}

	return false;
}

bool ps::find_str(const char *find_str, string &full_str, const char *delims, bool remove_whitespace)
{
	vector<string>		strVector;

	split_str(full_str, delims, remove_whitespace, strVector);

	for (vector<string>::iterator i = strVector.begin(); i != strVector.end(); ++i)
	{
		if (*i == find_str)
			return true;
	}

	return false;
}

bool ps::find_str(string &find_str, vector<string> &str_vec)
{
	for (vector<string>::iterator i = str_vec.begin(); i != str_vec.end(); ++i)
	{
		if (*i == find_str)
			return true;
	}

	return false;
}

bool ps::find_str(const char *find_str, vector<string> &str_vec)
{
	for (vector<string>::iterator i = str_vec.begin(); i != str_vec.end(); ++i)
	{
		if (*i == find_str)
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