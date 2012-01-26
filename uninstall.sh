#!/bin/sh
if [ -d "/usr/share/flashyslideshows" ]; then
  echo "FlashySlideshows Installation detected , uninstalling it " 
  sudo rmdir -r /usr/share/flashyslideshows
  sudo rmdir /usr/share/flashyslideshows
  sudo rm /usr/share/applications/FlashySlideshows.desktop
  sudo rm /usr/share/icons/flashyicon.png
else 
   #nothing to do :P
fi 

  if [ -e /usr/bin/FlashySlideShowStarterApp  ]
  then
    sudo rm  /usr/bin/FlashySlideShowStarterApp 
  else  
   #nothing to do :P
  fi

  if [ -e bin/Release/FlashySlideShow ]
  then
    sudo rm  /usr/bin/FlashySlideShow 
  else 
   #nothing to do :P
  fi

 
  echo "Done" 
exit 0
