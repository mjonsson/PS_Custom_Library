#include "ps_global.hxx"
#include "ps_rule_handlers.hxx"
#include "ps_action_handlers.hxx"
#include "ps_register.hxx"

int libps_register(int *decision, va_list args)
{
	try
	{
		itkex(EPM_register_rule_handler("PS-check-in-process-RH", "Verify that no targets are in other processes.", (EPM_rule_handler_t) ps_check_in_process_rh));
		itkex(EPM_register_rule_handler("PS-check-initiator-RH", "Verify that initiating user is valid.", (EPM_rule_handler_t) ps_check_initiator_rh));
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
		itkex(CUSTOM_register_exit("libpshandlers", "USER_gs_shell_init_module", (CUSTOM_EXIT_ftn_t) libps_register));
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
