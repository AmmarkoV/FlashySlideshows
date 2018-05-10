#!/bin/bash

  if [ -e src/flashyslideshowsD ]
  then
    echo "Running debug version with valgrind .."
    valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes src/flashyslideshowsD -alwaysRedraw -no_hud -fullscreen -sfx -fd -v -images_per_line 3 -q 3 -d 5000 ~ 2> debug.txt

  else
    echo "Could not find the debug version of flashyslideshows , try compiling it with the -g ( debug ) flag or use the Debug profile on code::blocks! or the Debug profile of Cmake"
    exit 1
  fi


exit 0
