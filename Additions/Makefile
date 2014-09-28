#===============================================================================
#                Copyright Siemens PLM Software 2014. 
#                 Unpublished - All rights reserved
#===============================================================================
# File			: Makefile.nt
# Description	: Handlers Makefile
#===============================================================================
# Developer            Date            Description
# Mattias Jonsson      2014-04-14      Initial Version
#===============================================================================

#
# Include the Makefile definition
#
include ../Makefile.nt

#
# Include source code files which needs to be compiled
#
OBJS = ps_check_in_process_rh.$(OBJ_EXT) \
		ps_check_initiator_rh.$(OBJ_EXT) \
		ps_check_privileges_rh.$(OBJ_EXT) \
		ps_check_properties_rh.$(OBJ_EXT) \
		ps_check_child_structure_rh.$(OBJ_EXT) \
		ps_check_where_used_rh.$(OBJ_EXT) \
		ps_create_dispatcher_request_ah.$(OBJ_EXT) \
		ps_copy_task_properties_ah.$(OBJ_EXT) \
		ps_move_attachments_ah.$(OBJ_EXT) \
		ps_timer_start_ah.$(OBJ_EXT) \
		ps_timer_stop_ah.$(OBJ_EXT) \
		ps_set_statuses_ah.$(OBJ_EXT) \
		ps_name_rules.$(OBJ_EXT) \
		ps_find_referencers.$(OBJ_EXT) \
		ps_register.$(OBJ_EXT)
		
#
# Define the DIBLIP option, lib name will be defined in Makefile.nt
#
EXPIMP = libpsadds

#
# Define additional library dependencies
#
DEP_LIBS=

#
# Define the objects required for build
#
ALL_LIB_OBJS=*.$(OBJ_EXT) \
				../Utils/*.$(OBJ_EXT)

all:  	build_obj_message $(OBJS) build_dll_message
		$(LINK_LINE)
		$(MANIFEST_LINE)

debug: 	all

rebuild: clean all

debug_rebuild: clean debug

install: copy_message copy

clean: 	clean_message
		-$(RM) $(NT_LINK_EXT) $(GEN_OBJS)

copy:
		-$(CHMOD) 777 $(EXPIMP).dll
		-$(CHMOD) 777 $(EXPIMP).pdb	
		-$(CHMOD) 777 $(EXPIMP).lib
		-$(CP) $(EXPIMP).dll $(TC_USER_BIN)
		-$(CP) $(EXPIMP).pdb $(TC_USER_BIN)
		-$(CP) $(EXPIMP).lib $(TC_USER_LIB)

build_obj_message:
			@echo -------------------------------------------------------------
			@echo Generating object files
			@echo -------------------------------------------------------------
			
build_dll_message:
			@echo -------------------------------------------------------------
			@echo Building $(EXPIMP).lib and $(EXPIMP).dll dynamic library
			@echo -------------------------------------------------------------
			
copy_message:
			@echo -------------------------------------------------------------
			@echo Copying lib, pdb and dll files to build directory
			@echo -------------------------------------------------------------

clean_message:
			@echo -------------------------------------------------------------
			@echo Cleaning objects and libs for '$(EXPIMP)' dynamic library
			@echo -------------------------------------------------------------
