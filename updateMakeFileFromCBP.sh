#!/bin/bash
echo "Check https://sourceforge.net/projects/cbp2make/ if you dont have cbp2make"

cbp2make -in CodeBlocks.workspace -unix
#mv CodeBlocks.workspace.mak makefile


echo "Do not forget to add "
echo "INSTALLATIONSCRIPT:=\"./install.sh\" " 
echo "and "
echo ".PHONY: install"
echo "install :"
echo "	@echo \"INSTALLATIONSCRIPT=$(INSTALLATIONSCRIPT)\""
echo "	$(shell $(INSTALLATIONSCRIPT))"
echo "inside makefile"
exit 0
