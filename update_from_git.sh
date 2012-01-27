#!/bin/bash
#Reminder , to get a single file out of the repo use  "git checkout -- path/to/file.c"
git reset --hard HEAD
git pull
./install.sh
exit 0
