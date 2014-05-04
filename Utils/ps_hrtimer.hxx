/*!
 * \file ps_hrtimer.hxx
 * \date 2014/04/30 21:58
 *
 * \author Mattias Jonsson (jonssonm)
 * Contact: jonsson.mattias@siemens.com
 *
 * \brief Implements a high resolution timer that can be used to measure time.
 *
 * \note To enable the high resolution timer functionality, set the Teamcenter boolean site preference
 * "PS_PerfTimerEnabled" to true.
*/

#pragma once

namespace ps
{
#undef min
#undef max

	using namespace std;

	//! Performance data node
	/*!
	 *  \note Used internally only
	 */
	class PerfData {
	public:
		double accumulated;
		long iterations;
		double max;
		double min;
		LARGE_INTEGER start_t;

		PerfData(void) {
			accumulated = 0.0;
			iterations = 0;
			max = numeric_limits<double>::min();
			min = numeric_limits<double>::max();
		}

		PerfData(const PerfData& other) {
			accumulated = other.accumulated;
			iterations = other.iterations;
			max = other.max;
			min = other.min;
			start_t = other.start_t;
		}
	};

	typedef map<string, PerfData> PerfMap;
	typedef pair<string, PerfData> PerfPair;

	//! High resolution timer implementation class
	class HRTimer {
	private:
		static bool initialized;
		static logical enabled;
		static LONGLONG frequency;
		static PerfMap perfMap;

		static void getFrequency(void);

	public:
		//! Initializes the HRTimer class
		static logical ps::HRTimer::init();
		//! Starts a timer instance with identity \a marker_name
		static void start(const char *marker_name);
		//! Stops a timer instance with identity \a marker_name
		static void stop(const char *marker_name);
		//! Starts a debug timer instance with identity \a marker_name
		static void start_debug(const char *marker_name);
		//! Stops a debug timer instance with identity \a marker_name
		static void stop_debug(const char *marker_name);
		//! Resets all timers
		static void reset();
		//! Prints statistics of all timers
		static void print(void);
		//! Prints statistics for timer with identity \a marker_name
		static void print(const char *marker_name);
		//! Prints debug statistics for timer with identity \a marker_name
		static void print_debug(const char *marker_name);
		//! Returns all collected performance data nodes
		static PerfMap getPerfData(void) { return perfMap; }
	};

	//! C-wrapper for HRTimer::init()
	logical hr_init();
	//! C-wrapper for HRTimer::start(const char *marker_name)
	void hr_start(const char *marker_name);
	//! C-wrapper fr HRTimer::stop(const char *marker_name)
	void hr_stop(const char *marker_name);
	//! C-wrapper for HRTimer::start_debug(const char *marker_name)
	void hr_start_debug(const char *marker_name);
	//! C-wrapper fr HRTimer::stop_debug(const char *marker_name)
	void hr_stop_debug(const char *marker_name);
	//! C-wrapper fr HRTimer::reset()
	void hr_reset();
	//! C-wrapper fr HRTimer::print(void)
	void hr_print_all(void);
	//! C-wrapper fr HRTimer::print(const char *marker_name)
	void hr_print(const char *marker_name);
	//! C-wrapper fr HRTimer::print_debug(const char *marker_name)
	void hr_print_debug(const char *marker_name);
}