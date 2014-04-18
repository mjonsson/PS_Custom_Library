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
			// Check if allowed role
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
