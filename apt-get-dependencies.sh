#!/bin/bash
echo "This should install all packages that are needed by this program , if you dont have Ubuntu/Debian - apt-get you should install them manualy!"
sudo apt-get install g++ gcc wx-common libwxgtk2.8-0 libwxgtk2.8-dev  libalut0 libalut-dev freeglut3 freeglut3-dev libxxf86vm-dev libopencv-dev libcv-dev libcvaux-dev libhighgui-dev sed imagemagick libnotify-bin zenity
exit 0
