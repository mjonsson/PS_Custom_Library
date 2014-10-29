#include "ps_global.hxx"
#include "ps_defines.hxx"
#include "ps_dynamic_folders.hxx"
#include <time.h>

using namespace ps;

int ps::ps_dynamic_folders(METHOD_message_t *m, va_list  args)
{
	const char		*debug_name = "PS2 Smart Folders";
	int				result = ITK_ok;
	tag_t			tQuery = 0;
	static c_ptr<tag_t>	results;
	c_pptr<char>		entries;
	c_pptr<char>		values;
	c_pptr<char>		sortKeys;
	c_ptr<int>			sortOrder;
	logical				isActive;
	int					maxHits = 0;
	static int			numReturn = 0;
	time_t				epoch = time(NULL);
	static time_t		lastRun = epoch - ps2_dynamic_folder_delay_prop - 1;
	tag_t				tDynamicFolder = m->object_tag;
	tag_t				tProp = va_arg(args, tag_t);
	int					*pNumResults = va_arg(args, int*);
	tag_t				**ptResults = va_arg(args, tag_t**);

	log_debug("[START] %s", debug_name);
	hr_start_debug(debug_name);

	try
	{
		itk(AOM_ask_value_logical(tDynamicFolder, ps2_dynamic_folder_active_prop.ptr(), &isActive));
		if (isActive)
		{
			if (epoch - lastRun > ps2_dynamic_folder_delay_prop)
			{
				itk(AOM_ask_value_tag(tDynamicFolder, ps2_dynamic_folder_query_prop.ptr(), &tQuery));
				if (tQuery != 0)
				{
					itk(AOM_ask_value_strings(tDynamicFolder, ps2_dynamic_folder_entries_prop.ptr(), entries.plen(), entries.pptr()));
					if (entries.len() > 0)
					{
						itk(AOM_ask_value_strings(tDynamicFolder, ps2_dynamic_folder_values_prop.ptr(), values.plen(), values.pptr()));
						itk(AOM_ask_value_strings(tDynamicFolder, ps2_dynamic_folder_sortkeys_prop.ptr(), sortKeys.plen(), sortKeys.pptr()));
						itk(AOM_ask_value_ints(tDynamicFolder, ps2_dynamic_folder_sortorder_prop.ptr(), sortOrder.plen(), sortOrder.pptr()));
						itk(AOM_ask_value_int(tDynamicFolder, ps2_dynamic_folder_maxhits_prop.ptr(), &maxHits));

						results.dealloc();
						itk(QRY_execute_with_sort(tQuery, entries.len(), entries.ptr(), values.ptr(),
							sortKeys.len(), sortKeys.ptr(), sortOrder.ptr(), results.plen(), results.pptr()));
						if (results.len() > maxHits)
						{
							numReturn = maxHits;
							results.realloc(numReturn);
						}
						else numReturn = results.len();
						*pNumResults = numReturn;
						if (numReturn > 0)
						{
							sm_alloc(*ptResults, tag_t, numReturn);
							memcpy(*ptResults, results.ptr(), numReturn * sizeof(tag_t));
						}
					}
				}
				lastRun = epoch;
			}
			else
			{
				*pNumResults = numReturn;
				if (numReturn > 0)
				{
					sm_alloc(*ptResults, tag_t, numReturn);
					memcpy(*ptResults, results.ptr(), numReturn * sizeof(tag_t));
				}
			}
		}
		else
		{
			results.dealloc();
			*pNumResults = 0;
		}
	}
	catch (tcexception& e)
	{
		result = e.getstat();
		EMH_store_error_s1(EMH_severity_error, DYNAMIC_FOLDERS_DEFAULT_IFAIL, e.what());
		log_error(e.what());
	}
	catch (psexception& e)
	{
		result = NAME_RULES_DEFAULT_IFAIL;
		EMH_store_error_s1(EMH_severity_error, DYNAMIC_FOLDERS_DEFAULT_IFAIL, e.what());
		log_error(e.what());
	}

	hr_stop_debug(debug_name);
	hr_print_debug(debug_name);
	log_debug("[STOP] %s", debug_name);

	return result;
}