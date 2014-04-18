#===============================================================================
#                Copyright Siemens PLM Software 2014. 
#                 Unpublished - All rights reserved
#===============================================================================
# File			: Makefile
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
OBJS = ps_hrtimer.$(OBJ_EXT) \
		ps_write_syslog.$(OBJ_EXT) \
		ps_misc.$(OBJ_EXT)
		
#
# Define the DIBLIP option, lib name will be defined in Makefile.nt
#
EXPORT/IMPORT=$(PS_UTILS)

#
# Define the library name, dll name will be defined in Makefile.nt
#
LIB_DLL =

#
# Define the libraries dependent on this
#
DEP_LIBS= 

#
# Define the objects required for this library
#
ALL_LIB_OBJS=*.$(OBJ_EXT)


all: 	build_obj_message $(OBJS)

debug:	all

clean: 	clean_message 	
		-$(RM) $(OBJS) $(NT_LINK_EXT)

rebuild: clean all

debug_rebuild: clean debug

install:

debug_install:	
			
copy_message:
			@echo -------------------------------------------------------------
			@echo Copying lib, pdb and dll files to build directory
			@echo -------------------------------------------------------------
			
build_obj_message:
			@echo -------------------------------------------------------------
			@echo Generating object files
			@echo -------------------------------------------------------------
			
build_dll_message:
			@echo -------------------------------------------------------------
			@echo Building $(PS_UTILS).lib and $(PS_UTILS).dll dynamic library
			@echo -------------------------------------------------------------
			
clean_message:
			@echo -------------------------------------------------------------
			@echo Cleaning objects and libs for '$(PS_UTILS)' dynamic library
			@echo -------------------------------------------------------------
