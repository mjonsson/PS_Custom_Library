#include "ps_global.hxx"
#include "ps_defines.hxx"
#include "ps_action_handlers.hxx"

using namespace ps;

int ps_copy_task_properties_ah(EPM_action_message_t msg)
{
	const char			*debug_name = "PS2-copy-task-properties-AH";
	string				taskProperties;
	string				targetProperties;
	vector<string>		vTaskProperties;
	vector<string>		vTargetProperties;
	string				sTargetType;
	string				sTargetAttachments;
	tag_t				tRootTask;
	c_ptr<tag_t>		tTargets;
	int					result = ITK_ok;
	int					x;
	vector<tag_t>		vInstancesToUpdate;
	h_args				args(msg.arguments);

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
		if (!args.getStr("include_type", sTargetType))
			throw psexception("Missing mandatory argument 'include_type'");
		if (!args.getStr("attachment_type", sTargetAttachments))
			throw psexception("Missing mandatory argument 'attachment_type'");

		if (vTaskProperties.size() != vTargetProperties.size())
			throw psexception("Task properties size and target properties size are not equal.");

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

			if (match_string(szObjectType, sTargetType))
			{
				vInstancesToUpdate.push_back(tTargets.val(i));
			}
		}

		// Only proceed if there are valid target types
		if (vInstancesToUpdate.size() > 0)
		{
			tag_t				sourceClassId;

			itk(POM_class_id_of_class("EPMTask", &sourceClassId));

			begin_trans(x);
			itk(POM_refresh_instances_any_class(vInstancesToUpdate.size(), &vInstancesToUpdate[0], POM_modify_lock));

			vector<string>::iterator taskAttrIt = vTaskProperties.begin();
			vector<string>::iterator targetAttrIt = vTargetProperties.begin();

			while (taskAttrIt != vTaskProperties.end())
			{
				logical				isEmpty;
				logical				isNull;
				logical				isArray;
				int					numElements;
				int					attrType;
				tag_t				taskAttrId;
				tag_t				targetAttrId;		
				int					intVal;
				tag_t				tagVal;
				c_ptr<char>			stringVal;
				c_ptr<int>			intValArr;
				c_ptr<tag_t>		tagValArr;
				c_pptr<char>		stringValArr;
				c_pptr<char>		srcAttrNames;
				c_ptr<int>			srcAttrTypes;
				c_ptr<int>			srcAttrMaxStrLen;
				c_ptr<tag_t>		srcRefClass;
				c_ptr<int>			srcAttrLen;
				c_ptr<int>			srcAttrDesc;
				c_ptr<int>			srcAttrFail;
				c_ptr<logical>		isNullArr;
				c_ptr<logical>		isEmptyArr;

				string taskProperty = *taskAttrIt;
				string targerProperty = *targetAttrIt;

				// Get attribute id of task and target properties
				itk(POM_attr_id_of_attr(taskProperty.c_str(), "EPMTask", &taskAttrId));
				itk(POM_attr_id_of_attr(targerProperty.c_str(), sTargetType.c_str(), &targetAttrId));

				// Fetch source attribute metadata
				itk(POM_describe_attrs(sourceClassId, 1, &taskAttrId, srcAttrNames.pptr(), srcAttrTypes.pptr(),
					srcAttrMaxStrLen.pptr(), srcRefClass.pptr(), srcAttrLen.pptr(), srcAttrDesc.pptr(), srcAttrFail.pptr()));
				itk(POM_length_of_attr(msg.task, taskAttrId, &numElements));

				attrType = srcAttrTypes.val(0);
				if (srcAttrLen.val(0) == 1)
					isArray = false;
				else
					isArray = true;

				// Fetch task property value and set to target property
				switch (attrType)
				{
				case POM_int:
					if (!isArray)
					{
						itk(POM_ask_attr_int(msg.task, taskAttrId, &intVal, &isNull, &isEmpty));
						itk(POM_set_attr_int(vInstancesToUpdate.size(), &vInstancesToUpdate[0], targetAttrId, intVal));
					}
					else
					{
						itk(POM_ask_attr_ints(msg.task, taskAttrId, 0, numElements, intValArr.pptr(), isNullArr.pptr(), isEmptyArr.pptr()));
						itk(POM_set_attr_ints(vInstancesToUpdate.size(), &vInstancesToUpdate[0], targetAttrId, 0, numElements, intValArr.ptr()));
					}
					break;
				case POM_string:
					if (!isArray)
					{
						itk(POM_ask_attr_string(msg.task, taskAttrId, stringVal.pptr(), &isNull, &isEmpty));
						itk(POM_set_attr_string(vInstancesToUpdate.size(), &vInstancesToUpdate[0], targetAttrId, stringVal.ptr()));
					}
					else
					{
						itk(POM_ask_attr_strings(msg.task, taskAttrId, 0, numElements, stringValArr.pptr(), isNullArr.pptr(), isEmptyArr.pptr()));
						itk(POM_set_attr_strings(vInstancesToUpdate.size(), &vInstancesToUpdate[0], targetAttrId, 0, numElements, stringValArr.ptr()));
					}
					break;
				case POM_typed_reference:
					if (!isArray)
					{
						itk(POM_ask_attr_tag(msg.task, taskAttrId, &tagVal, &isNull, &isEmpty));
						itk(POM_set_attr_tag(vInstancesToUpdate.size(), &vInstancesToUpdate[0], targetAttrId, tagVal));
					}
					else
					{
						itk(POM_ask_attr_tags(msg.task, taskAttrId, 0, numElements, tagValArr.pptr(), isNullArr.pptr(), isEmptyArr.pptr()));
						itk(POM_set_attr_tags(vInstancesToUpdate.size(), &vInstancesToUpdate[0], targetAttrId, 0, numElements, tagValArr.ptr()));
					}
					break;
				case POM_untyped_reference:
					if (!isArray)
					{
						itk(POM_ask_attr_tag(msg.task, taskAttrId, &tagVal, &isNull, &isEmpty));
						itk(POM_set_attr_tag(vInstancesToUpdate.size(), &vInstancesToUpdate[0], targetAttrId, tagVal));
					}
					else
					{
						itk(POM_ask_attr_tags(msg.task, taskAttrId, 0, numElements, tagValArr.pptr(), isNullArr.pptr(), isEmptyArr.pptr()));
						itk(POM_set_attr_tags(vInstancesToUpdate.size(), &vInstancesToUpdate[0], targetAttrId, 0, numElements, tagValArr.ptr()));
					}
					break;
				default:
					throw psexception("Unsupported property value type.");
					break;
				}

				++taskAttrIt;
				++targetAttrIt;
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
