#include "ps_global.hxx"
#include "ps_defines.hxx"
#include "ps_action_handlers.hxx"

using namespace ps;

int ps_copy_task_properties_ah(EPM_action_message_t msg)
{
	const char			*debug_name = "PS-copy-task-properties-AH";
	logical				isEmpty;
	logical				isNull;
	logical				isDescendant;
	logical				isArray;
	int					attrType;
	tag_t				sourceClassId;
	static tag_t		classIdOfTask;
	char				*pszArg = NULL;
	string				taskProperties,
		targetProperties;
	vector<string>		vTaskProperties;
	vector<string>		vTargetProperties;
	vector<string>		vTargetTypes;
	string				sTargetAttachments;
	tag_t				tRootTask;
	c_ptr<tag_t>		tTargets;
	int					result = ITK_ok,
		numElements,
		x;
	int					intVal;
	tag_t				tagVal;
	c_ptr<char>			stringVal;
	c_ptr<int>			intValArr;
	c_ptr<tag_t>		tagValArr;
	c_pptr<char>		stringValArr;
	tag_t				attrId;
	c_pptr<char>		srcAttrNames;
	c_ptr<int>			srcAttrTypes;
	c_ptr<int>			srcAttrMaxStrLen;
	c_ptr<tag_t>		srcRefClass;
	c_ptr<int>			srcAttrLen;
	c_ptr<int>			srcAttrDesc;
	c_ptr<int>			srcAttrFail;
	c_ptr<logical>		isNullArr;
	c_ptr<logical>		isEmptyArr;
	h_args				args(msg.arguments);
	vector<tag_t>		vInstancesToUpdate;

	log_debug("[START] %s", debug_name);
	hr_start_debug(debug_name);

	try
	{
		if (args.size() == 0)
			throw psexception("Missing mandatory arguments.");

		if (!args.getVec("task_properties", vTaskProperties))
			throw psexception("Missing mandatory argument 'task_properties'");
		if (!args.getVec("target_properties", vTargetProperties))
			throw psexception("Missing mandatory argument 'target_properties'");
		if (!args.getVec("include_types", vTargetTypes))
			throw psexception("Missing mandatory argument 'include_types'");
		if (!args.getStr("attachment_type", sTargetAttachments))
			throw psexception("Missing mandatory argument 'attachment_type'");

		// Get target objects
		itk(EPM_ask_root_task(msg.task, &tRootTask));
		to_lower(sTargetAttachments);
		if (sTargetAttachments == "target")
		{
			itk(EPM_ask_attachments(tRootTask, EPM_target_attachment, tTargets.plen(), tTargets.pptr()));
		}
		else if (sTargetAttachments == "reference")
		{
			itk(EPM_ask_attachments(tRootTask, EPM_reference_attachment, tTargets.plen(), tTargets.pptr()));
		}

		// Find included types
		for (int i = 0; i < tTargets.len(); i++)
		{
			char	szObjectType[WSO_object_type_size_c+1];

			itk(WSOM_ask_object_type(tTargets.val(i), szObjectType));

			if (find_string(szObjectType, vTargetTypes))
			{
				vInstancesToUpdate.push_back(tTargets.val(i));
			}
		}

		// Only proceed if there are valid target types
		if (vInstancesToUpdate.size() > 0)
		{
			itk(POM_class_id_of_class("EPMTask", &sourceClassId));

			begin_trans(x);
			itk(POM_refresh_instances_any_class(vInstancesToUpdate.size(), &vInstancesToUpdate[0], POM_modify_lock));

			// Fetch source attribute metadata
			for (vector<string>::iterator it = vTaskProperties.begin(); it < vTaskProperties.end(); ++it)
			{
				string taskProperty = *it;
				itk(POM_attr_id_of_attr(taskProperty.c_str(), "EPMTask", &attrId));
				itk(POM_describe_attrs(sourceClassId, 1, &attrId, srcAttrNames.pptr(), srcAttrTypes.pptr(),
					srcAttrMaxStrLen.pptr(), srcRefClass.pptr(), srcAttrLen.pptr(), srcAttrDesc.pptr(), srcAttrFail.pptr()));
				itk(POM_length_of_attr(msg.task, attrId, &numElements));

				attrType = srcAttrTypes.val(0);
				if (srcAttrLen.val(0) == 1)
					isArray = false;
				else
					isArray = true;

				// Fetch source attribute data
				if (attrType == POM_int)
				{
					if (!isArray)
					{
						itk(POM_ask_attr_int(msg.task, attrId, &intVal, &isNull, &isEmpty));
						itk(POM_set_attr_int(vInstancesToUpdate.size(), &vInstancesToUpdate[0], attrId, intVal));
					}
					else
					{
						itk(POM_ask_attr_ints(msg.task, attrId, 0, numElements, intValArr.pptr(), isNullArr.pptr(), isEmptyArr.pptr()));
						itk(POM_set_attr_ints(vInstancesToUpdate.size(), &vInstancesToUpdate[0], attrId, 0, numElements, intValArr.ptr()));
					}
				}
				else if (attrType == POM_string)
				{
					if (!isArray)
					{
						itk(POM_ask_attr_string(msg.task, attrId, stringVal.pptr(), &isNull, &isEmpty));
						itk(POM_set_attr_string(vInstancesToUpdate.size(), &vInstancesToUpdate[0], attrId, stringVal.ptr()));
					}
					else
					{
						itk(POM_ask_attr_strings(msg.task, attrId, 0, numElements, stringValArr.pptr(), isNullArr.pptr(), isEmptyArr.pptr()));
						itk(POM_set_attr_strings(vInstancesToUpdate.size(), &vInstancesToUpdate[0], attrId, 0, numElements, stringValArr.ptr()));
					}
				}
				else if (attrType == POM_typed_reference || attrType == POM_untyped_reference)
				{
					if (!isArray)
					{
						itk(POM_ask_attr_tag(msg.task, attrId, &tagVal, &isNull, &isEmpty));
						itk(POM_set_attr_tag(vInstancesToUpdate.size(), &vInstancesToUpdate[0], attrId, tagVal));
					}
					else
					{
						itk(POM_ask_attr_tags(msg.task, attrId, 0, numElements, tagValArr.pptr(), isNullArr.pptr(), isEmptyArr.pptr()));
						itk(POM_set_attr_tags(vInstancesToUpdate.size(), &vInstancesToUpdate[0], attrId, 0, numElements, tagValArr.ptr()));
					}
				}
				else
					throw psexception("Unsupported property value type.");
			}
			itk(POM_save_instances(vInstancesToUpdate.size(), &vInstancesToUpdate[0], false));
			itk(POM_refresh_instances_any_class(vInstancesToUpdate.size(), &vInstancesToUpdate[0], POM_no_lock));
			commit_trans(x);
		}
	}
	catch (tcexception& e)
	{
		rollback_trans(x);
		result = e.getstat();
		EMH_store_error_s1(EMH_severity_error, ACTION_HANDLER_DEFAULT_IFAIL, e.what());
		log_error(e.what());
	}
	catch (psexception& e)
	{
		rollback_trans(x);
		result = ACTION_HANDLER_DEFAULT_IFAIL;
		EMH_store_error_s1(EMH_severity_error, ACTION_HANDLER_DEFAULT_IFAIL, e.what());
		log_error(e.what());
	}

	hr_stop_debug(debug_name);
	hr_print_debug(debug_name);
	log_debug("[STOP] %s", debug_name);

	return result;
}
