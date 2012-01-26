#!/bin/sh
if [ -d "/usr/share/flashyslideshows" ]; then
  echo "FlashySlideshows Installation detected , uninstalling it " 
else 
  sudo rmdir -r /usr/share/flashyslideshows
  sudo rmdir /usr/share/flashyslideshows
fi 

  if [ -e /usr/bin/FlashySlideShowStarterApp  ]
  then
    sudo rm  /usr/bin/FlashySlideShowStarterApp 
  else  
   #nothing :P
  fi

  if [ -e bin/Release/FlashySlideShow ]
  then
    sudo rm  /usr/bin/FlashySlideShow 
  else 
   #nothing :P
  fi

 
  echo "Done" 
exit 0
