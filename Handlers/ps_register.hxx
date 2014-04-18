#pragma once

// Name		: libori_register_handlers
// Purpose	: Returns all VI:s connect to a generic
//
// Dir	Name						Description
// ---	---------------				--------------------------------------

int libps_register(int *decision, va_list args);

// Name		: libori_register_callbacks
// Purpose	: Callback function called by Teamcenter.
//
// Dir	Name						Description
// ---	---------------				--------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

extern DLLAPI int libpshandlers_register_callbacks();

#ifdef __cplusplus
}
#endif