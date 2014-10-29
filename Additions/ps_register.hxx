/*!
 * \file ps_register.hxx
 * \date 2014/05/05 21:32
 *
 * \author Mattias Jonsson (jonssonm)
 * Contact: jonsson.mattias@siemens.com
 *
 * \brief Register userexits
 *
*/

#pragma once

namespace ps
{
	void ps_register_handlers();
	void ps_register_name_rules();
	void ps_register_referencers();
	void ps_register_dynamic_folders();
	int libps_oninit(int *decision, va_list args);
	int libps_onexit(int *decision, va_list args);
}

#ifdef __cplusplus
extern "C" {
#endif

	extern DLLAPI int libpsadds_register_callbacks();

#ifdef __cplusplus
}
#endif