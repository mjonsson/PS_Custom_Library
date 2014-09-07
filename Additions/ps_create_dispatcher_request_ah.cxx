#include "ps_global.hxx"
#include "ps_defines.hxx"
#include "ps_action_handlers.hxx"

using namespace ps;

int ps_create_dispatcher_request_ah(EPM_action_message_t msg)
{
	const char			*debug_name = "PS2-create-dispatcher-request-AH";
	char				*pszArg = NULL;
	string				provider,
						service,
						primary_type,
						secondary_type,
						request_type = "ON_DEMAND";
	logical				owning_user = false;
	logical				owning_group = false;
	logical				lMultipleRequests = false;
	logical				lFillOutPrimaryIfNull = false;
	vector<string>		arguments;
	vector<tag_t>		primaryObjects,
						secondaryObjects;
	tag_t				tRootTask,
						tRequest;
	c_ptr<tag_t>		tTargets;
	int					priority = 3,
						result = ITK_ok;
	h_args				args(msg.arguments);

	log_debug("[START] %s", debug_name);
	hr_start_debug(debug_name);

	try
	{
		if (args.size() == 0)
			throw psexception("Missing mandatory arguments.");

		if (!args.getStr("provider", provider))
			throw psexception("Missing mandatory argument 'provider'.");
		if (!args.getStr("service", service))
			throw psexception("Missing mandatory argument 'service'.");
		args.getStr("primary_type", primary_type);
		if (!args.getStr("secondary_type", secondary_type))
			throw psexception("Missing mandatory argument 'secondary_type'.");
		args.getInt("priority", priority);
		args.getVec("arguments", arguments);
		args.getStr("request_type", request_type);
		args.getFlag("multiple_requests", lMultipleRequests);
		args.getFlag("fill_out_primary", lFillOutPrimaryIfNull);

		itk(EPM_ask_root_task(msg.task, &tRootTask));
		itk(EPM_ask_attachments(tRootTask, EPM_target_attachment, tTargets.plen(), tTargets.pptr()));

		for (int i = 0; i < tTargets.len(); i++)
		{
			tag_t			tTarget = tTargets.val(i);
			c_ptr<char>		secondaryType;
			c_ptr<tag_t>	tSecondary;

			itk(AOM_ask_value_string(tTarget, "object_type", secondaryType.pptr()));

			// Check if the target attachment is of correct type
			if (match_string(secondary_type, secondaryType.ptr()))
			{
				logical		objFound = false;

				// If primary type is set, try to find it
				if (!primary_type.empty())
				{

					itk(GRM_list_secondary_objects_only(tTarget, 0, tSecondary.plen(), tSecondary.pptr()));

					for (int j = 0; j < tSecondary.len(); j++)
					{
						c_ptr<char>		primaryType;

						itk(AOM_ask_value_string(tSecondary.val(j), "object_type", primaryType.pptr()));

						if (match_string(primary_type, primaryType.ptr()))
						{
							primaryObjects.push_back(tSecondary.val(j));
							objFound = true;
							break;
						}
					}
					// If no object of primary type found there is nothing to process, so continue to next target
					if (!objFound)
						continue;
				}
				// If primary type is not set or it was not found push NULLTAG into vector
				else
				{
					if (lFillOutPrimaryIfNull)
					{
						primaryObjects.push_back(tTarget);
					}
					else
					{
						primaryObjects.push_back(0);
					}
				}
				secondaryObjects.push_back(tTarget);
			}
		}

		if (lMultipleRequests)
		{
			for (int i = 0; i < secondaryObjects.size(); i++)
			{
				DISPATCHER_create_request(provider.c_str(),							//< Service provider
											service.c_str(),						//< Service name
											priority,								//< Request priority
											NULL,									//< Start time (not currently used)
											NULL,									//< End time (not currently used)
											0,										//< Interval (not currently used)
											1,										//< Number of primary and secondary objects
											&primaryObjects[i],						//< Primary objects (Note that dataset to be translated is primary)
											&secondaryObjects[i],					//< Secondary objects (Secondary object that relates to dataset that is to be translated)
											arguments.size(),						//< Number of arguments
											&vec_string_to_char(arguments)[0],		//< Arguments
											request_type.c_str(),					//< Request type string
											0,										//< Number of datafiles (not currently used)
											NULL,									//< Datafile keys (not currently used)
											NULL,									//< Datafiles (not currently used)
											&tRequest);								//< The dispatcher request

				if (tRequest == 0)
					throw psexception("Dispatcher request was not created successfully.");
			}
		}
		else
		{
			DISPATCHER_create_request(provider.c_str(),							//< Service provider
										service.c_str(),						//< Service name
										priority,								//< Request priority
										NULL,									//< Start time (not currently used)
										NULL,									//< End time (not currently used)
										0,										//< Interval (not currently used)
										secondaryObjects.size(),				//< Number of primary and secondary objects
										&primaryObjects[0],						//< Primary objects (Note that dataset to be translated is primary)
										&secondaryObjects[0],					//< Secondary objects (Secondary object that relates to dataset that is to be translated)
										arguments.size(),						//< Number of arguments
										&vec_string_to_char(arguments)[0],		//< Arguments
										request_type.c_str(),					//< Request type string
										0,										//< Number of datafiles (not currently used)
										NULL,									//< Datafile keys (not currently used)
										NULL,									//< Datafiles (not currently used)
										&tRequest);								//< The dispatcher request

			if (tRequest == 0)
				throw psexception("Dispatcher request was not created successfully.");
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
