/*!
 * \file ps_name_rules.hxx
 * \date 2014/05/04 19:14
 *
 * \author Mattias Jonsson (jonssonm)
 * Contact: jonsson.mattias@siemens.com
 *
 * \brief Apply a naming rule to any property
 *
*/

#pragma once

#include "ps_global.hxx"

namespace ps
{
	//! Configurable naming rule function
	int ps_validate_name_rule(METHOD_message_t *m, va_list  args);
}