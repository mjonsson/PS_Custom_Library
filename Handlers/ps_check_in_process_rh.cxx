#include "ps_global.hxx"
#include "ps_defines.hxx"
#include "ps_rule_handlers.hxx"

using namespace ps;

int ps_check_in_process_rh(EPM_rule_message_t msg)
{
	const char *debug_name = "PS2-check-in-process-RH";
    tag_t           tRootTask;
	c_ptr<tag_t>	tTargets;
	vector<tag_t>	tObjInProcess;
	EPM_decision_t  decision = EPM_go;

	log_debug("[START] %s", debug_name);
	hr_start_debug(debug_name);

	try
	{
		itk(EPM_ask_root_task(msg.task, &tRootTask));
		itk(EPM_ask_attachments(tRootTask, EPM_target_attachment, tTargets.plen(), tTargets.pptr()));

		// Loop over all target attachments
		for (int i = 0; i < tTargets.len(); i++)
		{
			c_ptr<tag_t>	processTags;
			tag_t			tTarget = tTargets.val(i);
			int				counter = 0;

			itk(AOM_ask_value_tags(tTarget, "process_stage_list", processTags.plen(), processTags.pptr()));

			// Iterate over all the tasks that relates to this process target
			for (int j = 0; j < processTags.len(); j++)
			{
				tag_t	tParentTag;

				itk(AOM_ask_value_tag(processTags.val(j), "parent_task", &tParentTag));

				// If parent tag is not set, this means we have a root task
				if (tParentTag == NULLTAG)
				{
					counter++;
				}
				// If more than 1 root task points to this process target
				if (counter > 1)
				{
					tObjInProcess.push_back(tTarget);
					break;
				}
			}
		}

		// If we have any process target that is in more than one process
		if (tObjInProcess.size() > 0)
		{
			decision = EPM_nogo;

			// Loop over targets to generate error message
			for (vector<tag_t>::iterator ptTarget = tObjInProcess.begin(); ptTarget != tObjInProcess.end(); ++ptTarget)
			{
				c_ptr<char>		targetDispName;

				itk(AOM_ask_value_string(*ptTarget, "object_string", targetDispName.pptr()));
				itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, c_ptr<char>("Process target '%s' is already within another process.", targetDispName.ptr()).ptr()));
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