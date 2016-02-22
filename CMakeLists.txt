project( FlashySlideshowsProject ) 
cmake_minimum_required( VERSION 2.8.7 )

OPTION (ENABLE_OPTIMIZATIONS OFF)
OPTION (ENABLE_WEB_INTERFACE OFF)
OPTION (ENABLE_FACE_DETECTION OFF)

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


#This is the core application 
add_subdirectory (src/)
#These are unit tests for the core library
add_subdirectory (src/FlashySlideShowStarter)
   
INSTALL(PROGRAMS "src/flashyslideshows" DESTINATION "/usr/bin" ) 
INSTALL(PROGRAMS "src/FlashySlideShowStarter/flashyslideshowsgui" DESTINATION "/usr/bin" ) 

INSTALL(DIRECTORY "app_clipart" DESTINATION "/usr/share/flashyslideshows/" DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )   
INSTALL(DIRECTORY "sounds" DESTINATION "/usr/share/flashyslideshows/"  DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE ) 
 

INSTALL(FILES "app_clipart/flashyicon.png" DESTINATION "/usr/share/icons/" ) 
INSTALL(FILES "FlashySlideshows.desktop" DESTINATION "/usr/share/applications/" ) 
 
