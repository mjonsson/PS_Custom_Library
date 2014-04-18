#pragma once

#ifdef __cplusplus

#define	PS_PERF_TIMER_ENABLED "PS_PerfTimerEnabled"

#undef min
#undef max

using namespace std;

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

class HRTimer {
private:
	static bool initialized;
	static logical enabled;
	static LONGLONG frequency;
	static PerfMap perfMap;

	static void getFrequency(void);

public:
	static void start(const char *marker_name);
	static void stop(const char *marker_name);
	static void reset();
	static void print(void);
	static void print(const char *marker_name);
	static PerfMap getPerfData(void) { return perfMap; }
};

#endif

#ifdef __cplusplus
extern "C"
{
#endif

// C-wrapper implementation
void hr_start(const char *marker_name);
void hr_stop(const char *marker_name);
void hr_reset();
void hr_print_all(void);
void hr_print(const char *marker_name);

#ifdef __cplusplus
}
#endif
