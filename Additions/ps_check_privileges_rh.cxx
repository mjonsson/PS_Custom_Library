#include "ps_global.hxx"
#include "ps_defines.hxx"
#include "ps_rule_handlers.hxx"

using namespace ps;

int ps_check_privileges_rh(EPM_rule_message_t msg)
{
	const char		*debug_name = "PS2-check-privileges-RH";
	char			*pszArg = NULL;
	vector<string>	privileges;
	vector<string>	objectTypes;
	vector<string>	statuses;
	logical			owning_user = false,
					owning_group = false;
	h_args			args(msg.arguments);
	EPM_decision_t  decision = EPM_go;
	tag_t			tRootTask;
	c_ptr<tag_t>	tTargetAttach;
	char			*pszUserId = NULL;

	log_debug("[START] %s", debug_name);
	hr_start_debug(debug_name);

	try
	{
		if (args.size() == 0)
			throw psexception("Missing mandatory arguments.");

		args.getVec("privileges", privileges);
		args.getFlag("owning_user", owning_user);
		args.getFlag("owning_group", owning_group);
		
		if (privileges.empty() && !owning_user && !owning_group)
			throw psexception("Missing mandatory arguments.");

		args.getVec("object_types", objectTypes);
		args.getVec("include_statuses", statuses);

		itk(EPM_ask_root_task(msg.task, &tRootTask));
		itk(EPM_ask_attachments(tRootTask, EPM_target_attachment, tTargetAttach.plen(), tTargetAttach.pptr()));
				
		for (int i = 0; i < tTargetAttach.len(); i++)
		{
			logical			typeOk = true;
			logical			statusOk = true;
			logical			hasAccess;
			c_ptr<char>		targetType;
			tag_t			tTarget = tTargetAttach.val(i);
			c_ptr<tag_t>	targetStatuses;

			itk(AOM_ask_value_string(tTarget, "object_type", targetType.pptr()));

			// Check if target object is valid type
			if (!objectTypes.empty())
			{
				typeOk = false;

				if (find_string(targetType.ptr(), objectTypes))
					typeOk = true;
			}

			// Check if target object has valid status
			if (!statuses.empty())
			{
				statusOk = false;
				
				itk(AOM_ask_value_tags(tTarget, "release_status_list", targetStatuses.plen(), targetStatuses.pptr()));

				if (targetStatuses.len() == 0 && find_string("Working", statuses))
				{
					statusOk = true;
				}
				else
				{
					for (int j = 0; j < targetStatuses.len(); j++)
					{
						c_ptr<char>		statusName;

						itk(AOM_ask_value_string(targetStatuses.val(j), "object_name", statusName.pptr()));

						if (find_string(statusName.ptr(), statuses))
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
						itk(AOM_ask_value_string(tTargetAttach.val(i), "object_string", targetDispName.pptr()));
						itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, c_ptr<char>("Required privilege(s) not met on object '%s' (%s).", targetDispName.ptr(), concat_string(privileges, ',', false).c_str()).ptr()));
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
						itk(AOM_ask_value_string(tTargetAttach.val(i), "object_string", targetDispName.pptr()));
						itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, c_ptr<char>("Current user is not the owner of object '%s'.", targetDispName.ptr()).ptr()));
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
						itk(AOM_ask_value_string(tTargetAttach.val(i), "object_string", targetDispName.pptr()));
						itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, c_ptr<char>("Current group is not the owner of object '%s'.", targetDispName.ptr()).ptr()));
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

	hr_stop_debug(debug_name);
	hr_print_debug(debug_name);
	log_debug("[STOP] %s", debug_name);

	return decision;
}
