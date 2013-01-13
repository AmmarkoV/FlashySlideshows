#!/bin/bash
echo "This should install all packages that are needed by this program , if you dont have Ubuntu/Debian - apt-get you should install them manualy!"


#Build Dependencies
BUILD_FLASHY_SLIDESHOWS="g++ gcc wx-common libwxgtk2.8-0 libwxgtk2.8-dev  libalut0 libalut-dev freeglut3 freeglut3-dev libxxf86vm-dev"
FACE_DETECTION="libopencv-dev libcv-dev libcvaux-dev libhighgui-dev"

#Runtime Dependencies
UPDATE_CHECKER="wget sed libnotify-bin"
IMAGE_CONVERSIONS_AND_RESIZING="imagemagick"
DISPLAYING_FAILSAFE_ERRORS="zenity"

sudo apt-get install $BUILD_FLASHY_SLIDESHOWS $FACE_DETECTION  $UPDATE_CHECKER $IMAGE_CONVERSIONS_AND_RESIZING $DISPLAYING_FAILSAFE_ERRORS
  
exit 0
