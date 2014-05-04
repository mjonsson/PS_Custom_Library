#pragma once

//! Logging definitions
#define	LOGMASK_ERROR			1
#define	LOGMASK_WARNING			2
#define LOGMASK_INFO			4
#define LOGMASK_DEBUG			8

#define LOGSTREAMS_ERR			1
#define LOGSTREAMS_OUT			2
#define LOGSTREAMS_SYSLOG		4

//! Preference definitions
//! Set Teamcenter site preference PS_LogMask to desired log level.
/*!
*  \note Logging errors and warnings means setting preference to a value of 3.
 */
#define	LOGMASK_PREFERENCE "PS2_Log_Mask"

//! Set Teamcenter site preference PS_LogMask to desired log sources.
/*!
 *  \note Logging to std err and syslog means setting preference to a value of 5.
 */
#define LOGSTREAMS_PREFERENCE "PS2_Log_Streams"

//! Whether to enable performance timers or not
#define	PS_PERF_TIMER_ENABLED "PS2_PerfTimer_Enabled"
