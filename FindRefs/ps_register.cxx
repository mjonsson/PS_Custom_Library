#include "ps_global.hxx"
#include "ps_defines.hxx"
#include "ps_register.hxx"
#include "ps_find_referencers.hxx"

using namespace ps;

void ps::ps_register_referencers()
{
	c_pptr<char>	referencerLines;
	TC_preference_search_scope_t oldScope;

	referencerLines.set_free_container_only(true);
	itk(PREF_ask_search_scope(&oldScope));
	itk(PREF_set_search_scope(TC_preference_site));
	try
	{
		itk(PREF_ask_char_values(PREF_REFERENCER_PROPERTIES, referencerLines.plen(), referencerLines.pptr()));
	}
	catch (tcexception& e)
	{
		if (e.getstat() == PF_NOTFOUND)
		{
			return;
		}
	}
	itk(PREF_set_search_scope(oldScope));

	for (int i = 0; i < referencerLines.len(); i++)
	{
		const char			*referencerLine = referencerLines.val(i);
		vector<string>		referencerLinesSplitted; //!< 0 = Source Type, 1 = Source Property, 2 = Target Relation, 3 = Filter types
		bool				filterDefined = false;
		METHOD_id_t 		method;
		c_ptr<TC_argument_list_t>	argument;

		split_string(referencerLine, ":", true, referencerLinesSplitted);

		// Don't register if incorrect setting
		if (referencerLinesSplitted.size() != 4)
			continue;
		if (referencerLinesSplitted[0].empty() || referencerLinesSplitted[1].empty() ||
			referencerLinesSplitted[2].empty() || referencerLinesSplitted[3].empty())
			continue;

		// Allocate all memory on the heap
		argument.set_free(false);
		argument.alloc(1);
		argument.ptr()->number_of_arguments = 2;
		sm_alloc(argument.ptr()->arguments, TC_argument_t, argument.ptr()->number_of_arguments);
		// Target relation
		argument.ptr()->arguments[0].type = POM_string;
		argument.ptr()->arguments[0].array_size = 1;
		sm_alloc(argument.ptr()->arguments[0].val_union.str_value, char, referencerLinesSplitted[2].length() + 1);
		tc_strcpy(argument.ptr()->arguments[0].val_union.str_value, referencerLinesSplitted[2].c_str());
		// Filter types
		argument.ptr()->arguments[1].type = POM_string;
		argument.ptr()->arguments[1].array_size = 1;
		sm_alloc(argument.ptr()->arguments[1].val_union.str_value, char, referencerLinesSplitted[3].length() + 1);
		tc_strcpy(argument.ptr()->arguments[1].val_union.str_value, referencerLinesSplitted[3].c_str());

		itk(METHOD_register_prop_method(referencerLinesSplitted[0].c_str(), referencerLinesSplitted[1].c_str(), PROP_ask_value_tags_msg, ps_find_referencers, argument.ptr(), &method)); 
	}
}

int ps::libps_oninit(int *decision, va_list args)
{
	try
	{
		// Initialize logging mechanism
		if (initialize_logging())
			printf("Logging module enabled.\n");
		// If enabled, initialize HRTimer class
		if (hr_init())
			printf("HRTimer module enabled.\n");

		//char ch;
		//ch = getchar();

		// Register referencer properties
		try
		{
			ps_register_referencers();
		}
		catch (exception &e)
		{
			TC_write_syslog("%s\n", e.what());
		}
	}
	catch (tcexception& e)
	{
		TC_write_syslog("%s\n", e.what());

		return e.getstat();
	}
	catch (exception& e)
	{
		TC_write_syslog("%s\n", e.what());

		return 1;
	}

	return ITK_ok;
}

int ps::libps_onexit(int *decision, va_list args)
{
	try
	{
		// Print performance statistics when logging out
		hr_print_all();
	}
	catch (tcexception& e)
	{
		TC_write_syslog("%s\n", e.what());

		return e.getstat();
	}
	catch (exception& e)
	{
		TC_write_syslog("%s\n", e.what());

		return 1;
	}

	return ITK_ok;
}

int libpsadds_register_callbacks()
{ 
	printf("Installing PS User Exits Library - %s, "__DATE__" "__TIME__".\n", PS_LIBNAME);

	try
	{
		itk(CUSTOM_register_exit(PS_LIBNAME, "USER_init_module", (CUSTOM_EXIT_ftn_t) libps_oninit));
		itk(CUSTOM_register_exit(PS_LIBNAME, "USER_exit_module", (CUSTOM_EXIT_ftn_t) libps_onexit));
	}
	catch (tcexception& e)
	{
		TC_write_syslog(e.what());

		return e.getstat();
	}
	catch (exception& e)
	{
		TC_write_syslog(e.what());

		return 1;
	}

	return ITK_ok;
}