#!/bin/bash  

which apt-get &>/dev/null
if [ $? -eq 0 ] 
then
 ./apt-get-dependencies.sh
 exit 0
else
 echo "Apt-Get package manager was not found in this system"
fi

which pacman &>/dev/null
if [ $? -eq 0 ] 
then
 ./pacman-dependendies.sh
 exit 0
else
 echo "Pacman package manager was not found in this system"
fi

echo " Could not find a compatible package manager for automatic dependency resolution"} 

exit 1
