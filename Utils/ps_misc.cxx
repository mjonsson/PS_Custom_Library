#include "ps_global.hxx"

using namespace ps;

h_args::h_args(TC_argument_list_t *args)
{
	char	*pszArg = NULL;

	while ((pszArg = TC_next_argument(args)) != NULL )
	{
		c_ptr<char>		flag, value;
		vector<string>	argVector;

		itk(ITK_ask_argument_named_value(pszArg, flag.get_ptr(), value.get_ptr()));

		ArgsMap::iterator it;
		if ((it = argsMap.find(string(flag.get()))) != argsMap.end())
			continue;

		// If separated string, split it and remove whitespace
		split_str((const char*)value.get(), ",;:", true, argVector);
		// Insert vector into map
		argsMap.insert(ArgPair(string(flag.get()), argVector));
	}
}
string h_args::getStr(string flag)
{
	string	arg;
	ArgsMap::iterator it;

	if ((it = argsMap.find(flag)) != argsMap.end())
	{
		arg = *(it->second.begin());
	}

	return arg;
}
bool h_args::getFlag(string flag)
{
	if (argsMap.find(flag) != argsMap.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool h_args::getStr(string flag, string &value)
{
	ArgsMap::iterator it;

	if ((it = argsMap.find(flag)) != argsMap.end())
	{
		value = it->second[0];
		if (value.length() > 0)
			return true;
	}
	return false;
}
bool h_args::getVec(string flag, vector<string> &value)
{
	ArgsMap::iterator it;

	if ((it = argsMap.find(flag)) != argsMap.end())
	{
		value = it->second;
		if (value.size() > 0)
			return true;
	}
	return false;
}
bool h_args::getFlag(string flag, logical &value)
{
	if (argsMap.find(flag) != argsMap.end())
	{
		value = true;
		return true;
	}
	else
	{
		value = false;
		return false;
	}
}
bool h_args::getInt(string flag, int &value)
{
	ArgsMap::iterator it;

	if ((it = argsMap.find(flag)) != argsMap.end())
	{
		value = atoi(it->second[0].c_str());
		return true;
	}
	return false;
}
int h_args::size()
{
	return argsMap.size();
}

bool ps::null_or_empty(const char *str)
{
	if (str == NULL) return true;
	else if (strlen(str) == 0) return true;
	else return false;
}

void ps::trim_left(string &str)
{
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

		if (splitted_str.length() > 0)
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

		if (splitted_str.length() > 0)
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

		if (i != --(str_vec.end())) concStr += delim;
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