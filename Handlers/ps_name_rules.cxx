#include "ps_global.hxx"
#include "ps_defines.hxx"
#include "ps_name_rules.hxx"
#include <regex>
#include <sstream>

using namespace tr1;
using namespace ps;

int ps::ps_validate_name_rule(METHOD_message_t *m, va_list  args)
{
	int				result = ITK_ok;
	string			propType = m->user_args->arguments[0].val_union.str_value;
	string			regEx = m->user_args->arguments[1].val_union.str_value;
	int				maxLength = m->user_args->arguments[2].val_union.int_value;
	string			instruction = m->user_args->arguments[3].val_union.str_value;
	string			value;
	c_ptr<char>		propertyName;
	tag_t			tProp = va_arg(args, tag_t);
	
	if (propType == "int")
	{
		value = va_arg(args, int);
	}
	else if (propType == "float")
	{
		value = va_arg(args, float);
	}
	else if (propType == "double")
	{
		value = va_arg(args, double);
	}
	else if (propType == "string")
	{
		value = va_arg(args, char*);
	}

	try
	{
		if (value.length() > maxLength && maxLength > 0)
		{
			throw psexception(instruction);
		}
		regex rx(regEx);
		if (!regex_match(value.begin(), value.end(), rx))
		{
			throw psexception(instruction);
		}
	}
	catch (tcexception& e)
	{
		result = e.getstat();
		EMH_store_error_s1(EMH_severity_error, NAME_RULES_DEFAULT_IFAIL, e.what());
		log_error(e.what());
	}
	catch (psexception& e)
	{
		result = NAME_RULES_DEFAULT_IFAIL;
		EMH_store_error_s1(EMH_severity_error, NAME_RULES_DEFAULT_IFAIL, e.what());
		log_error(e.what());
	}

	return result;
}