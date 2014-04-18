#===============================================================================
#                Copyright Siemens PLM Software 2014. 
#                 Unpublished - All rights reserved
#===============================================================================
# File			: Makefile.nt
# Description	: This file contains all variables which are 
#				  required for compilation.
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
		ps_register.$(OBJ_EXT)
		
#
# Define the DIBLIP option, lib name will be defined in Makefile.nt
#
EXPORT/IMPORT = $(PS_HANDLERS)

#
# Define the library name, dll name will be defined in Makefile.nt
#
LIB_DLL = $(PS_HANDLERS).$(DLL_EXT)

#
# Define the libraries dependent on this
#
DEP_LIBS=

#
# Define the objects required for this library
#
ALL_LIB_OBJS=*.$(OBJ_EXT) \
				../Utils/*.$(OBJ_EXT)

all:  	build_obj_message $(OBJS) build_dll_message
		$(LINK_LINE)

debug:	all

rebuild: clean all

debug_rebuild: clean debug

install: copy_message copy

clean: 	clean_message
		-$(RM) $(OBJS) $(NT_LINK_EXT)

copy:
		-$(CHMOD) 777 $(LIB_DLL)
		-$(CHMOD) 777 $(PS_HANDLERS).lib
		-$(CHMOD) 777 $(PS_HANDLERS).pdb	
		-$(CP) *.lib $(TC_USER_LIB)
		-$(CP) *.pdb $(TC_USER_BIN)
		-$(CP) $(LIB_DLL) $(TC_USER_BIN)

build_obj_message:
			@echo -------------------------------------------------------------
			@echo Generating object files
			@echo -------------------------------------------------------------
			
build_dll_message:
			@echo -------------------------------------------------------------
			@echo Building $(PS_HANDLERS).lib and $(PS_HANDLERS).dll dynamic library
			@echo -------------------------------------------------------------
			
copy_message:
			@echo -------------------------------------------------------------
			@echo Copying lib, pdb and dll files to build directory
			@echo -------------------------------------------------------------

clean_message:
			@echo -------------------------------------------------------------
			@echo Cleaning objects and libs for '$(PS_HANDLERS)' dynamic library
			@echo -------------------------------------------------------------
