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
		while ((pszArg = TC_next_argument(msg.arguments)) != NULL )
		{
			itkex(ITK_ask_argument_named_value(pszArg, &pszArgFlag, &pszArgValue));

			// Check if write access
			if (tc_strcasecmp(pszArgFlag, "privilege") == 0)
			{
				tag_t			tRootTask;
				c_ptr<tag_t>	tTargetAttach;
				char			*pszUserId = NULL;
				logical			hasAccess;

				itkex(EPM_ask_root_task(msg.task, &tRootTask));
				itkex(EPM_ask_attachments(tRootTask, EPM_target_attachment, tTargetAttach.get_len_ptr(), tTargetAttach.get_ptr()));
				
				for (int i = 0; i < tTargetAttach.get_len(); i++)
				{
					c_ptr<char>	pszArgValueUpr;

					tc_strupr(pszArgValue, pszArgValueUpr.get_ptr());
					itkex(AM_check_privilege(tTargetAttach.get(i), pszArgValueUpr.get(), &hasAccess));

					if (!hasAccess)
					{
						c_ptr<char>		targetDispName;

						decision = EPM_nogo;
						itkex(AOM_ask_value_string(tTargetAttach.get(i), "object_string", targetDispName.get_ptr()));
						itkex(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_NOGO_IFAIL,
							string("Not required privilege(s) to object '" + string(targetDispName.get()) + "' (" + string(pszArgValue) + ").").c_str()));
					}
				}
			}
			// Check if allowed user
			if (tc_strcasecmp(pszArgFlag, "user") == 0)
			{
				char	*pszUserId = NULL;

				itkex(POM_get_user_id(&pszUserId));

				if (strstr(pszArgValue, pszUserId) == NULL)
				{
					decision = EPM_nogo;
					itkex(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_NOGO_IFAIL,
						string("Process can only be initiated by user(s) '" + string(pszArgValue) + "'.").c_str()));
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
