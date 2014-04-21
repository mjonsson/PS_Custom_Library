#pragma once

#include "ps_global.hxx"

#define begin_trans(x)														\
{																			\
	POM_place_markpoint(&x);												\
}

#define rollback_trans(x)													\
{																			\
	logical state_changed;													\
	if (x > 0)																\
	{																		\
		POM_roll_to_markpoint(x, &state_changed);							\
		x = 0;																\
	}																		\
}

#define commit_trans(x)														\
{																			\
	if (x > 0)																\
	{																		\
		POM_forget_markpoint(x);											\
		x = 0;																\
	}																		\
}

// General macro for embedding itk functions that throws exception when error occurs
#define itk(x) {																													\
	int			stat,																												\
				n_iFails;																											\
	const int	*iSeverities = NULL,																								\
				*iFails = NULL;																										\
	const char	**pszErrorTexts = NULL;																								\
	char		buf[10000];																											\
	string		error_msg;																											\
	if((stat = (x)) != ITK_ok)																										\
	{																																\
		EMH_ask_errors(&n_iFails, &iSeverities, &iFails, &pszErrorTexts);															\
		sprintf(buf, "\nTeamcenter exception detected:\n\n  FUNCTION: %s FILE: %s LINE: %d\n\nError stack:\n\n", #x, __FILE__, __LINE__); \
		error_msg = string(buf);																									\
		for (int i = 0; i < n_iFails; i++) {																						\
			sprintf(buf, "  Code: %d Severity: %d Message: %s\n", iFails[i], iSeverities[i], pszErrorTexts[i]);						\
			error_msg += string(buf);																								\
		}																															\
		throw tcexception(stat, error_msg);																							\
    }																																\
}
#define sm_alloc(x,y) ((x*) MEM_alloc((y) * sizeof(x)))
#define sm_realloc(x,y,z) ((y*) MEM_realloc(x, (z) * sizeof(y)))
#define sm_free(x) {																												\
	SAFE_SM_FREE(x);																												\
}

#define sm_free_arr(x,y) {																											\
	int i;																															\
	if (x != NULL) {																												\
		for (i = 0; i < y; i++) {																									\
			SAFE_SM_FREE(x[i]);																										\
		}																															\
	}																																\
	SAFE_SM_FREE(x);																												\
}
