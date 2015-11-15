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

//! Library name
#define PS_LIBNAME "libpsdf"

//! Teamcenter iFail definitions
#define DYNAMIC_FOLDERS_DEFAULT_IFAIL 919100

//! Preference definitions
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
