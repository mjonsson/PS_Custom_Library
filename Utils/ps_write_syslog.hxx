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

#define	LOGMASK_ERROR			1
#define	LOGMASK_WARNING			2
#define LOGMASK_INFO			4
#define LOGMASK_DEBUG			8

#define LOGSTREAMS_ERR			1
#define LOGSTREAMS_OUT			2
#define LOGSTREAMS_SYSLOG		4

//! Set Teamcenter site preference PS_LogMask to desired log level.
/*!
*  \note Logging errors and warnings means setting preference to a value of 3.
 */
#define	LOGMASK_PREFERENCE		"PS_LogMask"

//! Set Teamcenter site preference PS_LogMask to desired log sources.
/*!
 *  \note Logging to std err and syslog means setting preference to a value of 5.
 */
#define LOGSTREAMS_PREFERENCE	"PS_LogStreams"

namespace ps
{
	//! Internal function called for initalizing the logging framework.
	static void initialize_logging();

	//!  Logs input string to target defined for error output.
	void log_error(const char *fmt, ...);

	//!  Logs input string to target defined for warn output.
	void log_warn(const char *fmt, ...);
	
	//!  Logs input string to target defined for info output.
	void log_info(const char *fmt, ...);

	//!  Logs input string to target defined for debug output.
	void log_debug(const char *fmt, ...);
}