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

		string flagStr(flag.ptr());
		// Jump to next argument if current already exist in map
		if (argsMap.find(flagStr) != argsMap.end())
			continue;

		// If separated string, split it and remove whitespace
		split_string((const char*)value.ptr(), ",", true, argVector);
		// Insert vector into map
		argsMap.insert(ArgPair(flagStr, argVector));
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

bool ps::find_tag(tag_t find_tag, int tag_list_len, tag_t *tag_list)
{
	for (int i = 0; i < tag_list_len; i++)
	{
		if (find_tag = tag_list[i]) return true;
	}
	return false;
}

bool ps::find_tag(tag_t find_tag, vector<tag_t> tag_list)
{
	for (vector<tag_t>::iterator i = tag_list.begin(); i != tag_list.end(); ++i)
	{
		if (find_tag = *i) return true;
	}
	return false;
}

bool ps::get_preference(const char *pref_name, TC_preference_search_scope_t scope, int *num_prefs, char ***prefs)
{
	logical result = true;
	TC_preference_search_scope_t oldScope;

	itk(PREF_ask_search_scope(&oldScope));
	itk(PREF_set_search_scope(TC_preference_site));
	try
	{
		itk(PREF_ask_char_values(pref_name, num_prefs, prefs));
	}
	catch (tcexception& e)
	{
		if (e.getstat() == PF_NOTFOUND)
		{
			result = false;
		}
	}
	itk(PREF_set_search_scope(oldScope));
	return result;
}

bool ps::get_preference(const char *pref_name, TC_preference_search_scope_t scope, char **pref)
{
	logical result = true;
	TC_preference_search_scope_t oldScope;

	itk(PREF_ask_search_scope(&oldScope));
	itk(PREF_set_search_scope(TC_preference_site));
	try
	{
		itk(PREF_ask_char_value(pref_name, 0, pref));
	}
	catch (tcexception& e)
	{
		if (e.getstat() == PF_NOTFOUND)
		{
			result = false;
		}
	}
	itk(PREF_set_search_scope(oldScope));
	return result;
}

bool ps::get_preference(const char *pref_name, TC_preference_search_scope_t scope, int *pref)
{
	logical result = true;
	TC_preference_search_scope_t oldScope;

	itk(PREF_ask_search_scope(&oldScope));
	itk(PREF_set_search_scope(TC_preference_site));
	try
	{
		itk(PREF_ask_int_value(pref_name, 0, pref));
	}
	catch (tcexception& e)
	{
		if (e.getstat() == PF_NOTFOUND)
		{
			result = false;
		}
	}
	itk(PREF_set_search_scope(oldScope));
	return result;
}