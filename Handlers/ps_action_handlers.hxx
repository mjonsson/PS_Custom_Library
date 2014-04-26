#pragma once

#include "ps_global.hxx"

#define ACTION_HANDLER_DEFAULT_IFAIL 919100

int ps_create_dispatcher_request_ah(EPM_action_message_t msg);
int ps_copy_task_properties_ah(EPM_action_message_t msg);
