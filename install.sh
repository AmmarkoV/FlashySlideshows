#!/bin/bash
echo "Installation needs super user privileges.."
sudo echo "..." 

if [ -d "/usr/share/flashyslideshows" ]; then
  echo "FlashySlideshows Installation detected , patching it up :)" 
else
  echo "Installing Flashy Slideshows in the system.. :)" 
  sudo mkdir /usr/share/flashyslideshows
fi

  ./make

  if [ -e FlashySlideShowStarterApp ]
  then
    echo "FlashySlideShow GUI App is OK :) , including it to system binaries .."
    sudo cp FlashySlideShowStarterApp /usr/bin/FlashySlideShowStarterApp 
  else
    echo "FlashySlideShow GUI App could not be installed , you probably got a library missing"
    exit 1
  fi

  if [ -e bin/Release/FlashySlideShow ]
  then
    echo "FlashySlideShow Main App is OK :) , including it to system binaries .."
    sudo cp bin/Release/FlashySlideShow /usr/bin/FlashySlideShow
  else
    echo "FlashySlideShow Main App could not be installed , you probably got a library missing"
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
  sudo  touch /usr/share/flashyslideshows/install_time
  echo "$TIME_STAMP" | sudo cat >> /usr/share/flashyslideshows/install_time
  #sudo echo "$TIME_STAMP" >> /usr/share/flashyslideshows/install_time


  echo "Done" 
exit 0
