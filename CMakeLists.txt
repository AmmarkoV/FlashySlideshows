project( FlashySlideshowsProject ) 
cmake_minimum_required( VERSION 2.8.7 )

set(CMAKE_CXX_FLAGS "-fPIC -march=native -mtune=native -O3 -fexpensive-optimizations -s") 
    

#These are unit tests for the core library
add_subdirectory (src/AmmarServer)
#This is the core application 
add_subdirectory (src/)
#These are unit tests for the core library
add_subdirectory (src/FlashySlideShowStarter)
  

# TODO FIX INSTALLATION DIRECTORIES 

#    install(TARGETS AmmarServer
#                    LIBRARY DESTINATION lib
#                    ARCHIVE DESTINATION lib
#                    RUNTIME DESTINATION bin)
 

