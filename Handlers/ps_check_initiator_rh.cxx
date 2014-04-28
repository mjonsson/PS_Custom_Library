#include "ps_global.hxx"
#include "ps_rule_handlers.hxx"

using namespace ps;

int ps_check_initiator_rh(EPM_rule_message_t msg)
{
	const char		*debug_name = "PS-check-initiator-RH";
	char			*pszArg = NULL;
	EPM_decision_t  decision = EPM_go;

	log_debug("[START] %s", debug_name);
	hr_start(debug_name);

	try
	{
		if (msg.arguments->number_of_arguments == 0)
			throw psexception("Missing mandatory arguments.");

		while ((pszArg = TC_next_argument(msg.arguments)) != NULL)
		{
			c_ptr<char>		flag, value;

			itk(ITK_ask_argument_named_value(pszArg, flag.get_ptr(), value.get_ptr()));

			// Check if allowed user
			if (tc_strcasecmp(flag.get(), "user") == 0)
			{
				char	*userId = NULL;

				itk(POM_get_user_id(&userId));

				if (tc_strstr(value.get(), userId) == NULL)
				{
					decision = EPM_nogo;
					itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, str_format("Process can only be initiated by user(s) '%s'.", value.get()).get()));
				}
			}
			// Check if allowed group
			else if (tc_strcasecmp(flag.get(), "current_group") == 0)
			{
				tag_t		tGroupMember,
							tGroup;
				c_ptr<char>	groupName;
							
				itk(SA_ask_current_groupmember(&tGroupMember));
				itk(SA_ask_groupmember_group(tGroupMember, &tGroup));
				itk(SA_ask_group_name2(tGroup, groupName.get_ptr()));

				if (tc_strstr(value.get(), groupName.get()) == NULL)
				{
					decision = EPM_nogo;
					itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, str_format("Process can only be initiated by group(s) '%s'.", value.get()).get()));
				}
			}
			// Check if allowed role
			else if (tc_strcasecmp(flag.get(), "current_role") == 0)
			{
				tag_t		tGroupMember,
							tRole;
				c_ptr<char>	roleName;
							
				itk(SA_ask_current_groupmember(&tGroupMember));
				itk(SA_ask_groupmember_role(tGroupMember, &tRole));
				itk(SA_ask_role_name2(tRole, roleName.get_ptr()));

				if (tc_strstr(value.get(), roleName.get()) == NULL)
				{
					decision = EPM_nogo;
					itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, str_format("Process can only be initiated by role(s) '%s'.", value.get()).get()));
				}
			}
			// Check if allowed full group
			else if (tc_strcasecmp(flag.get(), "current_full_group") == 0)
			{
				tag_t		tGroupMember,
							tGroup;
				c_ptr<char>	groupName;
							
				itk(SA_ask_current_groupmember(&tGroupMember));
				itk(SA_ask_groupmember_group(tGroupMember, &tGroup));
				itk(SA_ask_group_full_name(tGroup, groupName.get_ptr()));

				if (tc_strstr(value.get(), groupName.get()) == NULL)
				{
					decision = EPM_nogo;
					itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, str_format("Process can only be initiated by full group(s) '%s'.", value.get()).get()));
				}
			}
			// Check if allowed effective role
			else if (tc_strcasecmp(flag.get(), "effective_role") == 0)
			{
				char		*userId = NULL;
				tag_t		tUser,
							tRole;
				c_ptr<char>	roleName;
				c_ptr<tag_t>	groupMembers;
				logical		roleFound = false;
							
				itk(POM_get_user_id(&userId));
				itk(SA_find_user2(userId, &tUser));
				itk(SA_find_all_groupmember_by_user(tUser, false, groupMembers.get_len_ptr(), groupMembers.get_ptr()));

				for (int i = 0; i < groupMembers.get_len(); i++)
				{
					itk(SA_ask_groupmember_role(groupMembers.get(i), &tRole));
					itk(SA_ask_role_name2(tRole, roleName.get_ptr()));

					if (tc_strstr(value.get(), roleName.get()) != NULL)
					{
						roleFound = true;
						break;
					}
				}

				if (!roleFound)
				{
					decision = EPM_nogo;
					itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, str_format("Process can only be initiated by role(s) '%s'.", value.get()).get()));
				}
			}
			else
			{
				throw psexception("Illegal argument.");
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
