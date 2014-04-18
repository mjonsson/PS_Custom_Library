#include "ps_global.hxx"
#include "ps_rule_handlers.hxx"

int ps_check_in_process_rh(EPM_rule_message_t msg)
{
	const char *debug_name = "PS-check-in-process-RH";
    tag_t           tRootTask;
	c_ptr<tag_t>	tTargets;
	vector<tag_t>	tObjInProcess;
	EPM_decision_t  decision = EPM_go;

	ps_write_debug("[START] %s", debug_name);
	hr_start(debug_name);

	try
	{
		itkex(EPM_ask_root_task(msg.task, &tRootTask));
		itkex(EPM_ask_attachments(tRootTask, EPM_target_attachment, tTargets.get_len_ptr(), tTargets.get_ptr()));

		// Loop over all target attachments
		for (int i = 0; i < tTargets.get_len(); i++)
		{
			c_ptr<tag_t>	processTags;
			tag_t			tTarget = tTargets.get(i);
			int				counter = 0;

			itkex(AOM_ask_value_tags(tTarget, "process_stage_list", processTags.get_len_ptr(), processTags.get_ptr()));

			// Iterate over all the tasks that relates to this process target
			for (int j = 0; j < processTags.get_len(); j++)
			{
				tag_t	tParentTag;

				itkex(AOM_ask_value_tag(processTags.get(j), "parent_task", &tParentTag));

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
			for (vector<tag_t>::iterator ptTarget = tObjInProcess.begin(); ptTarget < tObjInProcess.end(); ++ptTarget)
			{
				c_ptr<char>		targetDispName;

				itkex(AOM_ask_value_string(*ptTarget, "object_string", targetDispName.get_ptr()));
				itkex(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_NOGO_IFAIL,
					string("Process target '" + string(targetDispName.get()) + "' is already within another process.").c_str()));
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