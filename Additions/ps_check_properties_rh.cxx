#include "ps_global.hxx"
#include "ps_defines.hxx"
#include "ps_rule_handlers.hxx"

using namespace ps;

int ps_check_properties_rh(EPM_rule_message_t msg)
{
	const char		*debug_name = "PS2-check-properties-RH";
	char			*pszArg = NULL;
	vector<string>	vProperties;
	vector<string>	vValues;
	vector<string>	vIncludeTypes;
	string			sInvalidProps;
	h_args			args(msg.arguments);
	EPM_decision_t  decision = EPM_go;
	tag_t			tRootTask;
	c_ptr<tag_t>	tTargetAttach;

	log_debug("[START] %s", debug_name);
	hr_start_debug(debug_name);

	try
	{
		if (args.size() == 0)
			throw psexception("Missing mandatory arguments.");

		args.getVec("properties", vProperties);
		args.getVec("values", vValues);
		args.getVec("include_types", vIncludeTypes);

		if (vProperties.empty() && vValues.empty())
			throw psexception("Missing mandatory arguments.");
		if (vProperties.size() != vValues.size())
			throw psexception("Properties to check and defined values mismatch.");

		itk(EPM_ask_root_task(msg.task, &tRootTask));
		itk(EPM_ask_attachments(tRootTask, EPM_target_attachment, tTargetAttach.plen(), tTargetAttach.pptr()));

		for (int i = 0; i < tTargetAttach.len(); i++)
		{
			tag_t			tTarget = tTargetAttach.val(i);
			c_ptr<char>		targetType;

			itk(AOM_ask_value_string(tTarget, "object_type", targetType.pptr()));

			// Check if target object is valid type
			if (!vIncludeTypes.empty())
			{
				if (!find_string(targetType.ptr(), vIncludeTypes))
					continue;
			}

			// Find invalid properties
			vector<string> vInvalidProperties;
			for (int j = 0; j < vProperties.size(); j++)
			{
				string			sProperty = vProperties[j];
				string			sValue = vValues[j];
				string			sValueNull = vValues[j];
				c_ptr<char>		displayName;
				logical			lIsNullOrEmpty;
				logical			lInvalidProp = false;

				to_lower(sValueNull);
				// Check if set/unset
				if (sValueNull == "<set>" || sValueNull == "<unset>")
				{
					itk(AOM_is_null_empty(tTarget, sProperty.c_str(), true, &lIsNullOrEmpty));

					if (sValueNull == "<set>" && lIsNullOrEmpty ||
						sValueNull == "<unset>" && !lIsNullOrEmpty)
					{
						lInvalidProp = true;
					}
				}
				// Check specific string
				else
				{
					c_ptr<char>		valueTypeStr;
					c_ptr<char>		valueStr;
					PROP_value_type_t valueTypeDesc;

					itk(AOM_ask_value_type(tTarget, sProperty.c_str(), &valueTypeDesc, valueTypeStr.pptr()));

					if (valueTypeDesc == PROP_string)
					{
						itk(AOM_ask_value_string(tTarget, sProperty.c_str(), valueStr.pptr()));

						if (!match_string(sValue, valueStr.ptr()))
						{
							lInvalidProp = true;
						}
					}
				}
				if (lInvalidProp)
				{
					itk(AOM_UIF_ask_name(tTarget, sProperty.c_str(), displayName.pptr()));
					vInvalidProperties.push_back(string(displayName.ptr()));
				}
			}

			// Construct error message
			if (!vInvalidProperties.empty())
			{
				c_ptr<char>		targetDispName;

				decision = EPM_nogo;
				itk(AOM_ask_value_string(tTarget, "object_string", targetDispName.pptr()));

				sInvalidProps += "Process target '" + string(targetDispName.ptr()) + "' has invalid values for the properties: " + concat_string(vInvalidProperties, ',', false);
				//for (vector<string>::iterator str = vInvalidProperties.begin(); str != vInvalidProperties.end(); ++str)
				//{
				//	if (str + 1 != vInvalidProperties.end())
				//		sInvalidProps += "'" + *str + "', ";
				//	else
				//		sInvalidProps += "'" + *str + "'.";
				//}

				if (i + 1 < tTargetAttach.len())
				{
					sInvalidProps += "\n";
				}
			}
		}

		if (decision == EPM_nogo)
		{
			itk(EMH_store_error_s1(EMH_severity_error, RULE_HANDLER_DEFAULT_IFAIL, sInvalidProps.c_str()));
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
