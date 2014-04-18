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

define NEWLINE


endef

DIRS = Utils Handlers

all: 
	$(foreach dir,$(DIRS),make -C $(dir)$(NEWLINE))

debug:
	$(foreach dir,$(DIRS),make -C $(dir)$(NEWLINE))

clean:
	-$(RM) journal_file system_log
	$(foreach dir,$(DIRS),make -C $(dir) clean$(NEWLINE))

rebuild: 
	-$(RM) journal_file system_log
	$(foreach dir,$(DIRS),make -C $(dir) rebuild$(NEWLINE))

debug_rebuild:
	-$(RM) journal_file system_log
	$(foreach dir,$(DIRS),make -C $(dir) debug_rebuild$(NEWLINE))

install:	
	$(foreach dir,$(DIRS),make -C $(dir) install$(NEWLINE))

debug_install:	
	$(foreach dir,$(DIRS),make -C $(dir) debug_install$(NEWLINE))
