#include "ps_global.hxx"
#include "ps_rule_handlers.hxx"

int ps_check_privileges_rh(EPM_rule_message_t msg)
{
	const char		*debug_name = "PS-check-privileges-RH";
	char			*pszArg = NULL,
					*pszArgFlag = NULL,
					*pszArgValue = NULL,
					*pszArgValueUpr = NULL,
					*types = NULL,
					*statuses = NULL,
					*privileges = NULL;
	EPM_decision_t  decision = EPM_go;

	ps_write_debug("[START] %s", debug_name);
	hr_start(debug_name);

	try
	{
		while ((pszArg = TC_next_argument(msg.arguments)) != NULL )
		{
			itkex(ITK_ask_argument_named_value(pszArg, &pszArgFlag, &pszArgValue));

			// Get types to include
			if (tc_strcasecmp(pszArgFlag, "types") == 0)
			{
				types = pszArgValue;
			}
			// Get statuses to include
			if (tc_strcasecmp(pszArgFlag, "statuses") == 0)
			{
				statuses = pszArgValue;
			}
			// Get privilege string
			if (tc_strcasecmp(pszArgFlag, "privilege") == 0)
			{
				tc_strupr(pszArgValue, &pszArgValueUpr);
				privileges = pszArgValueUpr;
			}
		}
		if (!ps_null_or_empty(privileges))
		{
			tag_t			tRootTask;
			c_ptr<tag_t>	tTargetAttach;
			char			*pszUserId = NULL;
			logical			hasAccess,
							typeOk = true,
							statusOk = false;
			

			itkex(EPM_ask_root_task(msg.task, &tRootTask));
			itkex(EPM_ask_attachments(tRootTask, EPM_target_attachment, tTargetAttach.get_len_ptr(), tTargetAttach.get_ptr()));
				
			for (int i = 0; i < tTargetAttach.get_len(); i++)
			{
				c_ptr<char>		targetType;
				tag_t			tTarget = tTargetAttach.get(i);
				c_ptr<tag_t>	targetStatuses;

				itkex(AOM_ask_value_string(tTarget, "object_type", targetType.get_ptr()));

				// Check if target object is valid type
				if (!ps_null_or_empty(types))
				{
					if (tc_strstr(types, targetType.get()) == NULL)
						typeOk = false;
				}

				// Check if target object has valid status
				if (ps_null_or_empty(statuses))
				{
					statusOk = true;
				}
				else
				{
					itkex(AOM_ask_value_tags(tTarget, "release_status_list", targetStatuses.get_len_ptr(), targetStatuses.get_ptr()));
					for (int j = 0; j < targetStatuses.get_len(); j++)
					{
						c_ptr<char>		statusName;

						itkex(AOM_ask_value_string(targetStatuses.get(j), "object_name", statusName.get_ptr()));

						if (tc_strstr(statuses, statusName.get()) != NULL)
						{
								statusOk = true;
								break;
						}
					}
				}
				// Check privileges
				if (typeOk && statusOk)
				{
					itkex(AM_check_privilege(tTarget, pszArgValueUpr, &hasAccess));

					if (!hasAccess)
					{
						c_ptr<char>		targetDispName;

						decision = EPM_nogo;
						itkex(AOM_ask_value_string(tTargetAttach.get(i), "object_string", targetDispName.get_ptr()));
						itkex(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_NOGO_IFAIL,
							string("Required privilege(s) not met on object '" + string(targetDispName.get()) + "' (" + string(pszArgValue) + ").").c_str()));
					}
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
