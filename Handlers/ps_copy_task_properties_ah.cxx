#include "ps_global.hxx"
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
	string				taskProperty,
						targetProperty,
						includeType;
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
	vector<tag_t>		instancesToUpdate;


	log_debug("[START] %s", debug_name);
	hr_start(debug_name);

	try
	{
		if (msg.arguments->number_of_arguments == 0)
			throw psexception("Missing mandatory arguments.");

		while ((pszArg = TC_next_argument(msg.arguments)) != NULL )
		{
			c_ptr<char>		flag, value;

			itk(ITK_ask_argument_named_value(pszArg, flag.get_ptr(), value.get_ptr()));

			// Get task property to copy from
			if (tc_strcasecmp(flag.get(), "task_property") == 0)
			{
				taskProperty = value.get();
			}
			// Get target property to copy to
			else if (tc_strcasecmp(flag.get(), "target_property") == 0)
			{
				targetProperty = value.get();
			}
			// Get types to include in the target list
			else if (tc_strcasecmp(flag.get(), "include_type") == 0)
			{
				includeType = value.get();
			}
			else
			{
				throw psexception("Illegal argument.");
			}
		}

		// Verify that we have all mandatory parameters required
		if (taskProperty.empty() || targetProperty.empty())
			throw psexception("Missing mandatory arguments.");

		// Fetch class id of EPMTask class if not already done previously
		if (classIdOfTask == 0)
			itk(POM_class_id_of_class("EPMTask", &classIdOfTask));

		// Verify source object is of EPMTask type
		itk(POM_class_of_instance(msg.task, &sourceClassId));
		itk(POM_is_descendant(classIdOfTask, sourceClassId, &isDescendant));

		if (!isDescendant)
		{
			throw psexception("Source class type is not of the type 'EPMTask'.");
		}

		// Fetch source attribute metadata
		itk(POM_attr_id_of_attr(taskProperty.c_str(), "EPMTask", &attrId))
		itk(POM_describe_attrs(sourceClassId, 1, &attrId, srcAttrNames.get_pptr(), srcAttrTypes.get_ptr(),
			srcAttrMaxStrLen.get_ptr(), srcRefClass.get_ptr(), srcAttrLen.get_ptr(), srcAttrDesc.get_ptr(), srcAttrFail.get_ptr()));
		itk(POM_length_of_attr(msg.task, attrId, &numElements));

		attrType = srcAttrTypes.get(0);
		if (srcAttrLen.get(0) == 1)
			isArray = false;
		else
			isArray = true;

		// Fetch source attribute data
		if (attrType == POM_int)
		{
			if (!isArray)
			{
				itk(POM_ask_attr_int(msg.task, attrId, &intVal, &isNull, &isEmpty));
			}
			else
			{
				itk(POM_ask_attr_ints(msg.task, attrId, 0, numElements, intValArr.get_ptr(), isNullArr.get_ptr(), isEmptyArr.get_ptr()));
			}
		}
		else if (attrType == POM_string)
		{
			if (!isArray)
			{
				itk(POM_ask_attr_string(msg.task, attrId, stringVal.get_ptr(), &isNull, &isEmpty));
			}
			else
			{
				itk(POM_ask_attr_strings(msg.task, attrId, 0, numElements, stringValArr.get_pptr(), isNullArr.get_ptr(), isEmptyArr.get_ptr()));
			}
		}
		else if (attrType == POM_typed_reference || attrType == POM_untyped_reference)
		{
			if (!isArray)
			{
				itk(POM_ask_attr_tag(msg.task, attrId, &tagVal, &isNull, &isEmpty));
			}
			else
			{
				itk(POM_ask_attr_tags(msg.task, attrId, 0, numElements, tagValArr.get_ptr(), isNullArr.get_ptr(), isEmptyArr.get_ptr()));
			}
		}
		else
			throw psexception("Unsupported property value type.");

		// Loop over all process target objects
		itk(EPM_ask_root_task(msg.task, &tRootTask));
		itk(EPM_ask_attachments(tRootTask, EPM_target_attachment, tTargets.get_len_ptr(), tTargets.get_ptr()));

		for (int i = 0; i < tTargets.get_len(); i++)
		{
			tag_t			tTarget = tTargets.get(i);
			tag_t			targetClassId;
			c_ptr<char>		objectType;
			c_pptr<char>	trgAttrNames;
			c_ptr<int>		trgAttrTypes;
			c_ptr<int>		trgAttrMaxStrLen;
			c_ptr<tag_t>	trgRefClass;
			c_ptr<int>		trgAttrLen;
			c_ptr<int>		trgAttrDesc;
			c_ptr<int>		trgAttrFail;

			itk(AOM_ask_value_string(tTarget, "object_type", objectType.get_ptr()));

			// Check if the target attachment is of correct type
			if (includeType.empty() || tc_strcmp(includeType.c_str(), objectType.get()) == 0)
			{
				itk(POM_attr_id_of_attr(targetProperty.c_str(), objectType.get(), &attrId));
				itk(POM_class_of_instance(tTarget, &targetClassId));
				itk(POM_describe_attrs(sourceClassId, 1, &attrId, trgAttrNames.get_pptr(), trgAttrTypes.get_ptr(),
					trgAttrMaxStrLen.get_ptr(), trgRefClass.get_ptr(), trgAttrLen.get_ptr(), trgAttrDesc.get_ptr(), trgAttrFail.get_ptr()));

				// Verify that source and target attribute metadata match
				if (attrType != trgAttrTypes.get(0))
					throw psexception("Source and target attributes are of different type.");
				else if (srcAttrLen.get(0) != trgAttrLen.get(0))
					throw psexception("Source and target attributes have different length declaration.");
				else if (srcAttrMaxStrLen.get(0) != trgAttrMaxStrLen.get(0))
					throw psexception("Source and target attributes have different have different string length declarations.");

				// Add target to objects that is to be updated
				instancesToUpdate.push_back(tTarget);
			}
		}

		// Copy source attribute data to target attribute
		if (instancesToUpdate.size() > 0)
		{
			begin_trans(x);
			itk(POM_refresh_instances_any_class(instancesToUpdate.size(), &instancesToUpdate[0], POM_modify_lock));
			if (attrType == POM_int)
			{
				if (!isArray)
				{
					itk(POM_set_attr_int(instancesToUpdate.size(), &instancesToUpdate[0], attrId, intVal));
				}
				else
				{
					itk(POM_set_attr_ints(instancesToUpdate.size(), &instancesToUpdate[0], attrId, 0, numElements, intValArr.get()));
				}
			}
			else if (attrType == POM_string)
			{
				if (!isArray)
				{
					itk(POM_set_attr_string(instancesToUpdate.size(), &instancesToUpdate[0], attrId, stringVal.get()));
				}
				else
				{
					itk(POM_set_attr_strings(instancesToUpdate.size(), &instancesToUpdate[0], attrId, 0, numElements, stringValArr.get()));
				}
			}
			else if (attrType == POM_typed_reference || attrType == POM_untyped_reference)
			{
				if (!isArray)
				{
					itk(POM_set_attr_tag(instancesToUpdate.size(), &instancesToUpdate[0], attrId, tagVal));
				}
				else
				{
					itk(POM_set_attr_tags(instancesToUpdate.size(), &instancesToUpdate[0], attrId, 0, numElements, tagValArr.get()));
				}
			}
			itk(POM_save_instances(instancesToUpdate.size(), &instancesToUpdate[0], false));
			itk(POM_refresh_instances_any_class(instancesToUpdate.size(), &instancesToUpdate[0], POM_no_lock));
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

	hr_stop(debug_name);
	hr_print(debug_name);
	log_debug("[STOP] %s", debug_name);

	return result;
}
