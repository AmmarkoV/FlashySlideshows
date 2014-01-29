#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$DIR"

cd src/FlashySlideShowStarter
./flashyslideshowsgui


cd "$STARTDIR"

exit 0
