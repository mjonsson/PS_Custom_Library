#pragma once

#include "ps_global.hxx"

#define RULE_HANDLER_DEFAULT_IFAIL 919100

// Name		: libori_remove_objects
// Purpose	: Action handler that will remove objects from a process.
//
// Dir	Name						Description
// ---	---------------				--------------------------------------
// IN	EPM_action_message_t msg	Process task message

int ps_check_in_process_rh(EPM_rule_message_t msg);

// Name		: libori_remove_objects
// Purpose	: Action handler that will remove objects from a process.
//
// Dir	Name						Description
// ---	---------------				--------------------------------------
// IN	EPM_action_message_t msg	Process task message

int ps_check_initiator_rh(EPM_rule_message_t msg);

// Name		: libori_remove_objects
// Purpose	: Action handler that will remove objects from a process.
//
// Dir	Name						Description
// ---	---------------				--------------------------------------
// IN	EPM_action_message_t msg	Process task message

int ps_check_privileges_rh(EPM_rule_message_t msg);
int ps_check_child_structure_rh(EPM_rule_message_t msg);
