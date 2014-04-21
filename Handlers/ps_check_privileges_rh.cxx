#include "ps_global.hxx"
#include "ps_rule_handlers.hxx"

int ps_check_privileges_rh(EPM_rule_message_t msg)
{
	const char		*debug_name = "PS-check-privileges-RH";
	char			*pszArg = NULL;
	string			types,
					statuses,
					privileges;
	logical			owning_user = false,
					owning_group = false;
	EPM_decision_t  decision = EPM_go;

	ps_write_debug("[START] %s", debug_name);
	hr_start(debug_name);

	try
	{
		while ((pszArg = TC_next_argument(msg.arguments)) != NULL )
		{
			c_ptr<char>		flag, value;

			itk(ITK_ask_argument_named_value(pszArg, flag.get_ptr(), value.get_ptr()));

			// Get types to include
			if (tc_strcasecmp(flag.get(), "types") == 0)
			{
				types = value.get();
			}
			// Get statuses to include
			else if (tc_strcasecmp(flag.get(), "statuses") == 0)
			{
				statuses = value.get();
			}
			// Get privilege string
			else if (tc_strcasecmp(flag.get(), "privileges") == 0)
			{
				privileges = value.get();
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
		}

		if (privileges.empty() && !owning_user && !owning_group)
			throw psexception("Missing mandatory parameters.");

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
			if (!types.empty())
			{
				if (tc_strstr(types.c_str(), targetType.get()) == NULL)
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
			else
			{
				itk(AOM_ask_value_tags(tTarget, "release_status_list", targetStatuses.get_len_ptr(), targetStatuses.get_ptr()));

				if (targetStatuses.get_len() == 0 && tc_strstr(statuses.c_str(), "Working") != NULL)
				{
					statusOk = true;
				}
				else
				{
					for (int j = 0; j < targetStatuses.get_len(); j++)
					{
						c_ptr<char>		statusName;

						itk(AOM_ask_value_string(targetStatuses.get(j), "object_name", statusName.get_ptr()));

						if (tc_strstr(statuses.c_str(), statusName.get()) != NULL)
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
					itk(AM_check_privilege(tTarget, privileges.c_str(), &hasAccess));

					if (!hasAccess)
					{
						c_ptr<char>		targetDispName;

						decision = EPM_nogo;
						itk(AOM_ask_value_string(tTargetAttach.get(i), "object_string", targetDispName.get_ptr()));
						itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL,
							string("Required privilege(s) not met on object '" + string(targetDispName.get()) + "' (" + privileges + ").").c_str()));
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
						itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL,
							string("Current user is not the owner of object '" + string(targetDispName.get()) + "'.").c_str()));
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
						itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL,
							string("Current group is not the owning group of object '" + string(targetDispName.get()) + "'.").c_str()));
					}
				}
			}
		}
	}
	catch (tcexception& e)
	{
		decision = EPM_nogo;
		EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, e.what());
		ps_write_error(e.what());
	}
	catch (psexception& e)
	{
		decision = EPM_nogo;
		EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, e.what());
		ps_write_error(e.what());
	}

	hr_stop(debug_name);
	hr_print(debug_name);
	ps_write_debug("[STOP] %s", debug_name);

	return decision;
}
