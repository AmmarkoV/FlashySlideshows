project( flashyslideshowsgui ) 
cmake_minimum_required( VERSION 2.8.7 ) 
# Extend cmake's module path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules ${CMAKE_MODULE_PATH})
    
add_executable(flashyslideshowsgui 
               ../filesystem/directory_listing.cpp
               ../filesystem/directory_listing.h
               ../filesystem/directory_sorting.cpp
               ../filesystem/directory_sorting.h 
               FlashySlideShowStarterApp.cpp
               FlashySlideShowStarterApp.h
               FlashySlideShowStarterMain.cpp
               FlashySlideShowStarterMain.h
               EmptyThumbnail.h
               FlashyLogo.h 
              ) 
 

 SET(wxWidgets_USE_LIBS)
 FIND_PACKAGE(wxWidgets)
 IF(wxWidgets_FOUND)
   INCLUDE("${wxWidgets_USE_FILE}") 
   target_link_libraries(flashyslideshowsgui ${wxWidgets_LIBRARIES} )
   # and for each of your dependant executable/library targets: 
 ELSE(wxWidgets_FOUND)
   # For convenience. When we cannot continue, inform the user
   MESSAGE("wxWidgets not found!")
 ENDIF(wxWidgets_FOUND)


set_target_properties(flashyslideshowsgui PROPERTIES 
                       DEBUG_POSTFIX "D"
                       ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                       LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                       RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                      )


#    install(TARGETS flashyslideshowsgui
#                    LIBRARY DESTINATION lib
#                    ARCHIVE DESTINATION lib
#                    RUNTIME DESTINATION bin)

