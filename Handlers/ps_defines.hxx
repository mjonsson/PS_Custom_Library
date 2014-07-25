/*!
 * \file ps_defines.hxx
 * \date 2014/05/05 21:31
 *
 * \author Mattias Jonsson (jonssonm)
 * Contact: jonsson.mattias@siemens.com
 *
 * \brief Pre-processor definitions
 *
*/

#pragma once

//! Action handler definitions
#define AH_CREATE_DISPATCHER_REQUEST "PS2-create-dispatcher-request-AH"
#define AH_COPY_TASK_PROPERTIES "PS2-copy-task-property-AH"
#define AH_MOVE_ATTACHMENTS "PS2-move-attachments-AH"
#define AH_TIMER_START "PS2-timer-start-AH"
#define AH_TIMER_STOP "PS2-timer-stop-AH"

//! Rule handler definitions
#define AH_CHECK_IN_PROCESS "PS2-check-in-process-RH"
#define AH_CHECK_INITIATOR "PS2-check-initiator-RH"
#define AH_CHECK_PRIVILEGES "PS2-check-privileges-RH"
#define AH_CHECK_CHILD_STRUCTURE "PS2-check-child-structure-RH"

//! Teamcenter iFail definitions
#define RULE_HANDLER_DEFAULT_IFAIL 919100
#define ACTION_HANDLER_DEFAULT_IFAIL 919100
#define NAME_RULES_DEFAULT_IFAIL 919100
#define FIND_REFERENCERS_DEFAULT_IFAIL 919100

//! Preference definitions
#define PREF_REFERENCER_PROPERTIES "PS2_Referencer_Properties"
#define PREF_NAME_RULES "PS2_Naming_Rules"
