#===============================================================================
#                Copyright Siemens PLM Software 2014. 
#                 Unpublished - All rights reserved
#===============================================================================
# File			: Makefile
# Description	: Main Makefile 
#===============================================================================
# Developer            Date            Description
# Mattias Jonsson      2014-04-14      Initial Version
#===============================================================================

#
# Include the Makefile definition
#
include ./Makefile.nt

DIRS = Utils Additions

all: 
				$(foreach dir,$(DIRS),make -C $(dir)$(NEWLINE))

debug:
				$(foreach dir,$(DIRS),make -C $(dir) debug$(NEWLINE))

clean:
				-$(RM) *.sdf journal_file system_log Debug x64 Docs
				$(foreach dir,$(DIRS),make -C $(dir) clean$(NEWLINE))

rebuild: 
				-$(RM) journal_file system_log
				$(foreach dir,$(DIRS),make -C $(dir) rebuild$(NEWLINE))

debug_rebuild:
				-$(RM) journal_file system_log
				$(foreach dir,$(DIRS),make -C $(dir) debug_rebuild$(NEWLINE))

install: 		install_exec install_message

doxy:
				-$(RM) ./Docs
				-$(MKDIR) ./Docs
				-$(DOXYGEN) $(DOXYGEN_CONFIG)

install_exec:
				$(foreach dir,$(DIRS),make -C $(dir) install$(NEWLINE))
			
				
install_message:
				@echo -------------------------------------------------------------
				@echo !!! IMPORTANT MESSAGE !!!
				@echo -------------------------------------------------------------
				@echo Don't forget to follow the steps provided in the README.docx.
				@echo There are a couple of manual steps that needs to be performed
				@echo after the installation of the binaries.
				@echo -------------------------------------------------------------
