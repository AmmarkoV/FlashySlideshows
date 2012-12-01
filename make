#!/bin/bash
echo "Making FlashySlideshows.."

#Build AmmarServer first of all!
#OK this is kind of an ugly hack so lets explain a little. . 
#I have included a Null Version of AmmarServer with this library so that everything will compile without additional dependencies
#The repository starts off with only this Null library so if we cant find the real one ( AmmServerlib ) we copy the things from the Null Lib to emulate
#the real libary.. the rest of the project should then compile allright..  , To aquire a working AmmarServer just issue ./update_from_git.sh from repo root dir
  if [ -d src/AmmarServer/src/AmmServerlib ] 
   then
        echo "AmmServerlib , seems to already exist..! leaving it alone.."
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

LIBRARIES="-lglut -lGL -lGLU -lXxf86vm -lopenal -lalut AmmarServer/src/AmmServerlib/libAmmServerlib.a"

cd src

g++ $Optimizations main.cpp environment.cpp jpegexiforient_embed.cpp load_images.cpp load_textures.cpp sound.cpp directory_listing.cpp directory_sorting.cpp directory_transactions.cpp slideshow.cpp controls.cpp camera_control.cpp pictures_control.cpp visuals.cpp scene_objects.cpp memory_hypervisor.cpp joystick.cpp math_3d.cpp image_sensing.cpp webinterface.cpp $TRANSITIONS $VISUALS $LAYOUTS wxwidgets_stuff.cpp $LIBRARIES `wx-config --libs` `wx-config --cxxflags` $OpenCVStuff -L. -o bin/Release/flashyslideshows

cp bin/Release/flashyslideshows ../flashyslideshows

cd FlashySlideShowStarter

g++ $Optimizations FlashySlideShowStarterApp.cpp FlashySlideShowStarterMain.cpp ../directory_listing.cpp ../directory_sorting.cpp `wx-config --libs` `wx-config --cxxflags` -L. -o bin/Release/flashyslideshowsgui

cp bin/Release/flashyslideshowsgui ../../flashyslideshowsgui

cd ..

cd ..

echo "Done.."

exit 0
