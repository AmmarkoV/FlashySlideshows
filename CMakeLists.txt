cmake_minimum_required( VERSION 3.10 )
project( FlashySlideshowsProject )

OPTION (ENABLE_OPTIMIZATIONS "Build with -O3 -march=native" OFF)
OPTION (ENABLE_WEB_INTERFACE "Build the AmmarServer web remote control" OFF)
OPTION (ENABLE_FACE_DETECTION "Build with face detection" OFF)

OPTION (ENABLE_JPG "Build with JPEG image support" ON)
OPTION (ENABLE_PNG "Build with PNG image support" ON)

#These are unit tests for the core library

IF( ENABLE_FACE_DETECTION )
  add_definitions(-DUSE_FACEDETECTION) 
ENDIF( ENABLE_FACE_DETECTION )


IF(  ENABLE_OPTIMIZATIONS )
  set(CMAKE_CXX_FLAGS "-fPIC -march=native -mtune=native -O3 -fexpensive-optimizations -s") 
ENDIF( ENABLE_OPTIMIZATIONS )


IF( ENABLE_WEB_INTERFACE )
  add_definitions(-DWITH_WEBINTERFACE)
  add_subdirectory (src/AmmarServer)
ENDIF( ENABLE_WEB_INTERFACE )


add_subdirectory (src/Codecs)
#This is the core application 
add_subdirectory (src/)
#These are unit tests for the core library
add_subdirectory (src/FlashySlideShowStarter)
   


INSTALL(PROGRAMS "src/flashyslideshows" DESTINATION "/usr/bin" ) 
INSTALL(PROGRAMS "src/FlashySlideShowStarter/flashyslideshowsgui" DESTINATION "/usr/bin" ) 

INSTALL(DIRECTORY "app_clipart" DESTINATION "/usr/share/flashyslideshows/" DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )   
INSTALL(DIRECTORY "sounds" DESTINATION "/usr/share/flashyslideshows/"  DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
# The animated backgrounds are plain text ShaderToy fragment shaders compiled at
# runtime , so they have to be installed next to the clipart , see visuals/dynamic_background.cpp
INSTALL(DIRECTORY "shaders" DESTINATION "/usr/share/flashyslideshows/"  DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )
 

INSTALL(FILES "app_clipart/flashyicon.png" DESTINATION "/usr/share/icons/" ) 
INSTALL(FILES "FlashySlideshows.desktop" DESTINATION "/usr/share/applications/" ) 
 
