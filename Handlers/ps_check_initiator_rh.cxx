#include "ps_global.hxx"
#include "ps_rule_handlers.hxx"

using namespace ps;

int ps_check_initiator_rh(EPM_rule_message_t msg)
{
	const char		*debug_name = "PS-check-initiator-RH";
	char			*pszArg = NULL;
	h_args			args(msg.arguments);
	string			valueStr;
	vector<string>	valueVec;
	EPM_decision_t  decision = EPM_go;

	log_debug("[START] %s", debug_name);
	hr_start_debug(debug_name);

	try
	{
		if (args.size() == 0)
			throw psexception("Missing mandatory arguments.");

		// Check current if current user is defined
		if (args.getVec("current_user", valueVec))
		{
			char	*userId = NULL;

			itk(POM_get_user_id(&userId));

			if (!find_str(userId, valueVec))
			{
				decision = EPM_nogo;
				itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, c_ptr<char>().format("Process can only be initiated by user(s) '%s'.", concat_str(valueVec, ',', false).c_str())));
			}
		}
		// Check if group of current user is defined
#ifdef _TC10
		if (args.getVec("current_group", valueVec))
		{
			tag_t		tGroupMember;
			tag_t		tGroup;
			c_ptr<char>	groupName;

			itk(SA_ask_current_groupmember(&tGroupMember));
			itk(SA_ask_groupmember_group(tGroupMember, &tGroup));
			itk(SA_ask_group_name2(tGroup, groupName.get_ptr()));

			if (!find_str(groupName.get(), valueVec))
			{
				decision = EPM_nogo;
				itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, c_ptr<char>().format("Process can only be initiated by group(s) '%s'.", concat_str(valueVec, ',', false).c_str())));
			}
		}
#else
		if (args.getVec("current_group", valueVec))
		{
			tag_t		tGroupMember;
			tag_t		tGroup;
			char		groupName[SA_name_size_c+1];

			itk(SA_ask_current_groupmember(&tGroupMember));
			itk(SA_ask_groupmember_group(tGroupMember, &tGroup));
			itk(SA_ask_group_name(tGroup, groupName));

			if (!find_str(groupName, valueVec))
			{
				decision = EPM_nogo;
				itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, c_ptr<char>().format("Process can only be initiated by group(s) '%s'.", concat_str(valueVec, ',', false).c_str())));
			}
		}
#endif
		// Check if role of current user is defined
#ifdef _TC10
		if (args.getVec("current_role", valueVec))
		{
			tag_t		tGroupMember;
			tag_t		tRole;
			c_ptr<char>	roleName;

			itk(SA_ask_current_groupmember(&tGroupMember));
			itk(SA_ask_groupmember_role(tGroupMember, &tRole));
			itk(SA_ask_role_name2(tRole, roleName.get_ptr()));

			if (!find_str(roleName.get(), valueVec))
			{
				decision = EPM_nogo;
				itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, c_ptr<char>().format("Process can only be initiated by role(s) '%s'.", concat_str(valueVec, ',', false).c_str())));
			}
		}
#else
		if (args.getVec("current_role", valueVec))
		{
			tag_t		tGroupMember;
			tag_t		tRole;
			char		roleName[SA_name_size_c+1];

			itk(SA_ask_current_groupmember(&tGroupMember));
			itk(SA_ask_groupmember_role(tGroupMember, &tRole));
			itk(SA_ask_role_name(tRole, roleName));

			if (!find_str(roleName, valueVec))
			{
				decision = EPM_nogo;
				itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, c_ptr<char>().format("Process can only be initiated by role(s) '%s'.", concat_str(valueVec, ',', false).c_str())));
			}
		}
#endif
		// Check if full group of current user is defined
		if (args.getVec("current_full_group", valueVec))
		{
			tag_t		tGroupMember;
			tag_t		tGroup;
			c_ptr<char>	groupName;

			itk(SA_ask_current_groupmember(&tGroupMember));
			itk(SA_ask_groupmember_group(tGroupMember, &tGroup));
			itk(SA_ask_group_full_name(tGroup, groupName.get_ptr()));

			if (!find_str(groupName.get(), valueVec))
			{
				decision = EPM_nogo;
				itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, c_ptr<char>().format("Process can only be initiated by full group(s) '%s'.", concat_str(valueVec, ',', false).c_str())));
			}
		}
		// Check if any of users roles are defined
#ifdef _TC10
		if (args.getVec("current_effective_role", valueVec))
		{
			char			*userId = NULL;
			tag_t			tUser;
			tag_t			tRole;
			c_ptr<char>		roleName;
			c_ptr<tag_t>	groupMembers;
			logical			roleFound = false;

			itk(POM_get_user_id(&userId));
			itk(SA_find_user2(userId, &tUser));
			itk(SA_find_all_groupmember_by_user(tUser, false, groupMembers.get_len_ptr(), groupMembers.get_ptr()));

			for (int i = 0; i < groupMembers.get_len(); i++)
			{
				itk(SA_ask_groupmember_role(groupMembers.get(i), &tRole));
				itk(SA_ask_role_name2(tRole, roleName.get_ptr()));

				if (find_str(roleName.get(), valueVec))
				{
					roleFound = true;
					break;
				}
			}

			if (!roleFound)
			{
				decision = EPM_nogo;
				itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, c_ptr<char>().format("Process can only be initiated by role(s) '%s'.", concat_str(valueVec, ',', false).c_str())));
			}
		}
#else
		if (args.getVec("current_effective_role", valueVec))
		{
			char			*userId = NULL;
			tag_t			tUser;
			tag_t			tRole;
			char			roleName[SA_name_size_c+1];
			c_ptr<tag_t>	groupMembers;
			logical			roleFound = false;

			itk(POM_get_user_id(&userId));
			itk(SA_find_user(userId, &tUser));
			itk(SA_find_groupmember_by_user(tUser, groupMembers.get_len_ptr(), groupMembers.get_ptr()));

			for (int i = 0; i < groupMembers.get_len(); i++)
			{
				itk(SA_ask_groupmember_role(groupMembers.get(i), &tRole));
				itk(SA_ask_role_name(tRole, roleName));

				if (find_str(roleName, valueVec))
				{
					roleFound = true;
					break;
				}
			}

			if (!roleFound)
			{
				decision = EPM_nogo;
				itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, c_ptr<char>().format("Process can only be initiated by role(s) '%s'.", concat_str(valueVec, ',', false).c_str())));
			}
		}
#endif
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

	hr_stop_debug(debug_name);
	hr_print_debug(debug_name);
	log_debug("[STOP] %s", debug_name);

	return decision;
}
