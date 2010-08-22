#!/bin/bash
g++ main.cpp load_images.cpp load_images.h slideshow.cpp slideshow.h -lglut -lGL -lGLU -lXxf86vm -L. -o bin/Release/FlashySlideShow
exit 0
