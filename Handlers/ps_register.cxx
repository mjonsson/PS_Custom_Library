#include "ps_global.hxx"
#include "ps_rule_handlers.hxx"
#include "ps_action_handlers.hxx"
#include "ps_register.hxx"

int libps_register(int *decision, va_list args)
{
	try
	{
		// Action handlers
		itk(EPM_register_action_handler("PS-create-dispatcher-request-AH", "Create dispatcher request.", (EPM_action_handler_t) ps_create_dispatcher_request_ah));
		itk(EPM_register_action_handler("PS-copy-task-properties-AH", "Copy properties from task to process target.", (EPM_action_handler_t) ps_copy_task_properties_ah));
		// Rule handlers
		itk(EPM_register_rule_handler("PS-check-in-process-RH", "Verify that no targets are in other processes.", (EPM_rule_handler_t) ps_check_in_process_rh));
		itk(EPM_register_rule_handler("PS-check-initiator-RH", "Verify that initiating user is valid.", (EPM_rule_handler_t) ps_check_initiator_rh));
		itk(EPM_register_rule_handler("PS-check-privileges-RH", "Verify that initiating user has correct privileges.", (EPM_rule_handler_t) ps_check_privileges_rh));
	}
	catch (tcexception& e)
	{
		ps_write_error(e.what());

		return e.getstat();
	}
	catch (exception& e)
	{
		ps_write_error(e.what());

		return 1;
	}

	return ITK_ok;
}

extern DLLAPI int libpshandlers_register_callbacks()
{ 
	ps_write_info("Installing PS User Exits Library - libps v1.0, "__DATE__" "__TIME__"\n");
	
	try {
		itk(CUSTOM_register_exit("libpshandlers", "USER_gs_shell_init_module", (CUSTOM_EXIT_ftn_t) libps_register));
	}
	catch (tcexception& e)
	{
		ps_write_error(e.what());

		return e.getstat();
	}
	catch (exception& e)
	{
		ps_write_error(e.what());

		return 1;
	}

	return ITK_ok;
}
