#include "ps_global.hxx"
#include "ps_defines.hxx"
#include "ps_rule_handlers.hxx"
#include "ps_action_handlers.hxx"
#include "ps_register.hxx"

using namespace ps;

void ps::ps_register_handlers()
{
	// Action handlers
	itk(EPM_register_action_handler(AH_CREATE_DISPATCHER_REQUEST, "Create dispatcher request.", (EPM_action_handler_t) ps_create_dispatcher_request_ah));
	itk(EPM_register_action_handler(AH_COPY_TASK_PROPERTIES, "Copy properties from task to process target.", (EPM_action_handler_t) ps_copy_task_properties_ah));
	itk(EPM_register_action_handler(AH_MOVE_ATTACHMENTS, "Move attachments from target to reference or vice versa.", (EPM_action_handler_t) ps_move_attachments_ah));
	itk(EPM_register_action_handler(AH_TIMER_START, "Starts a performance timer.", (EPM_action_handler_t) ps_timer_start_ah));
	itk(EPM_register_action_handler(AH_TIMER_STOP, "Stops a performance timer.", (EPM_action_handler_t) ps_timer_stop_ah));
	itk(EPM_register_action_handler(AH_SET_STATUS, "Creates and assigns release statuses.", (EPM_action_handler_t) ps_set_statuses_ah));
	// Rule handlers
	itk(EPM_register_rule_handler(AH_CHECK_IN_PROCESS, "Verify that no targets are in other processes.", (EPM_rule_handler_t) ps_check_in_process_rh));
	itk(EPM_register_rule_handler(AH_CHECK_INITIATOR, "Verify that initiating user is valid.", (EPM_rule_handler_t) ps_check_initiator_rh));
	itk(EPM_register_rule_handler(AH_CHECK_PRIVILEGES, "Verify that initiating user has correct privileges.", (EPM_rule_handler_t) ps_check_privileges_rh));
	itk(EPM_register_rule_handler(AH_CHECK_PROPERTIES, "Verify that mandatory properties are set.", (EPM_rule_handler_t) ps_check_properties_rh));
	itk(EPM_register_rule_handler(AH_CHECK_CHILD_STRUCTURE, "Verify that child structure have correct status.", (EPM_rule_handler_t) ps_check_child_structure_rh));
	itk(EPM_register_rule_handler(AH_CHECK_WHERE_USED, "Verify that target does not have an invalid structure parent.", (EPM_rule_handler_t) ps_check_where_used_rh));
}

int ps::libps_oninit(int *decision, va_list args)
{
	try
	{
		// Initialize logging mechanism
		if (initialize_logging())
			printf("Logging module enabled.\n");
		// If enabled, initialize HRTimer class
		if (hr_init())
			printf("HRTimer module enabled.\n");

		//char ch;
		//ch = getchar();

		// Register handlers
		try
		{
			ps_register_handlers();
		}
		catch (exception &e)
		{
			TC_write_syslog("%s\n", e.what());
		}
	}
	catch (tcexception& e)
	{
		TC_write_syslog("%s\n", e.what());

		return e.getstat();
	}
	catch (exception& e)
	{
		TC_write_syslog("%s\n", e.what());

		return 1;
	}

	return ITK_ok;
}

int ps::libps_onexit(int *decision, va_list args)
{
	try
	{
		// Print performance statistics when logging out
		hr_print_all();
	}
	catch (tcexception& e)
	{
		TC_write_syslog("%s\n", e.what());

		return e.getstat();
	}
	catch (exception& e)
	{
		TC_write_syslog("%s\n", e.what());

		return 1;
	}

	return ITK_ok;
}

int libpsadds_register_callbacks()
{ 
	printf("Installing PS Handler Library - %s, "__DATE__" "__TIME__".\n", PS_LIBNAME);

	try
	{
		itk(CUSTOM_register_exit(PS_LIBNAME, "USER_init_module", (CUSTOM_EXIT_ftn_t) libps_oninit));
		itk(CUSTOM_register_exit(PS_LIBNAME, "USER_exit_module", (CUSTOM_EXIT_ftn_t) libps_onexit));
	}
	catch (tcexception& e)
	{
		TC_write_syslog(e.what());

		return e.getstat();
	}
	catch (exception& e)
	{
		TC_write_syslog(e.what());

		return 1;
	}

	return ITK_ok;
}