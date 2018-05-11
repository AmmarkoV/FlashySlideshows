project( flashyslideshows ) 
cmake_minimum_required( VERSION 2.8.7 ) 
# Extend cmake's module path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules ${CMAKE_MODULE_PATH})
 
   IF( ENABLE_JPG )
 MESSAGE("JPGs will be included in this codec build")
 set(JPG_Libs jpeg ) 
 set(JPG_Parts jpgInput.c jpgInput.h jpgExifexternal.c jpgExifexternal.h jpgExiforient_embed.c jpgExiforient_embed.h )
 set(JPG_Includes ${CMAKE_SOURCE_DIR}/3dparty/OpenNI2/Include/ )
 add_definitions(-DUSE_JPG_FILES)
 add_definitions(-DENABLE_JPG)
ENDIF( ENABLE_JPG )


IF( ENABLE_PNG )
 MESSAGE("PNGs will be included in this codec build")
 set(PNG_Libs png ) 
 set(PNG_Parts pngInput.c pngInput.h)
 set(PNG_Includes ${CMAKE_SOURCE_DIR}/3dparty/OpenNI2/Include/ )
 add_definitions(-DUSE_PNG_FILES)
 add_definitions(-DENABLE_PNG)
ENDIF( ENABLE_PNG )



IF( ENABLE_WEB_INTERFACE ) 
 MESSAGE("AmmarServer will be included in this build")
 set(WEB_LIBS AmmarServer )
 add_definitions(-DWITH_WEBINTERFACE)
ENDIF( ENABLE_WEB_INTERFACE )



add_executable(flashyslideshows 
               main.cpp 
               controls.cpp
               controls.h
               camera_control.cpp
               camera_control.h
               pictures_control.cpp
               pictures_control.h
               scene_objects.cpp
               scene_objects.h
               slideshow.cpp
               slideshow.h
               visuals.cpp
               visuals.h
               #Visuals 
               visuals/background.cpp
               visuals/background.h
               visuals/effects.cpp
               visuals/effects.h
               visuals/hud.cpp
               visuals/hud.h
               visuals/picture_renderer.cpp
               visuals/picture_renderer.h
               #Transitions
               transitions/transition_handler.cpp
               transitions/transition_handler.h
               #Tools
               tools/commandlineParser.cpp 
               tools/commandlineParser.h 
               tools/environment.cpp 
               tools/environment.h 
               tools/image_sensing.cpp 
               tools/image_sensing.h 
               tools/joystick.cpp 
               tools/joystick.h 
               tools/jpegexiforient_embed.cpp 
               tools/jpegexiforient_embed.h 
               tools/math_3d.cpp 
               tools/math_3d.h 
               tools/scriptIntegration.cpp 
               tools/scriptIntegration.h 
               tools/sound.cpp 
               tools/sound.h 
               tools/webinterface.cpp
               tools/webinterface.h 
               tools/wxwidgets_stuff.cpp 
               tools/wxwidgets_stuff.h  
               #Layouts
               layouts/expo_layout.cpp
               layouts/expo_layout.h
               layouts/layout_handler.cpp
               layouts/layout_handler.h
               #HyperVisor
               hypervisor/load_images.cpp
               hypervisor/load_images.h
               hypervisor/load_textures.cpp
               hypervisor/load_textures.h
               hypervisor/memory_hypervisor.cpp
               hypervisor/memory_hypervisor.h
               #FileSystem
               filesystem/directory_listing.cpp
               filesystem/directory_listing.h
               filesystem/directory_sorting.cpp
               filesystem/directory_sorting.h
               filesystem/directory_transactions.cpp
               filesystem/directory_transactions.h

) 
 

 SET(wxWidgets_USE_LIBS)
 FIND_PACKAGE(wxWidgets)
 IF(wxWidgets_FOUND)
   INCLUDE("${wxWidgets_USE_FILE}") 
   target_link_libraries(flashyslideshows pthread glut GL GLU Xxf86vm openal alut rt Codecs ${PNG_Libs} ${JPG_Libs} ${WEB_LIBS} ${wxWidgets_LIBRARIES} )
   # and for each of your dependant executable/library targets: 
 ELSE(wxWidgets_FOUND)
   # For convenience. When we cannot continue, inform the user
   MESSAGE("wxWidgets not found!")
 ENDIF(wxWidgets_FOUND)


set_target_properties(flashyslideshows PROPERTIES 
                       DEBUG_POSTFIX "D"
                       ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                       LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                       RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                      )
 
add_dependencies(flashyslideshows ${WEB_LIBS} Codecs)    


#    install(TARGETS AmmarServer
#                    LIBRARY DESTINATION lib
#                    ARCHIVE DESTINATION lib
#                    RUNTIME DESTINATION bin)

