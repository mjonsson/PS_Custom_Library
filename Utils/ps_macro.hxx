/*!
 * \file ps_macro.hxx
 * \date 2014/04/30 20:53
 *
 * \author Mattias Jonsson (jonssonm)
 * Contact: jonsson.mattias@siemens.com
 *
 * \brief Various utility macros mainly for use with Teamcenter.
 *
*/

#pragma once

//! Teamcenter macro to place before beginning a transaction that spans over several database updates.
#define begin_trans(x)														\
{																			\
	POM_place_markpoint(&x);												\
}

//! Teamcenter macro used for rolling back a pending database transaction
#define rollback_trans(x)													\
{																			\
	logical state_changed;													\
	if (x > 0)																\
	{																		\
		POM_roll_to_markpoint(x, &state_changed);							\
		x = 0;																\
	}																		\
}

//! Teamcenter macro used to commit a pending database transaction
#define commit_trans(x)														\
{																			\
	if (x > 0)																\
	{																		\
		POM_forget_markpoint(x);											\
		x = 0;																\
	}																		\
}

//! Macro used for wrapping Teamcenter ITK API functions
/*!
 *  \warning Remember to use this macro inside try-catch blocks
 *  \throws tcexception
 */
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
		for (int i = n_iFails - 1; i >= 0; i--) {																					\
			sprintf(buf, "  Code: %d Severity: %d Message: %s\n", iFails[i], iSeverities[i], pszErrorTexts[i]);						\
			error_msg += string(buf);																								\
		}																															\
		throw tcexception(stat, error_msg);																							\
    }																																\
}
//! Allocates \a y bytes of type \a x
#define sm_alloc(var,typ,siz) {										\
	if ((var = (typ*) MEM_alloc((siz) * sizeof(typ))) == NULL)		\
		throw psexception("Memory allocation error.");				\
}

//! Reallocates \a z bytes of type \a y to variable \a x
#define sm_realloc(var,typ,siz) {										\
	if ((var = (typ*) MEM_realloc(var, (siz) * sizeof(typ))) == NULL)	\
		throw psexception("Memory allocation error.");					\
}
//! Deallocates memory of \a x
#define sm_free(var) {																												\
	SAFE_SM_FREE(var);																												\
}
//! Deallocates memory of \a y elements of variable \a x
#define sm_free_arr(var,siz) {																										\
	int i;																															\
	if (var != NULL) {																												\
		for (i = 0; i < siz; i++) {																									\
			SAFE_SM_FREE(var[i]);																									\
		}																															\
	}																																\
	SAFE_SM_FREE(var);																												\
}
