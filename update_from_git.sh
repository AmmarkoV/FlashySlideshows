#!/bin/bash

#Reminder , to get a single file out of the repo use  "git checkout -- path/to/file.c"
git reset --hard HEAD
git pull 

 # Since we want to update from git , this is the chance  to clone  a fresh verion of AmmarServer :) 
# First lets see if we already got a cloned version 
  if [ -e src/AmmarServer/src/main.c ] 
   then
      #We do , no need to clone everything just run the git pull commands and we are set..!
      cd src/AmmarServer
      ./update_from_git.sh
      cd ../../
     else 
     echo "Could not find AmmarServer , we have the stock NULL one only cloning a fresh copy from github!"
     rm -rf  src/AmmarServer
     cd src
     git clone git://github.com/AmmarkoV/AmmarServer.git
    cd ..
   fi

echo "Rebuilding a new version of the project"
./make

exit 0
