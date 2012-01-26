#!/bin/sh
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


  sudo mkdir /usr/share/flashyslideshows/app_clipart
  sudo mkdir /usr/share/flashyslideshows/sounds
  sudo cp app_clipart/* /usr/share/flashyslideshows/app_clipart
  sudo cp sounds/* /usr/share/flashyslideshows/sounds
  echo `date` >> /usr/share/flashyslideshows/install_time



  echo "Done" 
exit 0