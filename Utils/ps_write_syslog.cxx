#include "ps_global.hxx"

using namespace ps;

static int g_logMasksInitialized = false;
static int g_logMask = LOGMASK_ERROR;
static int g_logStreams = LOGSTREAMS_ERR;

static void ps::initialize_logging()
{
	if (!g_logMasksInitialized) {
		TC_preference_search_scope_t	oldScope;
		int status = ITK_ok,
			iMask = LOGMASK_ERROR,
			iStreams = LOGSTREAMS_ERR;

		status = PREF_ask_search_scope(&oldScope);
		status = PREF_set_search_scope(TC_preference_all);
		status = PREF_ask_int_value(LOGMASK_PREFERENCE, 0, &iMask); 
		status = PREF_ask_int_value(LOGSTREAMS_PREFERENCE, 0, &iStreams); 
		status = PREF_set_search_scope(oldScope); 

		g_logMask = LOGMASK_ERROR | iMask;

		g_logStreams = iStreams;

		g_logMasksInitialized = true;
	}
}

void ps::log_error(const char *fmt, ...)
{
	initialize_logging();

	if ((g_logMask & LOGMASK_ERROR) == LOGMASK_ERROR) {

		va_list		ap;
		char		*pbuff = NULL;

		int len = (int)strlen(fmt) + BUFSIZ;
		if ((pbuff = (char *)MEM_alloc(len)) == NULL) {
			return;
		}

		va_start(ap, fmt);
		int need = vsnprintf(pbuff, len, fmt, ap) + 1;
		if (need > len) {
			if ((pbuff = (char *)MEM_realloc(pbuff, need)) == NULL) {
				return;
			}
			vsnprintf(pbuff, need, fmt, ap);
		}
		va_end(ap);

		if ((g_logStreams & LOGSTREAMS_ERR) == LOGSTREAMS_ERR) {
			(void)fprintf(stderr, "[ERROR] : %s\n", pbuff);
		}

		if ((g_logStreams & LOGSTREAMS_OUT) == LOGSTREAMS_OUT) {
			(void)fprintf(stdout, "[ERROR] : %s\n", pbuff);
		}

		if ((g_logStreams & LOGSTREAMS_SYSLOG) == LOGSTREAMS_SYSLOG) {
			TC_write_syslog("[ERROR] : %s\n", pbuff);
		}

		if (pbuff) {
			MEM_free(pbuff);
		}
	}
}

void ps::log_warn(const char *fmt, ...)
{
	initialize_logging();

	if ((g_logMask & LOGMASK_WARNING) == LOGMASK_WARNING) {

		va_list		ap;
		char		*pbuff = NULL;

		int len = (int)strlen(fmt) + BUFSIZ;
		if ((pbuff = (char *)MEM_alloc(len)) == NULL) {
			return;
		}

		va_start(ap, fmt);
		int need = vsnprintf(pbuff, len, fmt, ap) + 1;
		if (need > len) {
			if ((pbuff = (char *)MEM_realloc(pbuff, need)) == NULL) {
				return;
			}
			vsnprintf(pbuff, need, fmt, ap);
		}
		va_end(ap);

		if ((g_logStreams & LOGSTREAMS_OUT) == LOGSTREAMS_OUT) {
			(void)fprintf(stdout, "[WARN] : %s\n", pbuff);
		}

		if ((g_logStreams & LOGSTREAMS_SYSLOG) == LOGSTREAMS_SYSLOG) {
			TC_write_syslog("[WARN] : %s\n", pbuff);
		}

		if (pbuff) {
			MEM_free(pbuff);
		}
	}
}

void ps::log_info(const char *fmt, ...)
{
	initialize_logging();

	if ((g_logMask & LOGMASK_INFO) == LOGMASK_INFO) {

		va_list		ap;
		char		*pbuff = NULL;

		int len = (int)strlen(fmt) + BUFSIZ;
		if ((pbuff = (char *)MEM_alloc(len)) == NULL) {
			return;
		}

		va_start(ap, fmt);
		int need = vsnprintf(pbuff, len, fmt, ap) + 1;
		if (need > len) {
			if ((pbuff = (char *)MEM_realloc(pbuff, need)) == NULL) {
				return;
			}
			vsnprintf(pbuff, need, fmt, ap);
		}
		va_end(ap);

		if ((g_logStreams & LOGSTREAMS_OUT) == LOGSTREAMS_OUT) {
			(void)fprintf(stdout, "[INFO] : %s\n", pbuff);
		}

		if ((g_logStreams & LOGSTREAMS_SYSLOG) == LOGSTREAMS_SYSLOG) {
			TC_write_syslog("[INFO] : %s\n", pbuff);
		}

		if (pbuff) {
			MEM_free(pbuff);
		}
	}
}

void ps::log_debug(const char *fmt, ...)
{
	initialize_logging();

	if ((g_logMask & LOGMASK_DEBUG) == LOGMASK_DEBUG) {

		va_list		ap;
		char		*pbuff = NULL;

		int len = (int)strlen(fmt) + BUFSIZ;
		if ((pbuff = (char *)MEM_alloc(len)) == NULL) {
			return;
		}

		va_start(ap, fmt);
		int need = vsnprintf(pbuff, len, fmt, ap) + 1;
		if (need > len) {
			if ((pbuff = (char *)MEM_realloc(pbuff, need)) == NULL) {
				return;
			}
			vsnprintf(pbuff, need, fmt, ap);
		}
		va_end(ap);

		if ((g_logStreams & LOGSTREAMS_OUT) == LOGSTREAMS_OUT) {
			(void)fprintf(stdout, "[DEBUG] : %s\n", pbuff);
		}

		if ((g_logStreams & LOGSTREAMS_SYSLOG) == LOGSTREAMS_SYSLOG) {
			TC_write_syslog("[DEBUG] : %s\n", pbuff);
		}

		if (pbuff) {
			MEM_free(pbuff);
		}
	}
}
