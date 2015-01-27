#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$DIR"

red=$(printf "\033[31m")
green=$(printf "\033[32m")
yellow=$(printf "\033[33m")
blue=$(printf "\033[34m")
magenta=$(printf "\033[35m")
cyan=$(printf "\033[36m")
white=$(printf "\033[37m")
normal=$(printf "\033[m")

normalChars=$(printf "\033[0m")
boldChars=$(printf "\033[1m")
underlinedChars=$(printf "\033[4m")
blinkingChars=$(printf "\033[5m") 

 
if [ $( id -u ) -eq 0 ]; then
echo "Will begin installation now"
else
 echo $red
 echo "Installer must be run as root in order to copy files to system.."
 echo "Please re run using sudo ./install.sh , exiting now.."
 echo $normal
 exit 0
fi


make

if [ -d "/usr/share/flashyslideshows" ]; then
  echo "FlashySlideshows Installation detected , patching it up :)" 
else
  echo "Installing Flashy Slideshows in the system.. :)" 
  sudo mkdir /usr/share/flashyslideshows
fi

  if [ -e src/FlashySlideShowStarter/flashyslideshowsgui ]
  then
    echo $green
    echo "FlashySlideShow GUI App is OK :) , including it to system binaries .." 
    echo $normal
    sudo cp src/FlashySlideShowStarter/flashyslideshowsgui /usr/bin/flashyslideshowsgui 
  else
    echo $red
    echo "FlashySlideShow GUI App could not be installed , you probably got a library missing"
    echo $normal
    exit 1
  fi

  if [ -e src/flashyslideshows ]
  then
    echo $green
    echo "FlashySlideShow Main App is OK :) , including it to system binaries .." 
    echo $normal
    sudo cp src/flashyslideshows /usr/bin/flashyslideshows
  else
    echo $red
    echo "FlashySlideShow Main App could not be installed , you probably got a library missing"
    echo $normal
    exit 1
  fi
 


if [ -d "/usr/share/flashyslideshows/app_clipart" ]; then
  echo "FlashySlideshows Clipart directory detected , patching it up :)" 
else
  echo "Creating Clipart Directory" 
  sudo mkdir /usr/share/flashyslideshows/app_clipart
fi


if [ -d "/usr/share/flashyslideshows/sounds" ]; then
  echo "FlashySlideshows Sounds directory detected , patching it up :)" 
else
  echo "Creating Sounds Directory" 
  sudo mkdir /usr/share/flashyslideshows/sounds
fi
 
  sudo cp app_clipart/* /usr/share/flashyslideshows/app_clipart
  sudo cp sounds/* /usr/share/flashyslideshows/sounds
  sudo cp app_clipart/flashyicon.png /usr/share/icons/flashyicon.png
  sudo cp FlashySlideshows.desktop /usr/share/applications/FlashySlideshows.desktop
  
  sudo chmod 744 /usr/share/flashyslideshows/app_clipart/*  
  sudo chmod 744 /usr/share/flashyslideshows/app_clipart/*  
  
  TIME_STAMP=`date`
  
  touch flashyslideshows_install.log
  echo "$TIME_STAMP" >> flashyslideshows_install.log 

  echo "Done" 
exit 0
