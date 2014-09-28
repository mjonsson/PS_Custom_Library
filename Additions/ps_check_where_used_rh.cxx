#include "ps_global.hxx"
#include "ps_defines.hxx"
#include "ps_rule_handlers.hxx"

using namespace ps;

int ps_check_where_used_rh(EPM_rule_message_t msg)
{
	const char		*debug_name = "PS2-check-where-used-RH";
	tag_t           tRootTask;
	c_ptr<tag_t>	tTargets;
	string			revRule;
	vector<string>	vincludeTargetTypes;
	vector<string>	vincludeParentTypes;
	static tag_t	tRevRule = 0;
	static tag_t	tItemRevClassId = 0;
	h_args			args(msg.arguments);
	EPM_decision_t  decision = EPM_go;

	log_debug("[START] %s", debug_name);
	hr_start_debug(debug_name);

	try
	{
		if (args.size() == 0)
			throw psexception("Missing mandatory arguments.");

		if (!args.getStr("rev_rule", revRule))
			throw psexception("Missing mandatory argument 'rev_rule'.");
		args.getVec("include_target_types", vincludeTargetTypes);
		args.getVec("include_parent_types", vincludeParentTypes);

		if (tRevRule == 0)
		{
			itk(CFM_find(revRule.c_str(), &tRevRule));
			if (tRevRule == 0)
				throw psexception("Revision rule '" + revRule + "' could not be found.");
		}

		if (tItemRevClassId == 0)
		{
			itk(POM_class_id_of_class("ItemRevision", &tItemRevClassId));
		}

		itk(EPM_ask_root_task(msg.task, &tRootTask));
		itk(EPM_ask_attachments(tRootTask, EPM_target_attachment, tTargets.plen(), tTargets.pptr()));

		// Iterate over all targets to pre-populate targets to process and their item ids
		for (int i = 0; i < tTargets.len(); i++)
		{
			tag_t			tTarget = tTargets.val(i);
			tag_t			tClassOfTarget;
			c_ptr<char>		itemId;
			c_ptr<char>		objectType;
			logical			isDescendant = false;
			c_ptr<tag_t>	tParents;
			c_ptr<int>		iLevels;

			itk(POM_class_of_instance(tTarget, &tClassOfTarget));
			itk(POM_is_descendant(tItemRevClassId, tClassOfTarget, &isDescendant));

			if (isDescendant)
			{
				if (!vincludeTargetTypes.empty())
				{
					itk(AOM_ask_value_string(tTarget, "object_type", objectType.pptr()));

					// Skip target if not of valid type
					if (!find_string(objectType.ptr(), vincludeTargetTypes))
						continue;
				}

				itk(PS_where_used_configured(tTarget, tRevRule, 1, tParents.plen(), iLevels.pptr(), tParents.pptr()));

				if (!vincludeParentTypes.empty())
				{
					// Loop over all structure children and verify the validity
					for (int j = 0; j < tParents.len(); j++)
					{
						tag_t			tParent = tParents.val(j);
						c_ptr<char>		parentObjectType;

						itk(AOM_ask_value_string(tParent, "object_type", parentObjectType.pptr()));

						// If not valid structure type, jump to next target
						if (!find_string(parentObjectType.ptr(), vincludeParentTypes))
							continue;
					}
				}

				if (tParents.len() > 0)
				{
					c_ptr<char>		targetObjectStr;

					decision = EPM_nogo;

					itk(AOM_ask_value_string(tTarget, "object_string", targetObjectStr.pptr()));
					itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, c_ptr<char>("Process target '%s' is referenced by one or several structure parents which is not valid for this process.", targetObjectStr.ptr()).ptr()));
				}
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