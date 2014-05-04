#include "ps_global.hxx"
#include "ps_defines.hxx"
#include "ps_find_referencers.hxx"

using namespace ps;

int ps::ps_find_referencers(METHOD_message_t *m, va_list  args)
{
	const char		*debug_name = "Find Referencers";
	int				result = ITK_ok;
	string			relationProp = m->user_args->arguments[0].val_union.str_value;
	string			filterTypes = m->user_args->arguments[1].val_union.str_value;
	tag_t			relationType;
	tag_t			tSource = m->object_tag;
	c_ptr<tag_t>	c_referencers;
	int				activeSeq;
	bool			applyTypeFilter = false;
	vector<tag_t>	tmpRefs;
	vector<string>	v_filterTypes;
	int				*numReferencers = NULL;
	tag_t			**referencers = NULL;
	tag_t			tProp = va_arg(args, tag_t);

	log_debug("[START] %s", debug_name);
	hr_start_debug(debug_name);

	numReferencers = va_arg(args, int*);
	referencers = va_arg(args, tag_t**);
	
	try
	{
		itk(GRM_find_relation_type(relationProp.c_str(), &relationType));
		itk(GRM_list_primary_objects_only(tSource, relationType, c_referencers.plen(), c_referencers.pptr()));

		if (filterTypes.length() > 0)
		{
			applyTypeFilter = true;
			split_str(filterTypes, ";", true, v_filterTypes);
		}

		for (int i = 0; i < c_referencers.len(); i++)
		{
			tag_t	otherObj = c_referencers.val(i);

			// Only get the last version of the wso
			itk(AOM_ask_value_int(otherObj, "active_seq", &activeSeq));

			if (activeSeq == 0)
				continue;

			if (!applyTypeFilter)
			{
				tmpRefs.push_back(otherObj);
			}
			else
			{
				c_ptr<char>		objectType;

				itk(AOM_ask_value_string(otherObj, "object_type", objectType.pptr()));

				if (find_str(objectType.ptr(), v_filterTypes))
				{
					tmpRefs.push_back(otherObj);
				}
			}
		}
		if (tmpRefs.size() > 0)
		{
			*numReferencers = tmpRefs.size();
			sm_alloc(*referencers, tag_t, *numReferencers);
			memcpy(*referencers, &tmpRefs[0], *numReferencers * sizeof(tag_t));
		}
	}
	catch (tcexception& e)
	{
		result = e.getstat();
		EMH_store_error_s1(EMH_severity_error, FIND_REFERENCERS_DEFAULT_IFAIL, e.what());
		log_error(e.what());
	}
	catch (psexception& e)
	{
		result = FIND_REFERENCERS_DEFAULT_IFAIL;
		EMH_store_error_s1(EMH_severity_error, FIND_REFERENCERS_DEFAULT_IFAIL, e.what());
		log_error(e.what());
	}

	hr_stop_debug(debug_name);
	hr_print_debug(debug_name);
	log_debug("[STOP] %s", debug_name);

	return result;
}