# Flashy Slide Shows

## A hardware accelerated slideshow application for linux

----------------------
 
![FlashySlideshows0](https://raw.githubusercontent.com/AmmarkoV/FlashySlideshows/master/screenshots/link.png)

This project [is now approximately 4.5k loc and has taken me 3+ months of development time](https://www.openhub.net/p/FlashySlideshows)..!
Since it meets my "photography" needs as it is I decided to pause it once again to concentrate on other projects :P .. Overall it is pretty stable , useful and I have done a good job profiling code and taken performance seriously while building it... 
If someone wants a particular feature or has a bug fix , etc , you are welcome [to add it on the issues tracker](https://github.com/AmmarkoV/FlashySlideshows/issues) so that it may be added or fixed..  
   
Screenshots :

[![FlashySlideshows1](https://raw.githubusercontent.com/AmmarkoV/FlashySlideshows/master/screenshots/screenshot1s.jpg)](https://raw.githubusercontent.com/AmmarkoV/FlashySlideshows/master/screenshots/screenshot1.jpg)
[![FlashySlideshows2](https://raw.githubusercontent.com/AmmarkoV/FlashySlideshows/master/screenshots/screenshot2s.jpg)](https://raw.githubusercontent.com/AmmarkoV/FlashySlideshows/master/screenshots/screenshot2.jpg)
[![FlashySlideshows3](https://raw.githubusercontent.com/AmmarkoV/FlashySlideshows/master/screenshots/screenshot3s.png)](https://raw.githubusercontent.com/AmmarkoV/FlashySlideshows/master/screenshots/screenshot3.png)
[![FlashySlideshows4](https://raw.githubusercontent.com/AmmarkoV/FlashySlideshows/master/screenshots/screenshot4s.jpg)](https://raw.githubusercontent.com/AmmarkoV/FlashySlideshows/master/screenshots/screenshot4.jpg)
 
Videos :

[![YouTube Link](https://raw.githubusercontent.com/AmmarkoV/FlashySlideshows/master/screenshots/utube1.png)  ](http://www.youtube.com/watch?v=QwYI-nkeCIw)
[![YouTube Link](https://raw.githubusercontent.com/AmmarkoV/FlashySlideshows/master/screenshots/utube2.png)  ](http://www.youtube.com/watch?v=zBEPWMlduAY)
 

## Dependencies
------------------------------------------------------------------ 

You can use the [./get-dependencies.sh](https://github.com/AmmarkoV/FlashySlideshows/blob/master/apt-get-dependencies.sh) script to get the required packages for compiling the project..!

Overall :

```
sudo apt-get install cmake git g++ gcc wx-common libwxgtk3.0-dev libalut0 libalut-dev freeglut3 freeglut3-dev libxxf86vm-dev libjpeg-dev  libpng-dev imagemagick wget sed libnotify-bin
```

should give you everything you need to build this application using the CMake build system.

## Building
------------------------------------------------------------------ 

To compile the project open a terminal cd to the root of the project and run

```
mkdir build && cd build && cmake .. && make 
```  

## Installation
------------------------------------------------------------------ 


if you want to install the app ( after compiling the project ) , run 

```
cd build && sudo make install
```

your password is required.. 

This will add a shortcut on the applications list and the binaries flashyslideshowsgui and flashyslideshows on the system ..

### Installation of latest version

[./update_from_git.sh](https://github.com/AmmarkoV/FlashySlideshows/blob/master/update_from_git.sh) will download the latest version of the software from the master repository on github
It also downloads AmmarServer which is an add on that enables a web interface for the application 
your password is required..  


### Uninstallation of current version

if you want to uninstall the app , run 
[./uninstall.sh](https://github.com/AmmarkoV/FlashySlideshows/blob/master/uninstall.sh)
your password is required..  

  

## TODO LIST
------------------------------------------------------------------ 

* Improve code structure

* Add save functionality for remembering settings..

* Fix Ken Burns effect and connect it to the output of OpenCV face detection which is currently disabled..!

* Add effects ( shiny floor ) to images  ,  transparency , particles  are done

* Add scripting capabilities for prerecorded (closed-loop) slideshows

* Make paper ( bending ) effects with 3D fonts

* Implement image_sensing.cpp

* Improve sound.cpp 


