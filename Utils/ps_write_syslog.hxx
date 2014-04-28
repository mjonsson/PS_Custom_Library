/*******************************************************************************
File Name  : ps_write_syslog.h
Type       : Custom wrapper function over Teamcenter to write to System log.
Functions written here ensures uniform output in log.

Author     : Mattias Jonsson

Change History

Who  | Date       | Description
=======|============|=========================================================
MJ   | 18/02/2013 | Initial
******************************************************************************/

#pragma once

#define	LOGMASK_ERROR			1
#define	LOGMASK_WARNING			2
#define LOGMASK_INFO			4
#define LOGMASK_DEBUG			8

#define LOGSTREAMS_ERR			1
#define LOGSTREAMS_OUT			2
#define LOGSTREAMS_SYSLOG		4

#define	LOGMASK_PREFERENCE		"PS_LogMask"
#define LOGSTREAMS_PREFERENCE	"PS_LogStreams"

namespace ps
{
	static void initialize_logging();

	/*******************************************************************************
	Function Name		: ps_write_err
	Input Parameters	: char **, variable arg
	Output paramters	: 
	Description			: Writes error information to Teamcenter System log
	Author				: Mattias Jonsson
	Change History

	Who  | Date       | Description
	=======|============|=========================================================
	MJ   | 18/02/2013 | Initial Creation
	******************************************************************************/

	void log_error(const char *fmt, ...);

	/*******************************************************************************
	Function Name		: write_warn
	Input Parameters	: char **, variable arg
	Output paramters	: 
	Description			: Writes WARN processing message information to Teamcenter System log
	Author				: Mattias Jonsson
	Change History

	Who  | Date       | Description
	=======|============|=========================================================
	MJ   | 18/02/2013 | Initial Creation
	******************************************************************************/

	void log_warn(const char *fmt, ...);

	/*******************************************************************************
	Function Name		: write_info
	Input Parameters	: char **, variable arg
	Output paramters	: 
	Description			: Writes INFO processing message information to Teamcenter System log
	Author				: Mattias Jonsson
	Change History

	Who  | Date       | Description
	=======|============|=========================================================
	MJ   | 18/02/2013 | Initial Creation
	******************************************************************************/

	void log_info(const char *fmt, ...);

	/*******************************************************************************
	Function Name		: write_debug
	Input Parameters	: char **, variable arg
	Output paramters	: 
	Description			: Writes DEBUG processing message information to Teamcenter System log
	Author				: Mattias Jonsson
	Change History

	Who  | Date       | Description
	=======|============|=========================================================
	MJ   | 18/02/2013 | Initial Creation
	******************************************************************************/

	void log_debug(const char *fmt, ...);
}