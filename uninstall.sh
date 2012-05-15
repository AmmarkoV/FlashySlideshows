#!/bin/bash
if [ -d "/usr/share/flashyslideshows" ]; then
  echo "FlashySlideshows Installation detected , uninstalling it " 
  sudo rm -rf /usr/share/flashyslideshows 
  sudo rm /usr/share/applications/FlashySlideshows.desktop
  sudo rm /usr/share/icons/flashyicon.png
else 
  echo "No Installed Program Resources found"
fi 

  if [ -e /usr/bin/flashyslideshowsgui ]
  then
    sudo rm  /usr/bin/flashyslideshowsgui 
  else  
    echo "No Flashy Slideshows GUI detected "
  fi

  if [ -e /usr/bin/flashyslideshows ]
  then
    sudo rm  /usr/bin/flashyslideshows 
  else 
   echo "No Flashy Slideshows Binary detected"
  fi
 
  echo "Done" 
exit 0
