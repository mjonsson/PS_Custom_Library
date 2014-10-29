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
#define AH_COPY_TASK_PROPERTIES "PS2-copy-task-properties-AH"
#define AH_MOVE_ATTACHMENTS "PS2-move-attachments-AH"
#define AH_TIMER_START "PS2-timer-start-AH"
#define AH_TIMER_STOP "PS2-timer-stop-AH"
#define AH_SET_STATUS "PS2-set-status-AH"

//! Rule handler definitions
#define AH_CHECK_IN_PROCESS "PS2-check-in-process-RH"
#define AH_CHECK_INITIATOR "PS2-check-initiator-RH"
#define AH_CHECK_PRIVILEGES "PS2-check-privileges-RH"
#define AH_CHECK_PROPERTIES "PS2-check-properties-RH"
#define AH_CHECK_CHILD_STRUCTURE "PS2-check-child-structure-RH"
#define AH_CHECK_WHERE_USED "PS2-check-where-used-RH"


//! Teamcenter iFail definitions
#define RULE_HANDLER_DEFAULT_IFAIL 919100
#define ACTION_HANDLER_DEFAULT_IFAIL 919100
#define NAME_RULES_DEFAULT_IFAIL 919100
#define FIND_REFERENCERS_DEFAULT_IFAIL 919100
#define DYNAMIC_FOLDERS_DEFAULT_IFAIL 919100

//! Preference definitions
#define PREF_REFERENCER_PROPERTIES "PS2_Referencer_Properties"
#define PREF_NAME_RULES "PS2_Naming_Rules"
#define PREF_DYNAMIC_FOLDER_TYPE "PS2_Dynamic_Folder_Type"
#define PREF_DYNAMIC_FOLDER_RUNTIME_PROP "PS2_Dynamic_Folder_Runtime_Prop"
#define PREF_DYNAMIC_FOLDER_ACTIVE_PROP "PS2_Dynamic_Folder_Active_Prop"
#define PREF_DYNAMIC_FOLDER_QUERY_PROP "PS2_Dynamic_Folder_Query_Prop"
#define PREF_DYNAMIC_FOLDER_ENTRIES_PROP "PS2_Dynamic_Folder_Entries_Prop"
#define PREF_DYNAMIC_FOLDER_VALUES_PROP "PS2_Dynamic_Folder_Values_Prop"
#define PREF_DYNAMIC_FOLDER_SORT_KEYS_PROP "PS2_Dynamic_Folder_Sort_Keys_Prop"
#define PREF_DYNAMIC_FOLDER_SORT_ORDER_PROP "PS2_Dynamic_Folder_Sort_Order_Prop"
#define PREF_DYNAMIC_FOLDER_MAX_HITS_PROP "PS2_Dynamic_Folder_Max_Hits_Prop"
#define PREF_DYNAMIC_FOLDER_DELAY_PROP "PS2_Dynamic_Folder_Delay_Prop"

//! Static variables
namespace ps
{
	static c_ptr<char>	ps2_dynamic_folder_type;
	static c_ptr<char>	ps2_dynamic_folder_runtime_prop;	
	static c_ptr<char>	ps2_dynamic_folder_active_prop;	
	static c_ptr<char>	ps2_dynamic_folder_query_prop;	
	static c_ptr<char>	ps2_dynamic_folder_entries_prop;	
	static c_ptr<char>	ps2_dynamic_folder_values_prop;	
	static c_ptr<char>	ps2_dynamic_folder_sortkeys_prop;	
	static c_ptr<char>	ps2_dynamic_folder_sortorder_prop;	
	static c_ptr<char>	ps2_dynamic_folder_maxhits_prop;
	static int ps2_dynamic_folder_delay_prop;
}
