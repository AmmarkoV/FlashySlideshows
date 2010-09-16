#!/bin/bash
g++ main.cpp load_images.cpp load_images.h load_textures.cpp load_textures.h sound.cpp sound.h directory_listing.cpp directory_listing.h slideshow.cpp slideshow.h controls.cpp controls.h camera_control.cpp camera_control.h visuals.cpp visuals.h InputParser_C.c InputParser_C.h memory_hypervisor.cpp memory_hypervisor.h math_3d.cpp math_3d.h image_sensing.cpp image_sensing.h -lglut -lGL -lGLU -lXxf86vm -lopenal -lalut -L. -o bin/Release/FlashySlideShow
exit 0
