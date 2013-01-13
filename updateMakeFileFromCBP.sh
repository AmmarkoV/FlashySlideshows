#!/bin/bash
echo "Check https://sourceforge.net/projects/cbp2make/ if you dont have cbp2make"

cbp2make -in CodeBlocks.workspace -unix
mv CodeBlocks.workspace.mak makefile


echo "Do not forget to add "
echo "INSTALLATIONSCRIPT:=\"./install.sh\" "
echo "FUNCTION:=$(shell $(INSTALLATIONSCRIPT))"
echo "and "
echo ".PHONY: install"
echo "install : all"
echo "          @echo \"INSTALLATIONSCRIPT=$(INSTALLATIONSCRIPT)\""
echo "          @echo \"FUNCTION=$(FUNCTION)\"" 
echo "inside makefile 
exit 0
