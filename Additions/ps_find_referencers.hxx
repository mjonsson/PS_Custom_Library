/*!
 * \file ps_find_referencers.hxx
 * \date 2014/05/04 19:12
 *
 * \author Mattias Jonsson (jonssonm)
 * Contact: jonsson.mattias@siemens.com
 *
 * \brief Runtime property returning referencers to the object and relation
 * it is configured for.
 *
*/

#pragma once

#include "ps_global.hxx"

namespace ps
{
	//! Returns referencing objects to configured object and relation. Type filter can be optionally set.
	int ps_find_referencers(METHOD_message_t *m, va_list  args);
}