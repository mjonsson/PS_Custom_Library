#include "ps_global.hxx"

bool ps_null_or_empty(const char *str)
{
	if (str == NULL) return true;
	if (strlen(str) == 0) return true;

	return false;
}

string ps_trim(string &str)
{
	string new_str(str);

	if (str.empty())
		return new_str;

	for (string::iterator i = new_str.begin(); i != new_str.end(); ++i)
	{
		if (isspace(*i))
			new_str.erase(i);
		else
			break;
	}

	for (string::reverse_iterator i = new_str.rbegin(); i != new_str.rend(); ++i)
	{
		if (isspace(*i))
			new_str.erase((++i).base());
		else
			break;
	}

	return new_str;
}


vector<string> ps_split_str(string &split_str, const char *delims, bool remove_whitespace)
{
	char			*ptr = NULL;
	vector<string>	strVector;

	if (split_str.empty() || ps_null_or_empty(delims))
		return strVector;

	ptr = strtok((char*) split_str.c_str(), delims);
	while (ptr != NULL)
	{
		string		splitted_str(ptr);

		if (remove_whitespace)
			splitted_str = ps_trim(splitted_str);

		strVector.push_back(splitted_str);
		ptr = strtok(NULL, delims);
	}

	return strVector;
}

vector<string> ps_split_str(const char *split_str, const char *delims, bool remove_whitespace)
{
	char			*ptr = NULL;
	vector<string>	strVector;

	if (ps_null_or_empty(split_str) || ps_null_or_empty(delims))
		return strVector;

	ptr = strtok((char*) split_str, delims);
	while (ptr != NULL)
	{
		string		splitted_str(ptr);

		if (remove_whitespace)
			splitted_str = ps_trim(splitted_str);

		strVector.push_back(splitted_str);
		ptr = strtok(NULL, delims);
	}

	return strVector;
}

const char *ps_concat_str(vector<string> &str_vec, const char delim, bool remove_whitespace)
{
	string str_ret = "";

	for (vector<string>::iterator i = str_vec.begin(); i != str_vec.end(); ++i)
	{
		string str = *i;

		if (remove_whitespace)
			str = ps_trim(str);

		str_ret += str;

		if (i != str_vec.end()) str_ret += delim;
	}

	return str_ret.c_str();
}

bool ps_find_str(string &find_str, string &full_str, const char *delims, bool remove_whitespace)
{
	vector<string>		strVector;

	if (find_str.empty() || full_str.empty() || ps_null_or_empty(delims))
		return false;

	strVector = ps_split_str(full_str, delims, remove_whitespace);

	for (vector<string>::iterator i = strVector.begin(); i != strVector.end(); ++i)
	{
		if (*i == find_str)
			return true;
	}

	return false;
}

bool ps_find_str(const char *find_str, string &full_str, const char *delims, bool remove_whitespace)
{
	vector<string>		strVector;

	if (ps_null_or_empty(find_str) || full_str.empty() || ps_null_or_empty(delims))
		return false;

	strVector = ps_split_str(full_str, delims, remove_whitespace);

	for (vector<string>::iterator i = strVector.begin(); i != strVector.end(); ++i)
	{
		if (*i == find_str)
			return true;
	}

	return false;
}

bool ps_find_str(string &find_str, vector<string> &str_vec)
{
	if (find_str.empty() || str_vec.empty())
		return false;

	for (vector<string>::iterator i = str_vec.begin(); i != str_vec.end(); ++i)
	{
		if (*i == find_str)
			return true;
	}

	return false;
}

bool ps_find_str(const char *find_str, vector<string> &str_vec)
{
	if (ps_null_or_empty(find_str) || str_vec.empty())
		return false;

	for (vector<string>::iterator i = str_vec.begin(); i != str_vec.end(); ++i)
	{
		if (*i == find_str)
			return true;
	}

	return false;
}