#===============================================================================
#                Copyright Siemens PLM Software 2014. 
#                 Unpublished - All rights reserved
#===============================================================================
# File			: Makefile
# Description	: Utils Makefile
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
		ps_string.$(OBJ_EXT) \
		ps_misc.$(OBJ_EXT)
		
#
# Define the DIBLIP option, lib name will be defined in Makefile.nt
#
EXPIMP = none

#
# Define additional library dependencies
#
DEP_LIBS= 

#
# Define the objects required for build
#
ALL_LIB_OBJS=*.$(OBJ_EXT)


all: 	build_obj_message $(OBJS)

debug:	all

clean: 	clean_message 	
		-$(RM) $(NT_LINK_EXT) $(GEN_OBJS)

rebuild: clean all

debug_rebuild: clean debug

install:

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
			@echo Building $(EXPIMP).lib and $(EXPIMP).dll dynamic library
			@echo -------------------------------------------------------------
			
clean_message:
			@echo -------------------------------------------------------------
			@echo Cleaning objects and libs for '$(EXPIMP)' dynamic library
			@echo -------------------------------------------------------------
