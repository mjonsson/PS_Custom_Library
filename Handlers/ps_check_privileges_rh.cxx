#include "ps_global.hxx"
#include "ps_rule_handlers.hxx"

using namespace ps;

int ps_check_privileges_rh(EPM_rule_message_t msg)
{
	const char		*debug_name = "PS-check-privileges-RH";
	char			*pszArg = NULL;
	vector<string>	privileges;
	vector<string>	objectTypes;
	vector<string>	statuses;
	logical			owning_user = false,
					owning_group = false;
	EPM_decision_t  decision = EPM_go;

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

			// Get types to include
			if (tc_strcasecmp(flag.get(), "include_types") == 0)
			{
				split_str(value.get(), ",;:", true, objectTypes);
			}
			// Get statuses to include
			else if (tc_strcasecmp(flag.get(), "include_statuses") == 0)
			{
				split_str(value.get(), ",;:", true, statuses);
			}
			// Get privilege string
			else if (tc_strcasecmp(flag.get(), "privileges") == 0)
			{
				split_str(value.get(), ",;:", true, privileges);
			}
			// Get owning_user string
			else if (tc_strcasecmp(flag.get(), "owning_user") == 0)
			{
				owning_user = true;
			}
			// Get owning_group string
			else if (tc_strcasecmp(flag.get(), "owning_group") == 0)
			{
				owning_group = true;
			}
			else
			{
				throw psexception("Illegal argument.");
			}
		}

		if (privileges.empty() && !owning_user && !owning_group)
			throw psexception("Missing mandatory arguments.");

		tag_t			tRootTask;
		c_ptr<tag_t>	tTargetAttach;
		char			*pszUserId = NULL;
		logical			hasAccess,
						typeOk = true,
						statusOk = false;
			
		itk(EPM_ask_root_task(msg.task, &tRootTask));
		itk(EPM_ask_attachments(tRootTask, EPM_target_attachment, tTargetAttach.get_len_ptr(), tTargetAttach.get_ptr()));
				
		for (int i = 0; i < tTargetAttach.get_len(); i++)
		{
			c_ptr<char>		targetType;
			tag_t			tTarget = tTargetAttach.get(i);
			c_ptr<tag_t>	targetStatuses;

			itk(AOM_ask_value_string(tTarget, "object_type", targetType.get_ptr()));

			// Check if target object is valid type
			if (!objectTypes.empty())
			{
				if (!find_str(targetType.get(), objectTypes))
					typeOk = false;
			}

			// Check if target object has valid status
			if (!statuses.empty())
			{
				int numStatus;

				itk(AOM_ask_num_elements(tTarget, "release_status_list", &numStatus));

				if (numStatus == 0)
					statusOk = true;
			}
			if (!statusOk)
			{
				itk(AOM_ask_value_tags(tTarget, "release_status_list", targetStatuses.get_len_ptr(), targetStatuses.get_ptr()));

				if (targetStatuses.get_len() == 0 && find_str("Working", statuses))
				{
					statusOk = true;
				}
				else
				{
					for (int j = 0; j < targetStatuses.get_len(); j++)
					{
						c_ptr<char>		statusName;

						itk(AOM_ask_value_string(targetStatuses.get(j), "object_name", statusName.get_ptr()));

						if (find_str(statusName.get(), statuses))
						{
								statusOk = true;
								break;
						}
					}
				}
			}
			// Check privileges
			if (typeOk && statusOk)
			{
				if (!privileges.empty())
				{
					// Check privileges, concatenate the privileges vector
					for (vector<string>::iterator it = privileges.begin(); it != privileges.end(); ++it)
					{
						itk(AM_check_privilege(tTarget, it->c_str(), &hasAccess));
						
						if (!hasAccess)
							break;
					}

					if (!hasAccess)
					{
						c_ptr<char>		targetDispName;

						decision = EPM_nogo;
						itk(AOM_ask_value_string(tTargetAttach.get(i), "object_string", targetDispName.get_ptr()));
						itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, str_format("Required privilege(s) not met on object '%s' (%s).", targetDispName.get(), concat_str(privileges, ',', false).c_str()).get()));
					}
				}
				if (owning_user)
				{
					tag_t		tOwningUser,
								tGroupMember,
								tCurrentUser;

					itk(AOM_ask_owner(tTarget, &tOwningUser));
					itk(SA_ask_current_groupmember(&tGroupMember));
					itk(SA_ask_groupmember_user(tGroupMember, &tCurrentUser));

					if (tCurrentUser != tOwningUser)
					{
						c_ptr<char>		targetDispName;

						decision = EPM_nogo;
						itk(AOM_ask_value_string(tTargetAttach.get(i), "object_string", targetDispName.get_ptr()));
						itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, str_format("Current user is not the owner of object '%s'.", targetDispName.get()).get()));
					}
				}
				if (owning_group)
				{
					tag_t		tOwningGroup,
								tGroupMember,
								tCurrentGroup;

					itk(AOM_ask_group(tTarget, &tOwningGroup));
					itk(SA_ask_current_groupmember(&tGroupMember));
					itk(SA_ask_groupmember_group(tGroupMember, &tCurrentGroup));

					if (tCurrentGroup != tOwningGroup)
					{
						c_ptr<char>		targetDispName;

						decision = EPM_nogo;
						itk(AOM_ask_value_string(tTargetAttach.get(i), "object_string", targetDispName.get_ptr()));
						itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, str_format("Current group is not the owner of object '%s'.", targetDispName.get()).get()));
					}
				}
			}
		}
	}
	catch (tcexception& e)
	{
		decision = EPM_nogo;
		EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, e.what());
		log_error(e.what());
	}
	catch (psexception& e)
	{
		decision = EPM_nogo;
		EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, e.what());
		log_error(e.what());
	}

	hr_stop(debug_name);
	hr_print(debug_name);
	log_debug("[STOP] %s", debug_name);

	return decision;
}
