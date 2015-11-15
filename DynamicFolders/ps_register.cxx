#include "ps_global.hxx"
#include "ps_defines.hxx"
#include "ps_register.hxx"
#include "ps_dynamic_folders.hxx"

using namespace ps;

void ps::ps_register_dynamic_folders()
{
	METHOD_id_t 		method;

	if (get_preference(PREF_DYNAMIC_FOLDER_TYPE, TC_preference_site, ps2_dynamic_folder_type.pptr()) &&
		get_preference(PREF_DYNAMIC_FOLDER_RUNTIME_PROP, TC_preference_site, ps2_dynamic_folder_runtime_prop.pptr()) &&
		get_preference(PREF_DYNAMIC_FOLDER_QUERY_PROP, TC_preference_site, ps2_dynamic_folder_query_prop.pptr()) &&
		get_preference(PREF_DYNAMIC_FOLDER_ACTIVE_PROP, TC_preference_site, ps2_dynamic_folder_active_prop.pptr()) &&
		get_preference(PREF_DYNAMIC_FOLDER_ENTRIES_PROP, TC_preference_site, ps2_dynamic_folder_entries_prop.pptr()) &&
		get_preference(PREF_DYNAMIC_FOLDER_VALUES_PROP, TC_preference_site, ps2_dynamic_folder_values_prop.pptr()) &&
		get_preference(PREF_DYNAMIC_FOLDER_SORT_KEYS_PROP, TC_preference_site, ps2_dynamic_folder_sortkeys_prop.pptr()) &&
		get_preference(PREF_DYNAMIC_FOLDER_SORT_ORDER_PROP, TC_preference_site, ps2_dynamic_folder_sortorder_prop.pptr()) &&
		get_preference(PREF_DYNAMIC_FOLDER_MAX_HITS_PROP, TC_preference_site, ps2_dynamic_folder_maxhits_prop.pptr()) &&
		get_preference(PREF_DYNAMIC_FOLDER_DELAY_PROP, TC_preference_site, &ps2_dynamic_folder_delay_prop))
	{
		printf("Dynamic folders enabled.\n");

		itk(METHOD_register_prop_method(ps2_dynamic_folder_type.ptr(), ps2_dynamic_folder_runtime_prop.ptr(),
			PROP_ask_value_tags_msg, ps_dynamic_folders, NULL, &method));
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

		// Register dynamic folders
		try
		{
			ps_register_dynamic_folders();
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