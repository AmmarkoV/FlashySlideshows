#!/bin/bash

  if [ -e src/AmmarServer/src/main.c ] 
   then
      cd src/AmmarServer
      ./update_from_git.sh
      cd ../../
     else
     echo "Could not find AmmarServer , cloning a fresh copy from github!"
     cd src
     git clone git://github.com/AmmarkoV/AmmarServer.git
    cd ..
   fi

#Reminder , to get a single file out of the repo use  "git checkout -- path/to/file.c"
git reset --hard HEAD
git pull
./install.sh
exit 0
