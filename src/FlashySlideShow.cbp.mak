#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.138                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC =  -Iusr/include
CFLAGS =  -Wall `wx-config --cflags` `pkg-config --cflags opencv` -march=native -mtune=native
RESINC = 
LIBDIR =  -Lusr/lib
LIB =  -lglut -lGL -lGLU -lXxf86vm -lpthread -lopenal -lalut AmmarServer/src/AmmServerlib/libAmmServerlib.a
LDFLAGS =  `wx-config --libs` `pkg-config --libs opencv`

INC_DEBUG =  $(INC)
CFLAGS_DEBUG =  $(CFLAGS) -pedantic -pg -g
RESINC_DEBUG =  $(RESINC)
RCFLAGS_DEBUG =  $(RCFLAGS)
LIBDIR_DEBUG =  $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG =  $(LDFLAGS) -pg
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = flashyslideshows

INC_RELEASE =  $(INC)
CFLAGS_RELEASE =  $(CFLAGS) -fexpensive-optimizations -O3
RESINC_RELEASE =  $(RESINC)
RCFLAGS_RELEASE =  $(RCFLAGS)
LIBDIR_RELEASE =  $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE =  $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = flashyslideshows

OBJ_DEBUG = $(OBJDIR_DEBUG)/tools/image_sensing.o $(OBJDIR_DEBUG)/tools/joystick.o $(OBJDIR_DEBUG)/tools/jpegexiforient_embed.o $(OBJDIR_DEBUG)/tools/math_3d.o $(OBJDIR_DEBUG)/tools/sound.o $(OBJDIR_DEBUG)/tools/webinterface.o $(OBJDIR_DEBUG)/tools/wxwidgets_stuff.o $(OBJDIR_DEBUG)/transitions/transition_handler.o $(OBJDIR_DEBUG)/visuals.o $(OBJDIR_DEBUG)/visuals/background.o $(OBJDIR_DEBUG)/visuals/effects.o $(OBJDIR_DEBUG)/visuals/hud.o $(OBJDIR_DEBUG)/visuals/picture_renderer.o $(OBJDIR_DEBUG)/hypervisor/memory_hypervisor.o $(OBJDIR_DEBUG)/controls.o $(OBJDIR_DEBUG)/filesystem/directory_listing.o $(OBJDIR_DEBUG)/filesystem/directory_sorting.o $(OBJDIR_DEBUG)/filesystem/directory_transactions.o $(OBJDIR_DEBUG)/hypervisor/load_images.o $(OBJDIR_DEBUG)/hypervisor/load_textures.o $(OBJDIR_DEBUG)/camera_control.o $(OBJDIR_DEBUG)/layouts/expo_layout.o $(OBJDIR_DEBUG)/layouts/layout_handler.o $(OBJDIR_DEBUG)/main.o $(OBJDIR_DEBUG)/pictures_control.o $(OBJDIR_DEBUG)/scene_objects.o $(OBJDIR_DEBUG)/slideshow.o $(OBJDIR_DEBUG)/tools/environment.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/tools/image_sensing.o $(OBJDIR_RELEASE)/tools/joystick.o $(OBJDIR_RELEASE)/tools/jpegexiforient_embed.o $(OBJDIR_RELEASE)/tools/math_3d.o $(OBJDIR_RELEASE)/tools/sound.o $(OBJDIR_RELEASE)/tools/webinterface.o $(OBJDIR_RELEASE)/tools/wxwidgets_stuff.o $(OBJDIR_RELEASE)/transitions/transition_handler.o $(OBJDIR_RELEASE)/visuals.o $(OBJDIR_RELEASE)/visuals/background.o $(OBJDIR_RELEASE)/visuals/effects.o $(OBJDIR_RELEASE)/visuals/hud.o $(OBJDIR_RELEASE)/visuals/picture_renderer.o $(OBJDIR_RELEASE)/hypervisor/memory_hypervisor.o $(OBJDIR_RELEASE)/controls.o $(OBJDIR_RELEASE)/filesystem/directory_listing.o $(OBJDIR_RELEASE)/filesystem/directory_sorting.o $(OBJDIR_RELEASE)/filesystem/directory_transactions.o $(OBJDIR_RELEASE)/hypervisor/load_images.o $(OBJDIR_RELEASE)/hypervisor/load_textures.o $(OBJDIR_RELEASE)/camera_control.o $(OBJDIR_RELEASE)/layouts/expo_layout.o $(OBJDIR_RELEASE)/layouts/layout_handler.o $(OBJDIR_RELEASE)/main.o $(OBJDIR_RELEASE)/pictures_control.o $(OBJDIR_RELEASE)/scene_objects.o $(OBJDIR_RELEASE)/slideshow.o $(OBJDIR_RELEASE)/tools/environment.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d $(OBJDIR_DEBUG)/tools || mkdir -p $(OBJDIR_DEBUG)/tools
	test -d $(OBJDIR_DEBUG)/transitions || mkdir -p $(OBJDIR_DEBUG)/transitions
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)
	test -d $(OBJDIR_DEBUG)/visuals || mkdir -p $(OBJDIR_DEBUG)/visuals
	test -d $(OBJDIR_DEBUG)/hypervisor || mkdir -p $(OBJDIR_DEBUG)/hypervisor
	test -d $(OBJDIR_DEBUG)/filesystem || mkdir -p $(OBJDIR_DEBUG)/filesystem
	test -d $(OBJDIR_DEBUG)/layouts || mkdir -p $(OBJDIR_DEBUG)/layouts

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/tools/image_sensing.o: tools/image_sensing.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c tools/image_sensing.cpp -o $(OBJDIR_DEBUG)/tools/image_sensing.o

$(OBJDIR_DEBUG)/tools/joystick.o: tools/joystick.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c tools/joystick.cpp -o $(OBJDIR_DEBUG)/tools/joystick.o

$(OBJDIR_DEBUG)/tools/jpegexiforient_embed.o: tools/jpegexiforient_embed.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c tools/jpegexiforient_embed.cpp -o $(OBJDIR_DEBUG)/tools/jpegexiforient_embed.o

$(OBJDIR_DEBUG)/tools/math_3d.o: tools/math_3d.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c tools/math_3d.cpp -o $(OBJDIR_DEBUG)/tools/math_3d.o

$(OBJDIR_DEBUG)/tools/sound.o: tools/sound.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c tools/sound.cpp -o $(OBJDIR_DEBUG)/tools/sound.o

$(OBJDIR_DEBUG)/tools/webinterface.o: tools/webinterface.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c tools/webinterface.cpp -o $(OBJDIR_DEBUG)/tools/webinterface.o

$(OBJDIR_DEBUG)/tools/wxwidgets_stuff.o: tools/wxwidgets_stuff.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c tools/wxwidgets_stuff.cpp -o $(OBJDIR_DEBUG)/tools/wxwidgets_stuff.o

$(OBJDIR_DEBUG)/transitions/transition_handler.o: transitions/transition_handler.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c transitions/transition_handler.cpp -o $(OBJDIR_DEBUG)/transitions/transition_handler.o

$(OBJDIR_DEBUG)/visuals.o: visuals.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c visuals.cpp -o $(OBJDIR_DEBUG)/visuals.o

$(OBJDIR_DEBUG)/visuals/background.o: visuals/background.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c visuals/background.cpp -o $(OBJDIR_DEBUG)/visuals/background.o

$(OBJDIR_DEBUG)/visuals/effects.o: visuals/effects.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c visuals/effects.cpp -o $(OBJDIR_DEBUG)/visuals/effects.o

$(OBJDIR_DEBUG)/visuals/hud.o: visuals/hud.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c visuals/hud.cpp -o $(OBJDIR_DEBUG)/visuals/hud.o

$(OBJDIR_DEBUG)/visuals/picture_renderer.o: visuals/picture_renderer.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c visuals/picture_renderer.cpp -o $(OBJDIR_DEBUG)/visuals/picture_renderer.o

$(OBJDIR_DEBUG)/hypervisor/memory_hypervisor.o: hypervisor/memory_hypervisor.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c hypervisor/memory_hypervisor.cpp -o $(OBJDIR_DEBUG)/hypervisor/memory_hypervisor.o

$(OBJDIR_DEBUG)/controls.o: controls.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c controls.cpp -o $(OBJDIR_DEBUG)/controls.o

$(OBJDIR_DEBUG)/filesystem/directory_listing.o: filesystem/directory_listing.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c filesystem/directory_listing.cpp -o $(OBJDIR_DEBUG)/filesystem/directory_listing.o

$(OBJDIR_DEBUG)/filesystem/directory_sorting.o: filesystem/directory_sorting.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c filesystem/directory_sorting.cpp -o $(OBJDIR_DEBUG)/filesystem/directory_sorting.o

$(OBJDIR_DEBUG)/filesystem/directory_transactions.o: filesystem/directory_transactions.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c filesystem/directory_transactions.cpp -o $(OBJDIR_DEBUG)/filesystem/directory_transactions.o

$(OBJDIR_DEBUG)/hypervisor/load_images.o: hypervisor/load_images.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c hypervisor/load_images.cpp -o $(OBJDIR_DEBUG)/hypervisor/load_images.o

$(OBJDIR_DEBUG)/hypervisor/load_textures.o: hypervisor/load_textures.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c hypervisor/load_textures.cpp -o $(OBJDIR_DEBUG)/hypervisor/load_textures.o

$(OBJDIR_DEBUG)/camera_control.o: camera_control.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c camera_control.cpp -o $(OBJDIR_DEBUG)/camera_control.o

$(OBJDIR_DEBUG)/layouts/expo_layout.o: layouts/expo_layout.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c layouts/expo_layout.cpp -o $(OBJDIR_DEBUG)/layouts/expo_layout.o

$(OBJDIR_DEBUG)/layouts/layout_handler.o: layouts/layout_handler.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c layouts/layout_handler.cpp -o $(OBJDIR_DEBUG)/layouts/layout_handler.o

$(OBJDIR_DEBUG)/main.o: main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.cpp -o $(OBJDIR_DEBUG)/main.o

$(OBJDIR_DEBUG)/pictures_control.o: pictures_control.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c pictures_control.cpp -o $(OBJDIR_DEBUG)/pictures_control.o

$(OBJDIR_DEBUG)/scene_objects.o: scene_objects.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c scene_objects.cpp -o $(OBJDIR_DEBUG)/scene_objects.o

$(OBJDIR_DEBUG)/slideshow.o: slideshow.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c slideshow.cpp -o $(OBJDIR_DEBUG)/slideshow.o

$(OBJDIR_DEBUG)/tools/environment.o: tools/environment.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c tools/environment.cpp -o $(OBJDIR_DEBUG)/tools/environment.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf $(OBJDIR_DEBUG)/tools
	rm -rf $(OBJDIR_DEBUG)/transitions
	rm -rf $(OBJDIR_DEBUG)
	rm -rf $(OBJDIR_DEBUG)/visuals
	rm -rf $(OBJDIR_DEBUG)/hypervisor
	rm -rf $(OBJDIR_DEBUG)/filesystem
	rm -rf $(OBJDIR_DEBUG)/layouts

before_release: 
	test -d $(OBJDIR_RELEASE)/tools || mkdir -p $(OBJDIR_RELEASE)/tools
	test -d $(OBJDIR_RELEASE)/transitions || mkdir -p $(OBJDIR_RELEASE)/transitions
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)
	test -d $(OBJDIR_RELEASE)/visuals || mkdir -p $(OBJDIR_RELEASE)/visuals
	test -d $(OBJDIR_RELEASE)/hypervisor || mkdir -p $(OBJDIR_RELEASE)/hypervisor
	test -d $(OBJDIR_RELEASE)/filesystem || mkdir -p $(OBJDIR_RELEASE)/filesystem
	test -d $(OBJDIR_RELEASE)/layouts || mkdir -p $(OBJDIR_RELEASE)/layouts

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/tools/image_sensing.o: tools/image_sensing.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c tools/image_sensing.cpp -o $(OBJDIR_RELEASE)/tools/image_sensing.o

$(OBJDIR_RELEASE)/tools/joystick.o: tools/joystick.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c tools/joystick.cpp -o $(OBJDIR_RELEASE)/tools/joystick.o

$(OBJDIR_RELEASE)/tools/jpegexiforient_embed.o: tools/jpegexiforient_embed.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c tools/jpegexiforient_embed.cpp -o $(OBJDIR_RELEASE)/tools/jpegexiforient_embed.o

$(OBJDIR_RELEASE)/tools/math_3d.o: tools/math_3d.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c tools/math_3d.cpp -o $(OBJDIR_RELEASE)/tools/math_3d.o

$(OBJDIR_RELEASE)/tools/sound.o: tools/sound.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c tools/sound.cpp -o $(OBJDIR_RELEASE)/tools/sound.o

$(OBJDIR_RELEASE)/tools/webinterface.o: tools/webinterface.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c tools/webinterface.cpp -o $(OBJDIR_RELEASE)/tools/webinterface.o

$(OBJDIR_RELEASE)/tools/wxwidgets_stuff.o: tools/wxwidgets_stuff.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c tools/wxwidgets_stuff.cpp -o $(OBJDIR_RELEASE)/tools/wxwidgets_stuff.o

$(OBJDIR_RELEASE)/transitions/transition_handler.o: transitions/transition_handler.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c transitions/transition_handler.cpp -o $(OBJDIR_RELEASE)/transitions/transition_handler.o

$(OBJDIR_RELEASE)/visuals.o: visuals.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c visuals.cpp -o $(OBJDIR_RELEASE)/visuals.o

$(OBJDIR_RELEASE)/visuals/background.o: visuals/background.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c visuals/background.cpp -o $(OBJDIR_RELEASE)/visuals/background.o

$(OBJDIR_RELEASE)/visuals/effects.o: visuals/effects.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c visuals/effects.cpp -o $(OBJDIR_RELEASE)/visuals/effects.o

$(OBJDIR_RELEASE)/visuals/hud.o: visuals/hud.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c visuals/hud.cpp -o $(OBJDIR_RELEASE)/visuals/hud.o

$(OBJDIR_RELEASE)/visuals/picture_renderer.o: visuals/picture_renderer.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c visuals/picture_renderer.cpp -o $(OBJDIR_RELEASE)/visuals/picture_renderer.o

$(OBJDIR_RELEASE)/hypervisor/memory_hypervisor.o: hypervisor/memory_hypervisor.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c hypervisor/memory_hypervisor.cpp -o $(OBJDIR_RELEASE)/hypervisor/memory_hypervisor.o

$(OBJDIR_RELEASE)/controls.o: controls.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c controls.cpp -o $(OBJDIR_RELEASE)/controls.o

$(OBJDIR_RELEASE)/filesystem/directory_listing.o: filesystem/directory_listing.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c filesystem/directory_listing.cpp -o $(OBJDIR_RELEASE)/filesystem/directory_listing.o

$(OBJDIR_RELEASE)/filesystem/directory_sorting.o: filesystem/directory_sorting.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c filesystem/directory_sorting.cpp -o $(OBJDIR_RELEASE)/filesystem/directory_sorting.o

$(OBJDIR_RELEASE)/filesystem/directory_transactions.o: filesystem/directory_transactions.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c filesystem/directory_transactions.cpp -o $(OBJDIR_RELEASE)/filesystem/directory_transactions.o

$(OBJDIR_RELEASE)/hypervisor/load_images.o: hypervisor/load_images.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c hypervisor/load_images.cpp -o $(OBJDIR_RELEASE)/hypervisor/load_images.o

$(OBJDIR_RELEASE)/hypervisor/load_textures.o: hypervisor/load_textures.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c hypervisor/load_textures.cpp -o $(OBJDIR_RELEASE)/hypervisor/load_textures.o

$(OBJDIR_RELEASE)/camera_control.o: camera_control.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c camera_control.cpp -o $(OBJDIR_RELEASE)/camera_control.o

$(OBJDIR_RELEASE)/layouts/expo_layout.o: layouts/expo_layout.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c layouts/expo_layout.cpp -o $(OBJDIR_RELEASE)/layouts/expo_layout.o

$(OBJDIR_RELEASE)/layouts/layout_handler.o: layouts/layout_handler.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c layouts/layout_handler.cpp -o $(OBJDIR_RELEASE)/layouts/layout_handler.o

$(OBJDIR_RELEASE)/main.o: main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c main.cpp -o $(OBJDIR_RELEASE)/main.o

$(OBJDIR_RELEASE)/pictures_control.o: pictures_control.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c pictures_control.cpp -o $(OBJDIR_RELEASE)/pictures_control.o

$(OBJDIR_RELEASE)/scene_objects.o: scene_objects.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c scene_objects.cpp -o $(OBJDIR_RELEASE)/scene_objects.o

$(OBJDIR_RELEASE)/slideshow.o: slideshow.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c slideshow.cpp -o $(OBJDIR_RELEASE)/slideshow.o

$(OBJDIR_RELEASE)/tools/environment.o: tools/environment.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c tools/environment.cpp -o $(OBJDIR_RELEASE)/tools/environment.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf $(OBJDIR_RELEASE)/tools
	rm -rf $(OBJDIR_RELEASE)/transitions
	rm -rf $(OBJDIR_RELEASE)
	rm -rf $(OBJDIR_RELEASE)/visuals
	rm -rf $(OBJDIR_RELEASE)/hypervisor
	rm -rf $(OBJDIR_RELEASE)/filesystem
	rm -rf $(OBJDIR_RELEASE)/layouts

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

