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
include ./Makefile.nt

DIRS = Utils Handlers

all: 
				$(foreach dir,$(DIRS),make -C $(dir)$(NEWLINE))

debug:
				$(foreach dir,$(DIRS),make -C $(dir)$(NEWLINE))

clean:
				-$(RM) journal_file system_log Debug x64 Docs
				$(foreach dir,$(DIRS),make -C $(dir) clean$(NEWLINE))

rebuild: 
				-$(RM) journal_file system_log
				$(foreach dir,$(DIRS),make -C $(dir) rebuild$(NEWLINE))

debug_rebuild:
				-$(RM) journal_file system_log
				$(foreach dir,$(DIRS),make -C $(dir) debug_rebuild$(NEWLINE))

install: 		install_message
				$(foreach dir,$(DIRS),make -C $(dir) install$(NEWLINE))

debug_install:	install_message
				$(foreach dir,$(DIRS),make -C $(dir) debug_install$(NEWLINE))

doxy:
				-$(RM) ./Docs
				-$(MKDIR) ./Docs
				-$(DOXYGEN) $(DOXYGEN_CONFIG)
	
install_message:
				@echo -------------------------------------------------------------
				@echo install_message:
				@echo -------------------------------------------------------------
				@echo After installing handlers make sure to add the line:
				@echo .
				@echo   "<error id="100">%1$</error>"
				@echo .
				@echo to the file:
				@echo .
				@echo   "TC_ROOT\lang\textserver\en_US\ue_errors.xml"
				@echo -------------------------------------------------------------
