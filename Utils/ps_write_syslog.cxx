#include "ps_global.hxx"
#include "ps_defines.hxx"

using namespace ps;

static int g_logMasksInitialized = false;
static int g_logMask = LOGMASK_ERROR;
static int g_logStreams = LOGSTREAMS_ERR;

logical ps::initialize_logging()
{
	if (!g_logMasksInitialized) {
		TC_preference_search_scope_t	oldScope;
		int status = ITK_ok,
			iMask = LOGMASK_ERROR,
			iStreams = LOGSTREAMS_ERR;

		try
		{
			itk(PREF_ask_search_scope(&oldScope));
			itk(PREF_set_search_scope(TC_preference_all));
			itk(PREF_ask_int_value(LOGMASK_PREFERENCE, 0, &iMask));
			itk(PREF_ask_int_value(LOGSTREAMS_PREFERENCE, 0, &iStreams));
			itk(PREF_set_search_scope(oldScope));
		}
		catch (tcexception& e)
		{
			return false;
		}

		g_logMask = LOGMASK_ERROR | iMask;

		g_logStreams = iStreams;

		g_logMasksInitialized = true;
	}

	return true;
}

void ps::log_error(const char *fmt, ...)
{
	if (!g_logMasksInitialized) return;

	if ((g_logMask & LOGMASK_ERROR) == LOGMASK_ERROR) {
		c_ptr<char>	log_message;
		va_list		ap;

		int bufsiz = strlen(fmt) + 512;
		log_message.alloc(bufsiz);

		va_start(ap, fmt);
		// Make sure we have enough memory to store parameters
		int need = vsnprintf(log_message.ptr(), bufsiz, fmt, ap) + 1;
		if (need > bufsiz)
		{
			log_message.realloc(need);
			vsnprintf(log_message.ptr(), need, fmt, ap);
		}
		va_end(ap);

		if ((g_logStreams & LOGSTREAMS_ERR) == LOGSTREAMS_ERR) {
			(void)fprintf(stderr, "[ERROR] : %s\n", log_message.ptr());
		}

		if ((g_logStreams & LOGSTREAMS_OUT) == LOGSTREAMS_OUT) {
			(void)fprintf(stdout, "[ERROR] : %s\n", log_message.ptr());
		}

		if ((g_logStreams & LOGSTREAMS_SYSLOG) == LOGSTREAMS_SYSLOG) {
			TC_write_syslog("[ERROR] : %s\n", log_message.ptr());
		}
	}
}

void ps::log_warn(const char *fmt, ...)
{
	if (!g_logMasksInitialized) return;

	if ((g_logMask & LOGMASK_WARNING) == LOGMASK_WARNING) {
		c_ptr<char>	log_message;
		va_list		ap;

		int bufsiz = strlen(fmt) + 512;
		log_message.alloc(bufsiz);

		va_start(ap, fmt);
		// Make sure we have enough memory to store parameters
		int need = vsnprintf(log_message.ptr(), bufsiz, fmt, ap) + 1;
		if (need > bufsiz)
		{
			log_message.realloc(need);
			vsnprintf(log_message.ptr(), need, fmt, ap);
		}
		va_end(ap);

		if ((g_logStreams & LOGSTREAMS_OUT) == LOGSTREAMS_OUT) {
			(void)fprintf(stdout, "[WARN] : %s\n", log_message.ptr());
		}

		if ((g_logStreams & LOGSTREAMS_SYSLOG) == LOGSTREAMS_SYSLOG) {
			TC_write_syslog("[WARN] : %s\n", log_message.ptr());
		}
	}
}

void ps::log_info(const char *fmt, ...)
{
	if (!g_logMasksInitialized) return;

	if ((g_logMask & LOGMASK_INFO) == LOGMASK_INFO) {
		c_ptr<char>	log_message;
		va_list		ap;

		int bufsiz = strlen(fmt) + 512;
		log_message.alloc(bufsiz);

		va_start(ap, fmt);
		// Make sure we have enough memory to store parameters
		int need = vsnprintf(log_message.ptr(), bufsiz, fmt, ap) + 1;
		if (need > bufsiz)
		{
			log_message.realloc(need);
			vsnprintf(log_message.ptr(), need, fmt, ap);
		}
		va_end(ap);

		if ((g_logStreams & LOGSTREAMS_OUT) == LOGSTREAMS_OUT) {
			(void)fprintf(stdout, "[INFO] : %s\n", log_message.ptr());
		}

		if ((g_logStreams & LOGSTREAMS_SYSLOG) == LOGSTREAMS_SYSLOG) {
			TC_write_syslog("[INFO] : %s\n", log_message.ptr());
		}
	}
}

void ps::log_debug(const char *fmt, ...)
{
	if (!g_logMasksInitialized) return;

	if ((g_logMask & LOGMASK_DEBUG) == LOGMASK_DEBUG) {
		c_ptr<char>	log_message;
		va_list		ap;

		int bufsiz = strlen(fmt) + 512;
		log_message.alloc(bufsiz);

		va_start(ap, fmt);
		// Make sure we have enough memory to store parameters
		int need = vsnprintf(log_message.ptr(), bufsiz, fmt, ap) + 1;
		if (need > bufsiz)
		{
			log_message.realloc(need);
			vsnprintf(log_message.ptr(), need, fmt, ap);
		}
		va_end(ap);

		if ((g_logStreams & LOGSTREAMS_OUT) == LOGSTREAMS_OUT) {
			(void)fprintf(stdout, "[DEBUG] : %s\n", log_message.ptr());
		}

		if ((g_logStreams & LOGSTREAMS_SYSLOG) == LOGSTREAMS_SYSLOG) {
			TC_write_syslog("[DEBUG] : %s\n", log_message.ptr());
		}
	}
}