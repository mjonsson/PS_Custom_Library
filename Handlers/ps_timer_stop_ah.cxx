#include "ps_global.hxx"
#include "ps_action_handlers.hxx"

using namespace ps;

int ps_timer_stop_ah(EPM_action_message_t msg)
{
	const char			*debug_name = "PS-timer-stop-AH";
	char				*pszArg = NULL;
	string				marker;
	int					result = ITK_ok;
	EPM_decision_t		decision = EPM_go;

	log_debug("[START] %s", debug_name);

	try
	{
		if (msg.arguments->number_of_arguments == 0)
			throw psexception("Missing mandatory arguments.");

		while ((pszArg = TC_next_argument(msg.arguments)) != NULL )
		{
			c_ptr<char>		flag, value;

			itk(ITK_ask_argument_named_value(pszArg, flag.get_ptr(), value.get_ptr()));

			// Get types to include
			if (tc_strcasecmp(flag.get(), "marker") == 0)
			{
				marker = value.get();
				trim(marker);
			}
		}

		if (marker.empty())
		{
			throw psexception("Missing mandatory arguments.");
		}

		hr_stop(marker.c_str());
		hr_print(marker.c_str());
	}
	catch (tcexception& e)
	{
		decision = EPM_nogo;
		EMH_store_error_s1(EMH_severity_error, ACTION_HANDLER_DEFAULT_IFAIL, e.what());
		log_error(e.what());
	}
	catch (psexception& e)
	{
		result = ACTION_HANDLER_DEFAULT_IFAIL;
		EMH_store_error_s1(EMH_severity_error, ACTION_HANDLER_DEFAULT_IFAIL, e.what());
		log_error(e.what());
	}

	log_debug("[STOP] %s", debug_name);

	return result;
}