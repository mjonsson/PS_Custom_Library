#include "ps_global.hxx"

using namespace std;
using namespace ps;

bool ps::HRTimer::initialized = false;
logical ps::HRTimer::enabled = false;
LONGLONG ps::HRTimer::frequency = 0;
PerfMap ps::HRTimer::perfMap;

void ps::HRTimer::getFrequency(void)
{
	LARGE_INTEGER proc_freq;

	if (!QueryPerformanceFrequency(&proc_freq))
		throw exception("Could not get processor frequency.");

	frequency = proc_freq.QuadPart;
}

void ps::HRTimer::start(const char *marker_name)
{
	if (!initialized)
	{
		initialized = true;
		TC_preference_search_scope_t oldScope;
		PREF_ask_search_scope(&oldScope);
		PREF_set_search_scope(TC_preference_site);
		PREF_ask_logical_value(PS_PERF_TIMER_ENABLED, 0, &enabled);
		PREF_set_search_scope(oldScope); 

		// Update frequency
		getFrequency();
	}

	if (!enabled) return;

	// Create a new perfdata node if needed
	string str_marker_name(marker_name);
	PerfMap::iterator iter;
	if ((iter = perfMap.find(str_marker_name)) == perfMap.end()) {
		PerfData perfData;
		perfMap.insert(PerfPair(str_marker_name, perfData));
		iter = perfMap.find(str_marker_name);
	}

	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&(iter->second.start_t));
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
}
	
void ps::HRTimer::stop(const char *marker_name)
{
	if (!enabled) return;

	LARGE_INTEGER stop_t;

	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&stop_t);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);

	string str_marker_name(marker_name);
	PerfMap::iterator iter;
	if ((iter = perfMap.find(str_marker_name)) != perfMap.end()) {
		PerfData *perfData = &(iter->second);

		double delta = (stop_t.QuadPart - perfData->start_t.QuadPart) / (double)frequency;

		if (delta > perfData->max) perfData->max = delta;
		if (delta < perfData->min) perfData->min = delta;
		perfData->accumulated += delta;
		perfData->iterations++;
	}
}

void ps::HRTimer::reset()
{
	if (!enabled) return;

	perfMap.clear();
}

void ps::HRTimer::print()
{
	if (!enabled) return;

	log_info("|Marker                             |Iters. |Acc. (s)|Avg. (s)|Min. (s)|Max. (s)|");
	log_info("|===================================|=======|========|========|========|========|");

	for (PerfMap::iterator iter = perfMap.begin(); iter != perfMap.end(); ++iter)
	{
		PerfData *perfData = &(iter->second);
		log_info("|%-35s|%7d|%8.2f|%8.4f|%8.4f|%8.4f|", iter->first.c_str(), perfData->iterations,
			perfData->accumulated, perfData->accumulated / (double)perfData->iterations,
			perfData->min, perfData->max);
	}
}

void ps::HRTimer::print(const char *marker_name)
{
	if (!enabled) return;

	log_info("|Marker                             |Iters. |Acc. (s)|Avg. (s)|Min. (s)|Max. (s)|");
	log_info("|===================================|=======|========|========|========|========|");
	
	PerfMap::iterator iter;
	string str_marker_name(marker_name);
	if ((iter = perfMap.find(str_marker_name)) != perfMap.end())
	{
		PerfData *perfData = &(iter->second);
		log_info("|%-35s|%7d|%8.2f|%8.4f|%8.4f|%8.4f|", iter->first.c_str(), perfData->iterations,
			perfData->accumulated, perfData->accumulated / (double)perfData->iterations,
			perfData->min, perfData->max);
	}
}

// C-wrapper implementation
void ps::hr_start(const char *marker_name)
{
	HRTimer::start(marker_name);
}
void ps::hr_stop(const char *marker_name)
{
	HRTimer::stop(marker_name);
}
void ps::hr_reset()
{
	HRTimer::reset();
}
void ps::hr_print_all(void)
{
	HRTimer::print();
}
void ps::hr_print(const char *marker_name)
{
	HRTimer::print(marker_name);
}