/*!
 * \file ps_misc.hxx
 * \date 2014/04/30 22:11
 *
 * \author Mattias Jonsson (jonssonm)
 * Contact: jonsson.mattias@siemens.com
 *
 * \brief Various utility functions.
 *
*/

#pragma once

namespace ps
{
	typedef map<string, vector<string>> ArgsMap;
	typedef pair<string, vector<string>> ArgPair;

	//! Convenience class for handling workflow handler arguments
	class h_args
	{
	private:
		ArgsMap argsMap;

	public:
		//! Construct the ArgsMap map
		h_args(TC_argument_list_t *args);
		//! Default destructor
		~h_args() { };
		//! Get and return a string by value
		string getStr(string flag);
		//! Get and return a logical by value
		bool getFlag(string flag);
		//! Get and return a string by reference
		bool getStr(string flag, string &value);
		//! Get and return a vector of string by reference
		bool getVec(string flag, vector<string> &value);
		//! Get and return a logical by reference
		bool getFlag(string flag, logical &value);
		//! Get and return a logical by reference
		bool getInt(string flag, int &value);
		//! Return size of ArgsMap
		int size();
	};

	template<typename T>
	void array_to_vector(T *anArray, int size, vector<T> &aVector)
	{
		for (int i = 0; i < size; i++)
		{
			aVector.push_back(anArray[i]);
		}
	}

	bool find_tag(tag_t find_tag, int tag_list_len, tag_t *tag_list);
	bool find_tag(tag_t find_tag, vector<tag_t> tag_list);
	bool get_preference(const char *pref_name, TC_preference_search_scope_t scope, int *num_prefs, char ***prefs);
	bool get_preference(const char *pref_name, TC_preference_search_scope_t scope, char **pref);
	bool get_preference(const char *pref_name, TC_preference_search_scope_t scope, int *pref);
}