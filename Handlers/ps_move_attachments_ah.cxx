#include "ps_global.hxx"
#include "ps_defines.hxx"
#include "ps_action_handlers.hxx"

using namespace ps;

int ps_move_attachments_ah(EPM_action_message_t msg)
{
	const char			*debug_name = "PS-move-attachments-AH";
	int					result = ITK_ok;
	string				sOperation;
	vector<string>		vTargetTypes;
	bool				bCopy = false;
	tag_t				tRootTask = 0;
	c_ptr<tag_t>		tTargets;
	vector<tag_t>		vAttachmentsToMove;
	vector<int>			vAttachmentsToMoveTypes;
	h_args				args(msg.arguments);

	log_debug("[START] %s", debug_name);
	hr_start_debug(debug_name);

	try
	{
		if (args.size() == 0)
			throw psexception("Missing mandatory arguments.");

		if (!args.getStr("operation", sOperation))
			throw psexception("Missing mandatory argument 'operation'");
		if (!args.getVec("include_types", vTargetTypes))
			throw psexception("Missing mandatory argument 'include_types'");
		args.getFlag("copy", bCopy);

		// Get target objects
		itk(EPM_ask_root_task(msg.task, &tRootTask));

		if (sOperation == "target_to_reference")
		{
			itk(EPM_ask_attachments(tRootTask, EPM_target_attachment, tTargets.plen(), tTargets.pptr()));
		}
		else if (sOperation == "reference_to_target")
		{
			itk(EPM_ask_attachments(tRootTask, EPM_reference_attachment, tTargets.plen(), tTargets.pptr()));
		}
		else
			throw psexception("Illegal operation entered.");

		// Find included types
		for (int i = 0; i < tTargets.len(); i++)
		{
			char	szObjectType[WSO_object_type_size_c+1];

			itk(WSOM_ask_object_type(tTargets.val(i), szObjectType));

			if (find_string(szObjectType, vTargetTypes))
			{
				vAttachmentsToMove.push_back(tTargets.val(i));
				if (sOperation == "target_to_reference")
				{
					vAttachmentsToMoveTypes.push_back(EPM_reference_attachment);
				}
				else
				{
					vAttachmentsToMoveTypes.push_back(EPM_target_attachment);
				}
			}

			if (vAttachmentsToMove.size() > 0)
			{
				if (!bCopy)
					itk(EPM_remove_attachments(tRootTask, vAttachmentsToMove.size(), &vAttachmentsToMove[0]));
				itk(EPM_add_attachments(tRootTask, vAttachmentsToMove.size(), &vAttachmentsToMove[0], &vAttachmentsToMoveTypes[0]));
			}
		}
	}
	catch (tcexception& e)
	{
		result = e.getstat();
		EMH_store_error_s1(EMH_severity_error, ACTION_HANDLER_DEFAULT_IFAIL, e.what());
		log_error(e.what());
	}
	catch (psexception& e)
	{
		result = ACTION_HANDLER_DEFAULT_IFAIL;
		EMH_store_error_s1(EMH_severity_error, ACTION_HANDLER_DEFAULT_IFAIL, e.what());
		log_error(e.what());
	}

	hr_stop_debug(debug_name);
	hr_print_debug(debug_name);
	log_debug("[STOP] %s", debug_name);

	return result;
}
