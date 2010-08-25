#!/bin/bash
g++ main.cpp load_images.cpp load_images.h load_textures.cpp load_textures.h sound.cpp sound.h slideshow.cpp slideshow.h -lglut -lGL -lGLU -lXxf86vm -L. -o bin/Release/FlashySlideShow
exit 0
