#include "ps_global.hxx"
#include "ps_action_handlers.hxx"

using namespace ps;

int ps_create_dispatcher_request_ah(EPM_action_message_t msg)
{
	const char			*debug_name = "PS-create-dispatcher-request-AH";
	char				*pszArg = NULL;
	string				provider,
						service,
						primary_type,
						secondary_type,
						argument,
						request_type = "ON_DEMAND";
	logical				owning_user = false,
						owning_group = false;
	c_pptr<char>		arguments(5);
	vector<tag_t>		primaryObjects,
						secondaryObjects;
	tag_t				tRootTask,
						tRequest;
	c_ptr<tag_t>		tTargets;
	int					priority = 3,
						result = ITK_ok;

	log_debug("[START] %s", debug_name);
	hr_start(debug_name);

	try
	{
		if (msg.arguments->number_of_arguments == 0)
			throw psexception("Missing mandatory arguments.");

		while ((pszArg = TC_next_argument(msg.arguments)) != NULL )
		{
			c_ptr<char>		flag, value;

			itk(ITK_ask_argument_named_value(pszArg, flag.get_ptr(), value.get_ptr()));

			// Get dispatcher provider
			if (tc_strcasecmp(flag.get(), "provider") == 0)
			{
				provider = value.get();
			}
			// Get dispatcher service
			else if (tc_strcasecmp(flag.get(), "service") == 0)
			{
				service = value.get();
			}
			// Get primary object type
			else if (tc_strcasecmp(flag.get(), "primary_type") == 0)
			{
				primary_type = value.get();
			}
			// Get secondary object type
			else if (tc_strcasecmp(flag.get(), "secondary_type") == 0)
			{
				secondary_type = value.get();
			}
			// Get the priority of the job
			else if (tc_strcasecmp(flag.get(), "priority") == 0)
			{
				priority = atoi(value.get());
			}
			// Get user arguments
			else if (tc_strcasecmp(flag.get(), "argument") == 0)
			{
				arguments.append(value.get());
			}
			// Get the request type
			else if (tc_strcasecmp(flag.get(), "request_type") == 0)
			{
				request_type = value.get();
			}
			else
			{
				throw psexception("Illegal argument.");
			}
		}

		if (provider.empty() || service.empty() || secondary_type.empty())
			throw psexception("Missing mandatory arguments.");

		itk(EPM_ask_root_task(msg.task, &tRootTask));
		itk(EPM_ask_attachments(tRootTask, EPM_target_attachment, tTargets.get_len_ptr(), tTargets.get_ptr()));

		for (int i = 0; i < tTargets.get_len(); i++)
		{
			tag_t			tTarget = tTargets.get(i);
			c_ptr<char>		secondaryType;
			c_ptr<tag_t>	tSecondary;

			itk(AOM_ask_value_string(tTarget, "object_type", secondaryType.get_ptr()));

			// Check if the target attachment is of correct type
			if (tc_strcmp(secondary_type.c_str(), secondaryType.get()) == 0)
			{
				logical		objFound = false;
				secondaryObjects.push_back(tTarget);

				// If primary type is set, try to find it
				if (!primary_type.empty())
				{

					itk(GRM_list_secondary_objects_only(tTarget, 0, tSecondary.get_len_ptr(), tSecondary.get_ptr()));

					for (int j = 0; j < tSecondary.get_len(); j++)
					{
						c_ptr<char>		primaryType;

						itk(AOM_ask_value_string(tSecondary.get(j), "object_type", primaryType.get_ptr()));

						if (tc_strcmp(primary_type.c_str(), primaryType.get()) == 0)
						{
							primaryObjects.push_back(tSecondary.get(j));
							objFound = true;
							break;
						}
					}
				}
				// If primary type is not set or it was not found push NULLTAG into vector
				if (primary_type.empty() || objFound == false)
				{
					primaryObjects.push_back(NULLTAG);
				}
			}
		}

		DISPATCHER_create_request(provider.c_str(), service.c_str(), priority, NULL, NULL, 0, secondaryObjects.size(), &primaryObjects[0], &secondaryObjects[0], arguments.get_len(),(const char**) arguments.get_ptr(), request_type.c_str(), 0, NULL, NULL, &tRequest);
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

	hr_stop(debug_name);
	hr_print(debug_name);
	log_debug("[STOP] %s", debug_name);

	return result;
}
