#include "ps_global.hxx"

using namespace ps;

h_args::h_args(TC_argument_list_t *args)
{
	char	*pszArg = NULL;

	while ((pszArg = TC_next_argument(args)) != NULL )
	{
		c_ptr<char>		flag, value;
		vector<string>	argVector;

		itk(ITK_ask_argument_named_value(pszArg, flag.pptr(), value.pptr()));

		// Jump to next argument if current already exist in map
		ArgsMap::iterator it;
		if ((it = argsMap.find(string(flag.ptr()))) != argsMap.end())
			continue;

		// If separated string, split it and remove whitespace
		split_string((const char*)value.ptr(), ",", true, argVector);
		// Insert vector into map
		argsMap.insert(ArgPair(string(flag.ptr()), argVector));
	}
}
string h_args::getStr(string flag)
{
	string	arg;
	ArgsMap::iterator it;

	if ((it = argsMap.find(flag)) != argsMap.end())
	{
		arg = it->second[0];
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