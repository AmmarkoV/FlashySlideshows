project( FlashySlideshowsProject ) 
cmake_minimum_required( VERSION 2.8.7 )

set(CMAKE_CXX_FLAGS "-fPIC -march=native -mtune=native -O3 -fexpensive-optimizations -s") 
    

#These are unit tests for the core library
add_subdirectory (src/AmmarServer)
#This is the core application 
add_subdirectory (src/)
#These are unit tests for the core library
add_subdirectory (src/FlashySlideShowStarter)
   
INSTALL(FILES "src/flashyslideshows" DESTINATION "/usr/bin" ) 
INSTALL(FILES "src/FlashySlideShowStarter/flashyslideshowsgui" DESTINATION "/usr/bin" ) 

INSTALL(DIRECTORY "app_clipart" DESTINATION "/usr/share/flashyslideshows/")   
INSTALL(DIRECTORY "sounds" DESTINATION "/usr/share/flashyslideshows/") 


