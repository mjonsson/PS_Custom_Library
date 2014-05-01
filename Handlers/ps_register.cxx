#include "ps_global.hxx"
#include "ps_rule_handlers.hxx"
#include "ps_action_handlers.hxx"
#include "ps_register.hxx"

using namespace ps;

int libps_oninit(int *decision, va_list args)
{
	try
	{
		// If enabled, initialize HRTimer class
		logical hr_enabled = hr_init();
		if (hr_enabled)
			log_info("HRTimer class is enabled.");

		// Action handlers
		itk(EPM_register_action_handler("PS-create-dispatcher-request-AH", "Create dispatcher request.", (EPM_action_handler_t) ps_create_dispatcher_request_ah));
		itk(EPM_register_action_handler("PS-copy-task-property-AH", "Copy properties from task to process target.", (EPM_action_handler_t) ps_copy_task_property_ah));
		itk(EPM_register_action_handler("PS-timer-start-AH", "Starts a performance timer.", (EPM_action_handler_t) ps_timer_start_ah));
		itk(EPM_register_action_handler("PS-timer-stop-AH", "Stops a performance timer.", (EPM_action_handler_t) ps_timer_stop_ah));
		// Rule handlers
		itk(EPM_register_rule_handler("PS-check-in-process-RH", "Verify that no targets are in other processes.", (EPM_rule_handler_t) ps_check_in_process_rh));
		itk(EPM_register_rule_handler("PS-check-initiator-RH", "Verify that initiating user is valid.", (EPM_rule_handler_t) ps_check_initiator_rh));
		itk(EPM_register_rule_handler("PS-check-privileges-RH", "Verify that initiating user has correct privileges.", (EPM_rule_handler_t) ps_check_privileges_rh));
		itk(EPM_register_rule_handler("PS-check-child-structure-RH", "Verify that child structure have correct status.", (EPM_rule_handler_t) ps_check_child_structure_rh));
	}
	catch (tcexception& e)
	{
		log_error(e.what());

		return e.getstat();
	}
	catch (exception& e)
	{
		log_error(e.what());

		return 1;
	}

	return ITK_ok;
}

int libps_onexit(int *decision, va_list args)
{
	try
	{
		hr_print_all();
	}
	catch (tcexception& e)
	{
		log_error(e.what());

		return e.getstat();
	}
	catch (exception& e)
	{
		log_error(e.what());

		return 1;
	}

	return ITK_ok;
}

int libpshandlers_register_callbacks()
{ 
	log_info("Installing PS User Exits Library - libps v1.0, "__DATE__" "__TIME__"\n");
	
	try
	{
		itk(CUSTOM_register_exit("libpshandlers", "USER_init_module", (CUSTOM_EXIT_ftn_t) libps_oninit));
		itk(CUSTOM_register_exit("libpshandlers", "USER_exit_module", (CUSTOM_EXIT_ftn_t) libps_onexit));
	}
	catch (tcexception& e)
	{
		log_error(e.what());

		return e.getstat();
	}
	catch (exception& e)
	{
		log_error(e.what());

		return 1;
	}

	return ITK_ok;
}
