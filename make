#!/bin/bash


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

echo $white
echo $boldChars
echo "Making FlashySlideshows.."
echo $normalChars
echo $normal

#Build AmmarServer first of all!
#OK this is kind of an ugly hack so lets explain a little. . 
#I have included a Null Version of AmmarServer with this library so that everything will compile without additional dependencies
#The repository starts off with only this Null library so if we cant find the real one ( AmmServerlib ) we copy the things from the Null Lib to emulate
#the real libary.. the rest of the project should then compile allright..  , To aquire a working AmmarServer just issue ./update_from_git.sh from repo root dir
  if [ -d src/AmmarServer/src/AmmServerlib ] 
   then
        echo $yellow
        echo "AmmServerlib , seems to already exist..! leaving it alone.."
        echo $normal
     else 
       mkdir src/AmmarServer/src/AmmServerlib
       cp src/AmmarServer/src/AmmServerNULLlib/* src/AmmarServer/src/AmmServerlib
   fi

  if [ -d src/AmmarServer/src/AmmServerlib ] 
   then
     cd src/AmmarServer/src/AmmServerlib
     ./make 
     cd ../../../../
     pwd
   fi
 
#We are done with ammarserver , lets procede to the main project..


OpenCVStuff="`pkg-config --cflags opencv` `pkg-config --libs opencv`"

Optimizations="-s -O3 -fexpensive-optimizations"
CPU=" "

#If we are on an 64bit intel card we might want to uncomment the line that follows for better performance 
#CPU="-mfpmath=sse -mtune=core2 -msse -msse2 -msse3"

LAYOUTS="layouts/layout_handler.cpp layouts/expo_layout.cpp"
VISUALS="visuals/hud.cpp visuals/background.cpp visuals/effects.cpp visuals/picture_renderer.cpp"
TRANSITIONS="transitions/transition_handler.cpp"
TOOLS="tools/environment.cpp tools/image_sensing.cpp tools/webinterface.cpp tools/sound.cpp tools/joystick.cpp tools/wxwidgets_stuff.cpp tools/math_3d.cpp tools/jpegexiforient_embed.cpp"
HYPERVISOR="hypervisor/memory_hypervisor.cpp hypervisor/load_images.cpp hypervisor/load_textures.cpp"

MAIN_PARTS="main.cpp directory_listing.cpp directory_sorting.cpp directory_transactions.cpp slideshow.cpp controls.cpp camera_control.cpp pictures_control.cpp visuals.cpp scene_objects.cpp"

LIBRARIES="-lglut -lGL -lGLU -lXxf86vm -lopenal -lalut AmmarServer/src/AmmServerlib/libAmmServerlib.a"

cd src

if [ -e flashyslideshows ]
then
rm  flashyslideshows
fi

g++ $Optimizations $MAIN_PARTS $TOOLS $HYPERVISOR $TRANSITIONS $VISUALS $LAYOUTS $LIBRARIES `wx-config --libs` `wx-config --cxxflags` $OpenCVStuff -L. -o flashyslideshows

strip flashyslideshows
  
cd FlashySlideShowStarter

rm  flashyslideshowsgui

g++ $Optimizations FlashySlideShowStarterApp.cpp FlashySlideShowStarterMain.cpp ../directory_listing.cpp ../directory_sorting.cpp `wx-config --libs` `wx-config --cxxflags` -L. -o flashyslideshowsgui

strip flashyslideshowsgui 

cd ..

cd ..
  
  if [ -e src/flashyslideshows ] 
   then
        echo $green
        echo "FlashySlideshows SUCCESS"
        echo $normal
     else  
        echo $red
        echo "FlashySlideshows FAILED"
        echo $normal
   fi

  if [ -e src/FlashySlideShowStarter/flashyslideshowsgui ] 
   then
        echo $green
        echo "FlashySlideshowsStarter SUCCESS"
        echo $normal
     else  
        echo $red
        echo "FlashySlideshowsStarter FAILED"
        echo $normal
   fi 

exit 0
