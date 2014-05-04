/*!
 * \file ps_write_syslog.hxx
 * \date 2014/04/30 20:20
 *
 * \author Mattias Jonsson (jonssonm)
 * Contact: jonsson.mattias@siemens.com
 *
 * \brief A set of C-logging functions for logging to different output sources.
 *
 * \warning To use this logging function make sure the preferences mentioned is set
 * to a valid value.
 *
*/

#pragma once

namespace ps
{
	//! Internal function called for initalizing the logging framework.
	logical initialize_logging();

	//!  Logs input string to target defined for error output.
	void log_error(const char *fmt, ...);

	//!  Logs input string to target defined for warn output.
	void log_warn(const char *fmt, ...);
	
	//!  Logs input string to target defined for info output.
	void log_info(const char *fmt, ...);

	//!  Logs input string to target defined for debug output.
	void log_debug(const char *fmt, ...);
}