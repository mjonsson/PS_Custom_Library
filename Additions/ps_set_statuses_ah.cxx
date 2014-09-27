#include "ps_global.hxx"
#include "ps_defines.hxx"
#include "ps_action_handlers.hxx"

using namespace ps;

int ps_set_statuses_ah(EPM_action_message_t msg)
{
	const char			*debug_name = "PS2-set-statuses-AH";
	int					result = ITK_ok;
	tag_t				tRootTask;
	c_ptr<tag_t>		tRootTargets;
	string				sAction("Add");
	string				sStatusType;
	vector<string>		vPropsToTraverse;
	vector<string>		vTypesToInclude;
	//logical				lSetEffectivity = false;;
	logical				lRetainReleasedDate = false;
	date_t				today;
	h_args				args(msg.arguments);

	log_debug("[START] %s", debug_name);
	hr_start_debug(debug_name);

	try
	{
		if (args.size() == 0)
			throw psexception("Missing mandatory arguments.");

		args.getStr("action", sAction);
		if (!args.getStr("status", sStatusType))
			throw psexception("Missing mandatory argument 'status'");
		args.getVec("include_types", vTypesToInclude);
		args.getVec("include_relations", vPropsToTraverse);
		//args.getFlag("set_effectivity", lSetEffectivity);
		args.getFlag("retain_release_date", lRetainReleasedDate);

		/*		if (lSetEffectivity)
		{
		time_t		ctime;
		struct		tm tm_ptr;

		time(&ctime);
		localtime_s(&tm_ptr, &ctime);

		today.year		= (short)tm_ptr.tm_year+1900;
		today.month		= (byte)tm_ptr.tm_mon;
		today.day		= (byte)tm_ptr.tm_mday;
		today.hour		= (byte)tm_ptr.tm_hour;
		today.minute	= (byte)tm_ptr.tm_min;
		today.second	= (byte)tm_ptr.tm_sec;
		}
		*/
		// Get target objects
		itk(EPM_ask_root_task(msg.task, &tRootTask));
		itk(EPM_ask_attachments(tRootTask, EPM_target_attachment, tRootTargets.plen(), tRootTargets.pptr()));

		for (int i = 0; i < tRootTargets.len(); i++)
		{
			tag_t				tRootTarget = tRootTargets.val(i);
			tag_t				tStatus;
			vector<tag_t>		vObjectsToStatus;

			// If types defined check target type
			if (!vTypesToInclude.empty())
			{
				c_ptr<char>		objectType;

				itk(AOM_ask_value_string(tRootTarget, "object_type", objectType.pptr()));

				if (!find_string(objectType.ptr(), vTypesToInclude))
				{
					continue;
				}
			}

			vObjectsToStatus.push_back(tRootTarget);

			// Add additional objects to status
			if (!vPropsToTraverse.empty())
			{
				for (vector<string>::iterator it = vPropsToTraverse.begin(); it != vPropsToTraverse.end(); ++it)
				{
					const char	*propName = it->c_str();
					c_ptr<tag_t>	wsoObjects;

					itk(AOM_ask_value_tags(tRootTarget, propName, wsoObjects.plen(), wsoObjects.pptr()));
					array_to_vector<tag_t>(wsoObjects.ptr(), wsoObjects.len(), vObjectsToStatus);
				}
			}

			// Create and assign release status
			//if (sAction == "replace")
			//{
			//	itk(POM_refresh_instances_any_class(vObjectsToStatus.size(), &vObjectsToStatus[0], POM_modify_lock));
			//	for (int j = 0; j < vObjectsToStatus.size(); j++)
			//	{
			//		tag_t	tClassId;
			//		tag_t	tAttrId;
			//		char	*pszClassName = NULL;

			//		itk(POM_class_of_instance(vObjectsToStatus[j], &tClassId));
			//		itk(POM_name_of_class(tClassId, &pszClassName));
			//		itk(POM_attr_id_of_attr("release_status_list", pszClassName, &tAttrId));
			//		
			//		itk(POM_set_attr_nulls(1, &vObjectsToStatus[j], tAttrId, 0, 1));
			//		//itk(POM_set_attr_null(1, &vObjectsToStatus[j], tAttrId));
			//	}
			//	itk(POM_save_instances(vObjectsToStatus.size(), &vObjectsToStatus[0], false));
			//	itk(POM_refresh_instances_any_class(vObjectsToStatus.size(), &vObjectsToStatus[0], POM_read_lock));
			//}

#ifdef _TC_10
			itk(RELSTAT_create_release_status(sStatusType.c_str(), &tStatus));
			itk(RELSTAT_add_release_status(tStatus, vObjectsToStatus.size(), &vObjectsToStatus[0], lRetainReleasedDate));
#else
			itk(CR_create_release_status(sStatusType.c_str(), &tStatus));
			itk(EPM_add_release_status(tStatus, vObjectsToStatus.size(), &vObjectsToStatus[0], lRetainReleasedDate));
#endif

			// Set the effectivity
			//if (lSetEffectivity)
			//{
			//	tag_t		tEffectivity;

			//	itk(WSOM_effectivity_create_with_dates(tStatus, NULLTAG, 1, &today, EFFECTIVITY_open_ended, &tEffectivity));
			//	itk(AOM_save(tEffectivity));
			//}

			//itk(AOM_refresh(tStatus, false));
		}
	}
	catch (tcexception& e)
	{
		result = e.getstat();
		EMH_store_error_s1(EMH_severity_error, ACTION_HANDLER_DEFAULT_IFAIL, e.what());
		log_error(e.what());
	}
	catch (psexception& e)
	{
		result = ACTION_HANDLER_DEFAULT_IFAIL;
		EMH_store_error_s1(EMH_severity_error, ACTION_HANDLER_DEFAULT_IFAIL, e.what());
		log_error(e.what());
	}

	hr_stop_debug(debug_name);
	hr_print_debug(debug_name);
	log_debug("[STOP] %s", debug_name);

	return result;
}
