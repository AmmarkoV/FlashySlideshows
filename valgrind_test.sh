#!/bin/bash

  if [ -e src/bin/Debug/flashyslideshows ]
  then
    echo "Running debug version with valgrind .."
    valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes src/bin/Debug/flashyslideshows
  else
    echo "Could not find the debug version of flashyslideshows , try compiling it with the -g ( debug ) flag or use the Debug profile on code::blocks! "
    exit 1
  fi


exit 0
