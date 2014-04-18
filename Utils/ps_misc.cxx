#include "ps_global.hxx"

bool ps_null_or_empty(const char *str)
{
	if (str == NULL) return true;
	if (strlen(str) == 0) return true;

	return false;
}
