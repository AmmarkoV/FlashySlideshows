/** @file dynamic_background.cpp
 *  @brief see dynamic_background.h
 *  @author Ammar Qammaz (AmmarkoV)
 */

/* GLEW has to come before any other GL header , it refuses to load after gl.h */
#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/freeglut.h>
#endif

#include "dynamic_background.h"
#include "shadertoy.h"
#include "background.h"
#include "../slideshow.h"
#include "../tools/environment.h"
#include "../hypervisor/load_textures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DYNAMIC_BACKGROUND_PREFIX "background_"

static struct shadertoyEffectList backgrounds;

/* 0 = the old static picture background , 1..backgrounds.count = a shader */
static unsigned int selectedBackground=0;

static unsigned int lastDrawTickCount=0;

/* An animated background needs a continuous redraw , but -alwaysRedraw is also a
   switch the user can ask for on its own , so remember what they wanted and go back
   to it instead of switching it off on their behalf */
static unsigned int alwaysRedrawWasRequested=0;

/* How long shaders/background_ambient.frag takes to fade from the photo we were on to
   the one we just moved to , in milliseconds */
#define PHOTO_CROSSFADE_MS 700.0

static int activePhotoIndex=-1;
static int previousPhotoIndex=-1;
static unsigned int photoChangedAtTick=0;


unsigned int InitDynamicBackgrounds()
{
  selectedBackground=0;
  alwaysRedrawWasRequested=frame.alwaysRedraw;

  if (!shadertoy_loadEffectDirectory(DYNAMIC_BACKGROUND_PREFIX,&backgrounds)) { return 0; }

  fprintf(stderr,"Dynamic backgrounds availiable :");
  shadertoy_printEffectNames(&backgrounds);
  fprintf(stderr,"\n");

  return backgrounds.count;
}


unsigned int GetNumberOfDynamicBackgrounds()
{
  return backgrounds.count;
}

const char * GetDynamicBackgroundName(unsigned int index)
{
  if (index>=backgrounds.count) { return ""; }
  return backgrounds.entry[index].name;
}


int SelectDynamicBackgroundByName(const char * name)
{
  int found = shadertoy_findEffectByName(&backgrounds,name);
  if (found<0)
   {
     fprintf(stderr,"There is no %s background , availiable ones are :",(name!=0)?name:"");
     shadertoy_printEffectNames(&backgrounds);
     fprintf(stderr,"\n");
     return 0;
   }

  selectedBackground=(unsigned int) found + 1;
  /* A shader background is a moving picture , the event driven redraw would freeze it
     on whatever frame it stopped at */
  frame.alwaysRedraw=1;
  fprintf(stderr,"Using the %s dynamic background\n",name);
  return 1;
}


void CycleDynamicBackground()
{
  if (backgrounds.count==0) { return; }

  selectedBackground = (selectedBackground+1) % (backgrounds.count+1);

  if (selectedBackground==0)
   {
     frame.alwaysRedraw=alwaysRedrawWasRequested;
     fprintf(stderr,"Back to the static picture background\n");
   } else
   {
     frame.alwaysRedraw=1;
     fprintf(stderr,"Using the %s dynamic background\n",backgrounds.entry[selectedBackground-1].name);
   }
  frame.forceDrawOneMoreTime=1;
}


unsigned int DynamicBackgroundIsActive()
{
  return ( (backgrounds.count>0) && (selectedBackground>0) );
}

const char * GetActiveDynamicBackgroundName()
{
  if (!DynamicBackgroundIsActive()) { return ""; }
  return backgrounds.entry[selectedBackground-1].name;
}


int DrawDynamicBackground()
{
  if (!DynamicBackgroundIsActive()) { return 0; }

  struct shadertoyEffect * fx = backgrounds.entry[selectedBackground-1].fx;
  if (fx==0) { return 0; }

  /* An animated background redraws continuously , so asking glutGet for the window
     size here meant an X server round trip on every frame. ResizeCallback keeps
     frame.viewportWidth/Height up to date , which is the same number for free. */
  float width  = (float) frame.viewportWidth;
  float height = (float) frame.viewportHeight;
  if ( (width<1) || (height<1) ) { return 0; }

  struct shadertoyInputs inputs;
  memset(&inputs,0,sizeof(inputs));

  inputs.time      = (float) frame.tick_count / 1000;
  inputs.timeDelta = (float) (frame.tick_count - lastDrawTickCount) / 1000;
  lastDrawTickCount = frame.tick_count;
  /* First frame , or a long stall , or the tick counter wrapping around */
  if ( (inputs.timeDelta<0.0) || (inputs.timeDelta>1.0) ) { inputs.timeDelta=1.0/60; }

  inputs.frameNumber = (int) times_drawn_background;
  inputs.width  = width;
  inputs.height = height;
  inputs.mouseX = frame.mouse.mouse_x;
  inputs.mouseY = frame.mouse.mouse_y;

  /* Notice a change of photo so the ambient background can fade across instead of
     cutting , and hold on to which one we were showing before */
  int currentPhoto = (int) frame.active_image_place;
  if (currentPhoto!=activePhotoIndex)
   {
     previousPhotoIndex = activePhotoIndex;
     activePhotoIndex   = currentPhoto;
     photoChangedAtTick = frame.tick_count;
   }

  inputs.channel[0] = GetPictureTextureForSampling(activePhotoIndex  ,&inputs.channelWidth[0],&inputs.channelHeight[0]);
  inputs.channel[1] = GetPictureTextureForSampling(previousPhotoIndex,&inputs.channelWidth[1],&inputs.channelHeight[1]);

  inputs.progress = (float) (frame.tick_count - photoChangedAtTick) / PHOTO_CROSSFADE_MS;
  if ( (inputs.progress<0.0) || (inputs.progress>1.0) ) { inputs.progress=1.0; }
  /* The picture we were fading from may have been unloaded halfway through , in which
     case there is nothing to fade from and the new one has to be shown outright */
  if (inputs.channel[1]==0) { inputs.progress=1.0; }

  shadertoy_draw(fx,&inputs);

  ++times_drawn_background;
  return 1;
}


void CloseDynamicBackgrounds()
{
  shadertoy_unloadEffectList(&backgrounds);
  selectedBackground=0;
}
