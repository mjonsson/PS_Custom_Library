#include "ps_global.hxx"
#include "ps_rule_handlers.hxx"

int ps_check_initiator_rh(EPM_rule_message_t msg)
{
	const char		*debug_name = "PS-check-initiator-RH";
	char			*pszArg = NULL,
					*pszArgFlag = NULL,
					*pszArgValue = NULL;
	unordered_map<string, string> mArguments;
	EPM_decision_t  decision = EPM_go;

	ps_write_debug("[START] %s", debug_name);
	hr_start(debug_name);

	try
	{
		while ((pszArg = TC_next_argument(msg.arguments)) != NULL)
		{
			itkex(ITK_ask_argument_named_value(pszArg, &pszArgFlag, &pszArgValue));

			// Check if allowed user
			if (tc_strcasecmp(pszArgFlag, "user") == 0)
			{
				char	*userId = NULL;

				itkex(POM_get_user_id(&userId));

				if (tc_strstr(pszArgValue, userId) == NULL)
				{
					decision = EPM_nogo;
					itkex(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_NOGO_IFAIL,
						string("Process can only be initiated by user(s) '" + string(pszArgValue) + "'.").c_str()));
				}
			}
			// Check if allowed group
			else if (tc_strcasecmp(pszArgFlag, "current_group") == 0)
			{
				tag_t		tGroupMember,
							tGroup;
				c_ptr<char>	groupName;
							
				itkex(SA_ask_current_groupmember(&tGroupMember));
				itkex(SA_ask_groupmember_group(tGroupMember, &tGroup));
				itkex(SA_ask_group_name2(tGroup, groupName.get_ptr()));

				if (tc_strstr(pszArgValue, groupName.get()) == NULL)
				{
					decision = EPM_nogo;
					itkex(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_NOGO_IFAIL,
						string("Process can only be initiated by group(s) '" + string(pszArgValue) + "'.").c_str()));
				}
			}
			// Check if allowed role
			else if (tc_strcasecmp(pszArgFlag, "current_role") == 0)
			{
				tag_t		tGroupMember,
							tRole;
				c_ptr<char>	roleName;
							
				itkex(SA_ask_current_groupmember(&tGroupMember));
				itkex(SA_ask_groupmember_role(tGroupMember, &tRole));
				itkex(SA_ask_role_name2(tRole, roleName.get_ptr()));

				if (tc_strstr(pszArgValue, roleName.get()) == NULL)
				{
					decision = EPM_nogo;
					itkex(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_NOGO_IFAIL,
						string("Process can only be initiated by role(s) '" + string(pszArgValue) + "'.").c_str()));
				}
			}
			// Check if allowed full group
			else if (tc_strcasecmp(pszArgFlag, "current_full_group") == 0)
			{
				tag_t		tGroupMember,
							tGroup;
				c_ptr<char>	groupName;
							
				itkex(SA_ask_current_groupmember(&tGroupMember));
				itkex(SA_ask_groupmember_group(tGroupMember, &tGroup));
				itkex(SA_ask_group_full_name(tGroup, groupName.get_ptr()));

				if (tc_strstr(pszArgValue, groupName.get()) == NULL)
				{
					decision = EPM_nogo;
					itkex(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_NOGO_IFAIL,
						string("Process can only be initiated by full group(s) '" + string(pszArgValue) + "'.").c_str()));
				}
			}
			// Check if allowed effective role
			else if (tc_strcasecmp(pszArgFlag, "effective_role") == 0)
			{
				char		*userId = NULL;
				tag_t		tUser,
							tRole;
				c_ptr<char>	roleName;
				c_ptr<tag_t>	groupMembers;
				logical		roleFound = false;
							
				itkex(POM_get_user_id(&userId));
				itkex(SA_find_user2(userId, &tUser));
				itkex(SA_find_all_groupmember_by_user(tUser, false, groupMembers.get_len_ptr(), groupMembers.get_ptr()));

				for (int i = 0; i < groupMembers.get_len(); i++)
				{
					itkex(SA_ask_groupmember_role(groupMembers.get(i), &tRole));
					itkex(SA_ask_role_name2(tRole, roleName.get_ptr()));

					if (tc_strstr(pszArgValue, roleName.get()) != NULL)
					{
						roleFound = true;
						break;
					}
				}

				if (!roleFound)
				{
					decision = EPM_nogo;
					itkex(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_NOGO_IFAIL,
						string("Process can only be initiated by role(s) '" + string(pszArgValue) + "'.").c_str()));
				}
			}
		}
	}
	catch (tcexception& e)
	{
		ps_write_error(e.what());
	}
	catch (psexception& e)
	{
		ps_write_error(e.what());
	}

	hr_stop(debug_name);
	hr_print(debug_name);
	ps_write_debug("[STOP] %s", debug_name);

	return decision;
}
