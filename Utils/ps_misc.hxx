#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
Function Name		: ps_null_or_empty
Input Parameters	: char *str
Output paramters	: bool
Description			: Return true if string is null or empty, else return false
Author				: Mattias Jonsson
Change History

  Who  | Date       | Description
=======|============|=========================================================
  MJ   | 22/01/2012 | Initial Creation
******************************************************************************/

bool ps_null_or_empty(const char *str);

#ifdef __cplusplus
}
#endif
