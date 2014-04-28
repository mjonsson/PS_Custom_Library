#include "ps_global.hxx"

using namespace ps;

bool ps::null_or_empty(const char *str)
{
	if (str == NULL) return true;
	if (strlen(str) == 0) return true;

	return false;
}

void ps::trim_left(string &str)
{
	if (str.empty())
		return;

	for (string::iterator i = str.begin(); i != str.end(); ++i)
	{
		if (isspace(*i))
			str.erase(i);
		else
			break;
	}
}

void ps::trim_right(string &str)
{
	if (str.empty())
		return;

	for (string::reverse_iterator i = str.rbegin(); i != str.rend(); ++i)
	{
		if (isspace(*i))
			str.erase((++i).base());
		else
			break;
	}
}

void ps::trim(string &str)
{
	if (str.empty())
		return;

	trim_left(str);
	trim_right(str);
}

vector<string> ps::split_str(string &split_str, const char *delims, bool remove_whitespace)
{
	char			*ptr = NULL;
	vector<string>	strVector;

	if (split_str.empty() || null_or_empty(delims))
		return strVector;

	ptr = strtok((char*) split_str.c_str(), delims);
	while (ptr != NULL)
	{
		string		splitted_str(ptr);

		if (remove_whitespace)
			trim(splitted_str);

		strVector.push_back(splitted_str);
		ptr = strtok(NULL, delims);
	}

	return strVector;
}

vector<string> ps::split_str(const char *split_str, const char *delims, bool remove_whitespace)
{
	char			*ptr = NULL;
	vector<string>	strVector;

	if (null_or_empty(split_str) || null_or_empty(delims))
		return strVector;

	ptr = strtok((char*) split_str, delims);
	while (ptr != NULL)
	{
		string		splitted_str(ptr);

		if (remove_whitespace)
			trim(splitted_str);

		strVector.push_back(splitted_str);
		ptr = strtok(NULL, delims);
	}

	return strVector;
}

string ps::concat_str(vector<string> &str_vec, const char delim, bool remove_whitespace)
{
	string str_ret = "";

	for (vector<string>::iterator i = str_vec.begin(); i != str_vec.end(); ++i)
	{
		string str = *i;

		if (remove_whitespace)
			trim(str);

		str_ret += str;

		if (i != --(str_vec.end())) str_ret += delim;
	}

	return str_ret;
}

bool ps::find_str(string &find_str, string &full_str, const char *delims, bool remove_whitespace)
{
	vector<string>		strVector;

	if (find_str.empty() || full_str.empty() || null_or_empty(delims))
		return false;

	strVector = split_str(full_str, delims, remove_whitespace);

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

	if (null_or_empty(find_str) || full_str.empty() || null_or_empty(delims))
		return false;

	strVector = split_str(full_str, delims, remove_whitespace);

	for (vector<string>::iterator i = strVector.begin(); i != strVector.end(); ++i)
	{
		if (*i == find_str)
			return true;
	}

	return false;
}

bool ps::find_str(string &find_str, vector<string> &str_vec)
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

bool ps::find_str(const char *find_str, vector<string> &str_vec)
{
	if (null_or_empty(find_str) || str_vec.empty())
		return false;

	for (vector<string>::iterator i = str_vec.begin(); i != str_vec.end(); ++i)
	{
		if (*i == find_str)
			return true;
	}

	return false;
}