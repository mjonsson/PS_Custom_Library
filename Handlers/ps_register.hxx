#pragma once

namespace ps
{
	void ps_register_handlers();
	void ps_register_name_rules();
	void ps_register_referencers();
	int libps_oninit(int *decision, va_list args);
	int libps_onexit(int *decision, va_list args);
}

#ifdef __cplusplus
extern "C" {
#endif

	extern DLLAPI int libpshandlers_register_callbacks();

#ifdef __cplusplus
}
#endif